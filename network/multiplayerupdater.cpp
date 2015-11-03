#include "multiplayerupdater.h"

MultiplayerUpdater::MultiplayerUpdater() :
    m_client(Q_NULLPTR), m_isHost(false)
{
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(PORT_COM);

    QList<QHostAddress> ipAdressList = QNetworkInterface::allAddresses();
    for(int i=0; i<ipAdressList.size(); i++)
    {
        if(ipAdressList.at(i).toString().startsWith("192.168."))
        {
            m_localAddress = ipAdressList.at(i);
            break;
        }
    }

    m_timer = new QTimer(this);

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(broadcastAddress()));
}

void MultiplayerUpdater::appendUpdate(const QString &update)
{
    m_updates.append(update);
}

void MultiplayerUpdater::sendUpdates()
{
    if(!isConnected())
    {
        return;
    }

    for(int i=0; i<m_updates.size(); i++)
    {
        sendUpdate(m_updates.at(i));
    }

    m_updates.clear();
}

QStringList MultiplayerUpdater::receivedUpdates()
{
    QStringList res = m_receivedUpdates;
    m_receivedUpdates.clear();
    return res;
}

bool MultiplayerUpdater::isHost() const
{
    return m_isHost;
}

bool MultiplayerUpdater::isConnected() const
{
    return ((m_client != Q_NULLPTR) && (m_client->isOpen()));
}

void MultiplayerUpdater::startHost(bool enable)
{
    if(enable)
    {
        broadcastAddress();
    }
    else if(m_timer != Q_NULLPTR)
    {
        if(isConnected())
        {
            m_client->disconnectFromHost();
        }
        else
        {
            m_timer->stop();
        }
    }
}

void MultiplayerUpdater::lookForLocalHost()
{
    qDebug() << "Looking for local host...";
    QObject::connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readUdp()));
}

void MultiplayerUpdater::connectToIP(const QString &ip)
{
    m_player2Address = QHostAddress(ip);
    connectToPlayer2();
}

void MultiplayerUpdater::sendUpdate(const QString &update)
{
    m_client->write(QString(update + '\n').toUtf8());
}

void MultiplayerUpdater::incomingConnection(int socketfd)
{
    if(m_client != Q_NULLPTR) return;

    m_client = new QTcpSocket(this);
    m_client->setSocketDescriptor(socketfd);

    qDebug() << "Nouvelle connexion :" << m_client->peerAddress().toString();

    QObject::connect(m_client, SIGNAL(readyRead()), this, SLOT(readTcp()));
    QObject::connect(m_client, SIGNAL(disconnected()), this, SLOT(disconnected()));

    emit gameConnected();
}

void MultiplayerUpdater::connectToPlayer2()
{
    if(m_client != Q_NULLPTR) return;

    m_client = new QTcpSocket(this);
    qDebug() << "Connexion au serveur en cours...";
    m_client->connectToHost(m_player2Address, PORT_COM);
    if(m_client->isOpen())
        qDebug() << "Connecté.";
    else
    {
        qDebug() << "Impossible de se connecter";
        delete m_client;
        m_client = Q_NULLPTR;
        return;
    }

    QObject::connect(m_client, SIGNAL(readyRead()), this, SLOT(readTcp()));
    QObject::connect(m_client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    emit gameConnected();
}

void MultiplayerUpdater::broadcastAddress()
{
    qDebug() << "Broadcasting...";
    if(!isListening())
        listen(m_localAddress, PORT_COM);

    if(!isConnected())
    {
        QByteArray adressDatagram = QString(m_localAddress.toString() + ',' + m_mapPath).toUtf8();
        m_udpSocket->writeDatagram(adressDatagram.data(), adressDatagram.size(), QHostAddress::Broadcast, PORT_COM);

        if(!m_timer->isActive())
            m_timer->start(1000);
    }
    else
    {
        if(m_timer->isActive())
            m_timer->stop();
    }
}

void MultiplayerUpdater::readUdp()
{
    QByteArray datagram;
    QString message;

    while(m_udpSocket->hasPendingDatagrams())
    {
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
        message = datagram.data();

        qDebug() << message;

        if((message.startsWith("192.168.")) && (!message.startsWith(m_localAddress.toString())))
        {
            m_player2Address = QHostAddress(message.section(',', 0, 0));
            m_mapPath = message.section(',', 1, 1);
            connectToPlayer2();
        }
    }
}

void MultiplayerUpdater::readTcp()
{
    while(m_client->canReadLine())
    {
        QString message = QString::fromUtf8(m_client->readLine());
        m_receivedUpdates.append(message);
    }
}

void MultiplayerUpdater::disconnected()
{
    qDebug() << "Deconnexion du joueur 2.";
    delete m_client;
    m_client = Q_NULLPTR;
}
QString MultiplayerUpdater::mapPath() const
{
    return m_mapPath;
}

void MultiplayerUpdater::setMapPath(const QString &mapPath)
{
    m_mapPath = mapPath;
}


