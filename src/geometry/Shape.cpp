#include "Shape.h"
#include "gl/GLUtilityMethods.h"

queue<unsigned int> Shape::avaibleIds;
unsigned int Shape::idsCounter = 0;

Shape::Shape() {
  if (avaibleIds.empty()) {
    id = idsCounter;
    idsCounter++;
  } else {
    id = avaibleIds.front();
    avaibleIds.pop();
  }
}

void Shape::drawWithName() const {
  glPushName(id);
  draw();
  glPopName();
}
