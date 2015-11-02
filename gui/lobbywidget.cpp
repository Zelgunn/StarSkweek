#include "lobbywidget.h"

LobbyWidget::LobbyWidget(const QList<Player *> *players, QWidget *parent)
    : QWidget(parent), m_leftPortraitIndex(1), m_rightPortraitIndex(0), m_selectedPortrait(m_leftPortraitIndex), m_players(players), m_mapSelected(false)
{
    Player *player;

    QString dir = QApplication::applicationDirPath();

    for(int i=0; i<m_players->size(); i++)
    {
        player = m_players->at(i);
        m_portraits.append(player->portrait());
        m_thumbnails.append(player->thumbnail());
    }

    m_portraits.append(QPixmap(dir + "/images/Portrait_random.png"));
    m_thumbnails.append(QPixmap(dir + "/images/TN_random.png"));
}

void LobbyWidget::onRight()
{
    m_leftPortraitIndex = (m_leftPortraitIndex + 1)%m_portraits.size();
}

void LobbyWidget::onLeft()
{
    m_leftPortraitIndex--;
    if(m_leftPortraitIndex < 0)
        m_leftPortraitIndex = m_portraits.size() - 1;
}

void LobbyWidget::paintEvent(QPaintEvent *)
{
    // Dessiner bande de personnages

    QPainter painter(this);

    paintPortrait(&painter, 0);
    paintPortrait(&painter, 1);
    paintThumbnails(&painter);

    painter.end();
}

void LobbyWidget::paintPortrait(QPainter *painter, int panel)
{
    int portraitIndex;
    if(panel == 0)
        portraitIndex = m_leftPortraitIndex;
    else
        portraitIndex = m_rightPortraitIndex;

    QPoint leftTopCorner(LOBBY_PADDING,LOBBY_PADDING);
    if(panel != 0)
    {
        leftTopCorner.setX(width()/2);
    }

    int w = width()/2 - LOBBY_PADDING * 2;
    int h = height() * 4 / 5 - LOBBY_PADDING * 2;


    if(w > (h * 100000/141414))
        w = (h * 100000/141414);
    else if(w < (h * 100000/141414))
        h = w * 141414/100000;

    leftTopCorner += QPoint((width()/2 - 50 - w)/2, 0);
    QRect panelRect(leftTopCorner, QSize(w, h));
    if((portraitIndex >= 0) && (portraitIndex < m_portraits.size()))
    {
        painter->drawPixmap(panelRect, m_portraits.at(portraitIndex));
    }

    painter->setPen(QPen(QColor(255,255,255)));
    painter->drawRect(panelRect);

    paintVersus(painter, QSize((width()/2 - 50 - w)*9/10, (width()/2 - 50 - w)*9/10));
}

void LobbyWidget::paintThumbnails(QPainter *painter)
{
    QRect panelRect(LOBBY_PADDING, height() * 4/5, width() - 2*LOBBY_PADDING, height()/5 - LOBBY_PADDING);

    painter->setPen(QPen(QColor(255,255,255)));

    QRect thumbnailRect(0,0,height()/8,height()/8);
    thumbnailRect.moveCenter(panelRect.center());

    QRect centralthumbnailRect(thumbnailRect);
    centralthumbnailRect.moveTop(centralthumbnailRect.top() -height()/48);
    centralthumbnailRect.moveLeft(centralthumbnailRect.left() -height()/48);
    centralthumbnailRect.setWidth(centralthumbnailRect.width() + height()/24);
    centralthumbnailRect.setHeight(centralthumbnailRect.height() + height()/24);

    QPixmap thumbnail;
    QRect tmpRect;
    for(int i=0; i<m_thumbnails.size(); i++)
    {
        thumbnail = m_thumbnails.at(i);
        if(i == m_leftPortraitIndex)
        {
            painter->drawPixmap(centralthumbnailRect, thumbnail);
            painter->drawRect(centralthumbnailRect);
        }
        else
        {
            tmpRect = thumbnailRect;
            tmpRect.moveCenter(tmpRect.center() + QPoint(height()/6 * (i - m_leftPortraitIndex), 0));

            painter->drawPixmap(tmpRect, thumbnail);
            if(i==m_rightPortraitIndex)
            {
                painter->setBrush(QBrush(QColor(10,0,0,150)));
                painter->setPen(QPen(QColor(255,0,0)));
                painter->drawRect(tmpRect);
                painter->setPen(QPen(QColor(255,255,255)));
            }
        }
    }
}

void LobbyWidget::paintVersus(QPainter *painter, const QSize &size)
{
    static QPixmap versus = QPixmap(QApplication::applicationDirPath() + "/images/Versus.png").scaled(200,200);
    static QSize storedSize = QSize(200,200);
    if(size != storedSize)
    {
        if((size.width() > storedSize.width())||
                (size.height() > storedSize.height()))
        {
            versus = QPixmap(QApplication::applicationDirPath() + "/images/Versus.png").scaled(size);
        }
        else
        {
            versus = versus.scaled(size);
        }
        storedSize = size;
    }
    QRect rect;
    rect.setSize(size);
    rect.moveCenter(QPoint(width()/2, height()/2));

    painter->drawPixmap(rect.center() - QPoint(versus.width()/2, versus.height()/2), versus);
}

int LobbyWidget::rightPortraitIndex() const
{
    return m_rightPortraitIndex;
}

void LobbyWidget::setRightPortraitIndex(int rightPortraitIndex)
{
    m_rightPortraitIndex = rightPortraitIndex;
}

int LobbyWidget::leftPortraitIndex() const
{
    return m_leftPortraitIndex;
}

void LobbyWidget::setLeftPortraitIndex(int leftPortraitIndex)
{
    m_leftPortraitIndex = leftPortraitIndex;
}


