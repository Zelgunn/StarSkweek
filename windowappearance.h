#ifndef WINDOWAPPEARANCE_H
#define WINDOWAPPEARANCE_H

#include <QtGlobal>
#define PHI 1.61803398875

class WindowAppearance
{
public:
    WindowAppearance();

    void setAppearance(uint totalWidth, uint totalHeight, uint tileCountWidth, uint tileCountHeight);

    uint width() const;
    uint height() const;
    uint tileWidth() const;
    uint tileHeight() const;
    uint dx() const;
    uint dy() const;

private:
    uint m_width;
    uint m_height;
    uint m_tileWidth;
    uint m_tileHeight;
    uint m_dx;
    uint m_dy;
};

#endif // WINDOWAPPEARANCE_H
