#include "windowappearance.h"

WindowAppearance::WindowAppearance()
{

}

void WindowAppearance::setAppearance(uint totalWidth, uint totalHeight, uint tileCountWidth, uint tileCountHeight)
{
//    m_tileWidth = totalWidth / tileCountWidth;
//    m_tileHeight = totalHeight / tileCountHeight;
    m_tileWidth = 200;
    m_tileHeight = 50;

    if(((qreal)m_tileWidth / PHI) > (qreal)m_tileHeight)
        m_tileWidth = (uint)((qreal)m_tileHeight * PHI);

    if(((qreal)m_tileWidth / PHI) < (qreal)m_tileHeight)
        m_tileHeight = (uint)((qreal) m_tileWidth / PHI);

    m_width = m_tileWidth * tileCountWidth;
    m_height = m_tileHeight * tileCountHeight;

//    m_dx = totalWidth - m_width;
//    m_dy = totalHeight - m_height;
    m_dx = 0;
    m_dy = 0;
}

uint WindowAppearance::width() const
{
    return m_width;
}

uint WindowAppearance::height() const
{
    return m_height;
}

uint WindowAppearance::tileWidth() const
{
    return m_tileWidth;
}

uint WindowAppearance::tileHeight() const
{
    return m_tileHeight;
}

uint WindowAppearance::dx() const
{
    return m_dx;
}

uint WindowAppearance::dy() const
{
    return m_dy;
}
