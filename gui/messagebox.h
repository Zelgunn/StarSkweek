#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QWidget>
#include <QPainter>

class MessageBox
{
public:
    MessageBox(QWidget *device);

    void drawNoProfileMessage();

    void moveCursor(int delta);
    void changeLineChar(int delta);
    void removeChar();
    bool isProfileCorrect() const;

    QString profile() const;

private:
    QWidget *m_device;
    QString m_profile;
    int m_cursor;
};

#endif // MESSAGEBOX_H
