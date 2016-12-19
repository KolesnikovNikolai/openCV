#include "qimageextend.h"

void QImageExtend::setWidth(int width)
{
    this->width = width;
}

void QImageExtend::setHeight(int height)
{
    this->height = height;
}

int QImageExtend::getWidth()
{
    return this->width;
}

int QImageExtend::getHeight()
{
    return this->height;
}

void QImageExtend::loadImage(QString file)
{
    QImage Picture(file);
    this->format = Picture.format();
    this->minOrigial = 9999.0;
    this->maxOrigial = -9999.0;

    if(Picture.width() != 0 && Picture.height() != 0){

        this->setWidth(Picture.width());
        this->setHeight(Picture.height());

        this->image = std::make_unique<double[]>(this->getWidth() * this->getHeight());

        for(int x = 0; x < this->getWidth(); x++){
            for(int y = 0; y < this->getHeight(); y++){
                this->setPixel(x,y,this->getMonoColor(Picture.pixel(x,y)));
            }
        }

        printf("Load succesfully! (%d x %d)\n",this->getWidth(), this->getHeight());
    } else {
        printf("Error: file not load!\n");
        exit(-1);
    }
}

void QImageExtend::saveImage(QString file)
{
    QImage Picture(this->getWidth(),this->getHeight(),this->format);

    this->max = this->getPixel(0,0);
    this->min = this->getPixel(0,0);

    for(int x = 0; x < this->getWidth()-1; x++){
        for(int y = 0; y < this->getHeight()-1; y++){

            if(this->max < this->getPixel(x,y))
                this->max = this->getPixel(x,y);
            if(this->min > this->getPixel(x,y))
                this->min = this->getPixel(x,y);
        }
    }
    for(int y = 0; y < this->getHeight(); y++){
        for(int x = 0; x < this->getWidth(); x++){
            Picture.setPixel(x, y, this->getColorPixel(x,y));
        }
    }

    Picture.save(file);

    printf("\nSave file.");
}

QRgb QImageExtend::getColorPixel(int x, int y)
{
    double color = this->getPixel(x,y);

    color = ((color - this->min) / (this->max - this->min))
            * (this->maxOrigial - this->minOrigial);
    return qRgb(color, color, color);

}

double QImageExtend::getPixel(int x, int y)
{
    return this->image[this->getIndex(x , y, this->getWidth(), this->getHeight())];
}

int QImageExtend::getIndex(int x, int y, int width, int height){
    return this->reflect(x, width) * height + this->reflect(y, height);
}

int QImageExtend::reflect(int x, int maxX)
{
    if(x >= maxX){
        return maxX - (x - maxX) - 1;
    }

    if(x < 0){
        return -1 * x;
    }

    return x;
}

double QImageExtend::getMonoColor(QRgb color)
{
    double colorOrigial = ((double)qRed(color) * 0.299 +
                           (double)qGreen(color) * 0.587 +
                           (double)qBlue(color) * 0.114);

    if(colorOrigial > this->maxOrigial) this->maxOrigial = colorOrigial;
    if(colorOrigial < this->minOrigial) this->minOrigial = colorOrigial;

    return colorOrigial / 255.0;
}

void QImageExtend::setPixel(int x, int y, double color)
{
    this->image[this->getIndex(x , y, this->getWidth(), this->getHeight())] = color;
}

QImageExtend::QImageExtend(QString path)
{
    this->loadImage(path);
}

QImageExtend::QImageExtend(QImageExtend &data)
{
    if(data.getWidth() != 0 && data.getHeight() != 0){
        this->image = std::make_unique<double[]>(data.getWidth() * data.getHeight());
        this->setWidth(data.getWidth());
        this->setHeight(data.getHeight());

        this->format = data.format;
        this->minOrigial = data.minOrigial;
        this->maxOrigial = data.maxOrigial;

        for(int x = 0; x < data.getWidth(); x++){
            for(int y = 0; y < data.getHeight(); y++){
                this->setPixel(x,y,data.getPixel(x,y));
            }
        }

    } else {
        printf("Error: Copy data is bad!\n");
        exit(-1);
    }
}

QImageExtend::QImageExtend(QImageExtend&& data)
{
    this->image = std::move(data.image);
    data.image = NULL;
    this->setWidth(data.getWidth());
    this->setHeight(data.getHeight());

    this->format = data.format;
    this->minOrigial = data.minOrigial;
    this->maxOrigial = data.maxOrigial;
}

QImageExtend& QImageExtend::operator=(QImageExtend&& data)
{
    if(this != &data){
        this->image = std::move(data.image);
        data.image = NULL;
        this->setWidth(data.getWidth());
        this->setHeight(data.getHeight());

        this->format = data.format;
        this->minOrigial = data.minOrigial;
        this->maxOrigial = data.maxOrigial;
    }
    return *this;
}

QImageExtend QImageExtend::convolution(const double *kernel,
                                       int column, int row)
{
    QImageExtend clone(*this);
    int x0, x1, y0, y1, index;
    double resultPixel;
    int width = this->getWidth();
    int height = this->getHeight();

    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++) {
            x0 = i - (column / 2);
            x1 = i + (column / 2);
            y0 = j - (row / 2);
            y1 = j + (row / 2);
            resultPixel = 0.0;
            index = column * row - 1;

            for(int y = y0 ; y <= y1; y++) {
                for(int x = x0; x <= x1; x++, index--){
                    resultPixel += (kernel[index] * this->getPixel(x,y));
                }
            }

            clone.setPixel(i,j,resultPixel);

        }
    }

    return std::move(clone);
}

QImageExtend QImageExtend::convolution(const double *kernel,
                                       const double *kernel_2,
                                       int column, int row)
{
    QImageExtend clone(*this);
    QImageExtend image_1 = this->convolution(kernel, column, row);
    QImageExtend image_2 = this->convolution(kernel_2, column, row);
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
