#ifndef UTILS_H
#define UTILS_H

#include <qglviewer.h>

namespace skippy {

struct Ray {
  qglviewer::Vec orig;
  qglviewer::Vec dir;
};

struct PointSequence {
  Ray ray;
  qglviewer::Vec pos;
  double heigth;
};

struct PointsSequence {
  std::vector<PointSequence> pointsSeq;
  void addPoint(const PointSequence &point) {
    pointsSeq.push_back(point);
  }
};

} // namespace skippy

#endif // UTILS_H
