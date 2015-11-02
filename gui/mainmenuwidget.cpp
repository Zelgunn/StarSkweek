#include "mainmenuwidget.h"

MainMenuWidget::MainMenuWidget(QWidget *parent)
    : QWidget(parent), m_selectMenuFrame(0), m_configLoaded(false), m_currentWidget(Q_NULLPTR), m_messageBox(this)
{
    // Menu
    QFile file(":/xml/xml/menus.xml");
    file.open(QIODevice::ReadOnly);

    QDomDocument dom;
    dom.setContent(&file);

    m_menu = new Menu(dom.documentElement());

    m_layout = new QGridLayout(this);
    this->setLayout(m_layout);

    m_optionsWidget =  new OptionWidget;

    m_configLoaded = openConfigFile();
    //createConfigFile("Zelgunn");
}

void MainMenuWidget::onRight()
{
    if(!m_configLoaded)
    {
        m_messageBox.moveCursor(1);
        return;
    }

    if((m_menu->hasOptions()) && (m_optionsWidget != Q_NULLPTR))
    {
        m_optionsWidget->onRight();
    }
}

void MainMenuWidget::onUp()
{
    if(!m_configLoaded)
    {
        m_messageBox.changeLineChar(1);
        return;
    }

    if((m_menu->hasOptions()) && (m_optionsWidget != Q_NULLPTR))
    {
        m_optionsWidget->selectPreviousOption();
    }
    else
    {
        m_menu->selectAboveMenu();
    }
}

void MainMenuWidget::onLeft()
{
    if(!m_configLoaded)
    {
        m_messageBox.moveCursor(-1);
        return;
    }

    if((m_menu->hasOptions()) && (m_optionsWidget != Q_NULLPTR))
    {
        m_optionsWidget->onLeft();
    }
}

void MainMenuWidget::onDown()
{
    if(!m_configLoaded)
    {
        m_messageBox.changeLineChar(-1);
        return;
    }

    if((m_menu->hasOptions()) && (m_optionsWidget != Q_NULLPTR))
    {
        m_optionsWidget->selectNextOption();
    }
    else
        m_menu->selectBelowMenu();
}

void MainMenuWidget::onEnter()
{
    if(!m_configLoaded)
    {
        if(m_messageBox.isProfileCorrect())
        {
            createConfigFile(m_messageBox.profile());
        }
        return;
    }

    if(m_selectMenuFrame != 0)
    {
        m_selectMenuFrame = 0;
        return;
    }


    if((m_menu->hasOptions()) && (m_optionsWidget != Q_NULLPTR))
    {
        if(m_optionsWidget->isCancelSelected())
            previousMenu();
        else if(m_optionsWidget->isApplySelected())
        {
            m_menu->setOptions(m_optionsWidget->options());
            saveConfig();
            previousMenu();
        }
        else
            m_optionsWidget->onEnter();
        return;
    }

    selectMenu();

    switch (m_menu->type())
    {
    case Menu::HostGame:
        emit hostGameSelected();
        break;
    case Menu::LocalGame:
        emit localGameSelected();
        break;
    case Menu::IPGame:
        break;
    case Menu::SimpleMenu:
        break;
    case Menu::OptionMenu:
        break;
    case Menu::Tutorial:
        break;
    }

}

void MainMenuWidget::onBackspace()
{
    if(!m_configLoaded)
    {
        m_messageBox.removeChar();
        return;
    }

    if((m_menu->hasOptions()) && (m_optionsWidget != Q_NULLPTR))
    {
        if(m_optionsWidget->isCancelSelected())
            previousMenu();
        else
            m_optionsWidget->onBackspace();
    }
    else
        previousMenu();
}

Menu *MainMenuWidget::optionsMenu() const
{
    Menu *menu = m_menu;
    while(menu->parent() != Q_NULLPTR) menu = menu->parent();
    QList<Menu *> subMenus = menu->subMenus();
    for(int i=0; i<subMenus.size(); i++)
    {
        if(subMenus.at(i)->name() == "Options")
        {
            menu = subMenus.at(i);
            return menu;
        }
    }
    return Q_NULLPTR;
}

QList<QDomElement> MainMenuWidget::allOptions() const
{
    QList<QDomElement> options;
    Menu *menu = optionsMenu();
    if(menu == Q_NULLPTR)
        return options;

    QList <Menu *> subMenus = menu->subMenus();
    for(int i=0; i<subMenus.size(); i++)
    {
        if(subMenus.at(i)->hasOptions())
        {
            options.append(subMenus.at(i)->options());
        }
    }

    return options;
}

int MainMenuWidget::musicVolume() const
{
    return spinValue("Musique");
}

int MainMenuWidget::mainVolume() const
{
    return spinValue("Principal");
}

int MainMenuWidget::spinValue(const QString &name) const
{
    QList<QDomElement> options = m_optionsWidget->options();
    foreach(QDomElement option, options)
    {
        if(option.attribute("name") == name)
        {
            return OptionWidget::optionValue(option).toInt();
        }
    }

    options = allOptions();
    foreach(QDomElement option, options)
    {
        if(option.attribute("name") == name)
        {
            return OptionWidget::optionValue(option).toInt();
        }
    }

    return -1;
}

OptionWidget *MainMenuWidget::optionsWidget() const
{
    return m_optionsWidget;
}

bool MainMenuWidget::isFullScreenChecked() const
{
    QList<QDomElement> options = allOptions();
    QDomElement option;

    for(int i=0; options.size(); i++)
    {
        option = options.at(i);

        if(option.attribute("name").startsWith("Plein"))
        {
            return OptionWidget::optionValue(option) == "true";
        }
    }

    return false;
}

void MainMenuWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setBrush(QBrush(QColor(255,255,255,100)));
    painter.setPen(QPen(QColor(255,255,255)));
    if(!m_configLoaded)
    {
        m_messageBox.drawNoProfileMessage();
    }
    else
    {
        paintMenu(&painter);
    }

    painter.end();
}

void MainMenuWidget::paintMenu(QPainter *painter)
{
    if(m_selectMenuFrame > 0)
    {
        m_selectMenuFrame = m_selectMenuFrame - 20;
        if(m_selectMenuFrame < 0) m_selectMenuFrame = 0;
    }

    if(m_selectMenuFrame < 0)
    {
        m_selectMenuFrame = m_selectMenuFrame + 20;
        if(m_selectMenuFrame > 0) m_selectMenuFrame = 0;
    }

    paintMenu(painter, m_menu, false);
    if(m_selectMenuFrame > 0)
    {
        paintMenu(painter, m_menu->parent(), true);
    }
    else if(m_selectMenuFrame < 0)
    {
        paintMenu(painter, m_menu->menuAt(m_menu->selectedMenu()), true);
    }
}

void MainMenuWidget::paintMenu(QPainter *painter, Menu *menu, bool offset)
{
    QStringList names = menu->menusNames();

    QPoint center(0, height()/2);

    if(!offset)
    {

        center += QPoint(width()/2, 0);
        QPoint nameCenter(center.x(), height()/8);
        paintTextMenu(painter, 42, menu->name(), nameCenter);
    }
    else if(m_selectMenuFrame < 0)
    {
        center += QPoint(width(), 0);
    }
    center += QPoint(m_selectMenuFrame, 0);

    if(names.size() == 0) return;

    QRect selectionRect, tmp;
    QPoint selectedCenter;

    for(int i=0; i<names.size(); i++)
    {
        tmp = paintTextMenu(painter, 20, names.at(i), center);
        if(tmp.width() > selectionRect.width())
            selectionRect = tmp;

        if(i == (int)menu->selectedMenu())
        {
            selectedCenter = center;
        }

        if(!offset)
        {
            center += QPoint(m_selectMenuFrame*(names.size())/(i+10), 60);
        }
        else
        {
            center += QPoint(0, 60);
        }
    }

    selectionRect.setWidth(selectionRect.width() + 20);
    selectionRect.setHeight(selectionRect.height() + 5);

    selectionRect.moveCenter(selectedCenter);
    painter->drawRect(selectionRect);
}

QRect MainMenuWidget::paintTextMenu(QPainter *painter, int fontSize, const QString &text, const QPoint &center)
{
    QFont font("Times", fontSize, QFont::Bold);
    painter->setFont(font);

    QFontMetrics metrics(font);
    QRect boundingRect = metrics.boundingRect(text);
    boundingRect.moveCenter(center);

    painter->drawText(boundingRect, Qt::AlignCenter, text);

    return boundingRect;
}

void MainMenuWidget::previousMenu()
{
    if(m_menu->hasOptions())
    {
        m_optionsWidget->setOptions(m_menu->options());
    }

    if(m_menu->parent() != Q_NULLPTR)
    {
        m_menu = m_menu->parent();
        m_selectMenuFrame = -width()/2;

        if(m_currentWidget)
        {
            m_currentWidget->hide();
        }
    }
}

void MainMenuWidget::selectMenu()
{
    Menu *selectedMenu = m_menu->menuAt(m_menu->selectedMenu());

    if(selectedMenu != Q_NULLPTR)
    {
        if(selectedMenu == m_menu->parent())
        {
            m_selectMenuFrame = -width()/2;
        }
        else
        {
            m_selectMenuFrame = width()/2;
        }

        if(selectedMenu->hasOptions())
        {
            showOptions(selectedMenu->options());
        }

        m_menu = selectedMenu;
    }

    if(m_menu->isExitMenu()) emit onExit();



//    if(m_menu->name() == "Local")
    //        emit onLocalSelected;
}

void MainMenuWidget::aboveMenu()
{
    m_menu->selectAboveMenu();
}

void MainMenuWidget::belowMenu()
{
    m_menu->selectBelowMenu();
}

void MainMenuWidget::showOptions(const QList<QDomElement> &options)
{
    if(m_currentWidget)
    {
        m_layout->removeWidget(m_currentWidget);
        //m_layout->replaceWidget(m_currentWidget, m_optionsWidget);
    }
//    else
//    {

//    }
    m_layout->addWidget(m_optionsWidget);

    m_optionsWidget->setOptions(options);
    m_currentWidget = m_optionsWidget;
    m_optionsWidget->show();
}

void MainMenuWidget::createConfigFile(const QString &profile)
{
    QDomDocument dom;
    QDomElement root, elem, option;
    QDomProcessingInstruction qdpi = dom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    dom.appendChild(qdpi);
    root = dom.createElement("root");
    dom.appendChild(root);

    QList <QDomElement> options = allOptions();

    for(int i=0; i<options.size(); i++)
    {
        option = options.at(i);
        elem = dom.createElement(option.tagName());
        elem.setAttribute("name", option.attribute("name"));
        if(option.attribute("name") == "Nom de profil")
            elem.setAttribute("value", profile);
        else
            elem.setAttribute("value", option.attribute("base"));
        root.appendChild(elem);
    }

    QFile file(QApplication::applicationDirPath() + "/config.xml");
    if (!file.open(QIODevice::WriteOnly))
    {
        file.close();
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << dom.toString();

    file.close();
    m_configLoaded = true;
    openConfigFile();
}

void MainMenuWidget::saveConfig() const
{
    QDomDocument dom;
    QDomElement root, elem, option;
    QDomProcessingInstruction qdpi = dom.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    dom.appendChild(qdpi);
    root = dom.createElement("root");
    dom.appendChild(root);

    QList <QDomElement> options = allOptions();

    for(int i=0; i<options.size(); i++)
    {
        option = options.at(i);
        elem = dom.createElement(option.tagName());
        elem.setAttribute("name", option.attribute("name"));
        elem.setAttribute("value", option.attribute("value"));
        root.appendChild(elem);
    }

    QFile file(QApplication::applicationDirPath() + "/config.xml");
    if (!file.open(QIODevice::WriteOnly))
    {
        file.close();
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << dom.toString();

    file.close();
}

bool MainMenuWidget::openConfigFile()
{
    QString configPath = QApplication::applicationDirPath() + "/config.xml";
    if(!QFile::exists(configPath))
        return false;

    QFile file(configPath);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    QDomDocument dom;
    if(!dom.setContent(&file))
        return false;

    QDomElement elem = dom.documentElement();
    QDomNode node = elem.firstChild();

    Menu *menu = optionsMenu();
    if(menu == Q_NULLPTR)
        return false;

    QList<Menu *> subMenus = menu->subMenus();

    while(!node.isNull())
    {
        elem = node.toElement();

        for(int i=0; i<subMenus.size(); i++)
        {
            menu = subMenus.at(i);
            if(menu->hasOption(elem.attribute("name")))
            {
                menu->setOption(elem.attribute("name"), elem.attribute("value"));
                break;
            }
        }

        node = node.nextSibling();
    }

    return true;
}

