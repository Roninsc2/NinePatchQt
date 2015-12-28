#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QFileDialog>
#include <iostream>
#include "ninepatch.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    TNinePatch * ninepatch;
    void paintEvent(QPaintEvent*);
    QScopedPointer<Ui::Widget> ui;
    QImage image;
};

#endif // WIDGET_H
