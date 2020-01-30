#include "SkippyPipeline.h"
#include "geometry/Sphere.h"
#include "gl/GLUtilityMethods.h"

#include <iostream>

using namespace skippy;

SkippyPipeline::SkippyPipeline() {}

void SkippyPipeline::addSketchPoint(const QPoint &point, const Camera *camera) {
  inputSketchedPoints.push_back(point);
  bool found;
  projectedInptSketchedpoints.push_back(camera->pointUnderPixel(point, found));

  Ray rayUnderMouse;
  camera->convertClickToLine(point, rayUnderMouse.orig, rayUnderMouse.dir);
  inputRays.push_back(rayUnderMouse);
}

void SkippyPipeline::addToOnSequence(const PointSequence &point) {
  onSequence.addPoint(point);
}

void SkippyPipeline::addToOffSequence(const PointSequence &point) {
  offSequence.addPoint(point);
}

void SkippyPipeline::drawInputSkechesPoint() {
  for (auto point : projectedInptSketchedpoints) {
    Sphere sphere(point, 0.3);
    sphere.draw();
  }
}

void SkippyPipeline::drawInputRays() {
  const double DISPLAYED_RAY_LENGHT = 20.0;
  for (auto ray : inputRays) {
    qglviewer::Vec dest = ray.orig + DISPLAYED_RAY_LENGHT * ray.dir;
    glBegin(GL_LINES);
    glVertex3dv(ray.orig);
    glVertex3dv(dest);
    glEnd();
  }
}

void SkippyPipeline::drawOnSequence() {
  const double DISPLAYED_RAY_LENGHT = 20.0;
  for (auto pointSeq : onSequence.pointsSeq) {
    qglviewer::Vec dest =
        pointSeq.ray.orig + DISPLAYED_RAY_LENGHT * pointSeq.ray.dir;
    glBegin(GL_LINES);
    glVertex3dv(pointSeq.ray.orig);
    glVertex3dv(dest);
    glEnd();

    Sphere sphere(pointSeq.pos, 0.3);
    sphere.draw();
  }
}

void SkippyPipeline::drawOnCandidates() {
  const double DISPLAYED_RAY_LENGHT = 20.0;
  for (auto onCandidate : onCandidates.pointsSeq) {
    qglviewer::Vec dest =
        onCandidate.ray.orig + DISPLAYED_RAY_LENGHT * onCandidate.ray.dir;
    glBegin(GL_LINES);
    glVertex3dv(onCandidate.ray.orig);
    glVertex3dv(dest);
    glEnd();

    Sphere sphere(onCandidate.pos, 0.3);
    glColor3f(0.5f, 0.2f, onCandidate.intersectOrder);
    sphere.draw();
  }
}

void SkippyPipeline::updateOffMaxHeight(double heigth, unsigned int noSeq) {
  if (noSeq == 0 && heigth > offMaxHeigths.first)
    offMaxHeigths.first = heigth;
  else if (noSeq == 1 && heigth > offMaxHeigths.second)
    offMaxHeigths.second = heigth;
  else if (noSeq > 1) {
    cout << "Sequence numero have to be 0 or 1." << endl;
    exit(EXIT_FAILURE);
  }
}

void SkippyPipeline::computeOnPointsHeigth() {
  for (uint i = 0; i < onSequence.pointsSeq.size(); i++)
    onSequence.pointsSeq[i].heigth =
        lerp(offMaxHeigths.first, offMaxHeigths.second,
             double(i) / (onSequence.pointsSeq.size() - 1));
}

void SkippyPipeline::computeOnCandidates() {
  // TODO : for the moment only 2 intersetions are possible
  for (auto pointSeq : onSequence.pointsSeq) {
    qglviewer::Vec p1(0, 0, 0);
    qglviewer::Vec p2(0, 0, 0);

    // TODO : hardcoded iso sphere, I assume there is only one sphere (pos :
    // 0,0,0 and radius : 1)
    Sphere isoSphere(qglviewer::Vec(0, 0, 0), 1.0 + pointSeq.heigth);
    bool isIntersected = isoSphere.intersect(pointSeq.ray, p1, p2);
    if (isIntersected) {
      pointSeq.pos = p1;
      pointSeq.intersectOrder = 0;
      onCandidates.addPoint(pointSeq);

      if (p1 != p2) {
        pointSeq.pos = p2;
        pointSeq.intersectOrder = 1;
        onCandidates.addPoint(pointSeq);
      }
    }
  }
}
