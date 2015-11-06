#ifndef LOBBYWIDGET_H
#define LOBBYWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QApplication>
#include <QPainter>
#include <QDirIterator>
#include <QFileInfo>
#include <QFile>

#include "game.h"

#include <QDebug>

#define LOBBY_PADDING 50

class LobbyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LobbyWidget(Game *game, QWidget *parent = 0);

    void onRight();
    void onLeft();
    void onUp();
    void onDown();
    void onEnter();
    void onBackspace();

    void selectNextChar(int player = 0);
    void selectPreviousChar(int player = 0);
    int selectedChar(int player = 0);

    int mapChoosen() const;
    bool hasChoosenMap() const;
    QString choosenMapName() const;
    QString choosenMapPath() const;

    void setMapChoosen(int mapChoosen);

protected:
    void paintEvent(QPaintEvent *);

    void paintMapList(QPainter *painter);
    void paintMapPreview(QPainter *painter);

    void paintServersPreview(QPainter *painter);

    void paintPortrait(QPainter *painter, int panel);
    void paintThumbnails(QPainter *painter);
    void paintVersus(QPainter *painter, const QSize &size);

    void loadMaps();
    void loadSingleMap(const QString &mapPath);
    void loadMap(const QDomElement &element, const QSize &tileSize, const QList<QPixmap> &textures, const QString &texturesIndexes);

private:
    QList<QPixmap> m_portraits;
    QList<QPixmap> m_thumbnails;

    int m_selectedMap;

    Game *m_game;

    int m_mapChoosen;
    QList<QPixmap> m_maps;
    QStringList m_mapNames;
    QStringList m_mapPaths;
};

#endif // LOBBYWIDGET_H
