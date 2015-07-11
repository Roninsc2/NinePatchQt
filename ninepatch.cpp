#include "ninepatch.h"
#include <QRect>
#include <iostream>

TNinePatch::TNinePatch(QImage& image)
    : Image(image)
{
    ContentArea = GetContentArea();
    GetResizeArea();
    if (!ResizeDistancesX.size() || !ResizeDistancesY.size()) {
        throw new ExceptionNot9Patch;
    }
}

TNinePatch::~TNinePatch() {
}

void TNinePatch::Draw(QPainter& painter, int  x, int  y, int  width, int  height) {
       if (width < (Image.width() - 2) && height < (Image.height() - 2)) {
           throw new ExceptionIncorrectWidthAndHeight(Image.width() - 2 , Image.height() - 2 );
       }
       if (width < (Image.width() - 2)) {
            throw new ExceptionIncorrectWidth(Image.width() - 2 , Image.height() - 2 );
       }
       if (height < (Image.height() - 2)) {
            throw new ExceptionIncorrectHeight(Image.width() - 2 , Image.height() - 2 );
       }
       if (width == old_widht && height == old_height) {
           painter.drawImage(x, y, NewImage);
       } else {
           old_widht = width;
           old_height = height;
           GetNewImage(width, height);
           painter.drawImage(x, y, NewImage);
       }
}

QRect TNinePatch::GetContentArea(int  width, int  height) {
    return (QRect(ContentArea.x(), ContentArea.y(), (width - (Image.width() - 2 -ContentArea.width())),
                  (height - (Image.height() - 2 -ContentArea.height()))));
}

void TNinePatch::DrawScaledPart(QRect oldRect, QRect newRect, QPainter& painter) {
    if (newRect.width() && newRect.height()) {
        QImage img = Image.copy(oldRect);
        img = img.scaled(newRect.width(), newRect.height());
        painter.drawImage(newRect.x(), newRect.y(), img, 0, 0, newRect.width(), newRect.height());
    }
}

void TNinePatch::DrawConstPart(QRect oldRect, QRect newRect, QPainter& painter) {
    QImage img = Image.copy(oldRect);
    painter.drawImage(newRect.x(), newRect.y(), img, 0, 0, newRect.width(), newRect.height());
}

inline bool IsColorBlack(QRgb color) {
    auto r = qRed(color);
    auto g = qGreen(color);
    auto b = qBlue(color);
    auto a = qAlpha(color);
    if (a < 128) {
        return false;
    }
    return (r < 128 && g < 128 && b < 128);
}

QRect TNinePatch::GetContentArea() {
    int  j = Image.height()-1;
    int  left = 0;
    int  right = 0;
    for(int  i = 0; i < Image.width() ; i++) {
        if (IsColorBlack(Image.pixel(i, j)) && left == 0) {
            left = i;
        } else {
            if (left != 0 && IsColorBlack(Image.pixel(i, j))) {
                right = i;
            }
        }
    }
    if (left && !right) {
        right = left;
    }
    left -= 1;
    int  i = Image.width()-1;
    int  top = 0;
    int  bot = 0;
    for(int  j = 0; j < Image.height() ; j++) {
        if (IsColorBlack(Image.pixel(i, j)) && top == 0) {
            top = j;
        } else {
            if (top && IsColorBlack(Image.pixel(i, j))) {
                bot = j;
            }
        }
    }
    if (top && !bot) {
        bot = top;
    }
    top -= 1;
    return (QRect(left, top, right-left, bot-top));
}

void TNinePatch::GetResizeArea() {
    int  j = 0;
    int  left = 0;
    int  right = 0;
    for(int  i = 0; i < Image.width(); i++) {
        if (IsColorBlack(Image.pixel(i, j)) && left == 0) {
            left = i;
        }
        if (left && IsColorBlack(Image.pixel(i, j)) && !IsColorBlack(Image.pixel(i+1, j))) {
            right = i;
            left -= 1;
            ResizeDistancesX.push_back(std::make_pair(left, right - left));
            right = 0;
            left = 0;
        }
    }
    int  i = 0;
    int  top = 0;
    int  bot = 0;
    for(int  j = 0; j < Image.height(); j++) {
        if (IsColorBlack(Image.pixel(i, j)) && top == 0) {
            top = j;
        }
        if (top && IsColorBlack(Image.pixel(i, j)) && !IsColorBlack(Image.pixel(i, j+1))) {
            bot = j;
            top -= 1;
            ResizeDistancesY.push_back(std::make_pair(top, bot - top));
            top = 0;
            bot = 0;
        }
    }
    //
}

void TNinePatch::GetFactor(size_t width, size_t height, double& factorX, double& factorY) {
    int topResize = width - (Image.width() - 2);
    int leftResize = height - (Image.height() - 2);
    for (int i = 0; i < ResizeDistancesX.size(); i++) {
        topResize += ResizeDistancesX[i].second;
        factorX += ResizeDistancesX[i].second;
    }
    for (int i = 0; i < ResizeDistancesY.size(); i++) {
        leftResize += ResizeDistancesY[i].second;
        factorY += ResizeDistancesY[i].second;
    }
    factorX = (double)topResize / factorX;
    factorY = (double)leftResize / factorY;
}

void TNinePatch::GetNewImage(int width, int height) {
    NewImage =  QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    NewImage.fill(QColor(0,0,0,0));
    QPainter painter(&NewImage);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    double factorX = 0.0;
    double factorY = 0.0;
    GetFactor(width, height, factorX, factorY);
    double lostX = 0.0;
    double lostY = 0.0;
    int x1 = 0;//for image parts X
    int y1 = 0;//for image parts Y
    int width_resize;//width for image parts
    int height_resize;//height for image parts
    int resizeX = 0;
    int resizeY = 0;
    int offsetX = 0;
    int offsetY = 0;
    for (int  i = 0; i < ResizeDistancesX.size(); i++) {
        y1 = 0;
        offsetY = 0;
        lostY = 0.0;
        for (int  j = 0; j < ResizeDistancesY.size(); j++) {
            width_resize = ResizeDistancesX[i].first - x1;
            height_resize = ResizeDistancesY[j].first - y1;

            DrawConstPart(QRect(x1 + 1, y1 + 1, width_resize, height_resize),
                          QRect(x1 + offsetX, y1 + offsetY, width_resize, height_resize), painter);

            int  y2 = ResizeDistancesY[j].first;
            height_resize = ResizeDistancesY[j].second;
            resizeY = round((double)height_resize * factorY);
            lostY += resizeY - ((double)height_resize * factorY);
            if (fabs(lostY) >= 1.0) {
                if (lostY < 0) {
                    resizeY += 1;
                    lostY += 1.0;
                } else {
                    resizeY -= 1;
                    lostY -= 1.0;
                }
            }
            DrawScaledPart(QRect(x1 + 1, y2 + 1, width_resize, height_resize),
                           QRect(x1 + offsetX, y2 + offsetY, width_resize, resizeY), painter);

            int  x2 = ResizeDistancesX[i].first;
            width_resize = ResizeDistancesX[i].second;
            height_resize = ResizeDistancesY[j].first - y1;
            resizeX = round((double)width_resize * factorX);
            lostX += resizeX - ((double)width_resize * factorX);
            if (fabs(lostX) >= 1.0) {
                if (lostX < 0) {
                    resizeX += 1;
                    lostX += 1.0;
                } else {
                    resizeX -= 1;
                    lostX -= 1.0;
                }
            }
            DrawScaledPart(QRect(x2 + 1, y1 + 1, width_resize, height_resize),
                           QRect(x2 + offsetX, y1 + offsetY, resizeX, height_resize), painter);

            height_resize = ResizeDistancesY[j].second;
            DrawScaledPart(QRect(x2 + 1, y2 + 1, width_resize, height_resize),
                           QRect(x2 + offsetX, y2 + offsetY, resizeX, resizeY), painter);

            y1 = ResizeDistancesY[j].first + ResizeDistancesY[j].second;
            offsetY += resizeY - ResizeDistancesY[j].second;
        }
        x1 = ResizeDistancesX[i].first + ResizeDistancesX[i].second;
        offsetX += resizeX - ResizeDistancesX[i].second;
    }
    x1 = ResizeDistancesX[ResizeDistancesX.size() - 1].first + ResizeDistancesX[ResizeDistancesX.size() - 1].second;
    width_resize = Image.width() - x1 - 2;
    y1 = 0;
    lostX = 0.0;
    lostY = 0.0;
    offsetY = 0;
    for (int i = 0; i < ResizeDistancesY.size(); i++) {
        DrawConstPart(QRect(x1 + 1, y1 + 1, width_resize, ResizeDistancesY[i].first - y1),
                      QRect(x1 + offsetX, y1 + offsetY, width_resize, ResizeDistancesY[i].first - y1), painter);
        y1 = ResizeDistancesY[i].first;
        resizeY = round((double)ResizeDistancesY[i].second * factorY);
        lostY += resizeY - ((double)ResizeDistancesY[i].second * factorY);
        if (fabs(lostY) >= 1.0) {
            if (lostY < 0) {
                resizeY += 1;
                lostY += 1.0;
            } else {
                resizeY -= 1;
                lostY -= 1.0;
            }
        }
        DrawScaledPart(QRect(x1 + 1, y1 + 1, width_resize, ResizeDistancesY[i].second),
                       QRect(x1 + offsetX, y1 + offsetY, width_resize, resizeY), painter);
        y1 = ResizeDistancesY[i].first + ResizeDistancesY[i].second;
        offsetY += resizeY - ResizeDistancesY[i].second;
    }
    y1 = ResizeDistancesY[ResizeDistancesY.size() - 1].first + ResizeDistancesY[ResizeDistancesY.size() - 1].second;
    height_resize = Image.height() - y1 - 2;
    x1 = 0;
    offsetX = 0;
    for (int i = 0; i < ResizeDistancesX.size(); i++) {
        DrawConstPart(QRect(x1 + 1, y1 + 1, ResizeDistancesX[i].first - x1, height_resize),
                      QRect(x1 + offsetX, y1 + offsetY, ResizeDistancesX[i].first - x1, height_resize), painter);
        x1 = ResizeDistancesX[i].first;
        resizeX = round((double)ResizeDistancesX[i].second * factorX);
        lostX += resizeX - ((double)ResizeDistancesX[i].second * factorX);
        if (fabs(lostX) >= 1.0) {
            if (lostX < 0) {
                resizeX += 1;
                lostX += 1.0;
            } else {
                resizeX -= 1;
                lostX += 1.0;
            }
        }
        DrawScaledPart(QRect(x1 + 1, y1 + 1, ResizeDistancesX[i].second, height_resize),
                       QRect(x1 + offsetX, y1 + offsetY, resizeX, height_resize), painter);
        x1 = ResizeDistancesX[i].first + ResizeDistancesX[i].second;
        offsetX += resizeX - ResizeDistancesX[i].second;
    }
    x1 = ResizeDistancesX[ResizeDistancesX.size() - 1].first + ResizeDistancesX[ResizeDistancesX.size() - 1].second;
    width_resize = Image.width() - x1 - 2;
    y1 = ResizeDistancesY[ResizeDistancesY.size() - 1].first + ResizeDistancesY[ResizeDistancesY.size() - 1].second;
    height_resize = Image.height() - y1 - 2;
    DrawConstPart(QRect(x1 + 1, y1 + 1, width_resize, height_resize),
                  QRect(x1 + offsetX, y1 + offsetY, width_resize, height_resize), painter);
}


