#ifndef SOBEL_H
#define SOBEL_H

#include <QString>

class Sobel
{
private:
    /**
     * @brief height - размерность по X
     */
    int const height = 3;
    /**
     * @brief width - размерность по Y
     */
    int const width = 3;
public:
    /**
     * @brief Sobel - контруктор
     */
    Sobel();
    /**
     * @brief kernelX - ядро свертки собеля по X
     */
    double kernelX[3][3] = {
        {-1.0, 0.0, 1.0},
        {-2.0, 0.0, 2.0},
        {-1.0, 0.0, 1.0}
    };
    /**
     * @brief kernelY - ядро свертки собеля по Y
     */
    double kernelY[3][3] = {
        {-1.0,-2.0,-1.0},
        { 0.0, 0.0, 0.0},
        { 1.0, 2.0, 1.0}
    };
    /**
     * @brief getKernel - получить ядро свертки
     * @param type - тип ядра "X","Y"
     * @return - ядро свертки
     */
    double *getKernel(QString type);
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

#endif // SOBEL_H
