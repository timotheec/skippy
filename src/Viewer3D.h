#ifndef MYVIEWER_H
#define MYVIEWER_H

#include "Camera.h"
#include "Mesh.h"
#include "SkippyPipeline.h"

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
#include "geometry/Scene3d.h"

class Viewer3D : public QGLViewer, public QOpenGLFunctions_4_3_Core {
  Q_OBJECT

  Mesh mesh;

  QWidget *controls;

  SkippyPipeline *skippyPipeline;

  bool isPressed = false;

  Scene3D scene;

public:
  Viewer3D(Camera *camera, SkippyPipeline *skippyPipeline);

  void draw() override;

  void postSelection(const QPoint &point) override;

  void drawWithNames() override;

  void pickBackgroundColor();

  void adjustCamera(point3d const &bb, point3d const &BB);

  void init() override;

  QString helpString() const override;

  void updateTitle(QString text);

  void keyPressEvent(QKeyEvent *event) override;

  void mouseDoubleClickEvent(QMouseEvent *e) override;

  void mousePressEvent(QMouseEvent *e) override;

  void mouseMoveEvent(QMouseEvent *e) override;

  void mouseReleaseEvent(QMouseEvent *e) override;

signals:
  void windowTitleUpdated(const QString &);

public slots:
  void open_mesh();

  void save_mesh();

  void showControls();

  void openCamera();
  void saveCamera();

  void saveSnapShotPlusPlus();
};

#endif // MYVIEWER_H
