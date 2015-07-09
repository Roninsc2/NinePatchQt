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
    QImage img;
    TNinePatch * ninepatch;
    void paintEvent(QPaintEvent*);
    QScopedPointer<Ui::Widget> ui;
};

#endif // WIDGET_H
