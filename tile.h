#ifndef TILE_H
#define TILE_H

#include <QApplication>
#include <QPixmap>

#include <QDebug>

#define TILES_COUNT 4

class Tile
{
public:
    enum TileType{
                  InvalidTile = -1,
                  Void,
                  Player1Tile,
                  Player2Tile,
                  ArrowTile
                 };
    Tile(TileType type = InvalidTile);
    Tile(const QString &filename);

    static Tile *tile(TileType type);

    TileType type() const;
    QPixmap texture() const;
    QPixmap resizedTexture(QSize size);
    QPixmap resizedTexture(uint width, uint height);

private:
    TileType m_type;
    QPixmap *m_texture;
    QPixmap *m_resizedTexture;
    QSize m_size;

    static Tile s_tiles[TILES_COUNT];
};

#endif // TILE_H
