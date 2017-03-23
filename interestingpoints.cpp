#include "interestingpoints.h"

InterestingPoints::InterestingPoints(Image &image)
{
    this->image = image;
}

std::vector<Point> InterestingPoints::getPointsANMS(int count, double robust)
{
    std::vector<Point> p = this->points;
    int r = 0;
    while (count < p.size() && r < std::max(this->image.getHeight(), this->image.getWidth())) {
        for(int x = 0; x < p.size(); ++x) {
            for(int y = 0; y < p.size(); ++y) {
                if (x != y) {
                    double size = sqrt((p[x].x - p[y].x) * (p[x].x - p[y].x) + (p[x].y - p[y].y) * (p[x].y - p[y].y));
                    if (size <= r && p[x].value < robust * p[y].value) {
                        p.erase(p.begin() + x);
                        --x;
                        break;
                    }
                }
            }
        }
        ++r;
    }
    return p;
}

Image InterestingPoints::filterANMS(int count, double robust, int window_size) {
    if(count < this->points.size()) {
        return this->getImagePoints(window_size, this->getPointsANMS(count, robust));
    } else {
        return this->getImagePoints(window_size);
    }
}

double InterestingPoints::getContrastValue(int x, int y) const {
    while (std::abs(x) >= this->image.getWidth()) {
        x = this->image.getWidth() + this->image.getWidth() - std::abs(x) - 1;
    }
    while (std::abs(y) >= this->image.getHeight()) {
        y = this->image.getHeight() + this->image.getHeight() - std::abs(y) - 1;
    }
    return this->contrast[(std::abs(x) * this->image.getHeight()) + std::abs(y)];
}

Image InterestingPoints::getImagePoints(int window_size) {
    Image img(this->image);
    for (Point &point : this->points) {
        for (int x = -window_size; x <= window_size; ++x) {
            for (int y = -window_size; y <= window_size; ++y) {
                if (point.x + x >= 0 && point.y + y >= 0 && point.x + x < img.getWidth() && point.y + y < img.getHeight()) {
                    img.setPixel(point.x + x, point.y + y, 1.0);
                }
            }
        }
    }
    return img;
}

Image InterestingPoints::getImagePoints(int window_size, std::vector<Point> points) {
    Image img(this->image);
    for (Point &point : points) {
        for (int x = -window_size; x <= window_size; ++x) {
            for (int y = -window_size; y <= window_size; ++y) {
                if (point.x + x >= 0 && point.y + y >= 0 && point.x + x < img.getWidth() && point.y + y < img.getHeight()) {
                    img.setPixel(point.x + x, point.y + y, 1.0);
                }
            }
        }
    }
    return img;
}

void InterestingPoints::filterS(int p_size, double sensibility) {
    bool maxi;
    this->points.clear();
    for (int x = 0; x < this->image.getWidth(); ++x) {
        for (int y = 0; y < this->image.getHeight(); ++y) {
            if(this->getContrastValue(x, y) > sensibility) {
                maxi = true;
                for(int px = -p_size; maxi && px <= p_size; ++px) {
                    for(int py = -p_size; maxi && py <= p_size; ++py) {
                        if (this->getContrastValue(x, y) < this->getContrastValue(x + px, y + py)) {
                            maxi = false;
                        }
                    }
                }
                if(maxi){
                    Point point;
                    point.x = x;
                    point.y = y;
                    point.value = this->getContrastValue(x, y);
                    this->points.push_back(point);
                }
            }
        }
    }
}


void InterestingPoints::harris(double sensibility, double k, int window_size) {
    Kernel kernel;
    Image sobelX = this->image.convolution(kernel.sobelX());
    Image sobelY = this->image.convolution(kernel.sobelY());
    this->contrast = std::make_unique<double[]>(this->image.getWidth() * this->image.getHeight());
    double a, b, c;
    for (int x = 0; x < image.getWidth(); ++x) {
        for (int y = 0; y < image.getHeight(); ++y) {
            a = 0;
            b = 0;
            c = 0;
            for (int u = -window_size; u < window_size; ++u) {
                for (int v = -window_size; v < kernel.getWidth(); ++v) {
                    a += sobelX.getPixel(x+u, y+v) * sobelX.getPixel(x+u, y+v);
                    b += sobelX.getPixel(x+u, y+v) * sobelY.getPixel(x+u, y+v);
                    c += sobelY.getPixel(x+u, y+v) * sobelY.getPixel(x+u, y+v);
                }
            }
            double det = a * c - b * b;
            double trace = a + c;
            this->contrast[x * this->image.getHeight() + y] = det - k * trace * trace;
        }
    }
    this->filterS(window_size, sensibility);
}

void InterestingPoints::moravek(double sensibility, int window_size) {
    this->contrast = std::make_unique<double[]>(this->image.getWidth() * this->image.getHeight());
    double min, value;
    for (int x = 0; x < this->image.getWidth(); ++x) {
        for (int y = 0; y < this->image.getHeight(); ++y) {
            min = std::numeric_limits<double>::max();
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx != 0 || dy != 0) {
                        value = 0.0;
                        for (int u = -window_size; u <= window_size; ++u) {
                            for (int v = -window_size; v <= window_size; ++v) {
                                    double diff = image.getPixel(x + u + dx, y + v + dy)
                                            - this->image.getPixel(x + u, y + v);
                                    value += diff * diff;
                            }
                        }
                        min = std::min(value, min);
                    }
                }
            }
            this->contrast[x * this->image.getHeight() + y] = min;
        }
    }
    this->filterS(window_size, sensibility);
}
