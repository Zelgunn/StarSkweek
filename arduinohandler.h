#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include "QSerialPort"

class ArduinoHandler
{
public:
    ArduinoHandler();
private:
    QSerialPort m_serialport;
};

#endif // ARDUINOHANDLER_H
