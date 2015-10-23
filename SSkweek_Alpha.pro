#-------------------------------------------------
#
# Project created by QtCreator 2015-10-10T11:26:21
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSkweek_Alpha
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    game.cpp \
    level.cpp \
    soundplayer.cpp \
    grid.cpp \
    unit.cpp \
    player.cpp \
    monster.cpp \
    gameobject.cpp \
    projectile.cpp \
    arduinohandler.cpp \
    multiplayerupdater.cpp \
    weapon.cpp \
    tile.cpp \
    animation.cpp \
    projectilelist.cpp \
    weaponlist.cpp \
    menu.cpp

HEADERS  += mainwindow.h \
    game.h \
    level.h \
    soundplayer.h \
    grid.h \
    unit.h \
    player.h \
    monster.h \
    gameobject.h \
    projectile.h \
    arduinohandler.h \
    multiplayerupdater.h \
    weapon.h \
    tile.h \
    animation.h \
    projectilelist.h \
    weaponlist.h \
    menu.h

FORMS    +=

RESOURCES += \
    data.qrc
