#include "gauss.h"

Gauss::Gauss()
{
    height = width = 0;
}
double* Gauss::getKernel(int height, int width, double sigma)
{
    this->height = height;
    this->width = width;
    double **kernel = new double* [width];
        for (int count = 0; count < width; count++)
            kernel[count] = new double [height];

    double sumTotal = 0;

    double distance = 0;

    double calculatedEuler = 1.0 /
    (2.0 * 3.1415926535897932385 * sigma * sigma);

    for (int filterY = 0; filterY < height; filterY++)
    {
        for (int filterX = 0; filterX < width; filterX++)
        {
            distance = ((filterX * filterX) +
                       (filterY * filterY)) /
                       (2 * (sigma * sigma));

            kernel[filterY][filterX] =
                   calculatedEuler * exp(-distance);

            sumTotal += kernel[filterY][filterX];
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            kernel[y][x] = kernel[y][x] * (1.0 / sumTotal);
        }
    }

    return *kernel;
}
int Gauss::getWidth()
{
    return this->width;
}

int Gauss::getHeight()
{
    return this->height;
}
