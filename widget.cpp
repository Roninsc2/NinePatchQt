#include "widget.h"
#include "ui_widget.h"
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Directory"),
                                                      "", "*.9.png");
    img = QImage(fileName);
    ninepatch = new TNinePatch(img);
}

Widget::~Widget()
{
    delete ninepatch;
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QImage* img = new QImage("D:/wdjall/wdjmaster/data/images/navigation/crew.9.png");
    painter.drawImage(0,0, *img,0,0, img->width(), img->height());
    try {
        ninepatch->Draw(painter, 20, 20, 107, 200);
    } catch (NinePatchException* e) {
        std::cerr << e->what() << std::endl;
        delete e;
    }
    //delete img;
}
