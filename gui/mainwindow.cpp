#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QStackedWidget(parent)
{
    QTime test = QTime::currentTime();
    setWindowTitle("Super Skweek v0.1 (alpha)");
    setWindowState(Qt::WindowFullScreen);

    // TMP (remplacement Arduino + Fix choix de niveau)
    new QShortcut(tr("Right"), this, SLOT(onRight()));
    new QShortcut(tr("Left"), this, SLOT(onLeft()));
    new QShortcut(tr("Up"), this, SLOT(onUp()));
    new QShortcut(tr("Down"), this, SLOT(onDown()));
    new QShortcut(tr("Return"), this, SLOT(onEnter()));
    new QShortcut(tr("Backspace"), this, SLOT(onBackpace()));

    // Timer de rafraichissement
    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(16);

    // Modèle de données
    QObject::connect(&m_game, SIGNAL(gameReady()), this, SLOT(onGameReady()));
    QObject::connect(&m_game, SIGNAL(stateChanged(Game::GameStates)), this, SLOT(onGameStateChanged(Game::GameStates)));

    // Widgets
    // // Menu
    m_menuWidget = new MainMenuWidget(this);
    checkFullscreen();
    addWidget(m_menuWidget);

    QObject::connect(m_menuWidget, SIGNAL(onExit()), this, SLOT(close()));
    QObject::connect(m_menuWidget, SIGNAL(hostGameSelected()), this, SLOT(onHostGame()));
    QObject::connect(m_menuWidget, SIGNAL(localGameSelected()), this, SLOT(onLocalGame()));
    QObject::connect(m_menuWidget, SIGNAL(ipGameSelected(QString)), this, SLOT(onIPGame(QString)));
    QObject::connect(m_menuWidget->optionsWidget(), SIGNAL(spinChanged(QString,int)), SLOT(onSpinOptionChanged(QString, int)));

    // // Lobby
    m_lobbyWidget = new LobbyWidget(&m_game ,this);
    addWidget(m_lobbyWidget);

    // // Jeu
    m_gameWidget = new GameWidget(&m_game, this);
    addWidget(m_gameWidget);
    // // // Le jeu est initialisé au choix entre Multijoueur et Solo

    setCurrentIndex(0);

    // Musique
    m_musicPlayer = new QMediaPlayer;

    m_playlist = new QMediaPlaylist;
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playlist->addMedia(QUrl("qrc:/musics/musics/Main.mp3"));
    onSpinOptionChanged("Musique", m_menuWidget->musicVolume());

    m_musicPlayer->setPlaylist(m_playlist);
    m_musicPlayer->play();

    // Gestionnaire Arduino
    m_arduinoHandler = new ArduinoHandler;

    QObject::connect(m_arduinoHandler, SIGNAL(rightPressed()), this, SLOT(onRight()));
    QObject::connect(m_arduinoHandler, SIGNAL(leftPressed()), this, SLOT(onLeft()));
    QObject::connect(m_arduinoHandler, SIGNAL(upPressed()), this, SLOT(onUp()));
    QObject::connect(m_arduinoHandler, SIGNAL(downPressed()), this, SLOT(onDown()));
    QObject::connect(m_arduinoHandler, SIGNAL(enterPressed()), this, SLOT(onEnter()));
    QObject::connect(m_arduinoHandler, SIGNAL(lightMeterValueChanged(int)), this, SLOT(onLightMeter(int)));

    m_arduinoHandler->start(500);
    qDebug() << "Temps de chargement :" << test.msecsTo(QTime::currentTime()) << "mscs";
}

MainWindow::~MainWindow()
{

}

void MainWindow::onGameReady()
{
    m_game.startGame();
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    paintBackground(&painter);

    painter.end();
}

void MainWindow::paintBackground(QPainter *painter)
{
    static QPixmap image = QPixmap(QApplication::applicationDirPath() + "/images/deathstar.png").scaledToHeight(100, Qt::SmoothTransformation);
    static QList<QPoint*> stars;
    QPoint *star;
    static QList<int> starsSpeed;
    static int deathstarMove = 0;
    deathstarMove ++;

    QRect rect = QApplication::desktop()->screenGeometry();
    int w = rect.width();
    int h = rect.height();

    painter->setBrush(QColor(0,0,0));
    painter->drawRect(rect);

    while(stars.size() < 1000)
    {
        stars.append(new QPoint(qrand()%w, qrand()%h));
        if(stars.size() == 1)
            starsSpeed.append(0);
        else
            starsSpeed.append((qrand()%12 + 1));
    }

    QPoint *tmp;
    painter->setBrush(QColor(255,255,255));
    painter->setPen(QColor(255,255,255));

    for(int i = 0; i < stars.size(); i++)
    {
        tmp = stars.at(i);
        painter->drawPoint(*tmp);
        tmp->setX(tmp->x() - starsSpeed.at(i));
        if(i==0)
        {
            if(deathstarMove == 5)
            {
                deathstarMove = 0;
                tmp->setX(tmp->x() - 1);
            }
            painter->drawPixmap(*tmp, image);
            if(tmp->x() < - image.width())
            {
                tmp->setY(qrand()%h - image.height()/2);
                tmp->setX(w);
            }
        }
        else if(tmp->x() < 0)
        {
            star = stars.at(i);
            star->setX(w);
            star->setY(qrand()%h);
        }
    }

    paintBackgroundLaser(painter);
}

void MainWindow::paintBackgroundLaser(QPainter *painter)
{
    static int frame = -1;
    static double p1, p2;
    static int htoh;
    static int cooldown = -1;
    static QColor color;
    static int size;
    int tmp;

    QRect rect = QApplication::desktop()->screenGeometry();

    if((frame < cooldown) && frame < 0)
    {
        p1 = ((double)(qrand()%100))/100.0;
        p2 = ((double)(qrand()%100))/100.0;
        frame = qrand()%12 + 5;
        size = frame;
        htoh = qrand()%2;
        cooldown = - (qrand()%10)*60 + 320;;
        tmp = qrand()%3;
        color = QColor((tmp==0)*255, (tmp==1)*255, (tmp==2)*255);
    }

    if(frame > 0)
    {
        QPen ppen = painter->pen();
        QPen pen(color);
        if(frame > size/2)
            pen.setWidth(size - frame);
        else
            pen.setWidth(frame);
        painter->setPen(pen);

        if(htoh == 1)
        {
            painter->drawLine(p1 * rect.width(),
                              0,
                              p2 * rect.width(),
                              rect.height());

        }
        else
        {
            painter->drawLine(0,
                              p1 * rect.width(),
                              rect.width(),
                              p2 *rect.height());
        }

        painter->setPen(ppen);
    }
    frame --;
}

void MainWindow::onRight()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->onRight();
        break;
    case Game::LobbyState:
        m_lobbyWidget->onRight();
        break;
    case Game::PlayingState:
        m_gameWidget->onRight();
        break;
    }
}

void MainWindow::onUp()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->onUp();
        break;
    case Game::LobbyState:
        m_lobbyWidget->onUp();
        break;
    case Game::PlayingState:
        m_gameWidget->onUp();
        break;
    }
}

void MainWindow::onLeft()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->onLeft();
        break;
    case Game::LobbyState:
        m_lobbyWidget->onLeft();
        break;
    case Game::PlayingState:
        m_gameWidget->onLeft();
        break;
    }
}

void MainWindow::onDown()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->onDown();
        break;
    case Game::LobbyState:
        m_lobbyWidget->onDown();
        break;
    case Game::PlayingState:
        m_gameWidget->onDown();
        break;
    }
}

void MainWindow::onEnter()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->onEnter();
        checkFullscreen();
        m_game.setPlayerNickname(m_menuWidget->playerNickname());

        break;
    case Game::LobbyState:
        m_lobbyWidget->onEnter();

        break;
    case Game::PlayingState:
        m_gameWidget->onEnter();
        break;
    }
}

void MainWindow::onBackpace()
{
    switch(m_game.state())
    {
    case Game::MenuState:
        m_menuWidget->onBackspace();
        break;
    case Game::LobbyState:

        if(!m_lobbyWidget->hasChoosenMap())
        {
            m_game.setState(Game::MenuState);
            m_menuWidget->onBackspace();
        }
        m_lobbyWidget->onBackspace();

        break;
    case Game::PlayingState:
        m_gameWidget->onBackpace();
        onLightMeter(500);
        break;
    }
}

void MainWindow::onLightMeter(int value)
{
    if(m_game.state() == Game::PlayingState)
    {
        m_game.level()->player()->setPowerRegeneration(value);
    }
}

void MainWindow::onSpinOptionChanged(const QString &name, int value)
{
    if(name == "Musique")
    {
        value *= m_menuWidget->mainVolume();
        value /= 100;
        m_musicPlayer->setVolume(value);
    }
    else if(name == "Principal")
    {
        value *= m_menuWidget->musicVolume();
        value /= 100;
        m_musicPlayer->setVolume(value);
    }
}

void MainWindow::onHostGame()
{
    m_game.setHosting(true);
    m_game.setState(Game::LobbyState);
}

void MainWindow::onLocalGame()
{
    m_game.lookForLocalHost();
}

void MainWindow::onIPGame(const QString &ip)
{
    m_game.connectToIP(ip);
}

void MainWindow::onGameStateChanged(Game::GameStates state)
{
    switch (state) {
    case Game::MenuState:
        m_arduinoHandler->setLoopDuration(500);
        setCurrentIndex(0);
        break;
    case Game::LobbyState:
        m_arduinoHandler->setLoopDuration(500);
        setCurrentIndex(1);
        break;
    case Game::PlayingState:
        m_arduinoHandler->setLoopDuration(16);
        setCurrentIndex(2);
        break;
    }
}

void MainWindow::checkFullscreen()
{
    static Qt::WindowState previousState = Qt::WindowMaximized;

    if(isFullScreen())
    {
        if(!m_menuWidget->isFullScreenChecked())
        {
            setWindowState(previousState);
        }
    }
    else
    {
        if(m_menuWidget->isFullScreenChecked())
        {
            if(isMaximized())
                previousState = Qt::WindowMaximized;
            else
                previousState = Qt::WindowNoState;

            setWindowState(Qt::WindowFullScreen);
        }
    }
}
