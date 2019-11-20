#include "Viewer3D.h"

Viewer3D::Viewer3D(QGLWidget *parent)
    : QGLViewer(parent), QOpenGLFunctions_4_3_Core() {}

void Viewer3D::add_actions_to_toolBar(QToolBar *toolBar) {
  // Specify the actions :
  DetailedAction *open_mesh =
      new DetailedAction(QIcon(":icons/open.png"), "Open Mesh", "Open Mesh",
                         this, this, SLOT(open_mesh()));
  DetailedAction *save_mesh =
      new DetailedAction(QIcon(":icons/save.png"), "Save model", "Save model",
                         this, this, SLOT(save_mesh()));
  DetailedAction *help = new DetailedAction(QIcon(":icons/help.png"), "HELP",
                                            "HELP", this, this, SLOT(help()));
  DetailedAction *saveCamera =
      new DetailedAction(QIcon(":icons/camera.png"), "Save camera",
                         "Save camera", this, this, SLOT(saveCamera()));
  DetailedAction *openCamera =
      new DetailedAction(QIcon(":icons/open_camera.png"), "Open camera",
                         "Open camera", this, this, SLOT(openCamera()));
  DetailedAction *saveSnapShotPlusPlus = new DetailedAction(
      QIcon(":icons/save_snapshot.png"), "Save snapshot", "Save snapshot", this,
      this, SLOT(saveSnapShotPlusPlus()));

  // Add them :
  toolBar->addAction(open_mesh);
  toolBar->addAction(save_mesh);
  toolBar->addAction(help);
  toolBar->addAction(saveCamera);
  toolBar->addAction(openCamera);
  toolBar->addAction(saveSnapShotPlusPlus);
}

void Viewer3D::draw() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glColor3f(0.5, 0.5, 0.8);
  mesh.draw();
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

void Viewer3D::mousePressEvent(QMouseEvent *e) {
  QGLViewer::mousePressEvent(e);
}

void Viewer3D::mouseMoveEvent(QMouseEvent *e) { QGLViewer::mouseMoveEvent(e); }

void Viewer3D::mouseReleaseEvent(QMouseEvent *e) {
  QGLViewer::mouseReleaseEvent(e);
}

void Viewer3D::open_mesh() {
  bool success = false;
  QString fileName = QFileDialog::getOpenFileName(NULL, "", "");
  if (!fileName.isNull()) { // got a file name
    if (fileName.endsWith(QString(".off")))
      success = OFFIO::openTriMesh(fileName.toStdString(), mesh.vertices,
                                   mesh.triangles);
    else if (fileName.endsWith(QString(".obj")))
      success = OBJIO::openTriMesh(fileName.toStdString(), mesh.vertices,
                                   mesh.triangles);
    if (success) {
      std::cout << fileName.toStdString() << " was opened successfully"
                << std::endl;
      point3d bb(FLT_MAX, FLT_MAX, FLT_MAX), BB(-FLT_MAX, -FLT_MAX, -FLT_MAX);
      for (unsigned int v = 0; v < mesh.vertices.size(); ++v) {
        bb = point3d::min(bb, mesh.vertices[v]);
        BB = point3d::max(BB, mesh.vertices[v]);
      }
      adjustCamera(bb, BB);
      update();
    } else
      std::cout << fileName.toStdString() << " could not be opened"
                << std::endl;
  }
}

void Viewer3D::save_mesh() {
  bool success = false;
  QString fileName = QFileDialog::getOpenFileName(NULL, "", "");
  if (!fileName.isNull()) { // got a file name
    mesh.clear();
    if (fileName.endsWith(QString(".off")))
      success =
          OFFIO::save(fileName.toStdString(), mesh.vertices, mesh.triangles);
    else if (fileName.endsWith(QString(".obj")))
      success =
          OBJIO::save(fileName.toStdString(), mesh.vertices, mesh.triangles);
    if (success)
      std::cout << fileName.toStdString() << " was saved" << std::endl;
    else
      std::cout << fileName.toStdString() << " could not be saved" << std::endl;
  }
}

void Viewer3D::showControls() {
  // Show controls :
  controls->close();
  controls->show();
}

void Viewer3D::saveCameraInFile(const QString &filename) {
  std::ofstream out(filename.toUtf8());
  if (!out)
    exit(EXIT_FAILURE);
  // << operator for point3 causes linking problem on windows
  out << camera()->position()[0] << " \t" << camera()->position()[1] << " \t"
      << camera()->position()[2]
      << " \t"
         " "
      << camera()->viewDirection()[0] << " \t" << camera()->viewDirection()[1]
      << " \t" << camera()->viewDirection()[2] << " \t"
      << " " << camera()->upVector()[0] << " \t" << camera()->upVector()[1]
      << " \t" << camera()->upVector()[2] << " \t"
      << " " << camera()->fieldOfView();
  out << std::endl;

  out.close();
}

void Viewer3D::openCameraFromFile(const QString &filename) {

  std::ifstream file;
  file.open(filename.toStdString().c_str());

  qglviewer::Vec pos;
  qglviewer::Vec view;
  qglviewer::Vec up;
  float fov;

  file >> (pos[0]) >> (pos[1]) >> (pos[2]) >> (view[0]) >> (view[1]) >>
      (view[2]) >> (up[0]) >> (up[1]) >> (up[2]) >> fov;

  camera()->setPosition(pos);
  camera()->setViewDirection(view);
  camera()->setUpVector(up);
  camera()->setFieldOfView(fov);

  camera()->computeModelViewMatrix();
  camera()->computeProjectionMatrix();

  update();
}

void Viewer3D::openCamera() {
  QString fileName = QFileDialog::getOpenFileName(NULL, "", "*.cam");
  if (!fileName.isNull()) { // got a file name
    openCameraFromFile(fileName);
  }
}

void Viewer3D::saveCamera() {
  QString fileName = QFileDialog::getSaveFileName(NULL, "", "*.cam");
  if (!fileName.isNull()) { // got a file name
    saveCameraInFile(fileName);
  }
}

void Viewer3D::saveSnapShotPlusPlus() {
  QString fileName = QFileDialog::getSaveFileName(NULL, "*.png", "");
  if (!fileName.isNull()) { // got a file name
    setSnapshotFormat("PNG");
    setSnapshotQuality(100);
    saveSnapshot(fileName);
    saveCameraInFile(fileName + QString(".cam"));
  }
}
