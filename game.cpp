#include "game.h"

Game::Game() :
    m_lifes(0), m_score(0), m_timer(NULL)
{
    m_levels.append(new Level);

    m_timer = new QTimer(this);
    QObject::connect(&m_multiplayerUpdater, SIGNAL(gameConnected()), this, SLOT(onGameConnected()));
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
}

const Level *Game::level(int i)
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < m_levels.size());
    return m_levels.at(i);
}

void Game::movePlayer(GameObject::Directions direction)
{
    if(!m_timer->isActive()) return;
    Level *level = m_levels.first();

    level->setPlayerDirection(0, direction);
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
    m_levels.first()->setPlayerDirection(1, dir);
}

void Game::playerFires(int playerID)
{
    Level *level = m_levels.first();
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

void Game::nextFrame()
{
    Level *level = m_levels.first();
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
        m_levels.first()->setMyPlayer(0);
    }
    else
    {
        m_levels.first()->setMyPlayer(1);
    }
    emit gameReady();
}
