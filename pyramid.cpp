#include "pyramid.h"

Pyramid::Pyramid(Image &image, int octavs, int levels)
{
    Kernel gauss;
    this->octavs = octavs;
    this->levels = levels;
    double s0 = 0.5;
    double k = pow(2.0, 1.0 / this->levels);
    double sk = s0;
    double delta_sigma;
    double delta_sigma_level_prev = std::sqrt(2.56 -  s0 * s0);
    for (int i = 0; i < this->octavs; ++i) {
        sk = delta_sigma_level_prev;
        if (i != 0) {
            sk = sk * k;
            delta_sigma = std::sqrt(sk * sk - delta_sigma_level_prev * delta_sigma_level_prev);
            this->images.push_back(
                this->images[(i-1) * this->levels]
                    .convolution(gauss.gaussX(delta_sigma))
                    .convolution(gauss.gaussY(delta_sigma))
                    .resize(1)
            );
            sk /= 2;
            delta_sigma_level_prev = sk;
        } else {
            this->images.push_back(
                image
                    .convolution(gauss.gaussX(delta_sigma_level_prev))
                    .convolution(gauss.gaussY(delta_sigma_level_prev))
            );
        }
        for (int j = 0; j < this->levels-1; ++j) {
            sk = sk * k;
            delta_sigma = std::sqrt(sk * sk - delta_sigma_level_prev * delta_sigma_level_prev);
            this->images.push_back(
                this->images[i * this->levels + j]
                    .convolution(gauss.gaussX(delta_sigma))
                    .convolution(gauss.gaussY(delta_sigma))
            );
            delta_sigma_level_prev = sk;
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
    return this->images[octave * this->levels + level];
}
