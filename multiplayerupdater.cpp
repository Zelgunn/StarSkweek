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

    m_playerDirection = -1;
    m_initTime = QTime::currentTime().msecsSinceStartOfDay();
    m_timer = new QTimer(this);

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(broadcastAddress()));
    QObject::connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()));
    QTimer::singleShot(50, this, SLOT(broadcastAddress()));

    m_server = new QTcpServer;
    if(!m_server->listen(m_localAddress, PORT_COM))
        qDebug() <<  m_server->errorString();
    else
        qDebug() << "Server démarré sur le port" << m_server->serverPort();
    QObject::connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewTcpConnection()));

    m_tcpSocket = new QTcpSocket;
    QObject::connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readTcpDatagram()));
}

void MultiplayerUpdater::sendUpdates()
{
    if(m_player2Address.isNull()) return;

    if(m_playerDirection > 0)
    {
        QString playerDirectionUpdate = "pm" + QString::number(m_playerDirection);
        sendUpdate(playerDirectionUpdate);
    }

    for(int i=0; i<m_updates.size(); i++)
    {
        sendUpdate(m_updates.at(i));
    }
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

    if(m_udpSocket->state() != QAbstractSocket::BoundState)
        return;

    while(m_udpSocket->hasPendingDatagrams())
    {
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
        message = datagram.data();

        if(m_player2Address.isNull())
        {
            if((message.startsWith("192.168.")) && (!message.startsWith(m_localAddress.toString())))
            {
                QObject::disconnect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagram()));
                m_udpSocket->abort();

                m_player2Address = QHostAddress(message.section('/', 0, 0));
                m_player2Time = message.section('/', -1).toInt();
                qDebug() << m_player2Address;
                m_tcpSocket->connectToHost(m_player2Address, PORT_COM);
            }
        }
        else
        {
            m_receivedUpdates.append(message);
        }
    }
}

void MultiplayerUpdater::readTcpDatagram()
{
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (m_datagramSize == 0)
    {
        if (m_tcpSocket->bytesAvailable() < (int)sizeof(quint16))
        {
            qDebug() << "Pas assez de données à lire (pas de taille).";
            return;
        }
        in >> m_datagramSize;
    }

    if (m_tcpSocket->bytesAvailable() < m_datagramSize)
    {
        qDebug() << "Pas assez de données à lire (pas de données).";
        return;
    }
    else
    {
        qDebug() << m_tcpSocket->bytesAvailable() << "octets lisibles";
    }

    QString message;
    in >> message;
    if(message.size() > 0)
        qDebug() << "Message recu :" << message;
}

void MultiplayerUpdater::onNewTcpConnection()
{
    m_clientSocket = m_server->nextPendingConnection();
    QObject::connect(m_clientSocket, SIGNAL(disconnected()), m_clientSocket, SLOT(deleteLater()));
    m_datagramSize = 0;
    qDebug() << "Connection initialisée";
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

void MultiplayerUpdater::sendUpdate(const QString &update)
{
    if(!m_clientSocket) return;

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << update;
    out.device()->seek(0);
    out << (quint16)(datagram.size() - sizeof(quint16));

    m_clientSocket->write(datagram);
    //m_clientSocket->flush();
}
void MultiplayerUpdater::setPlayerDirection(int playerDirection)
{
    m_playerDirection = playerDirection;
}

void MultiplayerUpdater::appendUpdate(const QString &update)
{
    m_updates.append(update);
}

