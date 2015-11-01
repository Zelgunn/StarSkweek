#include "messagebox.h"

MessageBox::MessageBox(QWidget *device)
    : m_device(device), m_profile('A'), m_cursor(0)
{

}

void MessageBox::drawNoProfileMessage()
{
    QPainter painter(m_device);

    painter.setBrush(QBrush(QColor(0,0,0,200)));
    painter.setPen(Qt::NoPen);

    painter.drawRect(0,0,m_device->width(), m_device->height());

    painter.setBrush(QBrush(QColor(255,255,255,50)));
    QPen pen(QColor(255,255,255));
    pen.setWidth(3);
    painter.setPen(pen);

    QRect panelRect(0,0,500, 300);
    panelRect.moveCenter(QPoint(m_device->width()/2, m_device->height()/2));

    painter.drawRect(panelRect);

    QRect informationRect(panelRect);
    informationRect.setHeight(150);
    informationRect.setWidth(450);
    informationRect.moveRight(informationRect.right() + 25);
    informationRect.moveBottom(informationRect.bottom() + 25);

    painter.setFont(QFont("Times", 16));
    painter.drawText(informationRect, Qt::AlignCenter | Qt::AlignHCenter | Qt::TextWordWrap,"Il s'agit de la première fois que vous démarrez"
                                       " SuperSkweek ou votre profil a été corrompu."
                                       "\n\n\nEntrez un nom de profil.");


    QRect profileRect(informationRect);
    profileRect.setHeight(50);
    profileRect.moveCenter(profileRect.center() + QPoint(0, 375/2));
    painter.drawRect(profileRect);

    QString profile(m_profile);
    profile.insert(m_cursor, '&');  // Ajout du Mnemonic => Soulignement de la lettre sélectionnée.
    painter.drawText(profileRect, Qt::TextShowMnemonic | Qt::AlignCenter, profile);

    painter.end();
}

void MessageBox::moveCursor(int delta)
{
    m_cursor += delta;
    if(m_cursor < 0) m_cursor = m_profile.size() - 1;
    if(m_cursor >= m_profile.size())
    {
        m_cursor = m_profile.size();
        m_profile.resize(m_profile.size() + 1);
        if(m_cursor == 0)
            m_profile.replace(m_cursor, 1, QChar('A'));
        if(m_cursor > 0)
        {
            if(m_profile.at(m_cursor - 1) == ' ')
                m_profile.replace(m_cursor, 1, QChar('A'));
            else
                m_profile.replace(m_cursor, 1, QChar(' '));
        }
    }
}

void MessageBox::changeLineChar(int delta)
{
    char c = (char)(m_profile.at(m_cursor).toLatin1() + delta);
    switch(c)
    {   // Espace => Min. => Maj. => Chiffres => Espace

    case ' ' + 1:   // Espace => Min.
        c = 'a';
        break;
    case 'a' - 1:   // Espace <= Min.
        c = ' ';
        break;

    case 'z' + 1:   // Min. => Maj.
        c = 'A';
        break;
    case 'A' - 1:   // Min. <= Maj.
        c = 'z';
        break;

    case 'Z' + 1:   // Maj. => Chiffres
        c = '0';
        break;
    case '0' - 1:   // Maj. <= Chiffres
        c = 'Z';
        break;

    case '9' + 1:   // Chiffres => Espace
        c = ' ';
        break;
    case ' ' - 1:   // Chiffres <= Espace
        c = '9';
        break;
    }

    m_profile.replace(m_cursor, 1, QChar(c));
}

void MessageBox::removeChar()
{
    m_profile.remove(m_cursor--, 1);
    if(m_profile.size() == 0)
    {
        m_cursor = 0;
        m_profile.append(' ');
    }
}

bool MessageBox::isProfileCorrect() const
{
    int count = m_profile.trimmed().size();
    return count > 2;
}

QString MessageBox::profile() const
{
    return m_profile.trimmed();
}

