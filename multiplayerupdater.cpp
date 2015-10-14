#include "multiplayerupdater.h"

MultiplayerUpdater::MultiplayerUpdater()
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

    m_initTime = QTime::currentTime().msecsSinceStartOfDay();
    m_timer = new QTimer(this);

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(broadcastAddress()));
    QObject::connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()));
    QTimer::singleShot(50, this, SLOT(broadcastAddress()));
}

void MultiplayerUpdater::sendUpdate(const QString &update)
{
    if(m_player2Address.isNull()) return;
    QByteArray adressDatagram = update.toUtf8();
    m_udpSocket->writeDatagram(adressDatagram.data(), adressDatagram.size(), m_player2Address, PORT_COM);
}

QStringList MultiplayerUpdater::receivedUpdates()
{
    QStringList res = m_receivedUpdates;
    m_receivedUpdates.clear();
    return res;
}

bool MultiplayerUpdater::isFirst() const
{
    return m_initTime < m_player2Time;
}

void MultiplayerUpdater::readDatagram()
{
    QByteArray datagram;
    QString message;

    while(m_udpSocket->hasPendingDatagrams())
    {
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
        message = datagram.data();

        if(m_player2Address.isNull())
        {
            if((message.startsWith("192.168.")) && (!message.startsWith(m_localAddress.toString())))
            {
                m_player2Address = QHostAddress(message.section('/', 0, 0));
                m_player2Time = message.section('/', -1).toInt();
            }
        }
        else
        {
            m_receivedUpdates.append(message);
        }
    }
}

void MultiplayerUpdater::broadcastAddress()
{
    if(m_player2Address.isNull())
    {
        if(!m_timer->isActive())
            m_timer->start(100);
    }
    else
    {
        if(m_timer->isActive())
        {
            m_timer->stop();
            emit gameConnected();
        }
    }

    QString message = m_localAddress.toString();
    message.append("/");
    message.append(QString::number(m_initTime));
    QByteArray adressDatagram = message.toUtf8();
    m_udpSocket->writeDatagram(adressDatagram.data(), adressDatagram.size(), QHostAddress::Broadcast, PORT_COM);
}
