#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTime>

#define BUF_LENGHT 32
#define LINE_SIZE 10

class ArduinoHandler : public QObject
{
    Q_OBJECT
public:
    ArduinoHandler(QObject *parent = Q_NULLPTR);
    void start(int loopDuration = 0);

    int loopDuration() const;
    void setLoopDuration(int loopDuration);
    int getLightValue(QString message);
    char processCommand(QByteArray data);
    static bool commandBlocks(char command);

protected slots:
    void mainloop();

signals:
    void leftPressed();
    void rightPressed();
    void upPressed();
    void downPressed();

    void enterPressed();
    void backspacePressed();

    void lightMeterValueChanged(int value);

private:
    /** @brief Timer de vérification du port série. */
    QTimer *m_timer;
    /** @brief Port série de communication. */
    QSerialPort *m_serialPort;
    int m_loopDuration;
};

#endif // ARDUINOHANDLER_H
