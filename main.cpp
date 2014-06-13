#include <QtGui/QApplication>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle(QString("SNAPGATE serial telnet tool"));
    w.show();
    
    return a.exec();
}
