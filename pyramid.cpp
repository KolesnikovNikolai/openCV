#include "pyramid.h"

Pyramid::Pyramid(Image &image, int octavs, int levels)
{
    Kernel gauss;
    this->octavs = octavs;
    this->levels = levels;
    double s0 = 0.5;
    double k = pow(2.0, 1.0 / this->levels);
    double sk = s0;
    for (int i = 0; i < this->levels; ++i) {
        if (i != 0) {
            this->images.push_back(
                this->images[(i-1) * this->octavs]
                    .resize(1)
                    .convolution(gauss.gaussX(s0 * pow(2.0, i)))
                    .convolution(gauss.gaussY(s0 * pow(2.0, i)))
            );
        } else {
            this->images.push_back(
                image
                    .convolution(gauss.gaussX(s0))
                    .convolution(gauss.gaussY(s0))
            );
        }
        sk = s0;
        for (int j = 0; j < this->octavs-1; ++j) {
            sk = sk * k;
            this->images.push_back(
                this->images[i * this->octavs + j]
                    .convolution(gauss.gaussX(sk))
                    .convolution(gauss.gaussY(sk))
            );
        }
    }
}

Image Pyramid::getImage(int octave, int level)
{
    if (this->octavs < octave) {
        octave = this->octavs;
    }
    if (this->levels < level) {
        level = this->levels;
    }
    return this->images[level * this->octavs + octave];
}
