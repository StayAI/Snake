#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setFixedSize(640, 480);
    w.setWindowTitle("snake");
    w.show();

    return a.exec();
}
