#pragma once

#include <QScopedPointer>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QString>
#include <exception>
#include <string>

class TNinePatch {
public:
    TNinePatch(const QString& fileName);
    void draw(QPainter& painter, int x, int y, int width, int height);
    ~TNinePatch();
    QRect GetContentArea(int widht, int height);

private:
    QRect GetContentArea();
    QRect GetResizeArea();
    void drawScaledPart(QRect oldRect, QRect newRect, QPainter& painter);

public:
    QScopedPointer<QImage> image;
    QRect newContentArea;

private:
    QRect resizeArea;
    QRect contentArea;
};


class NinePatchException : public std::exception
{

};

class ExceptionIncorrectWidth : public NinePatchException
{
public:
    ExceptionIncorrectWidth(int imgW, int imgH) {
        imgWidth = imgW;
        imgHeight = imgH;
    }
    virtual const char* what() const throw() {
        std::string str = ("Input incorrect width. Mimimum width = " + std::to_string(imgWidth));
        return str.c_str();
    }
public:
    int imgWidth;
    int imgHeight;
};

class ExceptionIncorrectWidthAndHeight : public NinePatchException
{
public:
    ExceptionIncorrectWidthAndHeight(int imgW,int imgH) {
        imgWidth = imgW;
        imgHeight = imgH;
    }

    virtual const char* what() const throw() {
        std::string str = ("Input incorrect width width and height. Minimum width = " + std::to_string(imgWidth)+ ". Minimum height = " + std::to_string(imgHeight));
        return str.c_str();
    }
public:
    int imgWidth;
    int imgHeight;
};

class ExceptionIncorrectHeight : public NinePatchException
{
public:
    ExceptionIncorrectHeight(int imgW, int imgH) {
        imgWidth = imgW;
        imgHeight = imgH;
    }

    virtual const char* what() const throw() {
        std::string str = ("Input incorrect heigh. Minimum height = " + std::to_string(imgHeight)) ;
        return str.c_str();
    }
public:
    int imgWidth;
    int imgHeight;
};

class ExceptionNot9Patch : public NinePatchException
{
    virtual const char* what() const throw() {
        return ("It is not nine patch image");
    }
};

