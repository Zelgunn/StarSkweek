#ifndef MULTIPLAYERUPDATER_H
#define MULTIPLAYERUPDATER_H

#include <QStringList>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTimer>
#include <QTime>

#include <QDebug>

#define PORT_COM 45454

class MultiplayerUpdater : public QObject
{
    Q_OBJECT
public:
    MultiplayerUpdater();

    void sendUpdate(const QString &update);
    QStringList receivedUpdates();
    bool isFirst() const;

public slots:
    void readDatagram();
    void broadcastAddress();

signals:
    void gameConnected();

private:
    QStringList m_receivedUpdates;
    QUdpSocket *m_udpSocket;
    QHostAddress m_localAddress;
    QHostAddress m_player2Address;
    QTimer *m_timer;
    int m_initTime;
    int m_player2Time;
};

#endif // MULTIPLAYERUPDATER_H
