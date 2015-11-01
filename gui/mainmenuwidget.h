#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QGridLayout>
#include <QTextStream>
#include <QApplication>

#include <QDebug>

#include "menu.h"
#include "optionwidget.h"
#include "messagebox.h"

class MainMenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainMenuWidget(QWidget *parent = 0);

    void onRight();
    void onUp();
    void onLeft();
    void onDown();
    void onEnter();
    void onBackspace();

    Menu *optionsMenu() const;
    QList<QDomElement> allOptions() const;

    bool isFullScreenChecked() const;

protected:
    void paintEvent(QPaintEvent *);

    void paintMenu(QPainter *painter);
    void paintMenu(QPainter *painter, Menu *menu, bool offset);
    QRect paintTextMenu(QPainter *painter, int fontSize, const QString &text, const QPoint &center);

    void previousMenu();
    void selectMenu();
    void aboveMenu();
    void belowMenu();

    void showOptions(const QList<QDomElement> &options);
    void createConfigFile(const QString &profile);
    void saveConfig() const;
    bool openConfigFile();

signals:
    void onExit();

private:
    Menu *m_menu;
    int m_selectMenuFrame;
    bool m_configLoaded;
    QGridLayout *m_layout;
    QWidget *m_currentWidget;
    OptionWidget *m_optionsWidget;
    QString m_profile;
    MessageBox m_messageBox;
};

#endif // MAINMENUWIDGET_H
