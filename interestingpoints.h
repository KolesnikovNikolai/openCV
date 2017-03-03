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
    static bool comparePoint(Point a, Point b);
    //const int window_option = 2, window_save = 1, sigma = 1;
    //const double moravek_step = 0.05, harris_step = 0.05, filter_ANMS = 0.9;
    //void stepPoints(Image &image, double step);
public:
    InterestingPoints(Image &image);
    Image getImagePoints(int windows_size = 1);
    void moravek(double sensibility, int window_size = 1);
    Image filterANMS(int count, double robust = 0.9, int window_size = 1);
    void harris(double sensibility, double k = 0.05, int window_size = 1);
    //void filterANMS(int count);
    //const std::vector<Point> & getPoints() const;
};

#endif // INTERESTINGPOINTS_H
