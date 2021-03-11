#include <QCoreApplication>

#include "audioconvert.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new AudioConvert(argc, argv);
    return a.exec();
}
