#include <QCoreApplication>
#include "vm1.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    VM1 vm1;
    vm1.start();

    return a.exec();
}
