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
    case 'm':
        movePlayer(player, command);
        break;
    case 's':
        playerFires(player);
        break;
    case 'r':
        setPlayerReady(!isPlayerReady(player), player);
        break;
    case 'n':
        setPlayerNickname(command.remove(0,1), player);
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

void Game::setPlayerNickname(const QString &nickname, int player)
{
    QList<PlayerInfo *> playersInfos = m_multiplayerUpdater.playersInfos();
    PlayerInfo *playerInfo = playersInfos.at(player);
    if(nickname != playerInfo->nickname())
        playerInfo->setNickname(nickname);
}

bool Game::isPlayerReady(int player) const
{
    QList<PlayerInfo *> playersInfos = m_multiplayerUpdater.playersInfos();
    PlayerInfo *playerInfo = playersInfos.at(player);
    return playerInfo->ready();
}

void Game::setPlayerReady(bool ready, int player)
{
    QList<PlayerInfo *> playersInfos = m_multiplayerUpdater.playersInfos();
    PlayerInfo *playerInfo = playersInfos.at(player);
    playerInfo->setReady(ready);

    if(player == 0)
    {
        m_multiplayerUpdater.appendUpdate("pr");
        m_multiplayerUpdater.sendUpdates();
    }

    foreach(playerInfo, playersInfos)
    {
        if(!playerInfo->ready()) return;
    }
    startGame();
}

void Game::nextFrame()
{
    m_multiplayerUpdater.appendUpdate("pm" + QString::number(m_level->player()->position().x)
                                      + ',' + QString::number(m_level->player()->position().y)
                                      + ',' + QString::number(m_level->player()->direction()));
    m_multiplayerUpdater.sendUpdates();
    Level *level = m_level;

    processCommands();

    level->nextFrame();
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

void Game::onRight()
{
    m_multiplayerUpdater.appendUpdate(QString::number(Qt::Key_Right));
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Right);
    }
    else
    {
        m_multiplayerUpdater.sendUpdates();
    }
}

void Game::onUp()
{
    m_multiplayerUpdater.appendUpdate(QString::number(Qt::Key_Up));
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Up);
    }
    else
    {
        m_multiplayerUpdater.sendUpdates();
    }
}

void Game::onLeft()
{
    m_multiplayerUpdater.appendUpdate(QString::number(Qt::Key_Left));
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Left);
    }
    else
    {
        m_multiplayerUpdater.sendUpdates();
    }
}

void Game::onDown()
{
    m_multiplayerUpdater.appendUpdate(QString::number(Qt::Key_Down));
    if(m_state == PlayingState)
    {
        movePlayer(GameObject::Down);
    }
    else
    {
        m_multiplayerUpdater.sendUpdates();
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
    setLevelPath(m_multiplayerUpdater.mapPath());

    setState(LobbyState);
    //emit gameReady();
}
