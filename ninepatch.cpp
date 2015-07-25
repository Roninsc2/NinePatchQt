#include "ninepatch.h"
#include <QRect>
#include <iostream>

TNinePatch::TNinePatch(const QString& fileName)
    : Image(QImage(fileName))
{
    ContentArea = GetContentArea();
    GetResizeArea();
    if (!ResizeDistancesX.size() || !ResizeDistancesY.size()) {
        throw new ExceptionNot9Patch;
    }
}

TNinePatch::~TNinePatch() {
}

void TNinePatch::Draw(QPainter& painter, int  x, int  y) {
    painter.drawImage(x, y, CachedImage);
}

void TNinePatch::SetImageSize(int width, int height) {
    int resizeWidth = 0;
    int resizeHeight = 0;
    for (int i = 0; i < ResizeDistancesX.size(); i++) {
          resizeWidth += ResizeDistancesX[i].second;
    }
    for (int i = 0; i < ResizeDistancesY.size(); i++) {
          resizeHeight += ResizeDistancesY[i].second;
    }
    if (width < (Image.width() - 2 - resizeWidth) && height < (Image.height() - 2 - resizeHeight)) {
        throw new ExceptionIncorrectWidthAndHeight(Image.width() - 2 , Image.height() - 2 );
    }
    if (width < (Image.width() - 2 - resizeWidth)) {
          throw new ExceptionIncorrectWidth(Image.width() - 2 , Image.height() - 2 );
    }
     if (height < (Image.height() - 2 - resizeHeight)) {
         throw new ExceptionIncorrectHeight(Image.width() - 2 , Image.height() - 2 );
    }
    if (width != OldWidth && height != OldHeight) {
        OldWidth = width;
        OldHeight = height;
        UpdateCachedImage(width, height);
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
    int  j = Image.height() - 1;
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
    int  i = Image.width() - 1;
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
    return (QRect(left, top, right - left, bot - top));
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
}

void TNinePatch::GetFactor(int width, int height, double& factorX, double& factorY) {
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

void TNinePatch::UpdateCachedImage(int width, int height) {
    CachedImage =  QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    CachedImage.fill(QColor(0,0,0,0));
    QPainter painter(&CachedImage);
    double factorX = 0.0;
    double factorY = 0.0;
    GetFactor(width, height, factorX, factorY);
    double lostX = 0.0;
    double lostY = 0.0;
    int x1 = 0; //for image parts X
    int y1 = 0; //for image parts Y
    int widthResize; //width for image parts
    int heightResize; //height for image parts
    int resizeX = 0;
    int resizeY = 0;
    int offsetX = 0;
    int offsetY = 0;
    for (int  i = 0; i < ResizeDistancesX.size(); i++) {
        y1 = 0;
        offsetY = 0;
        lostY = 0.0;
        for (int  j = 0; j < ResizeDistancesY.size(); j++) {
            widthResize = ResizeDistancesX[i].first - x1;
            heightResize = ResizeDistancesY[j].first - y1;

            DrawConstPart(QRect(x1 + 1, y1 + 1, widthResize, heightResize),
                          QRect(x1 + offsetX, y1 + offsetY, widthResize, heightResize), painter);

            int  y2 = ResizeDistancesY[j].first;
            heightResize = ResizeDistancesY[j].second;
            resizeY = round((double)heightResize * factorY);
            lostY += resizeY - ((double)heightResize * factorY);
            if (fabs(lostY) >= 1.0) {
                if (lostY < 0) {
                    resizeY += 1;
                    lostY += 1.0;
                } else {
                    resizeY -= 1;
                    lostY -= 1.0;
                }
            }
            DrawScaledPart(QRect(x1 + 1, y2 + 1, widthResize, heightResize),
                           QRect(x1 + offsetX, y2 + offsetY, widthResize, resizeY), painter);

            int  x2 = ResizeDistancesX[i].first;
            widthResize = ResizeDistancesX[i].second;
            heightResize = ResizeDistancesY[j].first - y1;
            resizeX = round((double)widthResize * factorX);
            lostX += resizeX - ((double)widthResize * factorX);
            if (fabs(lostX) >= 1.0) {
                if (lostX < 0) {
                    resizeX += 1;
                    lostX += 1.0;
                } else {
                    resizeX -= 1;
                    lostX -= 1.0;
                }
            }
            DrawScaledPart(QRect(x2 + 1, y1 + 1, widthResize, heightResize),
                           QRect(x2 + offsetX, y1 + offsetY, resizeX, heightResize), painter);

            heightResize = ResizeDistancesY[j].second;
            DrawScaledPart(QRect(x2 + 1, y2 + 1, widthResize, heightResize),
                           QRect(x2 + offsetX, y2 + offsetY, resizeX, resizeY), painter);

            y1 = ResizeDistancesY[j].first + ResizeDistancesY[j].second;
            offsetY += resizeY - ResizeDistancesY[j].second;
        }
        x1 = ResizeDistancesX[i].first + ResizeDistancesX[i].second;
        offsetX += resizeX - ResizeDistancesX[i].second;
    }
    x1 = ResizeDistancesX[ResizeDistancesX.size() - 1].first + ResizeDistancesX[ResizeDistancesX.size() - 1].second;
    widthResize = Image.width() - x1 - 2;
    y1 = 0;
    lostX = 0.0;
    lostY = 0.0;
    offsetY = 0;
    for (int i = 0; i < ResizeDistancesY.size(); i++) {
        DrawConstPart(QRect(x1 + 1, y1 + 1, widthResize, ResizeDistancesY[i].first - y1),
                      QRect(x1 + offsetX, y1 + offsetY, widthResize, ResizeDistancesY[i].first - y1), painter);
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
        DrawScaledPart(QRect(x1 + 1, y1 + 1, widthResize, ResizeDistancesY[i].second),
                       QRect(x1 + offsetX, y1 + offsetY, widthResize, resizeY), painter);
        y1 = ResizeDistancesY[i].first + ResizeDistancesY[i].second;
        offsetY += resizeY - ResizeDistancesY[i].second;
    }
    y1 = ResizeDistancesY[ResizeDistancesY.size() - 1].first + ResizeDistancesY[ResizeDistancesY.size() - 1].second;
    heightResize = Image.height() - y1 - 2;
    x1 = 0;
    offsetX = 0;
    for (int i = 0; i < ResizeDistancesX.size(); i++) {
        DrawConstPart(QRect(x1 + 1, y1 + 1, ResizeDistancesX[i].first - x1, heightResize),
                      QRect(x1 + offsetX, y1 + offsetY, ResizeDistancesX[i].first - x1, heightResize), painter);
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
        DrawScaledPart(QRect(x1 + 1, y1 + 1, ResizeDistancesX[i].second, heightResize),
                       QRect(x1 + offsetX, y1 + offsetY, resizeX, heightResize), painter);
        x1 = ResizeDistancesX[i].first + ResizeDistancesX[i].second;
        offsetX += resizeX - ResizeDistancesX[i].second;
    }
    x1 = ResizeDistancesX[ResizeDistancesX.size() - 1].first + ResizeDistancesX[ResizeDistancesX.size() - 1].second;
    widthResize = Image.width() - x1 - 2;
    y1 = ResizeDistancesY[ResizeDistancesY.size() - 1].first + ResizeDistancesY[ResizeDistancesY.size() - 1].second;
    heightResize = Image.height() - y1 - 2;
    DrawConstPart(QRect(x1 + 1, y1 + 1, widthResize, heightResize),
                  QRect(x1 + offsetX, y1 + offsetY, widthResize, heightResize), painter);
}


