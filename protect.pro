QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choosefiguremode.cpp \
    figure.cpp \
    main.cpp \
    mainscene.cpp \
    monster.cpp \
    mybutton.cpp \
    player.cpp \
    playscene.cpp \
    programplayer.cpp \
    rand.cpp \
    tower.cpp

HEADERS += \
    choosefiguremode.h \
    figure.h \
    mainscene.h \
    monster.h \
    monsterAttrs.h \
    mybutton.h \
    player.h \
    playscene.h \
    programplayer.h \
    rand.h \
    tower.h \
    towerAttrs.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images/res.qrc
