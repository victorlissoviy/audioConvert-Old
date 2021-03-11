#include "audioconvert.h"
#include <QProcess>

AudioConvert::AudioConvert(int argc, char *argv[]){
    int n = std::thread::hardware_concurrency();
    parametrs = new Parametrs();
    if(argc > 0){
        for(int i = 1; i < argc; i++){
            if(QString(argv[i]) == "-q"){
                parametrs->q = QString(argv[i + 1]).toDouble();
            }
            if(QString(argv[i]) == "-f"){
                QString f = argv[i + 1];
                QString formats = "mp3,flac,m4a,ogg";
                if(formats.indexOf(f) != -1){
                    parametrs->format = f;
                }
            }
            if(QString(argv[i]) == "-c"){
                n = QString(argv[i + 1]).toInt();
            }
        }
    }
    std::cout << "Формат:\t\t" << parametrs->format.toStdString() << std::endl;
    if(parametrs->format == "flac"){
        std::cout << "Якість:\t\tБез втрати якості" << std::endl;
    }else{
        std::cout << "Якість:\t\t" << parametrs->q << std::endl;
    }
    QString typeFiles = ".mp3,.ogg,.aac,.mp4,.m4a,.wma,.opus,.oga,.flac,.wav,.aiff,.webm,.matroska,webm,.matroska,.asf,.amr,.AVI,.MOV,.3gp";
    QDir dir;
    QFileInfoList listfiles = dir.entryInfoList();
    for(int i = 2; i < listfiles.length(); i++){
        QFileInfo file = listfiles.at(i);
        QStringList names = file.fileName().split(".");
        if(names.length() > 1){
            QString format = names.at(names.length() - 1);
            if(typeFiles.indexOf(format) != -1 && file.fileName()[0] != "."){
                list.push(listfiles.at(i).fileName());
            }
        }
    }
    if(list.isEmpty()){
        exit(0);
    }
    dir.mkdir("new");
    dir.mkdir("old");
    runners = new Thread*[n];
    runnersThread = new QThread*[n];
    N = 0;
    for(int i = 0; i < n && !list.isEmpty(); i++){
        runners[i] = new Thread(parametrs, list.pop(), i);
        runnersThread[i] = new QThread;
        runners[i]->moveToThread(runnersThread[i]);
        connect(runners[i], SIGNAL(endThreadSignal(int)), this, SLOT(endThreadSlot(int)));
        connect(runnersThread[i], SIGNAL(started()), runners[i], SLOT(Work()));
        runnersThread[i]->start();
        N += 1;
    }
}

void AudioConvert::endThreadSlot(int i){
    if(!list.isEmpty()){
        runners[i] = new Thread(parametrs, list.pop(), i);
        runnersThread[i] = new QThread;
        runners[i]->moveToThread(runnersThread[i]);
        connect(runners[i], SIGNAL(endThreadSignal(int)), this, SLOT(endThreadSlot(int)));
        connect(runnersThread[i], SIGNAL(started()), runners[i], SLOT(Work()));
        runnersThread[i]->start();
    }else{
        N -= 1;
    }
    if(N == 0){
        exit(0);
    }
}
