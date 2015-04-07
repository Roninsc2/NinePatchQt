#include "widget.h"
#include "ui_widget.h"
<<<<<<< HEAD
#include <iostream>
=======
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
<<<<<<< HEAD
    ui->setupUi(this);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Directory"),
                                                      "", "*.9.png");
    ninepatch = new TNinePatch(fileName);
=======
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Directory"),
                                                      "", "*.9.png");
    ninepatch = new TNinePatch(fileName);
    ui->setupUi(this);
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
}

Widget::~Widget()
{
<<<<<<< HEAD
    delete ninepatch;
=======
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
    delete ui;
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
<<<<<<< HEAD
    try {
        ninepatch->draw(painter, 10, 10, 100, 100);
    } catch (std::exception* e) {
        std::cerr << e->what() << std::endl;
        delete e;
    }
=======
    ninepatch->draw(painter, 10, 10, 400, 400);
>>>>>>> 533a9e21650accefad0c6885186a33d84a8698a0
}
