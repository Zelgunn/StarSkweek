#include "lobbywidget.h"

LobbyWidget::LobbyWidget(Game *game, QWidget *parent)
    : QWidget(parent), m_leftPortraitIndex(0), m_rightPortraitIndex(0), m_selectedPortrait(0), m_game(game), m_mapChoosen(-1)
{
    // Chargement des portraits et miniatures.
    const QList<Player *> *players = m_game->players();
    Player *player;
    QString dir = QApplication::applicationDirPath();
    for(int i=0; i<players->size(); i++)
    {
        player = players->at(i);
        m_portraits.append(player->portrait());
        m_thumbnails.append(player->thumbnail());
    }

    m_portraits.append(QPixmap(dir + "/images/Portrait_random.png"));
    m_thumbnails.append(QPixmap(dir + "/images/TN_random.png"));

    loadMaps();
}

void LobbyWidget::onRight()
{
    if(m_mapChoosen < 0) return;
    m_leftPortraitIndex = (m_leftPortraitIndex + 1)%m_portraits.size();
    m_game->onRight();
}

void LobbyWidget::onLeft()
{
    if(m_mapChoosen < 0) return;
    m_leftPortraitIndex--;
    if(m_leftPortraitIndex < 0)
        m_leftPortraitIndex = m_portraits.size() - 1;
    m_game->onLeft();
}

void LobbyWidget::onUp()
{
    if(m_mapChoosen > 0) return;
    m_selectedPortrait = (m_selectedPortrait + 1)%m_maps.size();
}

void LobbyWidget::onDown()
{
    if(m_mapChoosen > 0) return;
    m_selectedPortrait--;
    if(m_selectedPortrait < 0)
        m_selectedPortrait = m_maps.size() - 1;
}

void LobbyWidget::onEnter()
{
    if(m_mapChoosen < 0)
    {
        m_mapChoosen = m_selectedPortrait;
        m_selectedPortrait = 0;
        m_game->setLevelPath(choosenMapPath());
        m_game->startHost(true);
    }
    else
    {
        m_game->setLevelPath(choosenMapPath());
        m_game->loadLevel(m_game->levelPath());
        m_game->startGame();
    }
}

void LobbyWidget::onBackspace()
{
    if(m_mapChoosen < 0) return;
    m_selectedPortrait = m_mapChoosen;
    m_mapChoosen = -1;
}

void LobbyWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    checkGameUntreatedCommands();
    if(m_mapChoosen < 0)
    {
        paintMapList(&painter);
        paintMapPreview(&painter);
    }
    else
    {
        paintPortrait(&painter, 0);
        paintPortrait(&painter, 1);
        paintThumbnails(&painter);
        m_game->processCommands();
    }

    painter.end();
}

void LobbyWidget::paintMapList(QPainter *painter)
{
    static QList<QPixmap> thumbnails;
    static int storedWidth = width();

    if(storedWidth != width())
    {
        storedWidth = width();
        thumbnails.clear();
    }

    if(thumbnails.isEmpty())
    {
        for(int i=0; i<m_maps.size(); i++)
        {
            thumbnails.append(m_maps.at(i).scaledToWidth((width()/3 - LOBBY_PADDING)/2, Qt::SmoothTransformation));
        }
    }

    QFont font("Times", 16);
    painter->setFont(font);
    QPixmap thumbnail;
    int h = LOBBY_PADDING + 1;
    for(int i=0; i<thumbnails.size(); i++)
    {
        thumbnail = thumbnails.at(i);
        painter->drawPixmap(LOBBY_PADDING, h, thumbnail);
        if(i==m_selectedPortrait)
        {
            painter->setPen(QPen(QColor(0,0,255)));
        }
        else
        {
            painter->setPen(QPen(QColor(255,255,255)));
        }
        painter->drawRect(QRect(QPoint(LOBBY_PADDING + 1, h), thumbnail.size()));
        painter->drawText(QRect(QPoint(LOBBY_PADDING + 2 + thumbnail.width(), h),
                                thumbnail.size()), Qt::AlignCenter, m_mapNames.at(i));
        h += thumbnail.height() + 1;
    }

    painter->setPen(QPen(QColor(255,255,255)));
    QRect container(LOBBY_PADDING, LOBBY_PADDING, width()/3 - LOBBY_PADDING, height() - 2*LOBBY_PADDING);
    painter->drawRect(container);
}

void LobbyWidget::paintMapPreview(QPainter *painter)
{
    static QList<QPixmap> previews;
    static int storedHeight = height();
    static int storedWidth = width();

    if(storedWidth != width())
    {
        storedWidth = width();
        previews.clear();
    }

    if(storedHeight != height())
    {
        storedHeight = height();
        previews.clear();
    }

    QRect container(LOBBY_PADDING + width()/3, LOBBY_PADDING, width()*2/3 - 2*LOBBY_PADDING, height() - 2*LOBBY_PADDING);
    double previewRatio = (double)container.width()/(double)container.height();

    QPixmap map;
    if(previews.isEmpty())
    {
        for(int i=0; i<m_maps.size(); i++)
        {
            map = m_maps.at(i);
            double mapRatio = (double)map.width()/(double)map.height();
            if(mapRatio < previewRatio)
            {
                previews.append(m_maps.at(i).scaledToHeight(container.height(), Qt::SmoothTransformation));
            }
            else
            {
                previews.append(m_maps.at(i).scaledToWidth(container.width(), Qt::SmoothTransformation));
            }
        }
    }

    QPoint corner = container.topLeft();
    corner += QPoint((container.width() - previews.at(m_selectedPortrait).width())/2,
                     (container.height() - previews.at(m_selectedPortrait).height())/2);
    painter->drawPixmap(corner, previews.at(m_selectedPortrait));

    painter->setPen(QPen(QColor(255,255,255)));
    painter->drawRect(container);
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

void LobbyWidget::loadMaps()
{
    // Chargement des aperçus des niveaux.
    QString dir = QApplication::applicationDirPath();
    QDirIterator dirIt(dir + "/xml/levels",QDirIterator::Subdirectories);
    while (dirIt.hasNext())
    {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
            if (QFileInfo(dirIt.filePath()).suffix() == "xml")
                m_mapPaths.append(dirIt.filePath());
    }

    QFile file;
    QDomDocument dom;
    QDomElement elem, mapElem;
    QDomNode node;
    QSize size;
    QList<QPixmap> textures;
    QString texturesIndexes;

    foreach (QString level, m_mapPaths)
    {
        file.setFileName(level);
        if(!file.open(QIODevice::ReadOnly))
            continue;

        if(!dom.setContent(&file))
            continue;

        elem = dom.documentElement();
        m_mapNames.append(elem.attribute("name"));
        node = elem.firstChild();
        while(!node.isNull())
        {
            elem = node.toElement();

            if(elem.tagName() == "TileSize")
            {
                size.setHeight(elem.attribute("height").toInt());
                size.setWidth(elem.attribute("width").toInt());
            }

            if(elem.tagName() == "Grid")
            {
                mapElem = elem;
            }

            if(elem.tagName() == "Tile")
            {
                textures.append(QPixmap(dir + elem.attribute("filename")));
                texturesIndexes.append(elem.attribute("type").at(0));
            }

            node = node.nextSibling();
        }

        loadMap(mapElem, size, textures, texturesIndexes);
        file.close();
        textures.clear();
        texturesIndexes.clear();
    }
}

void LobbyWidget::loadMap(const QDomElement &element, const QSize &tileSize, const QList<QPixmap> &textures, const QString &texturesIndexes)
{
    QDomElement elem;
    QDomNode node = element.firstChild();

    QSize size;
    QString tiles;

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Size")
        {
            size.setHeight(elem.attribute("height").toInt());
            size.setWidth(elem.attribute("width").toInt());
        }
        if(elem.tagName() == "Tiles")
            tiles = elem.attribute("values");

        node = node.nextSibling();
    }

    QPixmap map(size.width() * tileSize.width(), size.height() * tileSize.height());
    QPainter painter(&map);
    for(int i = 0; i<size.width(); i++)
    {
        for(int j=0; j<size.height(); j++)
        {
            for(int t=0; t<texturesIndexes.size(); t++)
            {
                if(texturesIndexes.at(t) == tiles.at(i + j*size.width()))
                {
                    painter.drawPixmap(i*tileSize.width(), j*tileSize.height(), tileSize.width(), tileSize.height(),
                                       textures.at(t));
                    break;
                }
            }
        }
    }

    m_maps.append(map);
}

void LobbyWidget::checkGameUntreatedCommands()
{
    QStringList untreatedCommands = m_game->untreatedCommands();
    QString command;
    Qt::Key key;
    for(int i=0; i<untreatedCommands.size(); i++)
    {
        command = untreatedCommands.at(i);
        qDebug() << command << (command.toInt() == Qt::Key_Left) << (command.toInt() == Qt::Key_Right);
        key = (Qt::Key)command.toInt();
        if(key == Qt::Key_Left)
        {
            m_rightPortraitIndex --;
            if(m_rightPortraitIndex < 0) m_rightPortraitIndex = m_portraits.size() - 1;
        }

        if(key == Qt::Key_Right)
        {
            m_rightPortraitIndex = (m_rightPortraitIndex + 1)%m_portraits.size();
        }
    }
}

void LobbyWidget::setMapChoosen(int mapChoosen)
{
    m_mapChoosen = mapChoosen;
}

int LobbyWidget::mapChoosen() const
{
    return m_mapChoosen;
}

bool LobbyWidget::hasChoosenMap() const
{
    return (m_mapChoosen >= 0);
}

QString LobbyWidget::choosenMapName() const
{
    if(hasChoosenMap())
        return m_mapNames.at(m_mapChoosen);
    else
        return QString();
}

QString LobbyWidget::choosenMapPath() const
{
    if(hasChoosenMap())
        return m_mapPaths.at(m_mapChoosen);
    else
        return QString();
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
