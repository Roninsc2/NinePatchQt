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
    image = QImage(fileName);
    ninepatch = new TNinePatch(image);
}

Widget::~Widget()
{
    delete ninepatch;
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    try {
        ninepatch->SetImageSize(107,200);
        ninepatch->Draw(painter, 20, 20);
    } catch (NinePatchException* e) {
        std::cerr << e->what() << std::endl;
        delete e;
    }
}
