#ifndef UTILS_H
#define UTILS_H

#include <qglviewer.h>

namespace skippy {

struct Ray {
  qglviewer::Vec orig;
  qglviewer::Vec dir;
};

struct PointSquence {
  Ray ray;
  qglviewer::Vec pos;
  double heigth;
};

struct PointsSequence {
    std::vector<PointsSequence> pointsSeq;
};

} // namespace skippy

#endif // UTILS_H
