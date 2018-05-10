#include "imagebutton.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageButton w;
    w.show();

    return a.exec();
}
