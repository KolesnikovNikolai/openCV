#ifndef PYRAMID_H
#define PYRAMID_H

#include <vector>
#include <image.h>

class Pyramid
{
private:
    int levels;
    int octavs;
    std::vector<Image> images;

public:
    Pyramid(Image &image, int octavs, int levels);
    Image getImage(int octave, int level);
};

#endif // PYRAMID_H
