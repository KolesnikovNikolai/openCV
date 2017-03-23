#include "image.h"

Image::Image(QString path)
{
    QImage image(path);
    if(image.isNull()) {
        this->width = this->height = 0;
    } else {
        this->width = image.width();
        this->height = image.height();
        this->pixels = std::make_unique<double[]>(this->height * this->width);
        for(int x = 0; x < this->width; ++x){
            for(int y = 0; y < this->height; ++y){
                this->setPixel(
                            x,
                            y,
                            qRed(image.pixel(x, y)),
                            qGreen(image.pixel(x, y)),
                            qBlue(image.pixel(x, y))
                            );
            }
        }
    }
}

Image::Image(int width, int height)
{
        this->width = width;
        this->height = height;
        this->pixels = std::make_unique<double[]>(this->height * this->width);
}

void Image::setPixel(int x, int y, int r, int g, int b)
{
    this->pixels[(x * this->height) + y] = std::min(1.0, (0.299 * r + 0.587 * g + 0.114 * b) / 255.0);
}

void Image::setPixel(int x, int y, double value)
{
    this->pixels[(x * this->height) + y] = std::min(1.0, value);
}

double Image::getPixel(int x, int y)
{
    while (std::abs(x) >= this->width) {
        x = this->width + this->width - std::abs(x) - 1;
    }
    while (std::abs(y) >= this->height) {
        y = this->height + this->height - std::abs(y) - 1;
    }
    return this->pixels[(std::abs(x) * this->height) + std::abs(y)];
}

void Image::save(QString path)
{
    QImage image = QImage(this->width, this->height, QImage::Format_RGB32);
    int value;
    for (int x = 0; x < this->width; ++x) {
        for (int y = 0; y < this->height; ++y) {
            value = this->getPixel(x, y) * 255.0;
            image.setPixel(x, y, qRgb(value, value, value));
        }
    }
    image.save(path);
    printf("\nsave");
}

void Image::save(QString path, std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> lines)
{
    QImage image = QImage(this->width, this->height, QImage::Format_RGB32);
    int value;
    for (int x = 0; x < this->width; ++x) {
        for (int y = 0; y < this->height; ++y) {
            value = this->getPixel(x, y) * 255.0;
            image.setPixel(x, y, qRgb(value, value, value));
        }
    }
    QPainter draw(&image);
    for (std::pair<std::pair<int, int>, std::pair<int, int>> line : lines) {
        draw.setPen(QColor(abs(rand()) % 256, abs(rand()) % 256, abs(rand()) % 256));
        draw.drawLine(line.first.first, line.first.second, line.second.first, line.second.second);
    }
    image.save(path);
    printf("\nsave");
}

int Image::getWidth() const
{
    return this->width;
}

int Image::getHeight() const
{
    return this->height;
}

Image::Image(Image &data)
{
    if(data.getWidth() != 0 && data.getHeight() != 0) {
        this->pixels = std::make_unique<double[]>(data.getWidth() * data.getHeight());
        this->width = data.getWidth();
        this->height = data.getHeight();
        for(int x = 0; x < data.getWidth(); ++x){
            for(int y = 0; y < data.getHeight(); ++y){
                this->setPixel(x, y, data.getPixel(x,y));
            }
        }
    } else {
        this->width = this->height = 0;
    }
}

Image::Image(Image&& data)
{
    this->pixels = std::move(data.pixels);
    data.pixels = NULL;
    this->width = data.getWidth();
    this->height = data.getHeight();
}

Image& Image::operator=(Image&& data)
{
    if(this != &data){
        this->pixels = std::move(data.pixels);
        data.pixels = NULL;
        this->width = data.getWidth();
        this->height = data.getHeight();
    }
    return *this;
}

Image Image::convolution(const Kernel &kernel)
{
    Image clone(*this);
    int kx = kernel.getWidth() / 2;
    int ky = kernel.getHeight() / 2;
    double value;
    for(int x = 0; x < width; ++x) {
        for(int y = 0; y < height; ++y) {
            value = 0.0;
            for(int i = -kx ; i <= kx; ++i) {
                for(int j = -ky; j <= ky; ++j) {
                    value += (kernel.getValue(i + kx, j + ky) *
                              this->getPixel(x + i,y + j));
                }
            }
            clone.setPixel(x, y, value);
        }
    }
    return std::move(clone);
}

Image Image::convolution(const Kernel &kernel, const Kernel &kernel_2)
{
    Image clone(*this);
    Image image_1 = this->convolution(kernel);
    Image image_2 = this->convolution(kernel_2);
    for(int x = 0; x < this->getWidth(); x++){
        for(int y = 0; y < this->getHeight(); y++) {
            clone.setPixel(x, y, sqrt(
                            (image_1.getPixel(x,y) * image_1.getPixel(x,y))
                            +
                            (image_2.getPixel(x,y) *image_2.getPixel(x,y)))
                        );
        }
    }
    return std::move(clone);
}

double Image::min()
{
    return *std::min_element(
                &(this->pixels[0]),
                &(this->pixels[this->width * this->height])
            );
}

double Image::max()
{
    return *std::max_element(
                &(this->pixels[0]),
                &(this->pixels[this->width * this->height])
            );
}

Image Image::normal()
{
    Image clone(*this);
    double min = this->min();
    double max = this->max();
    if (max != min) {
        double delta = max - min;
        for (int x = 0; x < this->width; ++x) {
            for (int y = 0; y < this->height; ++y) {
                clone.setPixel(x, y, (clone.getPixel(x, y) - min) / delta);
            }
        }
    }
    return std::move(clone);
}

Image Image::resize(int level)
{
    Image clone(*this);
    int size_k = level == 0? 1: 2 * level;
    clone.pixels = std::make_unique<double[]>(int(this->getWidth()/size_k) * int(this->getHeight()/size_k));
    clone.width = int(this->getWidth()/size_k);
    clone.height = int(this->getHeight()/size_k);
    for(int x = 0; x < clone.width; ++x){
        for(int y = 0; y < clone.height; ++y){
            clone.setPixel(int(x),int(y),this->getPixel(x*size_k,y*size_k));
        }
    }

    return std::move(clone);
}

Image& Image::operator=(Image &data)
{
    if(data.getWidth() != 0 && data.getHeight() != 0){
        this->pixels = std::make_unique<double[]>(data.getWidth() * data.getHeight());
        this->width = data.getWidth();
        this->height = data.getHeight();
        for(int x = 0; x < data.getWidth(); x++){
            for(int y = 0; y < data.getHeight(); y++){
                this->setPixel(x,y,data.getPixel(x,y));
            }
        }
    }
    return *this;
}

Image::Image()
{
    this->width = this->height = 0;
}
