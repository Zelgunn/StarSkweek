#include "arduinohandler.h"

ArduinoHandler::ArduinoHandler(QObject *parent)
    : QObject(parent), m_serialPort(Q_NULLPTR), m_loopDuration(0)
{
    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(mainloop()));
}

void ArduinoHandler::start(int loopDuration)
{
    m_loopDuration = loopDuration;
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
    if(m_loopDuration == 0)
    {
        m_timer->stop();
        return;
    }
    if(!m_timer->isActive())
        m_timer->start(10); // Valeur à modifier dynamiquement (pour les menus : plus lent, pour le jeu : plus rapide).

    if((!m_serialPort)||(!m_serialPort->isOpen()))
        return;

    QByteArray data = m_serialPort->readAll();
    static char lastCommand = ' ';
    static QTime lastCommandTime = QTime::currentTime();
    if(lastCommandTime.msecsTo(QTime::currentTime()) > m_loopDuration)
        lastCommand = ' ';

    // Parsing arduino
    if(data.size() > 0)
    {
        if(lastCommand != data.at(0))
        {
            lastCommandTime = QTime::currentTime();
            lastCommand = data.at(0);
            switch (data.at(0)) {
            case 'r':
                emit rightPressed();
                break;
            case 'l': emit leftPressed();
                break;
            case 'u': emit upPressed();
                break;
            case 'd': emit downPressed();
                break;
            case 's': emit enterPressed();
                break;
            case 'b': emit backspacePressed();
                break;
            case ',':
                qDebug() << QString(data).section(',', 0, 0);
                //emit lightMeterValueChanged(QString(data).toInt()); // Parsing + Conversion vers un entier de la chaîne reçue.
                break;
            }
        }
    }
}
int ArduinoHandler::loopDuration() const
{
    return m_loopDuration;
}

void ArduinoHandler::setLoopDuration(int loopDuration)
{
    m_loopDuration = loopDuration;
}

