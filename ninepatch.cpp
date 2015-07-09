#include "ninepatch.h"
#include <QRect>

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

void TNinePatch::Draw(QPainter& painter, int  X, int  Y, int  WIDTH, int  HEIGHT) {
       if (WIDTH < (Image.width() - 2) && HEIGHT < (Image.height() - 2)) {
           throw new ExceptionIncorrectWidthAndHeight(Image.width() - 2 , Image.height() - 2 );
       }
       if (WIDTH < (Image.width() - 2)) {
            throw new ExceptionIncorrectWidth(Image.width() - 2 , Image.height() - 2 );
       }
       if (HEIGHT < (Image.height() - 2)) {
            throw new ExceptionIncorrectHeight(Image.width() - 2 , Image.height() - 2 );
       }

       int topResize = WIDTH - (Image.width() - 2);
       int leftResize = HEIGHT - (Image.height() - 2);
       double factorX = 0.0;
       double factorY = 0.0;
       for (int i = 0; i < ResizeDistancesX.size(); i++) {
           topResize += ResizeDistancesX[i].second;
           factorX += ResizeDistancesX[i].second;
       }
       for (int i = 0; i < ResizeDistancesY.size(); i++) {
           leftResize += ResizeDistancesY[i].second;
           factorY += ResizeDistancesY[i].second;
       }
       factorX = (float)topResize / factorX;
       factorY = (float)leftResize / factorY;
       double lostX = 0.0;
       double lostY = 0.0;
       int x1 = 0;
       int y1 = 0;
       int width;
       int height;
       int resizeX = 0;
       int resizeY = 0;
       int offsetX = 0;
       int offsetY = 0;
       for (int  i = 0; i < ResizeDistancesX.size(); i++) {
           y1 = 0;
           offsetY = 0;
           lostY = 0.0;
           for (int  j = 0; j < ResizeDistancesY.size(); j++) {
               width = ResizeDistancesX[i].first - x1;
               height = ResizeDistancesY[j].first - y1;

               DrawConstPart(QRect(x1 + 1, y1 + 1, width, height),
                             QRect(X + x1 + offsetX, Y + y1 + offsetY, width, height), painter);

               int  y2 = ResizeDistancesY[j].first;
               height = ResizeDistancesY[j].second;
               resizeY = round((float)height * factorY);
               lostY += resizeY - ((float)height * factorY);
               if (fabs(lostY) >= 1.0) {
                   if (lostY < 0) {
                       resizeY += 1;
                       lostY += 1.0;
                   } else {
                       resizeY -= 1;
                       lostY -= 1.0;
                   }
               }
               DrawScaledPart(QRect(x1 + 1, y2 + 1, width, height),
                              QRect(X + x1 + offsetX, Y + y2 + offsetY, width, resizeY), painter);

               int  x2 = ResizeDistancesX[i].first;
               width = ResizeDistancesX[i].second;
               height = ResizeDistancesY[j].first - y1;
               resizeX = round((float)width * factorX);
               lostX += resizeX - ((float)width * factorX);
               if (fabs(lostX) >= 1.0) {
                   if (lostX < 0) {
                       resizeX += 1;
                       lostX += 1.0;
                   } else {
                       resizeX -= 1;
                       lostX -= 1.0;
                   }
               }
               DrawScaledPart(QRect(x2 + 1, y1 + 1, width, height),
                              QRect(X + x2 + offsetX, Y + y1 + offsetY, resizeX, height), painter);

               height = ResizeDistancesY[j].second;
               DrawScaledPart(QRect(x2 + 1, y2 + 1, width, height),
                              QRect(X + x2 + offsetX, Y + y2 + offsetY, resizeX, resizeY), painter);

               y1 = ResizeDistancesY[j].first + ResizeDistancesY[j].second;
               offsetY += resizeY - ResizeDistancesY[j].second;
           }
           x1 = ResizeDistancesX[i].first + ResizeDistancesX[i].second;
           offsetX += resizeX - ResizeDistancesX[i].second;
       }
       int x = ResizeDistancesX[ResizeDistancesX.size() - 1].first + ResizeDistancesX[ResizeDistancesX.size() - 1].second;
       width = Image.width() - x - 2;
       int y = 0;
       lostX = 0.0;
       lostY = 0.0;
       offsetY = 0;
       for (int i = 0; i < ResizeDistancesY.size(); i++) {
           DrawConstPart(QRect(x + 1, y + 1, width, ResizeDistancesY[i].first - y),
                          QRect(X + x + offsetX, Y + y + offsetY, width, ResizeDistancesY[i].first - y), painter);
           y = ResizeDistancesY[i].first;
           resizeY = round((float)ResizeDistancesY[i].second * factorY);
           lostY += resizeY - ((float)ResizeDistancesY[i].second * factorY);
           if (fabs(lostY) >= 1.0) {
               if (lostY < 0) {
                   resizeY += 1;
                   lostY += 1.0;
               } else {
                   resizeY -= 1;
                   lostY -= 1.0;
               }
           }
           DrawScaledPart(QRect(x + 1, y + 1, width, ResizeDistancesY[i].second),
                          QRect(X + x + offsetX, Y + y + offsetY, width, resizeY), painter);
           y = ResizeDistancesY[i].first + ResizeDistancesY[i].second;
           offsetY += resizeY - ResizeDistancesY[i].second;
       }
       y = ResizeDistancesY[ResizeDistancesY.size() - 1].first + ResizeDistancesY[ResizeDistancesY.size() - 1].second;
       height = Image.height() - y - 2;
       x = 0;
       offsetX = 0;
       for (int i = 0; i < ResizeDistancesX.size(); i++) {
           DrawConstPart(QRect(x + 1, y + 1, ResizeDistancesX[i].first - x, height),
                         QRect(X + x + offsetX, Y + y + offsetY, ResizeDistancesX[i].first - x, height), painter);
           x = ResizeDistancesX[i].first;
           resizeX = round((float)ResizeDistancesX[i].second * factorX);
           lostX += resizeX - ((float)ResizeDistancesX[i].second * factorX);
           if (fabs(lostX) >= 1.0) {
               if (lostX < 0) {
                   resizeX += 1;
                   lostX += 1.0;
               } else {
                   resizeX -= 1;
                   lostX += 1.0;
               }
           }
           DrawScaledPart(QRect(x + 1, y + 1, ResizeDistancesX[i].second, height),
                          QRect(X + x + offsetX, Y + y + offsetY, resizeX, height), painter);
           x = ResizeDistancesX[i].first + ResizeDistancesX[i].second;
           offsetX += resizeX - ResizeDistancesX[i].second;
       }
       x = ResizeDistancesX[ResizeDistancesX.size() - 1].first + ResizeDistancesX[ResizeDistancesX.size() - 1].second;
       width = Image.width() - x - 2;
       y = ResizeDistancesY[ResizeDistancesY.size() - 1].first + ResizeDistancesY[ResizeDistancesY.size() - 1].second;
       height = Image.height() - y - 2;
       DrawConstPart(QRect(x + 1, y + 1, width, height),
                      QRect(X + x + offsetX, Y + y + offsetY, width, height), painter);
}

QRect TNinePatch::GetContentArea(int  width, int  height) {
    return (QRect(ContentArea.x(), ContentArea.y(), (width - (Image.width() - 2 -ContentArea.width())),
                  (height - (Image.height() - 2 -ContentArea.height()))));
}

void TNinePatch::DrawScaledPart(QRect oldRect, QRect newRect, QPainter& painter) {
    QImage img = Image.copy(oldRect);
    img = img.scaled(newRect.width(), newRect.height());
    painter.drawImage(newRect.x(), newRect.y(), img, 0, 0, newRect.width(), newRect.height());
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


