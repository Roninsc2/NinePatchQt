#ifndef NINEPATCH
#define NINEPATCH

#include <QScopedPointer>
#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QString>
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




#endif // NINEPATCH

