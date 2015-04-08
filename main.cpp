#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget* w;
    try {
        w = new Widget();
    } catch (NinePatchException* e) {
        std::cerr << e->what() << std::endl;
        delete e;
        return 1;
    }
    w->show();
    return a.exec();
}
