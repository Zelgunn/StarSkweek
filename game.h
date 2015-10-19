#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QShortcut>
#include <QDomDocument>
#include <QDirIterator>
#include <QFileInfo>

#include "level.h"
#include "soundplayer.h"
#include "multiplayerupdater.h"

class Game : public QObject
{
    Q_OBJECT
public:
    Game();

    const Level *level();
    void movePlayer(GameObject::Directions direction);
    void player2Command(QString command);
    void movePlayer2(char direction);

    void playerFires(int playerID);

    void startGame();
    bool isStarted() const;

    void loadLevel(int level);
    void loadLevel(const QString &filename);

public slots:
    void onGameConnected();
    void nextFrame();

signals:
    void gameReady();

private:
    int m_lifes;
    int m_score;

    QTimer *m_timer;
    QList<Player> m_characters;
    Level *m_level;
    SoundPlayer m_soundPlayer;
    MultiplayerUpdater m_multiplayerUpdater;
    QStringList m_levels;
};

#endif // GAME_H
