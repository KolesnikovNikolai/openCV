#ifndef QIMAGEEXTEND_H
#define QIMAGEEXTEND_H

#include <QString>
#include <QtGui/QImage>
#include <QRgb>
#include <QScopedPointer>
#include <memory>


class QImageExtend
{
private:
    /**
     * @brief height - высота изображения
     */
    int height;
    /**
     * @brief width - ширина изображения
     */
    int width;
    /**
     * @brief image - вектор пикселей
     */
    std::unique_ptr<double[]> image;
    /**
     * @brief format - формат изображения
     */
    QImage::Format format = QImage::Format_RGB32;
    /**
     * @brief setWidth - задает ширину изображения
     * @param width - ширина
     */
    void setWidth(int width);
    /**
     * @brief setHeight - задает высоту изображения
     * @param height - высота
     */
    void setHeight(int height);
    /**
     * @brief loadImage - загрузка изображения
     * @param path - путь к изображению
     */
    void loadImage(QString path);
    /**
     * @brief getMonoColor - показать градацию серого
     * @param color - исходный цвет
     * @return - цвет в сером
     */
    double getMonoColor(QRgb color);
    /**
     * @brief getColorPixel - получить цвет пикселя
     * @param x - координата по X
     * @param y - координата по Y
     * @return - цвет
     */
    QRgb getColorPixel(int x, int y);
    /**
     * @brief getIndex - получить индекс из вектора пикселей
     * @param x - координата по X
     * @param y - координата по Y
     * @param width - ширина изображения
     * @param height - высота изображения
     * @return - индекс пикселя
     */
    int getIndex(int x, int y, int width, int height);
    /**
     * @brief reflect - сворачиваем изображение для определения границ
     * @param x - координата по X,Y
     * @param maxX - ширина или высота изображения
     * @return - пиксель на границах
     */
    int reflect(int x, int maxX);
    /**
     * Переменные для нормирования
     */
    double min = 0;
    double max = 1;
    double minOrigial = 0;
    double maxOrigial = 256;
public:
    /**
     * @brief QImageExtend - создаем объект, загружая картинку
     * @param path - путь до картинки
     */
    QImageExtend(QString path);
    /**
     * @brief QImageExtend - конструктор копирования
     * @param data - копируемый объект
     */
    QImageExtend(QImageExtend &data);
    /**
     * @brief QImageExtend - конструктор перемещения
     * @param data - присваеваемый объект
     */
    QImageExtend(QImageExtend&& data);
    /**
     * @brief operator = - оператор перемещения
     * @param data - перемещаемая картинка
     * @return - картинка
     */
    QImageExtend& operator=(QImageExtend&& data);
    /**
     * @brief convolution - редактирование изображения с применением свертки
     * @param kernel - ядро свертки
     * @param column - размерность свертки
     * @param row - размерность свертки
     * @return - картинка с применением свертки
     */
    QImageExtend convolution(const double* kernel, int column, int row);
    /**
     * @brief convolution - редактирование изображения с использованием градиента свертки
     * @param kernel - ядро свертки 1
     * @param kernel_2 - ядро свертки 2
     * @param column - размерность свертки
     * @param row - размерность свертки
     * @return  - картинка с применением градиента свертки.
     */
    QImageExtend convolution(const double* kernel, const double* kernel_2, int column, int row);
    /**
     * @brief getWidth - получить ширину изображения
     * @return - ширина изображения
     */
    int getWidth();
    /**
     * @brief getHeight - получить высоту изображения
     * @return - высота изображения
     */
    int getHeight();
    /**
     * @brief saveImage - сохранить картинку
     * @param path - путь куда сохраняем картинку.
     */
    void saveImage(QString path);
    /**
     * @brief getPixel -показать пиксель
     * @param x - координата по X
     * @param y - координата по Y
     * @return - пиксель
     */
    double getPixel(int x, int y);
    /**
     * @brief setPixel - задаем пиксель
     * @param x - координата по X
     * @param y - координата по Y
     * @param color - цвет в сером
     */
    void setPixel(int x, int y, double color);

    QImageExtend resize(int level);
    QImageExtend& operator=(QImageExtend &data);
    QImageExtend();
};

#endif // QIMAGEEXTEND_H
