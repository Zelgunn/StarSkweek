#ifndef ANIMATION_H
#define ANIMATION_H

#include <QImage>

class Animation
{
public:
    Animation();

private:
    uint m_frame;
    QList<QImage> m_frames;
};

#endif // ANIMATION_H
