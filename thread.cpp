#include "thread.h"
#include <QFileInfo>

void Thread::toWav(){
    QProcess process;
    QString command = nullptr;
    if(name.endsWith("mp3")){
        command = "ffmpeg -y -acodec mp3float -i ";
    }else{
        command = "ffmpeg -y -i ";
    }
    command += "\"" + name + "\" -acodec pcm_f32le \"" + name2 + "\"";
    process.start(command);
    process.waitForFinished(-1);
    if(parametrs->format.contains("flac")){
        command = "wavegain -f \"" + nametxt + "\" \"" + name2 + "\"";
        process.start(command);
        process.waitForFinished(-1);
        QFile file(nametxt);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            return;
        }
        bool isGain = false;
        while (!file.atEnd()) {
            QString line = file.readLine();
            int n = line.indexOf("dB");
            if(n != -1){
                isGain = true;
                line.resize(n + 2);
                line.replace(" ", "");
                line.replace("dB", " dB");
                gain = line;
            }
        }
        file.close();
        removefile(nametxt);
        if(!isGain){
            QString nametemp =  "." + name2;
            if(parametrs->format.indexOf("mp3") != -1){
                command = "sox \"" + name2 + "\" -e floating-point \"" + nametemp + "\" rate -v -I -a -b 99.7 -p 100 48000";
                process.start(command);
                process.waitForFinished(-1);
                removefile(name2);
                command = "mv \"" + nametemp + "\" \"" + name2 + "\"";
                process.start(command);
                process.waitForFinished(-1);
                command = "wavegain -f \"" + nametxt + "\" \"" + name2 + "\"";
                process.start(command);
                process.waitForFinished(-1);
            }else if(parametrs->format.indexOf("m4a") != -1){
                command = "sox \"" + name2 + "\" -e floating-point \"" + nametemp + "\" rate -v -I -a -b 99.7 -p 100 96000";
                process.start(command);
                process.waitForFinished(-1);
                removefile(name2);
                command = "mv \"" + nametemp + "\" \"" + name2 + "\"";
                process.start(command);
                process.waitForFinished(-1);
                command = "wavegain -f \"" + nametxt + "\" \"" + name2 + "\"";
                process.start(command);
                process.waitForFinished(-1);
            }else{
                command = "sox \"" + name2 + "\" -e floating-point \"" + nametemp + "\" rate -v -I -a -b 99.7 -p 100 96000";
                process.start(command);
                process.waitForFinished(-1);
                command = "wavegain -f \"" + nametxt + "\" \"" + nametemp + "\"";
                process.start(command);
                process.waitForFinished(-1);
                removefile(nametemp);
            }
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
                return;
            }
            while (!file.atEnd()) {
                QString line = file.readLine();
                int n = line.indexOf("dB");
                if(n != -1){
                    line.resize(n + 2);
                    line.replace(" ", "");
                    line.replace("dB", " dB");
                    gain = line;
                }
            }
            file.close();
            removefile(nametxt);
        }
    }
}

void Thread::toMp3(){
    QProcess process;
    QString command = "lame -S --bitwidth 32 -o --buffer-constraint maximum --preset extreme ";
    if(parametrs->q > 640.0){
        parametrs->q = 640.0;
    }
    if(parametrs->q > 320.0){
        command += "--freeformat -V 0 -b " + QString::number(Q);
    }else if(parametrs->q <= 10.0){
        command += "-V " + QString::number(Q) + " -q 0";
    }else{
        command += "-V 0 -q 0 -b " + QString::number(Q) + " -B " + QString::number(Q);
    }
    QString nameonly = name;
    nameonly.resize(nameonly.lastIndexOf("."));
    QStringList metadata = nameonly.split(" - ");
    if(metadata.length() > 1){
        command += " --ta \"" + metadata[0] + "\" --tt \"" + metadata[1] + "\" \"" + name2 + "\" \"new/" + namelast + "\"";
    }else{
        command += " --tt \"" + metadata[0] + "\" \"" + name2 + "\" \"new/" + namelast + "\"";
    }
    process.start(command);
    process.waitForFinished(-1);
    removefile(name2);
    command = "mp3gain -q -e \"new/" + namelast + "\"";
    process.start(command);
    process.waitForFinished(-1);
}

void Thread::toFlac(){
    QProcess process;
    QString command = "";
    QString nameonly = name;
    nameonly.resize(nameonly.lastIndexOf("."));
    QStringList metadata = nameonly.split(" - ");
    if(metadata.length() > 1){
        command = "ffmpeg -y -i \"" + name2 + "\" -metadata replaygain_track_gain=\"" + gain + "\" -metadata artist=\"" + metadata[0] + "\" -metadata title=\"" + metadata[1] + "\" \"new/" + namelast + "\"";
    }else{
        command = "ffmpeg -y -i \"" + name2 + "\" -metadata replaygain_track_gain=\"" + gain + "\" -metadata title=\"" + metadata[0] + "\" \"new/" + namelast + "\"";
    }
    process.start(command);
    process.waitForFinished(-1);
    removefile(name2);
}

void Thread::toOgg(){
    QProcess process;
    QString quality = "";
    QString command;
    if(Q > 10){
        quality = "-b " + QString::number(Q) + " -m " + QString::number(Q) + " -M " + QString::number(Q);
    }else{
        quality = "-q " + QString::number(Q);
    }
    QString nameonly = name;
    nameonly.resize(nameonly.lastIndexOf("."));
    QStringList metadata = nameonly.split(" - ");
    if(metadata.length() > 1){
        command = "oggenc -Q --utf8 --ignorelength " + quality + " -a \"" + metadata[0] + "\" -t \"" + metadata[1] + "\" -o \"new/" + namelast + "\" \"" + name2 + "\"";
    }else{
        command = "oggenc -Q --utf8 --ignorelength " + quality + " -t \"" + metadata[0] + "\" -o \"new/" + namelast + "\" \"" + name2 + "\"";
    }
    process.start(command);
    process.waitForFinished(-1);
    removefile(name2);
    command = "vorbisgain \"new/" + namelast + "\"";
    process.start(command);
    process.waitForFinished(-1);
}

void Thread::toM4a(){
    QProcess process;
    QString command;
    if(parametrs->q > 1){
        command = "neroAacEnc -2pass -br " + QString::number(parametrs->q * 1000) + " -ignorelength -if \"" + name2 + "\" -of \"new/" + namelast + "\"";
    }else{
        command = "neroAacEnc -q " + QString::number(Q) + " -ignorelength -if \"" + name2 + "\" -of \"new/" + namelast + "\"";
    }
    process.start(command);
    process.waitForFinished(-1);
    removefile(name2);
    QString nameonly = name;
    nameonly.resize(nameonly.lastIndexOf("."));
    QStringList metadata = nameonly.split(" - ");
    if(metadata.length() > 1){
        command = "neroAacTag \"new/" + namelast + "\" -meta:artist=\"" + metadata[0] + "\" -meta:title=\"" + metadata[1] + "\"";
    }else{
        command = "neroAacTag \"new/" + namelast + "\" -meta:title=\"" + metadata[0] + "\"";
    }
    process.start(command);
    process.waitForFinished(-1);
    command = "aacgain -e \"new/" + namelast + "\"";
    process.start(command);
    process.waitForFinished(-1);
}

void Thread::removefile(QString name){
    QString command = "rm \"" + name + "\"";
    QProcess process;
    process.start(command);
    process.waitForFinished(-1);
}

Thread::Thread(QObject *parent) : QObject(parent){
    parametrs = new Parametrs;
    name = "";
    I = -1;
}

Thread::Thread(Parametrs *f, QString name, int i){
    parametrs = f;
    Q = parametrs->q;
    rchanged = false;
    this->name = name;
    I = i;
    name2 = "." + name;
    name2.resize(name2.lastIndexOf("."));
    nametxt = name2 + ".txt";
    name2.append(".wav");
    namelast = name;
    namelast.resize(namelast.lastIndexOf(".") + 1);
    namelast.append(parametrs->format);
}

void Thread::Work(){
    std::cout << namelast.toStdString() << std::endl;
    toWav();
    if(parametrs->format.indexOf("ogg") != -1){
        toOgg();
    }
    if(parametrs->format.indexOf("mp3") != -1){
        toMp3();
    }
    if(parametrs->format.indexOf("flac") != -1){
        toFlac();
    }
    if(parametrs->format.indexOf("m4a") != -1){
        toM4a();
    }
    QString command = "mv \"" + name + "\" \"old/" + name + "\"";
    QProcess process;
    process.start(command);
    process.waitForFinished(-1);
    emit endThreadSignal(I);
}

