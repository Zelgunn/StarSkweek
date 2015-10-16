#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPixmap>

class Animation
{
public:
    Animation();

    QPixmap nextFrame();

private:
    int m_frame;
    QList<QPixmap> m_frames;
};

#endif // ANIMATION_H
