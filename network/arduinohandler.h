#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QGraphicsPixmapItem>
#include <QPainter>

#define BUF_LENGHT 32
#define LINE_SIZE 10

class ArduinoHandler : public QObject
{
    Q_OBJECT
public:
    ArduinoHandler(QObject *parent = Q_NULLPTR);
    void start();

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
};

#endif // ARDUINOHANDLER_H
