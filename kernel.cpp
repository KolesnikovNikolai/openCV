#include "kernel.h"

Kernel::Kernel()
{
    this->height = this->width = 0;
}

Kernel::Kernel(int width, int height)
{
    this->height = height;
    this->width = width;
    this->kernel = std::make_unique<double []>(height * width);
}

Kernel Kernel::gauss(double sigma) const
{
    int size = (int) round(sigma);
    Kernel kernel(size * 2 + 1, size * 2 + 1);
    for (int y = 0; y <= 2 * size; ++y)
        for (int x = 0; x <= 2 * size; ++x)
            kernel.setValue(x,
                            y,
                            exp(-(x * x + y * y) / (2 * sigma * sigma)) / sqrt(2 * M_PI * sigma * sigma));
    return kernel;
}

int Kernel::getWidth() const
{
    return width;
}

int Kernel::getHeight() const
{
    return height;
}

double Kernel::getValue(int x, int y) const
{
    return this->kernel[x * this->height + y];
}

double Kernel::getValue(int index) const
{
    return this->kernel[index];
}

void Kernel::setValue(int x, int y, double value)
{
    this->kernel[x * this->height + y] = value;
}

Kernel::Kernel(Kernel &data)
{
    if(data.getWidth() != 0 && data.getHeight() != 0){
        this->kernel = std::make_unique<double[]>(data.getWidth() * data.getHeight());
        this->width = data.getWidth();
        this->height = data.getHeight();

        for(int x = 0; x < data.getWidth(); x++){
            for(int y = 0; y < data.getHeight(); y++){
                this->setValue(x,y,data.getValue(x,y));
            }
        }

    } else {
        printf("Error: Copy data is bad!\n");
        exit(-1);
    }
}

Kernel::Kernel(Kernel&& data)
{
    this->kernel = std::move(data.kernel);
    data.kernel = NULL;
    this->width = data.getWidth();
    this->height = data.getHeight();
}

Kernel& Kernel::operator=(Kernel&& data)
{
    if(this != &data){
        this->kernel = std::move(data.kernel);
        data.kernel = NULL;
        this->width = data.getWidth();
        this->height = data.getHeight();
    }
    return *this;
}

Kernel Kernel::gaussX(double sigma) const
{
    int size = (int) round(sigma);
    Kernel kernel(size * 2 + 1, 1);
    for (int i = 0; i <= 2 * size; ++i)
        kernel.setValue(i, 0, exp(-i*i/(2*sigma*sigma)) / (sqrt(2 * M_PI) * sigma));
    return kernel;
}

Kernel Kernel::gaussY(double sigma) const
{
    int size = (int) round(sigma);
    Kernel kernel(1, size * 2 + 1);
    for (int i = 0; i <= 2 * size; ++i)
        kernel.setValue(0, i, exp(-i*i/(2*sigma*sigma)) / (sqrt(2 * M_PI) * sigma));
    return kernel;
}

Kernel Kernel::sobelX() const
{
    double kerArr[9] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    Kernel kernel(3, 3);
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            kernel.setValue(x,y, kerArr[x*3 + y]);
    return kernel;
}

Kernel Kernel::sobelY() const
{
    double kerArr[9] = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1
    };
    Kernel kernel(3, 3);
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            kernel.setValue(x,y, kerArr[x*3 + y]);
    return kernel;
}
