#include "mainmenuwidget.h"

MainMenuWidget::MainMenuWidget(QWidget *parent)
    : QWidget(parent), m_selectMenuFrame(0)
{
    // Menu
    QFile file(":/xml/xml/menus.xml");
    file.open(QIODevice::ReadOnly);

    QDomDocument dom;
    dom.setContent(&file);

    m_menu = new Menu(dom.documentElement());
}

void MainMenuWidget::previousMenu()
{
    if(m_menu->parent() != Q_NULLPTR)
    {
        m_menu = m_menu->parent();
        m_selectMenuFrame = -width()/2;
    }
}

void MainMenuWidget::selectMenu()
{
    if(m_selectMenuFrame != 0)
    {
        m_selectMenuFrame = 0;
        return;
    }

    Menu *selectedMenu = m_menu->menuAt(m_menu->selectedMenu());

    if(selectedMenu != Q_NULLPTR)
    {
        if(selectedMenu == m_menu->parent())
        {
            m_selectMenuFrame = -width()/2;
        }
        else
        {
            m_selectMenuFrame = width()/2;
        }
        m_menu = selectedMenu;
    }
    if(m_menu->isExitMenu()) emit onExit();
//    if(m_menu->name() == "Local")
    //        emit onLocalSelected;
}

void MainMenuWidget::aboveMenu()
{
    m_menu->selectAboveMenu();
}

void MainMenuWidget::belowMenu()
{
    m_menu->selectBelowMenu();
}

void MainMenuWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(QColor(255,255,255,100)));
    painter.setPen(QPen(QColor(255,255,255)));
    paintMenu(&painter);

    painter.end();
}

void MainMenuWidget::paintMenu(QPainter *painter)
{
    if(m_selectMenuFrame > 0)
    {
        m_selectMenuFrame = m_selectMenuFrame - 20;
        if(m_selectMenuFrame < 0) m_selectMenuFrame = 0;
    }

    if(m_selectMenuFrame < 0)
    {
        m_selectMenuFrame = m_selectMenuFrame + 20;
        if(m_selectMenuFrame > 0) m_selectMenuFrame = 0;
    }

    paintMenu(painter, m_menu, false);
    if(m_selectMenuFrame > 0)
    {
        paintMenu(painter, m_menu->parent(), true);
    }
    else if(m_selectMenuFrame < 0)
    {
        paintMenu(painter, m_menu->menuAt(m_menu->selectedMenu()), true);
    }
}

void MainMenuWidget::paintMenu(QPainter *painter, Menu *menu, bool offset)
{
    QStringList names = menu->menusNames();

    QPoint center(0, height()/2);

    if(!offset)
    {
        center += QPoint(width()/2, 0);
        QPoint nameCenter(center.x(), height()/8);
        paintTextMenu(painter, 42, menu->name(), nameCenter);
    }
    else if(m_selectMenuFrame < 0)
    {
        center += QPoint(width(), 0);
    }
    center += QPoint(m_selectMenuFrame, 0);

    if(names.size() == 0) return;

    QRect selectionRect, tmp;
    QPoint selectedCenter;

    for(int i=0; i<names.size(); i++)
    {
        tmp = paintTextMenu(painter, 20, names.at(i), center);
        if(tmp.width() > selectionRect.width())
            selectionRect = tmp;

        if(i == (int)menu->selectedMenu())
        {
            selectedCenter = center;
        }

        if(!offset)
        {
            center += QPoint(m_selectMenuFrame*(names.size())/(i+10), 60);
        }
        else
        {
            center += QPoint(0, 60);
        }
    }

    selectionRect.setWidth(selectionRect.width() + 20);
    selectionRect.setHeight(selectionRect.height() + 5);

    selectionRect.moveCenter(selectedCenter);
    painter->drawRect(selectionRect);
}

QRect MainMenuWidget::paintTextMenu(QPainter *painter, int fontSize, const QString &text, const QPoint &center)
{
    QFont font("Times", fontSize, QFont::Bold);
    painter->setFont(font);

    QFontMetrics metrics(font);
    QRect boundingRect = metrics.boundingRect(text);
    boundingRect.moveCenter(center);

    painter->drawText(boundingRect, Qt::AlignCenter, text);

    return boundingRect;
}
