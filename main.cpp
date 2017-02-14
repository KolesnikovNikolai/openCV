#include <QCoreApplication>

#include <qimageextend.h>
#include <sobel.h>
#include <gauss.h>
#include <pyramid.h>
#include <interestingpoints.h>

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
    Sobel sobel;
    //загрузка
    QImageExtend resultImage(".//lb1.jpg");
    //сохранение в градациях серого
    resultImage.saveImage(".//lb1_res.bmp");
    //частная производная Собеля по Х
    QImageExtend sobleX = resultImage.convolution(sobel.getKernel("X"),
                            sobel.getHeight(),
                            sobel.getWidth());
    sobleX.saveImage("sobleX.bmp");
    //частная производная Собеля по Y
    QImageExtend sobleY = resultImage.convolution(sobel.getKernel("Y"),
                            sobel.getHeight(),
                            sobel.getWidth());
    sobleY.saveImage("sobleY.bmp");
    //оператор Собеля
    QImageExtend soblel = resultImage.convolution(sobel.getKernel("X"),
                            sobel.getKernel("Y"),
                            sobel.getHeight(),
                            sobel.getWidth());
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
    Gauss gauss;
    //загрузка
    QImageExtend resultImage("lb1.jpg");
    double *kernel = gauss.getKernel(5,5,3);
    QImageExtend gaussBlur = resultImage.convolution(kernel,
                            gauss.getHeight(),
                            gauss.getWidth());

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
    points.getImagePoints().saveImage("harrisFilter.png");
    points.harris();
    result = points.getImagePoints();
    result.saveImage("harris.png");
    points.filterANMS(150);
    points.getImagePoints().saveImage("harrisFilter.png");
}
