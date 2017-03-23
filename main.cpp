#include <QCoreApplication>

#include <image.h>
#include <pyramid.h>
#include <interestingpoints.h>
#include <kernel.h>
#include <descriptor.h>

void lb1();
void lb2();
void lb3();
void lb4();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    lb4();
    return a.exec();
}
/**
 * @brief lb1 - лабараторная №1
 *
 * Написать основу для представления изображений и их обработки свертками
 * Реализовать вычисление частных производных и оператора Собеля
 *      (+раздельную обработку по осям для сепарабельных фильтров)
 *      (+различные варианты обработки краевых эффектов при свертке)
 * Реализовать отображение полученных результатов
 *      NB! Нормирование
 */
void lb1(){
    Kernel sobel;
    //загрузка
    Image resultImage("lb1.jpg");
    //сохранение в градациях серого
    resultImage.save("lb1_res.bmp");
    //частная производная Собеля по Х
    Image sobleX = resultImage.convolution(sobel.sobelX()).normal();
    sobleX.save("sobleX.bmp");
    //частная производная Собеля по Y
    Image sobleY = resultImage.convolution(sobel.sobelY()).normal();
    sobleY.save("sobleY.bmp");
    //оператор Собеля
    Image soblel = resultImage.convolution(sobel.sobelX(),
                            sobel.sobelY());
    soblel.save("sobleGrad.bmp");
}

/**
 * @brief lb2 - лабараторная №2
 *
 * Из заданного изображения построить гауссову пирамиду
 * Реализовать функцию L(x,y,sigma)
 * Реализовать отображение результатов
 */
void lb2(){
    Kernel gauss;
    //загрузка
    Image resultImage("lb1.jpg");

    Image gaussBlur = resultImage.convolution(gauss.gauss(3));

    gaussBlur.save("gaussBlur.bmp");

    int octavs = 5;
    int layers = 3;
    Pyramid pyramid(resultImage, octavs, layers);
    for(int i = 0; i< octavs; ++i) {
        for(int j = 0; j< layers; ++j) {
            pyramid
                    .getImage(i, j)
                    .save(QString::number(j) + QString::number(i) + "gaussBlur.bmp");
        }
    }

}

void lb3(){
    Image resultImage("lb1.jpg");
    InterestingPoints points(resultImage);
    //points.moravek(0.05);
    Image result = points.getImagePoints();
    result.save("moravek.png");
    points.filterANMS(150).save("moravekFilter150.bmp");
    points.filterANMS(200).save("moravekFilter200.bmp");
    points.harris(0.05);
    result = points.getImagePoints();
    result.save("harris.png");
    points.filterANMS(150).save("harrisFilter150.bmp");
}

void lb4(){
    Image im1("lb4_1.jpg"), im2("lb4_2.jpg");
    Image res = Image(im1.getWidth() + im2.getWidth(), std::max(im1.getHeight(), im2.getHeight()));
    for (int x = 0; x < im1.getWidth(); ++x) {
        for (int y = 0; y < im1.getHeight(); ++y) {
            res.setPixel(x, y, im1.getPixel(x, y));
        }
    }
    for (int x = 0; x < im2.getWidth(); ++x) {
        for (int y = 0; y < im2.getHeight(); ++y) {
            res.setPixel(im1.getWidth() + x, y, im2.getPixel(x, y));
        }
    }
    Descriptor des1(im1, 200), des2(im2, 200);
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> points = des1.getDescriptPoints(des2);
    res.save("lb4.jpg", points);
}
