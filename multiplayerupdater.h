#ifndef MULTIPLAYERUPDATER_H
#define MULTIPLAYERUPDATER_H

#include <QStringList>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTimer>
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include <QDebug>

#define PORT_COM 45454

class MultiplayerUpdater : public QTcpServer
{
    Q_OBJECT
public:
    MultiplayerUpdater();

    void sendUpdates();
    QStringList receivedUpdates();
    bool isFirst() const;
    void setPlayerDirection(int playerDirection);
    void appendUpdate(const QString &update);

public slots:
    void readDatagram();
    void readTcpDatagram();
    void disconnected();
    void broadcastAddress();

signals:
    void gameConnected();

protected:
    void sendUpdate(const QString &update);
    void incomingConnection(int socketfd);

private:
    QStringList m_updates;
    QStringList m_receivedUpdates;
    QUdpSocket *m_udpSocket;
    QTcpSocket *m_tcpSocket;
    QTcpSocket *m_clientSocket;
    QHostAddress m_localAddress;
    QHostAddress m_player2Address;
    QTimer *m_timer;
    int m_initTime;
    int m_player2Time;
    int m_playerDirection;
    quint16 m_datagramSize;
};

#endif // MULTIPLAYERUPDATER_H
