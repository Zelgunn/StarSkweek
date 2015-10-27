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

namespace Ui {
class MainWindow;
}

class MainWindow : public QStackedWidget
{
    Q_OBJECT

public:
    enum WidgetsID{
        MenuWidget,
        GraphicsMenuWidget,
        AudioMenuWidget,
        ControlsMenuWidget,
        TutorialMenuWidget,
        LobbyWidget,
        GameWidget
    };
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

protected:
    void paintEvent(QPaintEvent *);

    void paintLobby(QPainter *painter);

    void paintGame(QPainter *painter);
    void paintBackground(QPainter *painter);
    void paintBackgroundLaser(QPainter *painter);
    void paintMap(QPainter *painter);
    void paintPlayer(QPainter *painter);
    void paintWaitingSign(QPainter *painter);
    void paintProjectiles(QPainter *painter);
    void paintHUD(QPainter *painter);

    void movePlayer(GameObject::Directions direction);

    QPoint toMap(Point p);
    QPoint relativePosition(Point p, QSize size = QSize(0,0));

private:
    Game m_game;
    QTimer *m_timer;
    QSize m_screenDim;
    MainMenuWidget *m_menuWidget;
    QMediaPlayer *m_musicPlayer;
    QMediaPlaylist *m_playlist;
};

#endif // MAINWINDOW_H
