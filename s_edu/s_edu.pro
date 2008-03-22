RESOURCES = s_edu.qrc
TEMPLATE = app
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../includes/
INCLUDEPATH += ../includes/camellia/

# Input
HEADERS += interfaces.h mainwindow.h pluginslist.h testwidget.h ../includes/translate.h ../includes/xcrypt.h ../includes/camellia/camellia.h
SOURCES += main.cpp mainwindow.cpp pluginslist.cpp testwidget.cpp ../includes/xcrypt.cpp ../includes/camellia/camellia.cpp
