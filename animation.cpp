#include "animation.h"

Animation::Animation() :
    m_frame(0)
{

}

QPixmap Animation::nextFrame()
{
    if(m_frame < m_frames.size() - 1)
    {
        m_frame ++;
    }
    return m_frames.at(m_frame);
}

