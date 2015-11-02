#include "optionwidget.h"

OptionWidget::OptionWidget(QWidget *parent)
    : QWidget(parent), m_currentLine(0), m_selectedLine(0), m_isSelectionLocked(false), m_cursor(0)
{

}

QList<QDomElement> OptionWidget::options() const
{
    return m_options;
}

void OptionWidget::setOptions(const QList<QDomElement> &options)
{
    m_selectedLine = 0;
    m_options.clear();
    for(int i=0; i<options.size(); i++)
    {
        m_options.append(options.at(i).cloneNode().toElement());
    }
}

void OptionWidget::selectNextOption()
{
    // Champ verrouillé
    if(m_isSelectionLocked)
    {
        if((m_options.at(m_selectedLine).attribute("type")=="line"))
            changeLineChar(1);
        return;
    }

    // Champ non verrouillé
    m_cursor = 0;
    if(m_options.size() == 0) return;
    m_selectedLine = (m_selectedLine + 1)%(m_options.size() + 2);
}

void OptionWidget::selectPreviousOption()
{
    // Champ verrouillé
    if(m_isSelectionLocked)
    {
        if((m_options.at(m_selectedLine).attribute("type")=="line"))
            changeLineChar(-1);
        return;
    }

    // Champ non verrouillé
    m_cursor = 0;
    if(m_options.size() == 0) return;
    m_selectedLine --;
    if(m_selectedLine < 0) m_selectedLine = m_options.size() + 1;
}

void OptionWidget::onEnter()
{
    if(m_selectedLine < m_options.size())
    {

        if(m_options.at(m_selectedLine).attribute("type")=="bool")
        {
            QDomElement option = m_options.at(m_selectedLine);
            if(optionValue(option) == "true")
                option.setAttribute("value", "false");
            else
                option.setAttribute("value", "true");

            m_options.replace(m_selectedLine, option);
            m_isSelectionLocked = false;
        }
        else if(m_options.at(m_selectedLine).attribute("type")=="line")
        {
            if(isProfileCorrect()) lockSelection();
        }
        else
        {
            lockSelection();
        }
    }
    else
    {
        m_isSelectionLocked = false;
    }
}

void OptionWidget::onRight()
{
    if(m_selectedLine >= m_options.size())
    {
        selectNextOption();
    }
    else
    {
        if(m_isSelectionLocked)
        {
            QDomElement option = m_options.at(m_selectedLine);
            if(option.attribute("type")=="line")
            {
                moveCursor(1);
            }
            else if(option.attribute("type")=="spin")
            {
                moveSpin(1);
            }
        }
        else
        {
            selectNextOption();
        }
    }
}

void OptionWidget::onLeft()
{
    if(m_selectedLine >= m_options.size())
    {
        selectPreviousOption();
    }
    else
    {
        if(m_isSelectionLocked)
        {
            if(m_options.at(m_selectedLine).attribute("type")=="line")
            {
                moveCursor(-1);
            }
            else if(m_options.at(m_selectedLine).attribute("type")=="spin")
            {
                moveSpin(-1);
            }
        }
        else
        {
            selectPreviousOption();
        }
    }
}

void OptionWidget::onBackspace()
{
    if(m_isSelectionLocked)
    {
        if(m_options.at(m_selectedLine).attribute("type")=="line")
        {
            removeChar();
        }
        else
        {
            m_isSelectionLocked = false;
        }
    }
    else
    {
        selectCancelButton();
    }
}

bool OptionWidget::isApplySelected() const
{
    return (m_options.size() + 1) == m_selectedLine;
}

bool OptionWidget::isCancelSelected() const
{
    return (m_options.size()) == m_selectedLine;
}

bool OptionWidget::isSelectionLocked() const
{
    return m_isSelectionLocked;
}

void OptionWidget::selectCancelButton()
{
    m_selectedLine = m_options.size();
}

void OptionWidget::lockSelection()
{
    m_isSelectionLocked = !m_isSelectionLocked;
}

QString OptionWidget::optionValue(const QDomElement &option)
{
    if(option.hasAttribute("value"))
        return option.attribute("value");
    else
        return option.attribute("base");
}

void OptionWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setBrush(QBrush(QColor(255,255,255,0)));
    painter.setFont(QFont("Times", 18));

    m_currentLine = 0;

    QDomElement option;
    for(int i=0; i<m_options.size(); i++)
    {
        option = m_options.at(i);
        if(option.attribute("type") == "bool")
        {
            paintCheckbox(&painter, option, i == m_selectedLine);
        }
        if(option.attribute("type") == "line")
        {
            paintLine(&painter, option, i == m_selectedLine);
        }
        if(option.attribute("type") == "spin")
        {
            paintSpin(&painter, option, i == m_selectedLine);
        }
    }

    paintButtons(&painter);

    painter.end();
}

int OptionWidget::paintOptionName(QPainter *painter, const QString &option, bool selected)
{
    QPoint center(width()/2 -CENTRAL_PADDING, height()/2 + m_currentLine*40);

    QFont tmp = painter->font(), font = painter->font();
    font.setBold(selected);
    painter->setFont(font);

    painter->setPen(QPen(QColor(255,255,255)));

    QFontMetrics metrics(painter->font());
    int h = metrics.boundingRect(option).height();
    QRect textRect(QPoint(0, center.y()), QPoint(center.x(), center.y() + h));

    painter->drawText(textRect, Qt::AlignRight, option);
    painter->setFont(tmp);

    return h;
}

void OptionWidget::paintRect(QPainter *painter, const QRect &rect)
{
    painter->drawLine(rect.topLeft(), rect.topRight());
    painter->drawLine(rect.topLeft(), rect.bottomLeft());
    painter->drawLine(rect.bottomLeft(), rect.bottomRight());
    painter->drawLine(rect.bottomRight(), rect.topRight());
}

void OptionWidget::paintCheckbox(QPainter *painter, const QDomElement &option, bool selected)
{
    // Paramètres de l'option
    QString name = option.attribute("name");
    bool value = option.attribute("value") == "true";

    // Paramètres de dessin
    int checkWidth = 4;
    if(selected) checkWidth ++;
    int h = paintOptionName(painter, name, selected) + 3;

    // Dessin de la case
    QPoint center(width()/2 + CENTRAL_PADDING, height()/2 + m_currentLine*40 + h/4);
    QRect box(center, QSize(h/2, h/2));
    QPen pen(QColor(50,50,255));
    pen.setWidth(checkWidth - 2);
    painter->setPen(pen);
    painter->drawRect(box);

    // Dessin du signe
    if(value)
    {
        pen.setColor(QColor(255,255,255));
        pen.setWidth(checkWidth);
        painter->setPen(pen);
        center += QPoint(0, h/2);
        painter->drawLine(center + QPoint(0, -h/4 - 2), center + QPoint(h/4, - 2));
        painter->drawLine(center + QPoint(h/4, - 2), center + QPoint(h/2 + h/4, - (h/2) - 2));
    }

    m_currentLine ++;
}

void OptionWidget::paintLine(QPainter *painter, const QDomElement &option, bool selected)
{
    // Paramètres de l'option
    QString name = option.attribute("name");
    QString value = option.attribute("value");

    // Sauvegarde Police
    QFont tmp = painter->font(), font = painter->font();
    font.setBold(selected);
    painter->setFont(font);

    // Paramètres de dessin
    int h = paintOptionName(painter, name, selected);
    QPoint center(width()/2 + CENTRAL_PADDING, height()/2 + m_currentLine*40);
    painter->setPen(QPen(QColor(50,50,255)));
    QRect textRect(center, QPoint(width(), center.y() + h));

    // Curseur
    if(selected && m_isSelectionLocked)
    {
        QFontMetrics metrics(painter->font());
        int w = metrics.boundingRect(value.mid(0, m_cursor + 1)).width();
        int basew = metrics.boundingRect(value.mid(0, m_cursor)).width();
        basew += 2*(m_cursor!=0);
        if(value.at(m_cursor).toLatin1() == ' ')
            basew += 6;
        painter->drawLine(center + QPoint(basew, h), center + QPoint(w, h));
    }

    painter->drawText(textRect, Qt::AlignLeft, value);

    painter->setFont(tmp);
    m_currentLine ++;
}

void OptionWidget::paintSpin(QPainter *painter, const QDomElement &option, bool selected)
{
    // Paramètres de l'option
    QString name = option.attribute("name");
    int value = option.attribute("value").toInt();
    int max = option.attribute("max").toInt();
    int min = option.attribute("min").toInt();

    // Réglages
    int penWidth = 3;
    if(selected) penWidth ++;
    const int lineLenght = 200;
    int h = paintOptionName(painter, name, selected) - selected*2;
    QPoint center(width()/2 + CENTRAL_PADDING, height()/2 + m_currentLine*40 + h);

    // Dessin de la barre
    painter->setPen(Qt::NoPen);

    QRect bar(center + QPoint(0, -h), center + QPoint(lineLenght*(value - min)/(max - min), 0));
    QPoint left(bar.left(), bar.height()/2), right(bar.right(), bar.height()/2);
    QLinearGradient gradient(left, right);
    gradient.setColorAt(0, QColor(255,255,255,0));
    gradient.setColorAt(1, QColor(255,255,255,255));
    painter->setBrush(gradient);
    painter->drawRect(bar);

    // Dessin du cadre
    QPen pen;
    if(selected && m_isSelectionLocked)
        pen.setColor(QColor(0,0,255));
    else
        pen.setColor(QColor(100,100,255));
    pen.setWidth(penWidth);
    painter->setPen(pen);

    QRect framework(center - QPoint(selected, h  + selected), center + QPoint(lineLenght + selected, selected));
    paintRect(painter, framework);

    m_currentLine ++;
}

void OptionWidget::paintButtons(QPainter *painter)
{
    // Settings
    painter->setPen(QPen(QColor(255,255,255)));
    QFont font(QFont("Times", 18, QFont::Bold));
    painter->setFont(font);
    painter->setBrush(QBrush(QColor(255,255,255,100)));
    QFontMetrics metrics(font);

    // Appliquer
    QString text("Appliquer");
    QRect textRect = metrics.boundingRect(text);
    QPoint center(width()/2, height()*4/5);
    QPoint delta(textRect.width(), 0);

    textRect.moveCenter(center + delta);
    painter->drawText(textRect,  Qt::AlignCenter, text);

    if(m_selectedLine == m_options.size() + 1)
    {
        textRect.setWidth(textRect.width() + 20);
        textRect.moveLeft(textRect.left() - 10);
        textRect.setHeight(textRect.height() + 10);
        textRect.moveBottom(textRect.bottom() - 5);
        painter->drawRect(textRect);
    }

    // Annuler
    text = "Annuler";
    textRect = metrics.boundingRect(text);
    textRect.moveCenter(center - delta);
    painter->drawText(textRect,  Qt::AlignCenter, text);

    if(m_selectedLine == m_options.size())
    {
        textRect.setWidth(textRect.width() + 20);
        textRect.moveLeft(textRect.left() - 10);
        textRect.setHeight(textRect.height() + 10);
        textRect.moveBottom(textRect.bottom() - 5);
        painter->drawRect(textRect);
    }
}

void OptionWidget::changeLineChar(int delta)
{
    QDomElement option = m_options.at(m_selectedLine);
    QString value = optionValue(option);
    char c = (char)(value.at(m_cursor).toLatin1() + delta);
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

    value.replace(m_cursor, 1, QChar(c));
    option.setAttribute("value", value);
    m_options.replace(m_selectedLine, option);
}

void OptionWidget::moveCursor(int delta)
{
    QDomElement option = m_options.at(m_selectedLine);
    QString value = optionValue(option);

    m_cursor += delta;
    if(m_cursor < 0) m_cursor = value.size() - 1;
    if(m_cursor >= value.size())
    {
        m_cursor = value.size();
        value.resize(value.size() + 1);
        if(m_cursor == 0)
            value.replace(m_cursor, 1, QChar('A'));
        if(m_cursor > 0)
        {
            if(value.at(m_cursor - 1) == ' ')
                value.replace(m_cursor, 1, QChar('A'));
            else
                value.replace(m_cursor, 1, QChar(' '));
        }
        option.setAttribute("value", value);
        m_options.replace(m_selectedLine, option);
    }
}

void OptionWidget::moveSpin(int delta)
{
    QDomElement option = m_options.at(m_selectedLine);
    int value = optionValue(option).toInt() + delta;
    int min = option.attribute("min").toInt();
    int max = option.attribute("max").toInt();

    if(value < min)
        value = min;

    if(value > max)
        value = max;

    option.setAttribute("value", value);
    m_options.replace(m_selectedLine, option);

    emit spinChanged(option.attribute("name"), optionValue(option).toInt());
}

void OptionWidget::removeChar()
{
    QDomElement option = m_options.at(m_selectedLine);
    QString value = optionValue(option);

    value.remove(m_cursor--, 1);
    if(value.size() == 0)
    {
        m_cursor = 0;
        value.append(' ');
    }

    option.setAttribute("value", value);
    m_options.replace(m_selectedLine, option);
}

bool OptionWidget::isProfileCorrect() const
{
    QDomElement option = m_options.at(m_selectedLine);
    QString value = optionValue(option);
    return value.trimmed().size() > 2;
}

