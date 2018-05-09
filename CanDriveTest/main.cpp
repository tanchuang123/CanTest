#include "TheMainForm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TheMainForm w;
    w.show();

    return a.exec();
}
