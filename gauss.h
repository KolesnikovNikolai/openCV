#ifndef GAUSS_H
#define GAUSS_H

#include <math.h>


class Gauss
{
private:
    /**
     * @brief height - размерность по X
     */
    int height;
    /**
     * @brief width - размерность по Y
     */
    int width;

public:
    Gauss();
    double *getKernel(int height, int width, double sigma);
    /**
     * @brief getHeight - получить размерность по X
     * @return - размерность ядра свертки по X
     */
    int getHeight();
    /**
     * @brief getWidth - получить размерность по Y
     * @return - размерность ядра свертки по Y
     */
    int getWidth();
};

#endif // GAUSS_H
