#include "imagefigure.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    imageFigure w;
    w.show();

    return a.exec();
}
