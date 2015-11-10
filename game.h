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

#define GAME_DURATION 300

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

    Level *level() const;
    const QList<Player *> *players() const;
    void movePlayer(GameObject::Directions direction);
    void processCommands();
    void playerCommand(int player, QString command);
    void movePlayer(int player, QString command);
    void activateVadorBlackStarBeam(QString command);
    void activateObiWanGhostForm();
    void sendCommands();

    void playerFires(int playerID = 0);

    void startGame();
    bool isStarted() const;
    void stopGame();
    bool isPlayerVictorious() const;
    bool isPlayerDefeated() const;

    void loadLevel(const QString &filename);

    GameStates state() const;
    void setState(const GameStates &state);
    QString levelPath() const;
    void setLevelPath(const QString &levelPath);
    QTime startTime() const;
    void setStartTime(const QTime &startTime);

    PlayerInfo *playerInfo(int player = 0) const;
    QString playerNickname(int player = 0) const;
    void setPlayerNickname(const QString &nickname, int player = 0);
    int playerChar(int player = 0) const;
    void setPlayerChar(int characterIndex, int player = 0);
    bool isPlayerReady(int player = 0) const;
    void setPlayerReady(bool ready, int player = 0);
    void selectRandomPlayer();

    void startHost(bool enable = true);
    void lookForLocalHost();
    void connectToIP(const QString &ip);
    bool hosting() const;
    void setHosting(bool hosting);
    void connectToPlayer2();

    void onRight();
    void onUp();
    void onLeft();
    void onDown();
    void onEnter();
    void onBackpace();

    QStringList untreatedCommands();
    void removeUntreadtedCommand(int index);

public slots:
    void onNewConnection();
    void nextFrame();

signals:
    void stateChanged(Game::GameStates state);
    void gameReady();
    void newHostFound();

private:
    GameStates m_state;
    QTimer *m_timer;
    QList<Player *> m_playersPrototypes;
    Level *m_level;
    QString m_levelPath;
    SoundPlayer m_soundPlayer;
    MultiplayerUpdater m_multiplayerUpdater;
    QStringList m_levels;
    QDomElement m_projectilesElement;
    QDomElement m_weaponsElement;
    QStringList m_untreatedCommands;
    bool m_hosting;
    QTime m_startTime;
};

#endif // GAME_H
