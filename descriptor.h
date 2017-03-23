#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <vector>
#include <math.h>
#include <interestingpoints.h>
#include <kernel.h>

class Descriptor
{
private:
    int gist_size = 4, gist_count = 4, gist_dimension = 8;
    std::vector<std::pair<std::pair<int, int>, std::vector<double>>> descriptors;
    Image image;
    void normal(std::vector<double> &vector_desc);
    void filterS(std::vector<double> &vector_desc, double sensibility);
    void setVectorDesc(Point point, std::vector<double> &vector_desc);
public:
    Descriptor(Image &image, int count = 150, double sensibility = 0.05, double k = 0.05);
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> getDescriptPoints(Descriptor &descript);
};

#endif // DESCRIPTOR_H
