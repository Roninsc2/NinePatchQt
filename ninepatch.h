#ifndef NINEPATCH
#define NINEPATCH

#include <QImage>
#include <QPainter>
#include <QPoint>
#include <QString>

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


#endif // NINEPATCH

