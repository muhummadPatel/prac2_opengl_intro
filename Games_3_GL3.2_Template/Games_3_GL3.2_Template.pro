HEADERS       = glheaders.h \
                glwidget.h \ 
    stlModel.h

SOURCES       = glwidget.cpp \
                main.cpp \
    stlModel.cpp
QT += core gui opengl widgets
LIBS += -lGLEW
RESOURCES += resources.qrc
#QMAKE_CXXFLAGS += -DSHOULD_LOAD_SHADERS
CONFIG += -std=c++11
# install
target.path = boom
INSTALLS += target
