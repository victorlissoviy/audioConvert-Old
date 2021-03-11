#ifndef AUDIOCONVERT_H
#define AUDIOCONVERT_H

#include <QObject>
#include <QStack>
#include <QDir>
#include <QFileInfoList>
#include <QThread>
#include "thread.h"

class AudioConvert : public QObject
{
    Q_OBJECT
private:
    Parametrs* parametrs;
    Thread** runners;
    QThread** runnersThread;
    QStack<QString> list;
    int N;
public:
    explicit AudioConvert(QObject *parent = nullptr);
    AudioConvert(int argc, char *argv[]);

public slots:
    void endThreadSlot(int i);

signals:

};

#endif // AUDIOCONVERT_H
