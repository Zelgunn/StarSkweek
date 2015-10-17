#include "tile.h"

Tile Tile::s_tiles[TILES_COUNT];


Tile::Tile(Tile::TileType type)
{
    m_type = type;

    QString filename;

    switch (m_type) {
    case Player1Tile:
        filename.append("/images/Tile_pink.png");
        break;
    case Player2Tile:
        filename.append("/images/Tile_blue.png");
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

    m_resizedTexture = Q_NULLPTR;
    m_texture = Q_NULLPTR;

    if(m_type != InvalidTile)
    {
        filename.insert(0,QApplication::applicationDirPath());
        m_texture = new QPixmap(filename);
    }
}

Tile::Tile(const QString &filename)
{
    m_texture = new QPixmap(filename);
    m_resizedTexture = Q_NULLPTR;
}

Tile *Tile::tile(Tile::TileType type)
{
    if(s_tiles[type].type() == InvalidTile)
    {
        s_tiles[type] = Tile(type);
    }
    return &s_tiles[type];
}

Tile::TileType Tile::type() const
{
    return m_type;
}

QPixmap Tile::texture() const
{
    if(!m_texture)
        return QPixmap(1,1);
    return *m_texture;
}

QPixmap Tile::resizedTexture(QSize size)
{
    if(!m_resizedTexture)
    {
        m_resizedTexture = new QPixmap;
    }

    if(m_resizedTexture->size() != size)
    {
        *m_resizedTexture = m_texture->scaled(size);
    }

    return *m_resizedTexture;
}

QPixmap Tile::resizedTexture(uint width, uint height)
{
    QSize size(width, height);
    if(!m_resizedTexture)
    {
        m_resizedTexture = new QPixmap;
    }

    if(m_resizedTexture->size() != size)
    {
        *m_resizedTexture = m_texture->scaled(size);
    }


    return *m_resizedTexture;
}

