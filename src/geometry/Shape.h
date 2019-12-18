#ifndef SHAPE_H
#define SHAPE_H

#include <queue>

using namespace std;

class Shape {
public:
  static unsigned int idsCounter;

  Shape();

  virtual ~Shape() { avaibleIds.push(id); }

  virtual void draw() const = 0;

  void drawWithName() const;

  Shape(const Shape &) : Shape() {}
  unsigned int getId() const { return id; }

protected:
  unsigned int id;

private:
  static queue<unsigned int> avaibleIds;
};

#endif // SHAPE_H
