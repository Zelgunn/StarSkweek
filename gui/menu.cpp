#include "menu.h"

Menu::Menu(Menu *parent)
    : QObject(parent), m_parent(parent), m_selectedMenu(0)
{

}

Menu::Menu(const QDomElement &element, Menu *parent)
    : QObject(parent), m_baseElement(element), m_type(SimpleMenu), m_parent(parent), m_selectedMenu(0)
{
    QDomNode node = element.firstChild();
    QDomElement elem;

    m_name = element.attribute("name");

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Menu")
            m_subMenus.append(new Menu(elem, this));

        if(elem.tagName() == "Option")
        {
            m_type = OptionMenu;
            m_options.append(elem);
        }

        node = node.nextSibling();
    }

    if(m_subMenus.size() > 0)
    {
        if(parent == Q_NULLPTR)
        {
            createExitMenu(this);
        }
        else
        {
            createBackMenu(this);
        }
    }
}

QStringList Menu::menusNames() const
{
    QStringList res;
    foreach(Menu* menu, m_subMenus)
        res << menu->name();

    return res;
}

Menu *Menu::menuAt(int index) const
{
    if((index < 0) || (index >= m_subMenus.size()))
    {
        return Q_NULLPTR;
    }
    Menu *menu = m_subMenus.at(index);
    if(menu->isBackMenu()) return m_parent;
    return menu;
}

QString Menu::name() const
{
    return m_name;
}

void Menu::setName(const QString &name)
{
    m_name = name;
}

bool Menu::isBackMenu() const
{
    return (m_name == "Retour");
}

bool Menu::isExitMenu() const
{
    return (m_name == "Quitter");
}

bool Menu::hasOption(const QString &name)
{
    for(int i=0; i<m_options.size(); i++)
    {
        if(m_options.at(i).attribute("name") == name)
            return true;
    }

    return false;
}

bool Menu::hasOptions() const
{
    return (m_type == OptionMenu);
}

void Menu::setOptions(const QList<QDomElement> &options)
{
    m_options.clear();
    for(int i=0; i<options.size(); i++)
    {
        m_options.append(options.at(i).cloneNode().toElement());
    }
}

void Menu::setOption(const QString &name, const QString &value)
{
    QDomElement elem;
    for(int i=0; i<m_options.size(); i++)
    {
        elem = m_options.at(i);
        if(elem.attribute("name") == name)
        {
            elem.setAttribute("value", value);
            m_options.replace(i, elem);
            return;
        }
    }
}

void Menu::appendMenu(Menu *child)
{
    if(child != Q_NULLPTR)
        m_subMenus.append(child);
}

Menu *Menu::createBackMenu(Menu *parent)
{
    Menu *menu = new Menu(parent);
    menu->setName("Retour");
    parent->appendMenu(menu);

    return menu;
}

Menu *Menu::createExitMenu(Menu *parent)
{
    Menu *menu = new Menu(parent);
    menu->setName("Quitter");
    parent->appendMenu(menu);

    return menu;
}
uint Menu::selectedMenu() const
{
    return m_selectedMenu;
}

void Menu::setSelectedMenu(const uint &selectedMenu)
{
    m_selectedMenu = selectedMenu;
}

void Menu::selectAboveMenu()
{
    if(m_subMenus.size() == 0) return;
    m_selectedMenu --;
    if(m_selectedMenu < 0) m_selectedMenu = m_subMenus.size() - 1;
}

void Menu::selectBelowMenu()
{
    if(m_subMenus.size() == 0) return;
    m_selectedMenu = (m_selectedMenu + 1)%m_subMenus.size();
}

QList<Menu *> Menu::subMenus() const
{
    return m_subMenus;
}

QList<QDomElement> Menu::options() const
{
    return m_options;
}

Menu *Menu::parent() const
{
    return m_parent;
}



