#include "ninepatch.h"
#include <QRect>
#include <iostream>

TNinePatch::TNinePatch(QString fileName):
    image( new QImage(fileName) )
{
    contentArea = GetContentArea();//x-left bot point, width - distance bot line
                                   //y-top right point, height - distance right line
    resizeArea = GetResizeArea();//x-left top point, width - distance top line
                                 //y-top left point, height - distance left line
}

TNinePatch::~TNinePatch()
{
}

void TNinePatch::draw(QPainter& painter, int x, int y, int width, int height)
{
       contentArea = GetContentArea();
       if (width < image->width() && height < image->height()) {
           throw new ExceptionIncorrectWidthAndHeight(image->width(), image->height());
       }
       if (width < image->width()) {
            throw new ExceptionIncorrectWidth(image->width(), image->height());
       }
       if (height < image->height()) {
            throw new ExceptionIncorrectHeight(image->width(), image->height());
       }
       if ((resizeArea.width()+resizeArea.x()-1 != 0) && (resizeArea.height()+resizeArea.y()-1 != 0)) {
           painter.drawImage(x, y, *image, 1, 1,  resizeArea.x(), resizeArea.y());//верхний левый угол

           painter.drawImage(x, y + height -(image->height()-resizeArea.height()-resizeArea.y() ), *image, 1, (resizeArea.height() +resizeArea.y()-1),
                             resizeArea.x(), (image->height()-resizeArea.height()-resizeArea.y() ));//левый нижний угол

           painter.drawImage(x + width - (image->width() -resizeArea.width()-resizeArea.x()),y, *image, (resizeArea.width() +resizeArea.x()-1),1,
                             (image->width() -resizeArea.width()-resizeArea.x()),resizeArea.y());//верхний правый угол

           painter.drawImage(x + width - (image->width()- resizeArea.width()-resizeArea.x()),
                             (y + height -(image->height()-resizeArea.height()-resizeArea.y() )), *image,
                             (resizeArea.width() +resizeArea.x()-1), (resizeArea.height() +resizeArea.y()-1),(image->width()-resizeArea.width()-resizeArea.x() ),
                             (image->height()-resizeArea.height()-resizeArea.y() ));//нижний правый угол

           int topResize = width - (image->width() - resizeArea.width());//ресайз по горизонтали
           int leftResize = height - (image->height()-resizeArea.height());//ресайз по вертикали

           //отрисовка верхней части
           drawScaledPart(QRect(resizeArea.x(), 1, resizeArea.width(), resizeArea.y()),
                          QRect(x+resizeArea.x(), y, topResize, resizeArea.y()), painter);

           //отрисовка левой части
           drawScaledPart(QRect(1, resizeArea.y(), resizeArea.x(), resizeArea.height()),
                          QRect(x, y+resizeArea.y(), resizeArea.x(), leftResize), painter);

           //отрисовка правой части
           drawScaledPart(QRect(resizeArea.x()+resizeArea.width()-1, resizeArea.y(),image->width()-resizeArea.width()-resizeArea.x(), resizeArea.height()),
                          QRect(x+width-(image->width()-resizeArea.width()-resizeArea.x() ),
                                y+resizeArea.y(), image->width()- resizeArea.width()-resizeArea.x(), leftResize), painter);
           //отрисовка нижней части
           drawScaledPart(QRect(resizeArea.x(), (resizeArea.height() +resizeArea.y()-1), resizeArea.width(),
                                image->height()-resizeArea.height()-resizeArea.y() ),
                          QRect(x+resizeArea.x(),  y + height - (image->height()-resizeArea.height()-resizeArea.y() ),
                          topResize , image->height()-resizeArea.height()-resizeArea.y() ), painter);
           //отрисовка центральной части
           drawScaledPart(QRect(resizeArea.x(), resizeArea.y(), resizeArea.width(), resizeArea.height()),
                          QRect(resizeArea.x() + x, y+resizeArea.y(),
                          topResize , leftResize), painter);
       } else {
           throw new ExceptionNot9Patch;
       }
       contentArea = GetContentArea(width, height);
}

QRect TNinePatch::GetContentArea(int width, int height)
{
    return (QRect(contentArea.x(), contentArea.y(), (width - (image->width()-contentArea.width())),
                  (height - (image->height()-contentArea.height()))));
}

void TNinePatch::drawScaledPart(QRect oldRect, QRect newRect, QPainter& painter)
{
    QImage img = image->copy(oldRect);
    img = img.scaled(newRect.width(), newRect.height());
    painter.drawImage(newRect.x(), newRect.y(), img, 0, 0, img.width(), img.height());//отрисовка верхней части
}

QRect TNinePatch::GetContentArea()
{

    int j = 74;
    int left = 0;
    int right = 0;
    for(int i = 0; i < image->width(); i++) {
        if (image->pixel(i, j) == 4278190080 && left == 0) {
            left = i;
        } else {
            if (left != 0 && image->pixel(i, j) == 4278190080) {
                right = i;
            }
        }
    }
    int i = 75;
    int top = 0;
    int bot = 0;
    for(int j = 0; j < image->height(); j++) {
        if (image->pixel(i, j)  == 4278190080 && top == 0) {
            top = j;
        } else {
            if (top && image->pixel(i, j)  == 4278190080) {
                bot = j;
            }
        }
    }
    return (QRect(left, top, right+1-left, bot+1-top));
}

QRect TNinePatch::GetResizeArea()
{
    int j = 0;
    int left = 0;
    int right = 0;
    for(int i = 0; i < image->width(); i++) {
        if (image->pixel(i, j) == 4278190080 && left == 0) {
            left = i;
        } else {
            if (left != 0 && image->pixel(i, j) == 4278190080) {
                right = i;
            }
        }
    }
    int i = 0;
    int top = 0;
    int bot = 0;
    for(int j = 0; j < image->height(); j++) {
        if (image->pixel(i, j) == 4278190080 && top == 0) {
            top = j;
        } else {
            if (top && image->pixel(i, j) == 4278190080) {
                bot = j;
            }
        }
    }
    return (QRect(left, top, right+1-left, bot+1-top));
}
