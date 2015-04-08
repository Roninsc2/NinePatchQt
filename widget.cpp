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
    ninepatch = new TNinePatch(fileName);
}

Widget::~Widget()
{
    delete ninepatch;
    delete ui;
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    try {
        ninepatch->draw(painter, 10, 10, 300, 270);
    } catch (NinePatchException* e) {
        std::cerr << e->what() << std::endl;
        delete e;
    }
}
