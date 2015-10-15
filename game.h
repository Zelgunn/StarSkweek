#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTimer>
#include <QShortcut>

#include "level.h"
#include "soundplayer.h"
#include "multiplayerupdater.h"

class Game : public QObject
{
    Q_OBJECT
public:
    Game();

    const Level *level(int i);
    void movePlayer(GameObject::Directions direction);
    void player2Command(QString command);
    void movePlayer2(char direction);

    void playerFires(int playerID);

    void startGame();
    bool isStarted() const;

public slots:
    void onGameConnected();
    void nextFrame();

signals:
    void gameReady();

private:
    int m_lifes;
    int m_score;

    QTimer *m_timer;
    QList<Level *> m_levels;
    SoundPlayer m_soundPlayer;
    MultiplayerUpdater m_multiplayerUpdater;
};

#endif // GAME_H
