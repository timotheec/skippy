#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Viewer3D.h"

#include <QMainWindow>

class MainWindow : public QMainWindow {
  Q_OBJECT

private:
  Viewer3D *viewer = new Viewer3D;
  QToolBar *toolBar = new QToolBar;

  void setupUi();
  void setupToolbar();

public:
  explicit MainWindow(QWidget *parent = nullptr,
                      Viewer3D *viewer = new Viewer3D);
  ~MainWindow();

signals:

public slots:
};

#endif // MAINWINDOW_H
