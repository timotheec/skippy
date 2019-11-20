#ifndef MYVIEWER_H
#define MYVIEWER_H

// Mesh stuff:
#include "Mesh.h"

// Parsing:
#include "BasicIO.h"

// opengl and basic gl utilities:
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>
#include <QGLViewer/qglviewer.h>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <gl/openglincludeQtComp.h>

#include <gl/GLUtilityMethods.h>

// Qt stuff:
#include <QColorDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <QToolBar>

#include "qt/QSmartAction.h"

class Viewer3D : public QGLViewer, public QOpenGLFunctions_4_3_Core {
  Q_OBJECT

  Mesh mesh;

  QWidget *controls;

public:
  Viewer3D(QGLWidget *parent = NULL);

  void add_actions_to_toolBar(QToolBar *toolBar);

  void draw();

  void pickBackgroundColor();

  void adjustCamera(point3d const &bb, point3d const &BB);

  void init();

  QString helpString() const;

  void updateTitle(QString text);

  void keyPressEvent(QKeyEvent *event);

  void mouseDoubleClickEvent(QMouseEvent *e);

  void mousePressEvent(QMouseEvent *e);

  void mouseMoveEvent(QMouseEvent *e);

  void mouseReleaseEvent(QMouseEvent *e);

signals:
  void windowTitleUpdated(const QString &);

public slots:
  void open_mesh();

  void save_mesh();

  void showControls();

  void saveCameraInFile(const QString &filename);

  void openCameraFromFile(const QString &filename);

  void openCamera();
  void saveCamera();

  void saveSnapShotPlusPlus();
};

#endif // MYVIEWER_H
