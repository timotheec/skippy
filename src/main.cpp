
#include "MainWindow.h"
#include "Viewer3D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  new MainWindow;

  return app.exec();
}
