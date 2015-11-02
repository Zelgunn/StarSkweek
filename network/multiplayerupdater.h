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

    void appendUpdate(const QString &update);
    void sendUpdates();
    QStringList receivedUpdates();
    bool isFirst() const;
    bool isConnected() const;

    void startHost();
    void lookForLocalHost();
    void connectToIP(const QString &ip);

protected:
    void sendUpdate(const QString &update);
    void incomingConnection(int socketfd);
    void connectToPlayer2();

protected slots:
    void broadcastAddress();
    void readUdp();
    void readTcp();
    void disconnected();

signals:
    void gameConnected();

private:
    QStringList m_updates;
    QStringList m_receivedUpdates;
    QUdpSocket *m_udpSocket;
    QTcpSocket *m_client;
    QHostAddress m_localAddress;
    QHostAddress m_player2Address;
    QTimer *m_timer;
    int m_initTime;
    int m_player2Time;
};

#endif // MULTIPLAYERUPDATER_H
