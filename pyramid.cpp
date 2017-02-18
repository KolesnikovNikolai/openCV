#include "pyramid.h"

Pyramid::Pyramid(QImageExtend &image, int octavs, int levels)
{
    this->image = image;
    this->octavs = octavs;
    this->levels = levels;
}

QImageExtend Pyramid::getImage(int octave, int level, double sigma)
{
    if (this->image.getWidth() > 0 && this->image.getHeight() > 0 ) {
        if (this->octavs < octave) {
            octave = this->octavs;
        }
        if (this->levels < level) {
            level = this->levels;
        }
        Kernel gauss;

        double k = pow(2.0, 1.0 / this->levels);
        k = sigma * pow(k, level);
        double local = octave == 0? k: k*2*octave;

        return this->image
                .convolution(gauss.gaussX(local))
                .convolution(gauss.gaussY(local))
                .resize(octave);
    }
}
