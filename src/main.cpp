
#include "MainWindow.h"
#include "Viewer3D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Camera *camera = new Camera();
  SkippyPipeline *skippyPipeline = new SkippyPipeline();
  Viewer3D *viewer = new Viewer3D(camera, skippyPipeline);
  new MainWindow(viewer);

  return app.exec();
}
