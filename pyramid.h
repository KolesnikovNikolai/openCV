#ifndef PYRAMID_H
#define PYRAMID_H

#include <qimageextend.h>

class Pyramid
{
private:
    int levels;
    int octavs;
    QImageExtend image;
public:
    Pyramid(QImageExtend &image, int octavs, int levels);
    QImageExtend getImage(int octave, int level, double sigma);
};

#endif // PYRAMID_H
