#include "Viewer3D.h"

Viewer3D::Viewer3D(Camera *camera, SkippyPipeline *skippyPipeline)
    : QOpenGLFunctions_4_3_Core(), skippyPipeline(skippyPipeline) {
  // Change the camera
  qglviewer::Camera *c = Viewer3D::camera();
  setCamera(camera);
  delete c;
}

void Viewer3D::draw() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glColor3f(0.5, 0.5, 0.8);

  // TODO : remove !! Juste for testing----- (remove orig at the same time)
  BasicGL::drawSphere(0, 0, 0, 1, BasicGL::optimalSlices(1, 0.5f),
                      BasicGL::optimalStacks(1, 0.5f));

  BasicGL::drawSphere(orig.x, orig.y, orig.z, 0.3,
                      BasicGL::optimalSlices(0.3, 0.5f),
                      BasicGL::optimalStacks(0.5, 0.5f));
  //---------------------------------------

  //  mesh.draw();
}

void Viewer3D::pickBackgroundColor() {
  QColor _bc = QColorDialog::getColor(this->backgroundColor(), this);
  if (_bc.isValid()) {
    this->setBackgroundColor(_bc);
    this->update();
  }
}

void Viewer3D::adjustCamera(const point3d &bb, const point3d &BB) {
  point3d const &center = (bb + BB) / 2.f;
  setSceneCenter(qglviewer::Vec(center[0], center[1], center[2]));
  setSceneRadius(1.5f * (BB - bb).norm());
  showEntireScene();
}

void Viewer3D::init() {
  makeCurrent();
  initializeOpenGLFunctions();

  setMouseTracking(true); // Needed for MouseGrabber.

  setBackgroundColor(QColor(255, 255, 255));

  // Lights:
  GLTools::initLights();
  GLTools::setSunsetLight();
  GLTools::setDefaultMaterial();

  //
  glShadeModel(GL_SMOOTH);
  glFrontFace(GL_CCW); // CCW ou CW

  glEnable(GL_DEPTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_CLIP_PLANE0);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_COLOR_MATERIAL);

  //
  setSceneCenter(qglviewer::Vec(0, 0, 0));
  setSceneRadius(10.f);
  showEntireScene();
}

QString Viewer3D::helpString() const {
  QString text("<h2>Our cool project</h2>");
  text += "<p>";
  text += "This is a research application, it can explode.";
  text += "<h3>Participants</h3>";
  text += "<ul>";
  text += "<li>jmt</li>";
  text += "<li>...</li>";
  text += "</ul>";
  text += "<h3>Basics</h3>";
  text += "<p>";
  text += "<ul>";
  text += "<li>H   :   make this help appear</li>";
  text += "<li>Ctrl + mouse right button double click   :   choose background "
          "color</li>";
  text += "<li>Ctrl + T   :   change window title</li>";
  text += "</ul>";
  return text;
}

void Viewer3D::updateTitle(QString text) {
  this->setWindowTitle(text);
  emit windowTitleUpdated(text);
}

void Viewer3D::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_H) {
    help();
  } else if (event->key() == Qt::Key_T) {
    if (event->modifiers() & Qt::CTRL) {
      bool ok;
      QString text =
          QInputDialog::getText(this, tr(""), tr("title:"), QLineEdit::Normal,
                                this->windowTitle(), &ok);
      if (ok && !text.isEmpty()) {
        updateTitle(text);
      }
    }
  }
}

void Viewer3D::mouseDoubleClickEvent(QMouseEvent *e) {
  if ((e->modifiers() & Qt::ControlModifier) &&
      (e->button() == Qt::RightButton)) {
    pickBackgroundColor();
    return;
  }

  if ((e->modifiers() & Qt::ControlModifier) &&
      (e->button() == Qt::LeftButton)) {
    showControls();
    return;
  }

  QGLViewer::mouseDoubleClickEvent(e);
}

void Viewer3D::mousePressEvent(QMouseEvent *e) { isPressed = true; }

void Viewer3D::mouseMoveEvent(QMouseEvent *e) {
  if (!isPressed)
    return;

  // TODO : remove ----- get mouse ray in real world coordinate.
  bool found;
  orig = camera()->pointUnderPixel(e->pos(), found);
  // ------------------

  skippyPipeline->addSketchPoint(e->pos());

  update();
}

void Viewer3D::mouseReleaseEvent(QMouseEvent *e) {
  isPressed = false;
  skippyPipeline->drawInputSkechesPoint();
}

void Viewer3D::open_mesh() {
  mesh.open();
  adjustCamera(mesh.minPoint, mesh.maxPoint);
  update();
}

void Viewer3D::save_mesh() { mesh.save(); }

void Viewer3D::showControls() {
  // Show controls :
  controls->close();
  controls->show();
}

void Viewer3D::openCamera() {
  static_cast<Camera *>(camera())->openCamera();
  update();
}

void Viewer3D::saveCamera() { static_cast<Camera *>(camera())->saveCamera(); }

void Viewer3D::saveSnapShotPlusPlus() {
  QString fileName = QFileDialog::getSaveFileName(NULL, "*.png", "");
  if (!fileName.isNull()) { // got a file name
    setSnapshotFormat("PNG");
    setSnapshotQuality(100);
    saveSnapshot(fileName);
    static_cast<Camera *>(camera())->saveCameraInFile(fileName +
                                                      QString(".cam"));
  }
}
