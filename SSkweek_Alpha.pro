#-------------------------------------------------
#
# Project created by QtCreator 2015-10-10T11:26:21
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network
QT       += xml
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSkweek_Alpha
TEMPLATE = app

SOURCES += main.cpp\
    gui/mainwindow.cpp \
    game.cpp \
    level.cpp \
    soundplayer.cpp \
    grid.cpp \
    gameobjets/unit.cpp \
    gameobjets/player.cpp \
    gameobjets/monster.cpp \
    gameobjets/gameobject.cpp \
    gameobjets/projectile.cpp \
    network/arduinohandler.cpp \
    network/multiplayerupdater.cpp \
    weapon.cpp \
    tile.cpp \
    animations/animation.cpp \
    projectilelist.cpp \
    weaponlist.cpp \
    gui/menu.cpp \
    gui/mainmenuwidget.cpp \
    gui/optionwidget.cpp \
    gui/messagebox.cpp \
    gui/lobbywidget.cpp \
    gui/gamewidget.cpp \
    playerinfo.cpp \
    animations/deathstarbeam.cpp \
    animations/bombtile.cpp

HEADERS  += gui/mainwindow.h \
    game.h \
    level.h \
    soundplayer.h \
    grid.h \
    gameobjets/unit.h \
    gameobjets/player.h \
    gameobjets/monster.h \
    gameobjets/gameobject.h \
    gameobjets/projectile.h \
    network/arduinohandler.h \
    network/multiplayerupdater.h \
    weapon.h \
    tile.h \
    animations/animation.h \
    projectilelist.h \
    weaponlist.h \
    gui/menu.h \
    gui/mainmenuwidget.h \
    gui/optionwidget.h \
    gui/messagebox.h \
    gui/lobbywidget.h \
    gui/gamewidget.h \
    playerinfo.h \
    animations/deathstarbeam.h \
    animations/bombtile.h

FORMS    +=

RESOURCES += \
    data.qrc
