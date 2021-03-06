#include "lobbywidget.h"

LobbyWidget::LobbyWidget(Game *game, QWidget *parent)
    : QWidget(parent), m_selectedMap(0), m_game(game), m_mapChoosen(-1)
{
    // Chargement des portraits et miniatures.
    const QList<Player *> *players = m_game->players();
    const Player *player;
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
    QObject::connect(m_game, SIGNAL(newHostFound()), this, SLOT(loadNewHost()));
}

void LobbyWidget::onRight()
{
    if(hasChoosenMap())
    {
        selectNextChar();
        m_game->onRight();
    }
}

void LobbyWidget::onLeft()
{
    if(hasChoosenMap())
    {
        selectPreviousChar();
        m_game->onLeft();
    }
}

void LobbyWidget::onUp()
{
    if(hasChoosenMap())
    {
        selectPreviousChar();
        m_game->onLeft();
    }
    else
    {
        int count = mapCount();
        if(count > 0)
        {
            m_selectedMap = (m_selectedMap + 1)%count;
        }
    }
}

void LobbyWidget::onDown()
{
    if(hasChoosenMap())
    {
        selectNextChar();
        m_game->onRight();
    }
    else
    {
        int count = mapCount();
        if(count > 0)
        {
            m_selectedMap--;
            if(m_selectedMap < 0)
                m_selectedMap = count - 1;
        }
    }
}

void LobbyWidget::onEnter()
{
    if(m_mapChoosen < 0)
    {
        if(mapCount() > 0)
        {
            m_mapChoosen = m_selectedMap;
            m_game->setLevelPath(choosenMapPath());
            if(m_game->hosting())
            {
                m_game->startHost(true);
            }
            else
            {
                m_game->connectToPlayer2();
            }
        }
    }
    else
    {
        m_game->setPlayerReady(!m_game->isPlayerReady());
    }
}

void LobbyWidget::onBackspace()
{
    if(hasChoosenMap())
    {
        m_game->startHost(false);
        m_mapChoosen = -1;
    }
}

void LobbyWidget::selectNextChar(int player)
{
    if(m_game->isPlayerReady()) return;
    int index = selectedChar(player);
    index = (index + 1)%m_portraits.size();
    m_game->setPlayerChar(index, player);
}

void LobbyWidget::selectPreviousChar(int player)
{
    if(m_game->isPlayerReady()) return;
    int index = selectedChar(player);
    index--;
    if(index < 0)
        index = m_portraits.size() - 1;
    m_game->setPlayerChar(index, player);
}

int LobbyWidget::selectedChar(int player)
{
    return m_game->playerChar(player);
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

    foreach (QString level, m_mapPaths)
    {
        m_maps.append(loadSingleMap(level));
    }
}

int LobbyWidget::mapCount() const
{
    if(m_game->hosting())
        return m_maps.size();
    else
        return m_localMaps.size();
}

void LobbyWidget::loadNewHost()
{
    QString hostMapPath = m_game->levelPath();

    // Pour le moment, un seul hôte disponible :/
    m_localMapNames.clear();
    m_localMapNames.append(hostMapPath);

    m_localMaps.clear();
    m_localMaps.append(loadSingleMap(hostMapPath));
}

void LobbyWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if(!hasChoosenMap())
    {
        if(m_game->hosting())
        {
            paintMapList(&painter);
            paintMapPreview(&painter);
        }
        else
        {
            paintServersPreview(&painter);
        }
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
    QPixmap thumbnail;
    static int storedWidth = width();
    static int storedHeight = height();

    if((storedWidth != width()) || (storedHeight != height()))
    {
        storedWidth = width();
        storedHeight = height();
        thumbnails.clear();
    }

    int thumbnailWidth = qMin(storedHeight * 2/5, storedWidth/6 - LOBBY_PADDING/2);
    int thumbnailHeight = thumbnailWidth/2;
    if(thumbnails.isEmpty())
    {
        for(int i=0; i<m_maps.size(); i++)
        {
            thumbnail = m_maps.at(i).scaledToWidth(thumbnailWidth, Qt::SmoothTransformation);
            if(thumbnail.height() > thumbnailHeight) thumbnail = thumbnail.scaledToHeight(thumbnailHeight, Qt::SmoothTransformation);
            thumbnails.append(thumbnail);
        }
    }

    QFont font("Times", 16);
    painter->setFont(font);
    int h = LOBBY_PADDING + 1;
    for(int i=0; i<thumbnails.size(); i++)
    {
        thumbnail = thumbnails.at(i);
        painter->drawPixmap(LOBBY_PADDING + (thumbnailWidth - thumbnail.width())/2, h + (thumbnailHeight - thumbnail.height())/2, thumbnail);
        if(i==m_selectedMap)
        {
            painter->setPen(QPen(QColor(75,125,255)));
        }
        else
        {
            painter->setPen(QPen(QColor(255,255,255)));
        }
        painter->drawRect(QRect(QPoint(LOBBY_PADDING + 1, h), QSize(thumbnailWidth, thumbnailHeight)));
        painter->drawText(QRect(QPoint(LOBBY_PADDING + 1 + thumbnailWidth, h), QSize(thumbnailWidth, thumbnailHeight)),
                          Qt::AlignCenter, m_mapNames.at(i));
        h += thumbnailHeight + 1;
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
    corner += QPoint((container.width() - previews.at(m_selectedMap).width())/2,
                     (container.height() - previews.at(m_selectedMap).height())/2);
    painter->drawPixmap(corner, previews.at(m_selectedMap));

    painter->setPen(QPen(QColor(255,255,255)));
    painter->drawRect(container);
}

void LobbyWidget::paintServersPreview(QPainter *painter)
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
        for(int i=0; i<m_localMaps.size(); i++)
        {
            thumbnails.append(m_localMaps.at(i).scaledToWidth((width()/3 - LOBBY_PADDING)/2, Qt::SmoothTransformation));
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
        if(i==m_selectedMap)
        {
            painter->setPen(QPen(QColor(100,100,255)));
        }
        else
        {
            painter->setPen(QPen(QColor(255,255,255)));
        }
        painter->drawRect(QRect(QPoint(LOBBY_PADDING + 1, h), QSize(width() - 2*LOBBY_PADDING,thumbnail.height())));
        painter->drawText(QRect(QPoint(LOBBY_PADDING + 2 + thumbnail.width(), h),
                                thumbnail.size()), Qt::AlignCenter, m_mapNames.at(i));
        h += thumbnail.height() + 1;
    }
}

void LobbyWidget::paintPortrait(QPainter *painter, int panel)
{
    int portraitIndex = selectedChar(panel);

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
    painter->setBrush(Qt::NoBrush);

    QRect thumbnailRect(0,0,height()/8,height()/8);
    thumbnailRect.moveCenter(panelRect.center());

    QRect centralthumbnailRect(thumbnailRect);
    centralthumbnailRect.moveTop(centralthumbnailRect.top() -height()/48);
    centralthumbnailRect.moveLeft(centralthumbnailRect.left() -height()/48);
    centralthumbnailRect.setWidth(centralthumbnailRect.width() + height()/24);
    centralthumbnailRect.setHeight(centralthumbnailRect.height() + height()/24);

    QPixmap thumbnail;
    QRect tmpRect;
    QPen pen;
    for(int i=0; i<m_thumbnails.size(); i++)
    {
        thumbnail = m_thumbnails.at(i);
        painter->setBrush(Qt::NoBrush);

        if(i == selectedChar())
        {
            tmpRect = centralthumbnailRect;
            pen.setColor(QColor(255,255,255));

            if(m_game->isPlayerReady())
            {
                pen.setWidth(3);
            }
        }
        else
        {
            tmpRect = thumbnailRect;
            tmpRect.moveCenter(tmpRect.center() + QPoint(height()/6 * (i - selectedChar()), 0));
            pen.setColor(QColor(100, 255, 100));
        }

        painter->drawPixmap(tmpRect, thumbnail);

        // Si le personnage est sélectionné par l'autre joueur
        if(i == selectedChar(1))
        {
            if(m_game->isPlayerReady(1))
            {
                painter->setBrush(QBrush(QColor(255,0,0,25)));
                pen.setColor(QColor(255, 0, 0));
            }
            else
            {
                painter->setBrush(Qt::NoBrush);
            }
        }

        painter->setPen(pen);
        painter->drawRect(tmpRect);
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

QPixmap LobbyWidget::loadSingleMap(const QString &mapPath)
{
    QString dir = QApplication::applicationDirPath();
    QDomDocument dom;
    QDomElement elem, mapElem;
    QDomNode node;
    QSize size;
    QList<QPixmap> textures;
    QString texturesIndexes;
    QFile file(mapPath);

    if(!file.open(QIODevice::ReadOnly))
        return QPixmap();

    if(!dom.setContent(&file))
        return QPixmap();
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

    file.close();
    return loadMap(mapElem, size, textures, texturesIndexes);
}

QPixmap LobbyWidget::loadMap(const QDomElement &element, const QSize &tileSize, const QList<QPixmap> &textures, const QString &texturesIndexes)
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

    return map;
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
    {
        if(m_game->hosting())
        {
            return m_mapPaths.at(m_mapChoosen);
        }
        else
        {
            return m_localMapNames.at(m_mapChoosen);
        }
    }
    else
        return QString();
}
