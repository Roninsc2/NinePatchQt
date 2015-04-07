#ifndef NINEPATCH
#define NINEPATCH

<<<<<<< HEAD
#include <QScopedPointer>
=======
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QString>
<<<<<<< HEAD
#include <exception>

class TNinePatch {
public:
    TNinePatch(QString fileName);
    void draw(QPainter& painter, int x, int y, int width, int height);
    ~TNinePatch();

private:
    QRect GetContentArea();
    QRect GetResizeArea();
    QRect resizeContentArea(int resizeX, int resizeY);
    void drawScaledPart(QRect oldRect, QRect newRect, QPainter& painter);

public:
    QScopedPointer<QImage> image;
    QRect newContentArea;

private:
    QRect resizeArea;
    QRect contentArea;
};


class MyExceptionWidth : public std::exception
{
    virtual const char* what() const throw() {
        return "Input incorrect width";
    }
};

class MyExceptionWidthAndHeight : public std::exception
{
    virtual const char* what() const throw() {
        return "Input incorrect width and height";
    }
};

class MyExceptionHeight : public std::exception
{
    virtual const char* what() const throw() {
        return "Input incorrect height";
    }
};

class MyExceptionNot9Patch : public std::exception
{
    virtual const char* what() const throw() {
        return "It is not nine patch image";
    }
};



=======

class TNinePatch
{
public:
    TNinePatch(QString fileName);
    void draw(QPainter& painter, int x, int y, int width, int height);

private:
    int GetImageCount(int x, int y, QPoint *top, QPoint *left);
    QPoint *GetLeftVerge();
    QPoint *GetTopVerge();

private:
    QImage *image;

};

>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0

#endif // NINEPATCH

