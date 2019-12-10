#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Viewer3D.h"

#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  Viewer3D *viewer;
  QToolBar *toolBar = new QToolBar;

  void setupUi();
  void setupToolbar();

public:
  explicit MainWindow(Viewer3D *viewer);
  ~MainWindow();

signals:

public slots:
};

#endif // MAINWINDOW_H
