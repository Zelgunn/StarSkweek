#include "tile.h"

Tile Tile::s_tiles[TILES_COUNT];


Tile::Tile(Tile::TileType type)
{
    m_type = type;

    QString filename;

    switch (m_type) {
    case Player1Tile:
        filename.append("/images/Tile_pink2.png");
        break;
    case Player2Tile:
        filename.append("/images/Tile_blue2.png");
        break;
    case Void:
        filename.append("/images/void.png");
        break;
    case ArrowTile:
        filename.append("/images/void.png");
        break;
    default:
        m_type = InvalidTile;
        break;
    }

    if(m_type != InvalidTile)
    {
        filename.insert(0,QApplication::applicationDirPath());
        m_texture = QImage(filename);
    }
}

Tile::Tile(const QString &filename)
    : m_texture(filename)
{

}

Tile *Tile::tile(Tile::TileType type)
{
    if(s_tiles[type].type() == InvalidTile)
        s_tiles[type] = Tile(type);
    return &s_tiles[type];
}

Tile::TileType Tile::type() const
{
    return m_type;
}

QImage Tile::texture() const
{
    return m_texture;
}

QImage Tile::resizedTexture(QSize size)
{
    if((m_resizedTexture.isNull()) || (m_resizedTexture.size() != size))
        m_resizedTexture = m_texture.scaled(size);

    return m_resizedTexture;
}

QImage Tile::resizedTexture(uint width, uint height)
{
    QSize size(width, height);
    if((m_resizedTexture.isNull()) || (m_resizedTexture.size() != size))
        m_resizedTexture = m_texture.scaled(size);

    return m_resizedTexture;
}

