#ifndef CAMERA_H
#define CAMERA_H

#include <QGLViewer/camera.h>

class Camera : public qglviewer::Camera {
public:
  void saveCamera();
  void openCamera();
  void saveCameraInFile(const QString &filename);

private:
  void openCameraFromFile(const QString &filename);
};

#endif // CAMERA_H
