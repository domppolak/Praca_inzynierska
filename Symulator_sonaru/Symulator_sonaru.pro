QT       += core gui opengl openglwidgets
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    inc/models/detectedpoint.cpp \
    inc/simulationthread.cpp \
    plotdialog.cpp \
    simulationwindow.cpp \
    src/air-parameters.cpp \
    src/filemanager.cpp \
    src/main.cpp \
    src/models/flatsurfacemodel.cpp \
    src/models/gridmodel.cpp \
    src/models/model.cpp \
    src/models/platformmodel.cpp \
    src/models/pointmodel.cpp \
    src/models/sonarmodel.cpp \
    src/models/stakemodel.cpp \
    src/scene/camera.cpp \
    src/scene/mainscene.cpp \
    src/scene/platformconfscene.cpp \
    src/scene/sonarconfscene.cpp \
    src/sim/platform.cpp \
    src/sim/receiver.cpp \
    src/sim/sonar.cpp \
    src/sim/transmittedsignal.cpp \
    src/sim/transmitter.cpp \
    src/simulation.cpp \
    src/windows/changesizewindow.cpp \
    src/windows/mainwindow.cpp \
    src/windows/objtoscene.cpp \
    src/windows/platformconfwindow.cpp \
    src/windows/signalconfwindown.cpp \
    src/windows/sonarconfwindow.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS += \
 inc/air-parameters.hh \
 inc/constant.hh \
    inc/filemanager.hh \
 inc/models/detectedpoint.hh \
 inc/models/flatsurfacemodel.hh \
 inc/models/gridmodel.hh \
    inc/models/model.hh \
    inc/models/platformmodel.hh \
 inc/models/pointmodel.hh \
    inc/models/sonarmodel.hh \
 inc/models/stakemodel.hh \
    inc/scene/camera.hh \
    inc/scene/mainscene.hh \
    inc/scene/platformconfscene.hh \
    inc/scene/sonarconfscene.hh \
    inc/sim/platform.hh \
    inc/sim/receiver.hh \
    inc/sim/sonar.hh \
    inc/sim/transmittedsignal.hh \
    inc/sim/transmitter.hh \
 inc/simulation.hh \
 inc/simulationthread.hh \
    inc/windows/changesizewindow.hh \
    inc/windows/mainwindow.hh \
    inc/windows/objtoscene.hh \
    inc/windows/platformconfwindow.hh \
    inc/windows/signalconfwindown.hh \
    inc/windows/sonarconfwindow.hh \
 plotdialog.hh \
    qcustomplot/qcustomplot.h \
 simulationwindow.hh

FORMS += \
    forms/changesizewindow.ui \
    forms/mainwindow.ui \
    forms/objtoscene.ui \
    forms/signalconfwindown.ui \
    forms/sonarConfWindow.ui \
    forms/platformconfwindow.ui \
    plotdialog.ui \
    simulationwindow.ui

TRANSLATIONS += \
    translation/Symulator_sonaru_pl_PL.ts

INCLUDEPATH += \
    inc\
    inc/models \
    inc/scene \
    inc/sim \
    inc/windows \
    src\
    src/models \
    src/scene \
    src/sim \
    src/windows \
    forms\
    translation\


CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

STATECHARTS +=

DISTFILES += \
 shaders/color_shader.frag \
 shaders/color_shader.vert

RESOURCES += \
 res.qrc
