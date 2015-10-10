#ifndef TILE_H
#define TILE_H

#include <QImage>

class Tile
{
public:
    Tile(const QString &filename);
    QImage texture() const;
    QImage resizedTexture(QSize size);
    QImage resizedTexture(uint width, uint height);

private:
    QImage m_texture;
    QImage m_resizedTexture;
    QSize m_size;
};

#endif // TILE_H
