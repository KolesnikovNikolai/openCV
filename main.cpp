#include <QCoreApplication>

#include <qimageextend.h>
#include <sobel.h>

void lb1();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    lb1();
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
