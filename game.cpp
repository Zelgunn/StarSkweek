#include "game.h"

Game::Game() :
    m_state(MenuState), m_timer(Q_NULLPTR)
{
    QString dir = QApplication::applicationDirPath() + "/xml/levels";
    QDirIterator dirIt(dir ,QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
            if (QFileInfo(dirIt.filePath()).suffix() == "xml")
                m_levels.append(dirIt.filePath());
    }

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
            m_characters.append(new Player(elem));
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

const QList<Player *> *Game::players() const
{
    return &m_characters;
}

void Game::movePlayer(GameObject::Directions direction)
{
    if(!m_timer->isActive()) return;

    m_level->setPlayerDirection(0, direction);
}

void Game::player2Command(QString command)
{
    char firstChar = command.at(0).toLatin1();

    switch (firstChar) {
    case 'm':
        movePlayer2(command);
        break;
    case 's':
        playerFires(1);
    default:
        break;
    }
}

void Game::movePlayer2(QString command)
{
    command = command.remove(0, 1);
    int x = command.section(',',0,0).toInt();
    int y = command.section(',',1,1).toInt();
    int dir = command.section(',',-1,-1).toInt();

    m_level->setPlayerPosition(1, x, y, (GameObject::Directions) dir);
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
    m_timer->start(16);
    setState(PlayingState);
}

bool Game::isStarted() const
{
    return m_timer->isActive();
}

void Game::loadLevel(int level)
{
    loadLevel(m_levels.at(level));
}

void Game::loadLevel(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    QDomDocument dom;
    dom.setContent(&file);

    QDomElement elem = dom.documentElement();
    elem.appendChild(m_projectilesElement);

    m_level = new Level(elem, &m_characters);
}

void Game::nextFrame()
{
    m_multiplayerUpdater.appendUpdate("pm" + QString::number(m_level->player()->position().x) + ',' + QString::number(m_level->player()->position().y));
    m_multiplayerUpdater.sendUpdates();
    Level *level = m_level;
    QStringList updates = m_multiplayerUpdater.receivedUpdates();
    QString update;
    char firstChar;

    for(int i=0; i<updates.size(); i++)
    {
        update = updates.at(i);
        firstChar = update.at(0).toLatin1();

        switch (firstChar) {
        case 'p':
            update = update.remove(0,1);
            player2Command(update);
            break;
        }
    }

    level->nextFrame();
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

void Game::startHost()
{
    m_multiplayerUpdater.startHost();
}

void Game::lookForLocalHost()
{
    m_multiplayerUpdater.lookForLocalHost();
}

void Game::connectToIP(const QString &ip)
{
    m_multiplayerUpdater.connectToIP(ip);
}

void Game::onGameConnected()
{
    if(m_multiplayerUpdater.isFirst())
    {
        m_level->setMyPlayer(0);
    }
    else
    {
        m_level->setMyPlayer(1);
    }

    setState(LobbyState);
    //emit gameReady();
}
