#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <iostream>
#include <QProcess>

struct Parametrs
{
    double q;
    int r;
    QString format;
    Parametrs() {
        q = 0.75;
        format = "m4a";
    }
};

class Thread : public QObject
{
    Q_OBJECT
private:
    Parametrs* parametrs;
    QString name;
    QString name2;
    QString nametxt;
    QString namelast;
    QString gain;
    double Q;
    int I;
    bool rchanged;
    void toWav();
    void toMp3();
    void toFlac();
    void toOgg();
    void toM4a();
    void removefile(QString name);
public:
    explicit Thread(QObject *parent = nullptr);
    Thread(Parametrs* f, QString name, int i);

public slots:
    void Work();

signals:
    void endThreadSignal(int i);
};

#endif // THREAD_H
