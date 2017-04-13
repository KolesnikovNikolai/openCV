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

void Descriptor::setVectorDesc(Point point, std::vector<double> &descript, int gist_size, int gist_count, int gist_dimension, double rotation)
{
    int size = gist_size * gist_count / 2;
    double sin = std::sin(rotation);
    double cos = std::cos(rotation);
    double yi, xi, angle, heft, pixel;
    for (int x = -size; x < size; ++x) {
        for (int y = -size; y < size; ++y) {
            int rotationX = size +  int(sin * x + cos * y);
            int rotationY = size + int(cos * x - sin * y);
            if (rotationX >= 0 && rotationX <= size/2 && rotationY >= 0 && rotationY <= size/2) {
                int gist = (rotationX * gist_size + rotationY * gist_size * gist_count) / (gist_size * gist_size * gist_count);
                yi = sY.getPixel(point.x + x, point.y + y);
                xi = sX.getPixel(point.x + x, point.y + y);
                angle =  M_PI + std::atan2(yi, xi) - rotation;
                while(angle < 0) {
                    angle += M_PI * 2;
                }
                while(angle > M_PI * 2) {
                    angle -= M_PI * 2;
                }
                angle = gist_dimension * angle / M_PI / 2;
                heft = exp(-(x * x + y * y)/(8)) * sqrt(xi*xi + yi*yi) / sqrt(8 * M_PI);
                pixel = (1 - angle + (int)angle) * heft;
                int i = (int)angle + gist * gist_dimension;
                descript[(int)angle + gist * this->gist_dimension] += pixel;
                pixel = (angle - (int)angle) * heft;
                i= (int)(gist * gist_dimension + (int)angle + 1) % gist_dimension;
                descript[(int)(gist * gist_dimension + (int)angle + 1) % gist_dimension] += pixel;
            }
        }
    }
}

std::pair<int, int> Descriptor::getTwoBestIndexDesc(std::vector<double> &vector_desc)
{
    std::pair<int, int> max;
    max.first = std::distance(vector_desc.begin(), std::max_element(vector_desc.begin(), vector_desc.begin() + 1));
    max.second = std::distance(vector_desc.begin(), std::min_element(vector_desc.begin(), vector_desc.begin() + 1));
    if (max.first == max.second) {
        max.second = 1;
    }
    for (int j = 0; j < vector_desc.size(); ++j) {
        if (vector_desc[j] > vector_desc[max.second]) {
            if (vector_desc[j] < vector_desc[max.first]) {
                max.second = j;
            } else {
                max.second = max.first;
                max.first = j;
            }
        }
    }
    return max;
}

double Descriptor::getValueInterpolation(std::vector<double> &vector_desc, int best_index)
{
    double x1, x2, x3;
    int size = vector_desc.size();
    x2 = vector_desc[best_index];
    x3 = vector_desc[(size + 1) % size];
    if (best_index == 0) {
            x1 = vector_desc[size - 1];
    } else {
            x1 = vector_desc[best_index - 1];
    }
    return ((((x1 + x3)/2 - x2) * (2 * best_index + 1) + x2) - x3) / size * M_PI / ((x1 + x3)/2 - x2);
}

Descriptor::Descriptor(Image &image, int count, double sensibility, double k)
{
    this->image = image;
    Kernel kernel;
    this->sX = image.convolution(kernel.sobelX());
    this->sY = image.convolution(kernel.sobelY());
    InterestingPoints points(this->image);
    points.harris(sensibility, k);
    std::vector<Point> points_filter_count = points.getPointsANMS(count);
    std::vector<double>* vector_desc;
    std::vector<double>* vector_rotation;
    std::pair<int, int> max_rotation;
    for (Point &point : points_filter_count) {
        vector_rotation = new std::vector<double>(this->gist_dimension_rotation * this->gist_size_rotation * this->gist_size_rotation);
        this->setVectorDesc(point, *vector_rotation,
                            this->gist_size_rotation,
                            this->gist_count_rotation,
                            this->gist_dimension_rotation);
        max_rotation = this->getTwoBestIndexDesc(*vector_rotation);
        vector_desc = new std::vector<double>(this->gist_dimension * this->gist_size * this->gist_size);
        this->setVectorDesc(point, *vector_desc,
                            this->gist_size,
                            this->gist_count,
                            this->gist_dimension,
                            this->getValueInterpolation(*vector_rotation, max_rotation.first));
        this->normal(*vector_desc);
        this->filterS(*vector_desc, sensibility);
        this->normal(*vector_desc);
        this->descriptors.push_back(std::make_pair(std::make_pair(point.x, point.y), *vector_desc));
        if (vector_rotation->at(max_rotation.first) * 0.8 < vector_rotation->at(max_rotation.second)) {
            vector_desc = new std::vector<double>(this->gist_dimension * this->gist_size * this->gist_size);
            this->setVectorDesc(point, *vector_desc,
                                this->gist_size,
                                this->gist_count,
                                this->gist_dimension,
                                this->getValueInterpolation(*vector_rotation, max_rotation.second));
            this->normal(*vector_desc);
            this->filterS(*vector_desc, sensibility);
            this->normal(*vector_desc);
            this->descriptors.push_back(std::make_pair(std::make_pair(point.x, point.y), *vector_desc));
        }
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
                if (interval[j + i * descript.descriptors.size()] > interval[min.first + i * descript.descriptors.size()]) {
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
