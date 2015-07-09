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
    try {
        ninepatch->Draw(painter, 20, 20, 107, 200);
    } catch (NinePatchException* e) {
        std::cerr << e->what() << std::endl;
        delete e;
    }
    //delete img;
}
