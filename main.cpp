#include <QCoreApplication>

#include <qimageextend.h>
#include <pyramid.h>
#include <interestingpoints.h>
#include <kernel.h>

void lb1();
void lb2();
void lb3();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    lb3();
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
    QImageExtend resultImage(".//lb1.jpg");
    //сохранение в градациях серого
    resultImage.saveImage(".//lb1_res.bmp");
    //частная производная Собеля по Х
    QImageExtend sobleX = resultImage.convolution(sobel.sobelX());
    sobleX.saveImage("sobleX.bmp");
    //частная производная Собеля по Y
    QImageExtend sobleY = resultImage.convolution(sobel.sobelY());
    sobleY.saveImage("sobleY.bmp");
    //оператор Собеля
    QImageExtend soblel = resultImage.convolution(sobel.sobelX(),
                            sobel.sobelY());
    soblel.saveImage("sobleGrad.bmp");
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
    QImageExtend resultImage("lb1.jpg");
    QImageExtend gaussBlur = resultImage.convolution(gauss.gauss(3));

    gaussBlur.saveImage("gaussBlur.bmp");

    int octavs = 5;
    int layers = 3;
    double sigma = 1.0;
    Pyramid pyramid(resultImage, octavs, layers);
    for(int i = 0; i< octavs; ++i) {
        for(int j = 0; j< layers; ++j) {
            pyramid
                    .getImage(i, j, sigma)
                    .saveImage(QString::number(i) + QString::number(j) + "gaussBlur.bmp");
        }
    }

}

void lb3(){
    QImageExtend resultImage("lb1.jpg");
    InterestingPoints points(resultImage);
    points.moravek();
    QImageExtend result = points.getImagePoints();
    result.saveImage("moravek.png");
    points.filterANMS(150);
    points.getImagePoints().saveImage("moravekFilter.png");
    points.harris();
    result = points.getImagePoints();
    result.saveImage("harris.png");
    points.filterANMS(150);
    points.getImagePoints().saveImage("harrisFilter.png");
}
