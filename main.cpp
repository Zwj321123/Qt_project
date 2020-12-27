#include "lr_drawer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LR_drawer w;
    w.show();
    return a.exec();
}
