#include "ninepatch.h"
#include <QRect>
#include <iostream>

TNinePatch::TNinePatch(QString fileName)
{
    image = new QImage(fileName);
}

void TNinePatch::draw(QPainter &painter, int x, int y, int width, int height)
{
       QPoint *pointTop = GetTopVerge();
       QPoint *pointLeft = GetLeftVerge();
       if(width < image->width()-(pointTop->y() - pointTop->x())) {
            width = image->width();
            std::cerr << "Incorrect width: autoresize to mimimum image size " << image->width() << " px" << std::endl;
       }
       if(height < image->height()-(pointLeft->y() - pointLeft->x())) {
           height = image->height();
           std::cerr << "Incorrect height: autoresize to minimum image size " << image->height() << " px" << std::endl;
       }
       painter.setRenderHint(QPainter::Antialiasing, true);

       painter.drawImage(x, y, *image, 1, 1, pointTop->x(), pointLeft->x());//верхний левый угол
       painter.drawImage(x, y + height -(image->height()-pointLeft->y() - 1), *image, 1, pointLeft->y(),
                         pointTop->x(), (image->height()-pointLeft->y() - 1));//левый нижний угол
       painter.drawImage(x + width - (image->width() - pointTop->y()-1),y, *image, pointTop->y(),1,
                         (image->width() - pointTop->y()-1),pointLeft->x());//верхний правый угол
       painter.drawImage(x + width - (image->width() - pointTop->y()-1), (y + height -(image->height()-pointLeft->y() - 1)), *image,
                         pointTop->y(), pointLeft->y(),(image->width() - pointTop->y()-1), (image->height()-pointLeft->y() - 1));//нижний правый угол
       int topResize = width - (image->width() - pointTop->y() - 1) - pointTop->x();//ресайз по горизонтали
       int leftResize = height - (image->height()-pointLeft->y() - 1)-pointLeft->x();//ресайз по вертикали
       QRect r;

       r.setX(pointTop->x());
       r.setY(1);
       r.setWidth(pointTop->y()-pointTop->x());
       r.setHeight(pointLeft->x());
       QImage img = image->copy(r);
       img = img.scaled(topResize, pointLeft->x());
       painter.drawImage(x+pointTop->x(), y, img, 0, 0, img.width(), img.height());//отрисовка верхней части

       r.setX(1);
       r.setY(pointLeft->x());
       r.setWidth(pointTop->x());
       r.setHeight(pointLeft->y()-pointLeft->x());
       img = image->copy(r);
       img = img.scaled(pointTop->x(), leftResize);
       painter.drawImage(x, y+pointLeft->x(), img, 0, 0, img.width(), img.height());//отрисовка левой части

       r.setX(pointTop->y());
       r.setY(pointLeft->x());
       r.setWidth(image->width()-pointTop->y()-1);
       r.setHeight(pointLeft->y()-pointLeft->x());
       img = image->copy(r);
       img = img.scaled(image->width() - pointTop->y()-1, leftResize);
       painter.drawImage(x+width-(image->width()-pointTop->y()-1), y+pointLeft->x(), img, 0, 0, img.width(), img.height());//отрисовка правой части

       r.setX(pointTop->x());
       r.setY(pointLeft->y());
       r.setHeight(image->height()-pointLeft->y()-1);
       r.setWidth(pointTop->y() - pointTop->x());
       img = image->copy(r);
       img = img.scaled(topResize, image->height()-pointLeft->y()-1);
       painter.drawImage(x+pointTop->x(), y + height - (image->height() - pointLeft->y() - 1), img,
                         0, 0, img.width(), img.height());//отрисовка нижней части

       r.setX(pointTop->x());
       r.setY(pointLeft->x());
       r.setWidth(pointTop->y() - pointTop->x());
       r.setHeight(pointLeft->y() - pointLeft->x());
       img = image->copy(r);
       img = img.scaled(topResize, leftResize);
       painter.drawImage(pointTop->x() + x, y+pointLeft->x(), img, 0, 0, img.width(), img.height());//отрисовка центральной части
}

QPoint *TNinePatch::GetLeftVerge()
{
    int i = 0;
    int top = 0;
    int bot = 0;
    for(int j = 0; j < image->height(); j++) {
        if(image->pixel(i, j) && top == 0) {
            top = j;
        } else {
            if(top && image->pixel(i, j)) {
                bot = j;
            }
        }
    }
    return (new QPoint(top, bot));
}

QPoint *TNinePatch::GetTopVerge()
{
    int j = 0;
    int left = 0;
    int right = 0;
    for(int i = 0; i < image->width(); i++) {
        if(image->pixel(i, j) != 0 && left == 0) {
            left = i;
        } else {
            if(left != 0 && image->pixel(i, j) != 0) {
                right = i;
            }
        }
    }
    return (new QPoint(left, right));
}
