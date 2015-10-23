#include "menu.h"

Menu::Menu(Menu *parent)
    : QObject(parent), m_parent(parent)
{

}

Menu::Menu(const QDomElement &element, Menu *parent)
    : QObject(parent), m_parent(parent)
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

    if(parent == Q_NULLPTR)
    {
        createExitMenu(this);
    }
    else
    {
        createBackMenu(this);
    }
}

QStringList Menu::menusNames() const
{
    QStringList res;
    foreach(Menu* menu, m_subMenus)
        res << menu->name();

    return res;
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
}

Menu *Menu::createExitMenu(Menu *parent)
{
    Menu *menu = new Menu(parent);
    menu->setName("Quitter");
    parent->appendMenu(menu);
}
