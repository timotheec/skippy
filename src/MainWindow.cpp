#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, Viewer3D *viewer)
    : QMainWindow(parent), viewer(viewer) {
  setupUi();
}

MainWindow::~MainWindow() { delete viewer; }

void MainWindow::setupUi() {
  setupToolbar();

  QObject::connect(viewer, SIGNAL(windowTitleUpdated(QString)), this,
                   SLOT(setWindowTitle(QString)));
  viewer->updateTitle("Skippy");

  show();
}

void MainWindow::setupToolbar() {
  toolBar->setIconSize(QSize(35, 35));
  toolBar->setAutoFillBackground(true);
  toolBar->setStyleSheet("QToolBar { background: white; }");

  // Create the actions
  DetailedAction *open_mesh =
      new DetailedAction(QIcon(":icons/open.png"), "Open Mesh", "Open Mesh",
                         this, viewer, SLOT(open_mesh()));
  DetailedAction *save_mesh =
      new DetailedAction(QIcon(":icons/save.png"), "Save model", "Save model",
                         this, viewer, SLOT(save_mesh()));
  DetailedAction *help = new DetailedAction(QIcon(":icons/help.png"), "HELP",
                                            "HELP", this, viewer, SLOT(help()));
  DetailedAction *saveCamera =
      new DetailedAction(QIcon(":icons/camera.png"), "Save camera",
                         "Save camera", this, viewer, SLOT(saveCamera()));
  DetailedAction *openCamera =
      new DetailedAction(QIcon(":icons/open_camera.png"), "Open camera",
                         "Open camera", this, viewer, SLOT(openCamera()));
  DetailedAction *saveSnapShotPlusPlus = new DetailedAction(
      QIcon(":icons/save_snapshot.png"), "Save snapshot", "Save snapshot", this,
      viewer, SLOT(saveSnapShotPlusPlus()));

  // Add them
  toolBar->addAction(open_mesh);
  toolBar->addAction(save_mesh);
  toolBar->addAction(help);
  toolBar->addAction(saveCamera);
  toolBar->addAction(openCamera);
  toolBar->addAction(saveSnapShotPlusPlus);

  addToolBar(toolBar);

  setCentralWidget(viewer);
}
