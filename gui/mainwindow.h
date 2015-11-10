#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QDesktopWidget>
#include <QLinearGradient>
#include <QTime>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStackedWidget>

#include "game.h"
#include "tile.h"
#include "mainmenuwidget.h"
#include "lobbywidget.h"
#include "gamewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QStackedWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onGameReady();
    void onRight();
    void onUp();
    void onLeft();
    void onDown();
    void onEnter();
    void onBackpace();
    void onLightMeter(int value);

    void onSpinOptionChanged(const QString &name, int value);

    void onHostGame();
    void onLocalGame();
    void onIPGame(const QString &ip);

    void onGameStateChanged(Game::GameStates state);

protected:
    void paintEvent(QPaintEvent *);

    void paintBackground(QPainter *painter);
    void paintBackgroundLaser(QPainter *painter);

    void checkFullscreen();

private:
    Game m_game;
    QTimer *m_timer;
    MainMenuWidget *m_menuWidget;
    LobbyWidget *m_lobbyWidget;
    GameWidget *m_gameWidget;
    QMediaPlayer *m_musicPlayer;
    QMediaPlaylist *m_playlist;
    ArduinoHandler *m_arduinoHandler;
};

#endif // MAINWINDOW_H
