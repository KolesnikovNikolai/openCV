#ifndef INTERESTINGPOINTS_H
#define INTERESTINGPOINTS_H

#include <qimageextend.h>
#include <math.h>
#include <vector>

struct Point {
    int x, y;
    double value;
};

class InterestingPoints {
private:
    const int window_option = 2, window_save = 1, sigma = 1;
    const double moravek_step = 0.05, harris_step = 0.05, filter_ANMS = 0.9;
    QImageExtend image;
    std::vector<Point> points;
    void stepPoints(QImageExtend &image, double step);
public:
    InterestingPoints(QImageExtend &image);
    void moravek();
    void harris();
    void filterANMS(int count);
    QImageExtend getImagePoints();
    const std::vector<Point> & getPoints() const;
};

#endif // INTERESTINGPOINTS_H
