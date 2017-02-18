#ifndef KERNEL_H
#define KERNEL_H

#include <memory>
#include <cmath>

class Kernel
{
private:
    int width, height;
    std::unique_ptr<double[]> kernel;
public:
    Kernel();
    Kernel(int width, int height);
    Kernel gauss(double sigma) const;
    Kernel gaussX(double sigma) const;
    Kernel gaussY(double sigma) const;
    Kernel sobelX() const;
    Kernel sobelY() const;
    /**
     * @brief getHeight - получить размерность по X
     * @return - размерность ядра свертки по X
     */
    int getHeight() const;
    /**
     * @brief getWidth - получить размерность по Y
     * @return - размерность ядра свертки по Y
     */
    int getWidth() const;
    
    double getValue(int x, int y) const;
    double getValue(int index) const;
    void setValue(int x, int y, double value);
    
    Kernel(Kernel &data);
    Kernel(Kernel&& data);
    Kernel& operator=(Kernel&& data);
};

#endif // KERNEL_H
