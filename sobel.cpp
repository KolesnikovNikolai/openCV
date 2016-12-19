#include "sobel.h"

Sobel::Sobel()
{

}
double* Sobel::getKernel(QString type)
{
    if(type == "X") {
        return *kernelX;
    } else if(type == "Y") {
        return *kernelY;
    }
    return *kernelX;
}

int Sobel::getWidth()
{
    return width;
}

int Sobel::getHeight()
{
    return height;
}
