#include "ninepatch.h"
#include <QRect>
#include <iostream>

<<<<<<< HEAD
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
           throw new MyExceptionWidthAndHeight;
       }
       if (width < image->width()) {
            throw new MyExceptionWidth;
       }
       if (height < image->height()) {
            throw new MyExceptionHeight;
       }
       if ((resizeArea.width() +resizeArea.x()-1) != 0 && (resizeArea.height() +resizeArea.y()-1) != 0) {
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
           throw new MyExceptionNot9Patch;
       }
       contentArea = resizeContentArea(width, height);
}

QRect TNinePatch::resizeContentArea(int width, int height)
{
    contentArea.setWidth( width - (image->width()-contentArea.width()));
    contentArea.setHeight(height - (image->height()-contentArea.height()));
    return (QRect(contentArea.x(), contentArea.y(), contentArea.width(), contentArea.height()));
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
=======
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
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
                bot = j;
            }
        }
    }
<<<<<<< HEAD
    return (QRect(left, top, right+1-left, bot+1-top));
}

QRect TNinePatch::GetResizeArea()
=======
    return (new QPoint(top, bot));
}

QPoint *TNinePatch::GetTopVerge()
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
{
    int j = 0;
    int left = 0;
    int right = 0;
    for(int i = 0; i < image->width(); i++) {
<<<<<<< HEAD
        if (image->pixel(i, j) == 4278190080 && left == 0) {
            left = i;
        } else {
            if (left != 0 && image->pixel(i, j) == 4278190080) {
=======
        if(image->pixel(i, j) != 0 && left == 0) {
            left = i;
        } else {
            if(left != 0 && image->pixel(i, j) != 0) {
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
                right = i;
            }
        }
    }
<<<<<<< HEAD
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
=======
    return (new QPoint(left, right));
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
}
