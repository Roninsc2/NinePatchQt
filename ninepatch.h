
#include <QImage>
#include <QPainter>
#include <QString>
#include <exception>
#include <string>

class TNinePatch {
public:
    TNinePatch(QImage& image);
    void Draw(QPainter& painter, int X, int Y, int WIDTH, int HEIGHT);
    ~TNinePatch();
    QRect GetContentArea(int  widht, int  height);

private:
    QRect GetContentArea();
    void GetResizeArea();
    void GetFactor(size_t width, size_t height, double& factorX, double& factorY);
    void GetNewImage(int width, int height);
    void DrawScaledPart(QRect oldRect, QRect newRect, QPainter& painter);
    void DrawConstPart(QRect oldRect, QRect newRect, QPainter& painter);
public:
    QImage& Image;
    QRect NewContentArea;

private:
    int old_widht = -1;
    int old_height = -1;
    QImage NewImage;
    QVector<std::pair< int, int >>ResizeDistancesY;
    QVector<std::pair< int, int >>ResizeDistancesX;
    QRect ContentArea;
};


class NinePatchException : public std::exception {
public:
    virtual const char* what() const throw() override {
        return "Nine patch error";
    }
};

class ExceptionIncorrectWidth : public NinePatchException {
public:
    ExceptionIncorrectWidth(int  imgW, int  imgH) {
        ImgWidth = imgW;
        ImgHeight = imgH;
    }
    virtual const char* what() const throw() override {
        std::string str = ("Input incorrect width. Mimimum width = " + std::to_string(ImgWidth));
        return str.c_str();
    }
public:
    int  ImgWidth;
    int  ImgHeight;
};

class ExceptionIncorrectWidthAndHeight : public NinePatchException {
public:
    ExceptionIncorrectWidthAndHeight(int  imgW,int  imgH) {
        ImgWidth = imgW;
        ImgHeight = imgH;
    }

    virtual const char* what() const throw() override {
        std::string str = ("Input incorrect width width and height. Minimum width = " + std::to_string(ImgWidth)+ ". Minimum height = " + std::to_string(ImgHeight));
        return str.c_str();
    }
public:
    int  ImgWidth;
    int  ImgHeight;
};

class ExceptionIncorrectHeight : public NinePatchException {
public:
    ExceptionIncorrectHeight(int  imgW, int  imgH) {
        ImgWidth = imgW;
        ImgHeight = imgH;
    }

    virtual const char* what() const throw() override {
        std::string str = ("Input incorrect heigh. Minimum height = " + std::to_string(ImgHeight)) ;
        return str.c_str();
    }
public:
    int  ImgWidth;
    int  ImgHeight;
};

class ExceptionNot9Patch : public NinePatchException {
    virtual const char* what() const throw() override {
        return "It is not nine patch image";
    }
};
