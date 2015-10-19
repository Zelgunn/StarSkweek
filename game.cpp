#include "game.h"

Game::Game() :
    m_lifes(0), m_score(0), m_timer(NULL)
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
            m_characters.append(Player(elem));
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

const Level *Game::level()
{
    return m_level;
}

void Game::movePlayer(GameObject::Directions direction)
{
    if(!m_timer->isActive()) return;

    m_level->setPlayerDirection(0, direction);
    QString update = "pm";  // p = player, m = move
    update.append('0' + direction);
    m_multiplayerUpdater.sendUpdate(update);
}

void Game::player2Command(QString command)
{
    char firstChar = command.at(0).toLatin1();

    switch (firstChar) {
    case 'm':
        movePlayer2(command.at(1).toLatin1());
        break;
    case 's':
        playerFires(1);
    default:
        break;
    }
}

void Game::movePlayer2(char direction)
{
    GameObject::Directions dir = (GameObject::Directions) (char)(direction - '0');
    m_level->setPlayerDirection(1, dir);
}

void Game::playerFires(int playerID)
{
    Level *level = m_level;
    if(level->playerFires(playerID))
    {
        if(playerID == 0)
        {
            QString update = "ps";  // p = player, s = shoot
            m_multiplayerUpdater.sendUpdate(update);
        }
    }
}

void Game::startGame()
{
    m_timer->start(16);
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
    emit gameReady();
}
