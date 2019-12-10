TEMPLATE = app
TARGET = skippy
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ./src
QT += opengl xml
CONFIG += qt \
    release
MOC_DIR = ./tmp/moc
OBJECTS_DIR = ./tmp/obj

# Input
HEADERS += \
    src/BasicIO.h \
    src/Mesh.h \
    src/point3.h \
    src/gl/BasicColors.h \
    src/gl/GLUtilityMethods.h \
    src/gl/openglincludeQtComp.h \
    src/Viewer3D.h \
    src/MainWindow.h \
    src/geometry/Vertex.h \
    src/geometry/Triangle.h \
    src/Camera.h
SOURCES += \
    src/main.cpp \
    src/gl/BasicColors.cpp \
    src/gl/GLUtilityMethods.cpp \
    src/MainWindow.cpp \
    src/Viewer3D.cpp \
    src/Camera.cpp

EXT_DIR = extern

#QGLViewer
{
    unix:!macx: LIBS += -L$$PWD/extern/libQGLViewer-2.7.1/QGLViewer/ -lQGLViewer-qt5

    INCLUDEPATH += $$PWD/extern/libQGLViewer-2.7.1/QGLViewer
    DEPENDPATH += $$PWD/extern/libQGLViewer-2.7.1/QGLViewer
}

#GSL
{
    unix:!macx: LIBS += -L$$PWD/extern/gsl/lib/ -lgsl

    INCLUDEPATH += $$PWD/extern/gsl/include
    DEPENDPATH += $$PWD/extern/gsl/include
}

LIBS += -lglut \
    -lGLU
LIBS += -lblas \
    -lgomp
release:QMAKE_CXXFLAGS_RELEASE += -O3 \
    -fopenmp
release:QMAKE_CFLAGS_RELEASE += -O3 \
    -fopenmp

RESOURCES += \
    icons.qrc
