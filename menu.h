#ifndef MENU_H
#define MENU_H

#include <QObject>
#include <QStringList>
#include <QDomElement>

class Menu : public QObject
{
    Q_OBJECT

public:
    Menu(Menu *parent = Q_NULLPTR);
    Menu(const QDomElement &element, Menu *parent = Q_NULLPTR);

    QStringList menusNames() const;

    QString name() const;
    void setName(const QString &name);

    bool isBackMenu() const;
    bool isExitMenu() const;

    void appendMenu(Menu *child);
    static Menu *createBackMenu(Menu *parent);
    static Menu *createExitMenu(Menu *parent);

signals:
    void currentMenuChanged(const Menu *previousMenu, const Menu *menu);

private:
    Menu *m_parent;
    QString m_name;
    QList<Menu *> m_subMenus;
};

#endif // MENU_H
