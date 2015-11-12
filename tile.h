#ifndef TILE_H
#define TILE_H

#include <QApplication>
#include <QPixmap>

class Tile
{
public:
    enum TileType{
                  InvalidTile = -1,
                  Void,
                  Player1Tile,
                  Player2Tile,
                  ArrowTileRight,
                  ArrowTileUp,
                  ArrowTileLeft,
                  ArrowTileDown,
                  UpstairsTile,
                  BombTile,
                  ExplodingTile,
                  TypeCount
                 };

    Tile();
    Tile(const QString &filename, const QSize &size, TileType type);

    void resize(QSize size);
    TileType type() const;
    QPixmap texture() const;

private:
    TileType m_type;
    QPixmap m_texture;
};

#endif // TILE_H
