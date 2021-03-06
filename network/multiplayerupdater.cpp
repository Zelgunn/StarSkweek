#include "multiplayerupdater.h"

MultiplayerUpdater::MultiplayerUpdater() :
    m_client(Q_NULLPTR)
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

    PlayerInfo *playerInfo = new PlayerInfo;
    playerInfo->setAddress(m_localAddress);
    m_playersInfos.append(playerInfo);

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
    QObject::connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readUdp()));
}

void MultiplayerUpdater::connectToIP(const QString &ip)
{
    PlayerInfo *playerInfo;
    if(m_playersInfos.size() > 1)
    {
        playerInfo = m_playersInfos.at(1);
        playerInfo->setAddress(QHostAddress(ip));
    }
    else
    {
        playerInfo = new PlayerInfo;
        playerInfo->setAddress(QHostAddress(ip));
        m_playersInfos.append(playerInfo);
    }

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

    PlayerInfo *playerInfo = new PlayerInfo;
    playerInfo->setAddress(m_client->peerAddress());
    m_playersInfos.append(playerInfo);

    QObject::connect(m_client, SIGNAL(readyRead()), this, SLOT(readTcp()));
    QObject::connect(m_client, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

void MultiplayerUpdater::connectToPlayer2()
{
    if(m_client != Q_NULLPTR) return;

    m_client = new QTcpSocket(this);
    m_client->connectToHost(m_playersInfos.at(1)->address(), PORT_COM);
    if(!m_client->isOpen())
    {
        delete m_client;
        m_client = Q_NULLPTR;
        return;
    }

    QObject::connect(m_client, SIGNAL(readyRead()), this, SLOT(readTcp()));
    QObject::connect(m_client, SIGNAL(disconnected()), this, SLOT(disconnected()));

}

void MultiplayerUpdater::broadcastAddress()
{
    if(!isListening())
        listen(m_localAddress, PORT_COM);

    if(!isConnected())
    {
        QByteArray addressDatagram = QString(m_localAddress.toString()
                                            + ',' + m_playersInfos.first()->nickname()
                                            + ',' + m_mapPath).toUtf8();

        m_udpSocket->writeDatagram(addressDatagram.data(), addressDatagram.size(), QHostAddress::Broadcast, PORT_COM);

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
    PlayerInfo *playerInfo;

    while(m_udpSocket->hasPendingDatagrams())
    {
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
        message = datagram.data();

        if((message.startsWith("192.168.")) && (!message.startsWith(m_localAddress.toString())))
        {
            QHostAddress address (message.section(',', 0, 0));
            bool playerAlreadyKnown = false;

            for(int i=0; i<m_playersInfos.size(); i++)
            {
                if(m_playersInfos.at(i)->address() == address)
                {
                    playerAlreadyKnown = true;
                    break;
                }
            }

            if(!playerAlreadyKnown)
            {
                playerInfo = new PlayerInfo;
                playerInfo->setAddress(address);
                playerInfo->setNickname(message.section(',', 1, 1));
                m_playersInfos.append(playerInfo);
                m_mapPath = message.section(',', 2, 2);
                emit newConnection();
            }
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
    delete m_client;
    m_client = Q_NULLPTR;
}

QList<PlayerInfo *> MultiplayerUpdater::playersInfos() const
{
    return m_playersInfos;
}

QString MultiplayerUpdater::mapPath() const
{
    return m_mapPath;
}

void MultiplayerUpdater::setMapPath(const QString &mapPath)
{
    m_mapPath = mapPath;
}


