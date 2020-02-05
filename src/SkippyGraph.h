#ifndef SKYPPYGRAPH_H
#define SKYPPYGRAPH_H

#include "Utils.h"

using namespace skippy;
using namespace std;

struct SkippyNode {
  OnSegment *onSegment;
  vector<SkippyNode *> adjacency;

  friend std::ostream &operator<<(std::ostream &out, const SkippyNode &node) {
    out << "{ " << *node.onSegment << ", {";
    for (auto &adjNode : node.adjacency)
      out << *(adjNode->onSegment) << ",";
    return out << " } }";
  }
};

class SkippyGraph {
public:
  SkippyGraph(const PointsSequence &onCandidates,
              const PointsSequence &offRays = PointsSequence());
  void print() const; // For debugging purpose

private:
  void buildOnSegments(const PointsSequence &onCandidates);
  void connectOnSegments();
  void createNodes();
  void findPaths();

  vector<OnSegment> onSegments;
  vector<SkippyNode> nodes;
  vector<vector<OnSegment *>> paths;
};

#endif // SKIPPYGRAPH_H
