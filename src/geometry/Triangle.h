#ifndef TRIANGLE_H
#define TRIANGLE_H

struct Triangle
{
    unsigned int corners[3];
    unsigned int &operator[](unsigned int c) { return corners[c]; }
    unsigned int operator[](unsigned int c) const { return corners[c]; }
    unsigned int size() const { return 3; }
};

#endif
