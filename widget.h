#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QFileDialog>
#include "ninepatch.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    QImage *image;
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    TNinePatch * ninepatch;
    void paintEvent(QPaintEvent*);
    Ui::Widget *ui;
};

#endif // WIDGET_H
