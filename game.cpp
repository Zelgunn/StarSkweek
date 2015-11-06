#include "game.h"

Game::Game() :
    m_state(MenuState), m_timer(Q_NULLPTR)
{
    QFile file(":/xml/xml/data.xml");
    file.open(QIODevice::ReadOnly);

    QDomDocument dom;
    dom.setContent(&file);

    QDomElement elem = dom.documentElement();
    QDomNode node = elem.firstChild();

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Character")
        {
            m_playersPrototypes.append(new Player(elem));
        }

        if(elem.tagName() == "Projectiles")
        {
            m_projectilesElement = elem;
        }

        if(elem.tagName() == "Weapons")
        {
            m_weaponsElement = elem;
        }

        node = node.nextSibling();
    }

    m_timer = new QTimer(this);
    QObject::connect(&m_multiplayerUpdater, SIGNAL(gameConnected()), this, SLOT(onGameConnected()));
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
}

const Level *Game::level() const
{
    return m_level;
}

const QList<const Player *> *Game::players() const
{
    return &m_playersPrototypes;
}

void Game::movePlayer(GameObject::Directions direction)
{
    if(!m_timer->isActive()) return;

    m_level->movePlayer(0, direction);
}

void Game::processCommands()
{
    QStringList updates = m_multiplayerUpdater.receivedUpdates();
    QString update;
    char firstChar;
    m_untreatedCommands.clear();

    for(int i=0; i<updates.size(); i++)
    {
        update = updates.at(i);
        firstChar = update.at(0).toLatin1();

        switch (firstChar) {
        case 'p':
            update = update.remove(0,1);
            playerCommand(1, update);
            break;
        default:
            m_untreatedCommands.append(update);
            break;
        }
    }
}

void Game::playerCommand(int player, QString command)
{
    char firstChar = command.at(0).toLatin1();

    switch (firstChar) {
    case 'm':   // Déplacement
        movePlayer(player, command);
        break;
    case 's':   // Tir
        playerFires(player);
        break;
    case 'r':   // Joueur prêt
        setPlayerReady(!isPlayerReady(player), player);
        break;
    case 'n':   // Pseudo
        setPlayerNickname(command.remove(0,1), player);
        break;
    case 'c':   // Sélection de personnage
        setPlayerChar(command.remove(0,1).toInt(), player);
        break;
    }
}

void Game::movePlayer(int player, QString command)
{
    command = command.remove(0, 1);
    int x = command.section(',',0,0).toInt();
    int y = command.section(',',1,1).toInt();
    int dir = command.section(',',-1,-1).toInt();

    m_level->setPlayerPosition(player, x, y, (GameObject::Directions) dir);
}

void Game::playerFires(int playerID)
{
    Level *level = m_level;
    if(level->playerFires(playerID))
    {
        if(playerID == 0)
        {
            QString update = "ps";  // p = player, s = shoot
            m_multiplayerUpdater.appendUpdate(update);
        }
    }
}

void Game::startGame()
{
    setLevelPath(m_multiplayerUpdater.mapPath());
    loadLevel(m_levelPath);

    if(m_multiplayerUpdater.isHost() || (m_multiplayerUpdater.playersInfos().size() == 1))
    {
        m_level->setMyPlayer(0);
    }
    else
    {
        m_level->setMyPlayer(1);
    }

    m_timer->start(16);
    setState(PlayingState);
}

bool Game::isStarted() const
{
    return m_timer->isActive();
}

void Game::stopGame()
{
    m_timer->stop();
}

bool Game::isPlayerVictorious() const
{
    qreal playerRatio = level()->playerTileRatio();
    return (playerRatio > 0.8);
}

bool Game::isPlayerDefeated() const
{
    qreal playerRatio = level()->playerTileRatio();
    return (playerRatio < 0.2);
}

void Game::loadLevel(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    QDomDocument dom;
    dom.setContent(&file);

    QDomElement elem = dom.documentElement();

    elem.appendChild(m_projectilesElement);
    elem.appendChild(m_weaponsElement);

    m_level = new Level(elem, &m_playersPrototypes, m_multiplayerUpdater.playersInfos());

    file.close();
}

Game::GameStates Game::state() const
{
    return m_state;
}

void Game::setState(const GameStates &state)
{
    m_state = state;
    emit stateChanged(state);
}

QString Game::levelPath() const
{
    return m_levelPath;
}

void Game::setLevelPath(const QString &levelPath)
{
    m_levelPath = levelPath;
    if(m_levelPath.contains('/'))
    {
        m_multiplayerUpdater.setMapPath(m_levelPath.section('/', -1, -1));
    }
    else
    {
        m_levelPath.insert(0, QApplication::applicationDirPath() + "/xml/levels/");
    }
}

PlayerInfo *Game::playerInfo(int player) const
{
    QList<PlayerInfo *> playersInfos = m_multiplayerUpdater.playersInfos();
    if((player < 0) || (player >= playersInfos.size()))
        return Q_NULLPTR;

    return playersInfos.at(player);
}

QString Game::playerNickname(int player) const
{
    PlayerInfo *pInfo = playerInfo(player);
    if(pInfo == Q_NULLPTR) return QString("");
    return pInfo->nickname();
}

void Game::setPlayerNickname(const QString &nickname, int player)
{
    PlayerInfo *pInfo = playerInfo(player);
    if(nickname != pInfo->nickname())
        pInfo->setNickname(nickname);
}

int Game::playerChar(int player) const
{
    PlayerInfo *pInfo = playerInfo(player);
    if(pInfo == Q_NULLPTR) return -1;
    return pInfo->characterSelected();
}

void Game::setPlayerChar(int characterIndex, int player)
{
    PlayerInfo *pInfo = playerInfo(player);
    pInfo->setCharacterSelected(characterIndex);

    if(player == 0)
    {
        m_multiplayerUpdater.appendUpdate("pc" + QString::number(playerChar()));
        m_multiplayerUpdater.sendUpdates();
    }
}

bool Game::isPlayerReady(int player) const
{
    PlayerInfo *pInfo = playerInfo(player);
    if(pInfo == Q_NULLPTR) return false;
    return playerInfo(player)->ready();
}

void Game::setPlayerReady(bool ready, int player)
{
    QList<PlayerInfo *> playersInfos = m_multiplayerUpdater.playersInfos();
    PlayerInfo *pInfo = playersInfos.at(player);

    // Personnage aléatoire
    if((player == 0) && ready && (playerChar() == m_playersPrototypes.size()))
    {
        selectRandomPlayer();
    }

    // Vérification que le personnage n'est pas déjà pris.
    for(int i=0; i<playersInfos.size(); i++)
    {
        if(i != player)
        {
            if((isPlayerReady(i)) && (playerChar(i) == playerChar(player)))
            {
                return;
            }
        }
    }
    pInfo->setReady(ready);

    // Si le joueur est le joueur courant, on envoie une notification.
    if(player == 0)
    {
        m_multiplayerUpdater.appendUpdate("pr");
        m_multiplayerUpdater.sendUpdates();
    }

    // Si tous les joueurs sont prêts, on lance le jeu.
    foreach(pInfo, playersInfos)
    {
        if(!pInfo->ready()) return;
    }
    startGame();
}

void Game::selectRandomPlayer()
{
    QList<PlayerInfo *> playersInfos = m_multiplayerUpdater.playersInfos();

    bool characterFree;
    int randomIndex = -1;
    while(playerChar() == m_playersPrototypes.size())
    {
        characterFree = true;
        randomIndex = qrand()%m_playersPrototypes.size();
        for(int i=1; i<playersInfos.size(); i++)
        {
            if((isPlayerReady(i)) && (playerChar(i) == randomIndex))
            {
                characterFree = false;
            }
        }
        if(characterFree)
        {
            setPlayerChar(randomIndex);
        }
    }
}

void Game::nextFrame()
{
    m_multiplayerUpdater.appendUpdate("pm" + QString::number(m_level->player()->position().x())
                                      + ',' + QString::number(m_level->player()->position().y())
                                      + ',' + QString::number(m_level->player()->direction()));
    m_multiplayerUpdater.sendUpdates();
    Level *level = m_level;

    processCommands();

    level->nextFrame();

    if((isPlayerDefeated()) || (isPlayerVictorious())) stopGame();
}

QStringList Game::untreatedCommands()
{
    if(m_untreatedCommands.isEmpty())
    {
        processCommands();
    }
    return m_untreatedCommands;
}

void Game::removeUntreadtedCommand(int index)
{
    m_untreatedCommands.removeAt(index);
}

void Game::startHost(bool enable)
{
    m_multiplayerUpdater.startHost(enable);
}

void Game::lookForLocalHost()
{
    m_multiplayerUpdater.lookForLocalHost();
}

void Game::connectToIP(const QString &ip)
{
    m_multiplayerUpdater.connectToIP(ip);
}

bool Game::isHost() const
{
    return m_multiplayerUpdater.isHost();
}

void Game::onRight()
{
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Right);
    }
}

void Game::onUp()
{
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Up);
    }
}

void Game::onLeft()
{
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Left);
    }
}

void Game::onDown()
{
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Down);
    }
}

void Game::onEnter()
{
    m_multiplayerUpdater.appendUpdate(QString::number(Qt::Key_Enter));
    if(m_state == PlayingState)
    {
        playerFires();
    }
    else
    {
        m_multiplayerUpdater.sendUpdates();
    }
}

void Game::onBackpace()
{
    m_multiplayerUpdater.appendUpdate(QString::number(Qt::Key_Backspace));
}

void Game::onGameConnected()
{
    if(m_state != LobbyState)
    {
        setLevelPath(m_multiplayerUpdater.mapPath());
        m_multiplayerUpdater.appendUpdate("pn" + playerNickname());
        m_multiplayerUpdater.sendUpdates();
        setState(LobbyState);
    }
}
