#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPixmap>
#include <QPainter>
#include <QRadialGradient>

class Animation
{
public:
    Animation();
    Animation(const QPoint &position);
    virtual ~Animation();

    virtual QPixmap nextFrame();
    virtual bool animationDone() const;

    QPoint position() const;
    void setPosition(const QPoint &position);

protected:
    int m_frame;
    QList<QPixmap> m_frames;
    QPoint m_position;
};

#endif // ANIMATION_H
