#ifndef UTILS_H
#define UTILS_H

#include <iostream>
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

  friend std::ostream &operator<<(std::ostream &out,
                                  const PointSequence &pointSeq) {
    return out << "{ pos : " << pointSeq.pos
               << "; heigth : " << pointSeq.heigth;
  }
};

struct PointsSequence {
  std::vector<PointSequence> pointsSeq;
  void addPoint(const PointSequence &point) { pointsSeq.push_back(point); }

  friend std::ostream &operator<<(std::ostream &out,
                                  const PointsSequence &pointsSeq) {
      out << "{ ";
      for(auto point : pointsSeq.pointsSeq)
          out << point << std::endl;
      return out << " }" << std::endl;
  }
};

inline double lerp(double v0, double v1, double t) { return (1 - t) * v0 + t * v1; }

} // namespace skippy

#endif // UTILS_H
