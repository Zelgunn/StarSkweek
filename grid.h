#ifndef GRID_H
#define GRID_H

#include <QDomElement>

class Grid
{
public:
    Grid();
    Grid(QDomElement elem);

    int tileAt(uint x, uint y) const;
    void setTileAt(uint x, uint y, int value);
    uint width() const;
    uint height() const;

private:
    uint m_width;
    uint m_height;
    int **m_values;
};

#endif // GRID_H
