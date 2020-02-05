#include "SkippyGraph.h"
#include <algorithm>
#include <iostream>

SkippyGraph::SkippyGraph(const PointsSequence &onCandidates,
                         const PointsSequence &offRays) {
  buildOnSegments(onCandidates);
  createNodes();
  connectOnSegments();
  findAllLongestPaths();
}

void SkippyGraph::print() const {
  //  for (const auto &onSegment : nodes) {
  //    cout << onSegment << endl;
  //  }
  for (auto &path : paths) {
    for (auto &onSegment : path)
      cout << *onSegment;
    cout << endl;
  }
}

void SkippyGraph::buildOnSegments(const PointsSequence &onCandidates) {
  auto it = begin(onCandidates.pointsSeq);
  OnSegment firstOnSegment = {
      {{*it}}, it->intersectOrder, it->ray.index, it->ray.index};
  onSegments.push_back(firstOnSegment);
  ++it;

  for (auto end = std::end(onCandidates.pointsSeq); it != end; ++it) {
    bool candidateAdded = false;
    for (auto &onSegment : onSegments) {
      int intersectOrder = onSegment.intersectOrder;
      int endRayIndex = onSegment.endVertex;
      if (it->intersectOrder == intersectOrder &&
          it->ray.index == endRayIndex + 1) {
        onSegment.vertices.addPoint(*it);
        onSegment.endVertex = it->ray.index;
        candidateAdded = true;
        break;
      }
    }
    if (!candidateAdded) {
      OnSegment onSegment = {
          {{*it}}, it->intersectOrder, it->ray.index, it->ray.index};
      onSegments.push_back(onSegment);
    }
  }
}

void SkippyGraph::connectOnSegments() {
  for (uint i = 0; i < nodes.size(); i++)
    for (uint j = 0; j < nodes.size(); j++) {
      if (i == j)
        continue;
      if (nodes[j].onSegment->startVertex + 1 > nodes[i].onSegment->endVertex)
        nodes[i].adjacency.push_back(&nodes[j]);
    }
}

void SkippyGraph::createNodes() {
  for (auto &onSegment : onSegments)
    nodes.push_back({&onSegment, {}});
}

void SkippyGraph::findAllLongestPaths() {
  for (auto &node : nodes)
    findAllPaths(node, {});

  uint maxSize = 0;
  for (auto &path : paths)
    if (path.size() > maxSize)
      maxSize = path.size();

  // erase all smaller paths
  paths.erase(remove_if(paths.begin(), paths.end(),
                        [maxSize](vector<OnSegment *> const &path) {
                          return path.size() < maxSize;
                        }),
              paths.end());
}

void SkippyGraph::findAllPaths(const SkippyNode &node,
                               vector<OnSegment *> path) {
  path.push_back(node.onSegment);
  if (node.adjacency.empty()) {
    paths.push_back(path);
    return;
  }
  for (auto &adjNode : node.adjacency) {
    vector<OnSegment *> tmp(path);
    findAllPaths(*adjNode, tmp);
  }
}
