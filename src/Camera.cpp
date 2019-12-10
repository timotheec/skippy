#include "Camera.h"

#include <QFileDialog>
#include <fstream>

void Camera::saveCameraInFile(const QString &filename) {
  std::ofstream out(filename.toUtf8());
  if (!out)
    exit(EXIT_FAILURE);
  // << operator for point3 causes linking problem on windows
  out << position()[0] << " \t" << position()[1] << " \t" << position()[2]
      << " \t"
         " "
      << viewDirection()[0] << " \t" << viewDirection()[1] << " \t"
      << viewDirection()[2] << " \t"
      << " " << upVector()[0] << " \t" << upVector()[1] << " \t"
      << upVector()[2] << " \t"
      << " " << fieldOfView();
  out << std::endl;

  out.close();
}

void Camera::openCameraFromFile(const QString &filename) {

  std::ifstream file;
  file.open(filename.toStdString().c_str());

  qglviewer::Vec pos;
  qglviewer::Vec view;
  qglviewer::Vec up;
  float fov;

  file >> (pos[0]) >> (pos[1]) >> (pos[2]) >> (view[0]) >> (view[1]) >>
      (view[2]) >> (up[0]) >> (up[1]) >> (up[2]) >> fov;

  setPosition(pos);
  setViewDirection(view);
  setUpVector(up);
  setFieldOfView(fov);

  computeModelViewMatrix();
  computeProjectionMatrix();
}

void Camera::saveCamera() {
  QString fileName = QFileDialog::getSaveFileName(NULL, "", "*.cam");
  if (!fileName.isNull()) // got a file name
    saveCameraInFile(fileName);
}

void Camera::openCamera() {
  QString fileName = QFileDialog::getOpenFileName(NULL, "", "*.cam");
  if (!fileName.isNull()) // got a file name
    openCameraFromFile(fileName);
}
