#include "arduinohandler.h"

ArduinoHandler::ArduinoHandler(QObject *parent)
    : QObject(parent), m_serialPort(Q_NULLPTR)
{
    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(mainloop()));
}

void ArduinoHandler::start()
{
    QList <QSerialPortInfo> infos = QSerialPortInfo::availablePorts();

    // Recherche du port connecté l'Arduino
    for(int i=0; i<infos.size(); i++)
    {
        QSerialPortInfo info = infos.at(i);
        m_serialPort = new QSerialPort(info);
        if((info.description() == "Arduino Uno")&&(m_serialPort->open(QIODevice::ReadOnly)))
        {
            m_serialPort->setBaudRate(9600);
            mainloop();
            return;
        }
    }
    mainloop();
}

void ArduinoHandler::mainloop()
{
    if(!m_timer->isActive())
        m_timer->start(20); // Valeur à modifier dynamiquement (pour les menus : plus lent, pour le jeu : plus rapide).

    if((!m_serialPort)||(!m_serialPort->isOpen()))
        return;

    QByteArray data = m_serialPort->readAll();

    // Parsing arduino
    if(data.size() > 0)
    {
        switch (data.at(1)) {
        case '0': emit rightPressed();
            break;
        case '1': emit leftPressed();
            break;
        case '2': emit upPressed();
            break;
        case '3': emit downPressed();
            break;
        case '4': emit enterPressed();
            break;
        case '5': emit backspacePressed();
            break;
        case 'l': emit lightMeterValueChanged(QString(data).mid(1).toInt()); // Parsing + Conversion vers un entier de la chaîne reçue.
            break;
        }
    }
}
