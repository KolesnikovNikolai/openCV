#include "interestingpoints.h"

InterestingPoints::InterestingPoints(QImageExtend &image)
{
    this->image = image;
}

void InterestingPoints::moravek() {
    QImageExtend img(this->image);
    for (int y = 0; y < this->image.getHeight(); ++y) {
        for (int x = 0; x < this->image.getWidth(); ++x) {
            double minValue = INFINITY;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    double value = 0;
                    for (int v = -this->window_option; v <= this->window_option; ++v) {
                        for (int u = -this->window_option; u <= this->window_option; ++u) {
                            auto tmp = this->image.getPixel(x+u, y+v) - image.getPixel(x+u+dx, y+v+dy);
                            value += tmp * tmp;
                        }
                    }
                    minValue = std::min(minValue, value);
                }
            }
            img.setPixel(x, y, minValue);
        }
    }
    stepPoints(img, this->moravek_step);
}

void InterestingPoints::harris() {
    Kernel kernel;
    QImageExtend img(this->image);
    QImageExtend sobelX = this->image.convolution(
                kernel.sobelX());
    QImageExtend sobelY = this->image.convolution(
                kernel.sobelY());
    kernel = kernel.gauss(this->sigma);
    int size = kernel.getWidth() / 2;
    for (int y = 0; y < image.getHeight(); ++y) {
        for (int x = 0; x < image.getWidth(); ++x) {
            double a = 0, b = 0, c = 0;
            for (int v = 0; v < kernel.getHeight(); ++v) {
                for (int u = 0; u < kernel.getWidth(); ++u) {
                    a += kernel.getValue(u*kernel.getHeight() + v)
                         * sobelX.getPixel(x+u-size, y+v-size) * sobelX.getPixel(x+u-size, y+v-size);
                    b += kernel.getValue(u*kernel.getHeight() + v)
                         * sobelX.getPixel(x+u-size, y+v-size) * sobelY.getPixel(x+u-size, y+v-size);
                    c += kernel.getValue(u*kernel.getHeight() + v)
                         * sobelY.getPixel(x+u-size, y+v-size) * sobelY.getPixel(x+u-size, y+v-size);
                }
            }
            double d = sqrt((a-c) * (a-c) + 4*b*b);
            double lambda1 = (a + c - d) / 2;
            double lambda2 = (a + c + d) / 2;
            img.setPixel(x, y, std::min(std::abs(lambda1), std::abs(lambda2)));
        }
    }
    stepPoints(img, this->harris_step);
}

void InterestingPoints::stepPoints(QImageExtend &contrast, double step) {
    points.clear();
    for (int y = 0; y < contrast.getHeight(); ++y) {
        for (int x = 0; x < contrast.getWidth(); ++x) {
            if (contrast.getPixel(x, y) < step) continue;
            bool next = false;
            for (int dy = -window_option; dy < window_option; ++dy) {
                for (int dx = -window_option; dx < window_option; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    if (contrast.getPixel(x+dx, y+dy) >= contrast.getPixel(x, y)) next = true;
                }
            }
            if (next) continue;
            auto point = Point();
            point.x = x;
            point.y = y;
            point.value = contrast.getPixel(x, y);
            points.emplace_back(point);
        }
    }
}


QImageExtend InterestingPoints::getImagePoints() {
    QImageExtend img(this->image);
    for (auto &point : points) {
        for (int i = -window_save; i <= window_save; ++i) {
            for (int j = -window_save; j <= window_save; ++j) {
                img.setPixel(point.x + i, point.y + j, 1);
            }
        }
    }
    return img;
}

void InterestingPoints::filterANMS(int count) {
    for (int k = 0; k < image.getWidth() + image.getHeight() && points.size() > count; ++k) {
        for (int i = 0; i < points.size(); ++i) {
            for (int j = i+1; j < points.size(); ++j) {
                if (sqrt((points[i].x - points[j].x) * (points[i].x - points[j].x)
                         + (points[i].y - points[j].y) * (points[i].y - points[j].y)) <= k
                    && this->filter_ANMS * points[i].value < points[j].value) {
                    points.erase(points.begin() + i);
                    i--;
                    break;
                }
            }
        }
    }
}
