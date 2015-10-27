#include "menu.h"

Menu::Menu(Menu *parent)
    : QObject(parent), m_parent(parent), m_selectedMenu(0)
{

}

Menu::Menu(const QDomElement &element, Menu *parent)
    : QObject(parent), m_parent(parent), m_selectedMenu(0)
{
    QDomNode node = element.firstChild();
    QDomElement elem;

    m_name = element.attribute("name");

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Menu")
            m_subMenus.append(new Menu(elem, this));

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
    m_selectedMenu --;
    if(m_selectedMenu < 0) m_selectedMenu = m_subMenus.size() - 1;
}

void Menu::selectBelowMenu()
{
    m_selectedMenu = (m_selectedMenu + 1)%m_subMenus.size();
}

QList<Menu *> Menu::subMenus() const
{
    return m_subMenus;
}
Menu *Menu::parent() const
{
    return m_parent;
}



