
#include "Viewer3D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  Viewer3D *viewer = new Viewer3D;

  QMainWindow *mainWindow = new QMainWindow;
  QToolBar *toolBar = new QToolBar;
  toolBar->setIconSize(QSize(35, 35));
  toolBar->setAutoFillBackground(true);
  toolBar->setStyleSheet("QToolBar { background: white; }");
  viewer->add_actions_to_toolBar(toolBar);

  mainWindow->addToolBar(toolBar);

  mainWindow->setCentralWidget(viewer);

  QObject::connect(viewer, SIGNAL(windowTitleUpdated(QString)), mainWindow,
                   SLOT(setWindowTitle(QString)));
  viewer->updateTitle("Skippy");

  mainWindow->show();

  return app.exec();
}
