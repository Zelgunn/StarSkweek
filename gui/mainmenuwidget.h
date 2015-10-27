#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QFile>

#include <QDebug>

#include "menu.h"

class MainMenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenuWidget(QWidget *parent = 0);

    void previousMenu();
    void selectMenu();
    void aboveMenu();
    void belowMenu();

protected:
    void paintEvent(QPaintEvent *);

    void paintMenu(QPainter *painter);
    void paintMenu(QPainter *painter, Menu *menu, bool offset);
    QRect paintTextMenu(QPainter *painter, int fontSize, const QString &text, const QPoint &center);

signals:
    void onExit();

private:
    Menu *m_menu;
    int m_selectMenuFrame;
};

#endif // MAINMENUWIDGET_H
