#ifndef UPLOADWORKER_H
#define UPLOADWORKER_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QString>
#include <QUrl>
#include <QNetworkRequest>
#include <QThread>

class UploadWorker : public QObject{
    Q_OBJECT
//class UploadWorker : public QThread{


public:
    UploadWorker(QString, QString fileNameAndDirectory);
    void run();

public slots:
    void erroron_filesend(QNetworkReply*);
    void fileUploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    QString token;
    QString fileNameAndDirectory;
    long bytesRead;
//    QNetworkAccessManager* manager;
    QNetworkRequest* request;
    QFile* fileToSend;



};

#endif // UPLOADWORKER_H
