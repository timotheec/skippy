#include "Viewer3D.h"
#include "Utils.h"
#include "geometry/Sphere.h"

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

  scene.draw();

  // skippyPipeline->drawInputSkechesPoint();
  //  skippyPipeline->drawInputRays();
  if (!isPressed)
    skippyPipeline->drawOnSequence();
  //---------------------------------------

  mesh.draw();
}

void Viewer3D::drawWithNames() { scene.drawWithNames(); }

void Viewer3D::postSelection(const QPoint &point) {
  //  skippyPipeline->addSketchPoint(point, static_cast<Camera *>(camera()));
  skippy::PointSequence seqPoint;
  camera()->convertClickToLine(point, seqPoint.ray.orig, seqPoint.ray.dir);
  bool found;
  seqPoint.pos = camera()->pointUnderPixel(point, found);
  // Small offset to make point clearly visible.
  seqPoint.pos -= 0.01f * seqPoint.ray.dir;

  // TODO: tenporary (only 2 seqence of off points are possible)
  static unsigned int noSeq = 0;

  if (selectedName() >= 0 && found) {
    noSeq = 1;
    skippyPipeline->addToOnSequence(seqPoint);
  } else {
    // TODO : temporary distance to the first sphere
    seqPoint.heigth = static_pointer_cast<Sphere>(scene.getObjects()[0])
                          ->distanceToLine(seqPoint.ray);
    skippyPipeline->addToOffSequence(seqPoint);
    skippyPipeline->updateOffMaxHeight(seqPoint.heigth, noSeq);
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

void Viewer3D::mousePressEvent(QMouseEvent *e) {
  QGLViewer::mousePressEvent(e);
  if (e->button() == Qt::LeftButton && e->modifiers() != Qt::ControlModifier)
    isPressed = true;
}

void Viewer3D::mouseMoveEvent(QMouseEvent *e) {
  QGLViewer::mouseMoveEvent(e);
  if (!isPressed)
    return;

  beginSelection(e->pos());
  drawWithNames();
  endSelection(e->pos());
  postSelection(e->pos());

  update();
}

void Viewer3D::mouseReleaseEvent(QMouseEvent *e) {
  QGLViewer::mouseReleaseEvent(e);
  isPressed = false;

  skippyPipeline->computeOnPointsHeigth();
}

void Viewer3D::init() {
  makeCurrent();
  initializeOpenGLFunctions();

  setMouseTracking(true); // Needed for MouseGrabber.

  // Change standard action mouse bindings
  setMouseBinding(Qt::NoModifier, Qt::RightButton, NO_CLICK_ACTION);
  setMouseBinding(Qt::NoModifier, Qt::LeftButton, NO_CLICK_ACTION);
  setMouseBinding(Qt::ShiftModifier, Qt::LeftButton, NO_CLICK_ACTION);
  setMouseBinding(Qt::ControlModifier, Qt::LeftButton, CAMERA, ROTATE);
  setMouseBinding(Qt::ControlModifier, Qt::RightButton, CAMERA, TRANSLATE);

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
  text += "<li>timotheec</li>";
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

void Viewer3D::open_mesh() {
  mesh.open();
  adjustCamera(mesh.minPoint, mesh.maxPoint);
  update();
}

void Viewer3D::save_mesh() { mesh.save(); }

void Viewer3D::showControls() {
  //   Show controls :
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
