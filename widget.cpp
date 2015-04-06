#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Directory"),
                                                      "", "*.9.png");
    ninepatch = new TNinePatch(fileName);
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    ninepatch->draw(painter, 10, 10, 400, 400);
}
