#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <qglviewer.h>

namespace skippy {

struct Ray {
  qglviewer::Vec orig;
  qglviewer::Vec dir;
  int index = -1; // if index < 0, that's mean the index is invalid
};

struct PointSequence {
  Ray ray;
  qglviewer::Vec pos;
  double heigth;
  int intersectOrder;

  friend std::ostream &operator<<(std::ostream &out,
                                  const PointSequence &pointSeq) {
    return out << "{ pos : " << pointSeq.pos
               << "; heigth : " << pointSeq.heigth;
  }
};

struct OnPointSegment {
    std::vector<PointSequence> vertices;
    int intersectOrder;
    int startVertex, endVertex;
};

struct OffPointSegment {
    std::vector<PointSequence> vertices;
    int startIntersectOrder, endIntersectOrder;
    int startVertex, endVertex;
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
