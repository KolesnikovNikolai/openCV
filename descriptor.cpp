#include "descriptor.h"

void Descriptor::normal(std::vector<double> &vector_desc)
{
    double normal = 0.0;
    for (double &d : vector_desc) {
        normal += d * d;
    }
    normal = sqrt(normal);
    for (double &d : vector_desc) {
        d = d/normal;
    }
}

void Descriptor::filterS(std::vector<double> &vector_desc, double sensibility)
{
    double normal = 0.0;
    for (double &d : vector_desc) {
        normal += d * d;
    }
    for (double &d : vector_desc) {
        d = std::min(d, sensibility);
    }
}

void Descriptor::setVectorDesc(Point point, std::vector<double> &descript)
{
    Kernel kernel;
    Image sX = image.convolution(kernel.sobelX());
    Image sY = image.convolution(kernel.sobelY());
    int size = this->gist_size * this->gist_count / 2;
    double yi, xi, angle, heft, pixel;
    for (int x = -size; x < size; ++x) {
        for (int y = -size; y < size; ++y) {
            int gist = ((x + size) * this->gist_size + (y + size) * this->gist_size * this->gist_count) / (this->gist_size * this->gist_size * this->gist_count);
            yi = sY.getPixel(point.x + x, point.y + y);
            xi = sX.getPixel(point.x + x, point.y + y);
            angle =  this->gist_dimension * (std::atan2(yi, xi) / M_PI + 1) / 2;
            heft = exp(-(x * x + y * y)/(8)) * sqrt(xi*xi + yi*yi) / sqrt(8 * M_PI);
            pixel = (1 - angle + (int)angle) * heft;
            int i = (int)angle + gist * this->gist_dimension;
            descript[(int)angle + gist * this->gist_dimension] += pixel;
            pixel = (angle - (int)angle) * heft;
            i= (int)(gist * this->gist_dimension + (int)angle + 1) % this->gist_dimension;
            descript[(int)(gist * this->gist_dimension + (int)angle + 1) % this->gist_dimension] += pixel;
        }
    }
}

Descriptor::Descriptor(Image &image, int count, double sensibility, double k)
{
    this->image = image;
    InterestingPoints points(this->image);
    points.harris(sensibility, k);
    std::vector<Point> points_filter_count = points.getPointsANMS(count);
    std::vector<double>* vector_desc;
    for (Point &point : points_filter_count) {
        vector_desc = new std::vector<double>(this->gist_dimension * this->gist_size * this->gist_size);
        this->setVectorDesc(point, *vector_desc);
        this->normal(*vector_desc);
        this->filterS(*vector_desc, sensibility);
        this->normal(*vector_desc);
        this->descriptors.push_back(std::make_pair(std::make_pair(point.x, point.y), *vector_desc));
    }
}

std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> Descriptor::getDescriptPoints(Descriptor &descript) {
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> points;
    double value, des_1, des_2;
    std::pair<int, int> min;
    std::unique_ptr<double[]> interval = std::make_unique<double []>(descript.descriptors.size() * descriptors.size());
    for (int i = 0; i < descriptors.size(); ++i) {
        for (int j = 0; j < descript.descriptors.size(); ++j) {
            value = 0;
            for (int k = 0; k < descriptors[i].second.size(); ++k) {
                value += (descript.descriptors[j].second[k] - descriptors[i].second[k]) * (descript.descriptors[j].second[k] - descriptors[i].second[k]);
            }
            interval[j + i * descript.descriptors.size()] =  value;
        }
        min.first = std::distance(&(interval[i * descript.descriptors.size()]), std::min_element(&(interval[i * descript.descriptors.size()]), &(interval[1 + i * descript.descriptors.size()])));
        min.second = std::distance(&(interval[i * descript.descriptors.size()]), std::max_element(&(interval[i * descript.descriptors.size()]), &(interval[1 + i * descript.descriptors.size()])));
        if (min.first == min.second) {
            min.second = 1;
        }
        for (int j = 0; j < descript.descriptors.size(); ++j) {
            if (interval[j + i * descript.descriptors.size()] < interval[min.second + i * descript.descriptors.size()]) {
                if (interval[j + i * descript.descriptors.size()] > interval[min.second + i * descript.descriptors.size()]) {
                    min.second = j;
                } else {
                    min.second = min.first;
                    min.first = j;
                }
            }
        }
        des_1 = interval[min.first + i * descript.descriptors.size()];
        des_2 = interval[min.second + i * descript.descriptors.size()];
        if (std::fabs(sqrt(des_1) - sqrt(des_2)) > 0.2) {
            points.emplace_back(std::make_pair(std::make_pair(descriptors[i].first.first,
                                                              descriptors[i].first.second),
                                               std::make_pair(descript.descriptors[min.first].first.first + image.getWidth(),
                                                              descript.descriptors[min.first].first.second)));
        }
    }
    return points;
}
