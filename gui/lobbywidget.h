#ifndef LOBBYWIDGET_H
#define LOBBYWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QApplication>
#include <QPainter>

#include "player.h"

#include <QDebug>

#define LOBBY_PADDING 50

class LobbyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LobbyWidget(const QList<Player *> *players, QWidget *parent = 0);

    void onRight();
    void onLeft();

    int leftPortraitIndex() const;
    void setLeftPortraitIndex(int leftPortraitIndex);

    int rightPortraitIndex() const;
    void setRightPortraitIndex(int rightPortraitIndex);

protected:
    void paintEvent(QPaintEvent *);
    void paintPortrait(QPainter *painter, int panel);
    void paintThumbnails(QPainter *painter);
    void paintVersus(QPainter *painter, const QSize &size);

private:
    QList<QPixmap> m_portraits;
    QList<QPixmap> m_thumbnails;

    int m_leftPortraitIndex;
    int m_rightPortraitIndex;
    int m_selectedPortrait;

    const QList<Player *> *m_players;
    bool m_mapSelected;
};

#endif // LOBBYWIDGET_H
