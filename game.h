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
#include "network/multiplayerupdater.h"

class Game : public QObject
{
    Q_OBJECT
public:
    enum GameStates{
        MenuState,
        LobbyState,
        PlayingState
    };
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

    GameStates state() const;
    void setState(const GameStates &state);

public slots:
    void onGameConnected();
    void nextFrame();

signals:
    void gameReady();

private:
    GameStates m_state;
    QTimer *m_timer;
    QList<Player *> m_characters;
    Level *m_level;
    SoundPlayer m_soundPlayer;
    MultiplayerUpdater m_multiplayerUpdater;
    QStringList m_levels;
    QDomElement m_projectilesElement;
    QDomElement m_weaponsElement;
};

#endif // GAME_H
