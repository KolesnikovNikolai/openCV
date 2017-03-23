#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <QString>
#include <QtGui/QImage>
#include <QPainter>
#include <kernel.h>

class Image
{
private:
    int height, width;
    std::unique_ptr<double[]> pixels;
    double min();
    double max();
public:
    Image(QString path);
    Image(int width, int height);
    Image(Image &data);
    Image(Image&& data);
    Image& operator=(Image&& data);
    void setPixel(int x, int y, double value);
    void setPixel(int x, int y, int r, int g, int b);
    double getPixel(int x, int y);
    void save(QString path);
    void save(QString path, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> lines);
    int getWidth() const;
    int getHeight() const;
    Image convolution(const Kernel &kernel);
    Image convolution(const Kernel &kernel, const Kernel &kernel_2);
    Image normal();
    Image resize(int level);
    Image& operator=(Image &data);
    Image();
};

#endif // IMAGE_H
