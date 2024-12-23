#include <QCoreApplication>
#include "vm2.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    VM2 vm2;
    vm2.start();
    return a.exec();
}
