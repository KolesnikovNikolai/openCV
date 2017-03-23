#ifndef INTERESTINGPOINTS_H
#define INTERESTINGPOINTS_H

#include <image.h>
#include <math.h>
#include <vector>
#include <algorithm>

struct Point {
    int x, y;
    double value;
};

class InterestingPoints {
private:
    Image image;
    std::vector<Point> points;
    std::unique_ptr<double[]> contrast;
    void filterS(int p_size, double sensibility);
    double getContrastValue(int x, int y) const;
    Image getImagePoints(int window_size, std::vector<Point> points);
public:
    InterestingPoints(Image &image);
    Image getImagePoints(int windows_size = 1);
    void moravek(double sensibility, int window_size = 1);
    Image filterANMS(int count, double robust = 0.9, int window_size = 1);
    void harris(double sensibility, double k = 0.05, int window_size = 1);
    std::vector<Point> getPointsANMS(int count, double robust = 0.9);
};

#endif // INTERESTINGPOINTS_H
