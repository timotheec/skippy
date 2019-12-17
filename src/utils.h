#ifndef UTILS_H
#define UTILS_H

#include<qglviewer.h>

namespace skippy {

struct Ray{
    qglviewer::Vec orig;
    qglviewer::Vec dir;
};

} // namespace skippy

#endif // UTILS_H
