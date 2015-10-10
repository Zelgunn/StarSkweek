#include "tile.h"

Tile::Tile(const QString &filename)
    : m_texture(filename)
{

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

