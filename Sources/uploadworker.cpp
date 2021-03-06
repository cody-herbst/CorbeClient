#include "uploadworker.h"

#include <QThread>
#include <QFileInfo>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <unistd.h> //sleep

UploadWorker::UploadWorker(QString token, QString fileNameAndDirectory){
    this->token = token;
    this->fileNameAndDirectory = fileNameAndDirectory;
    this->bytesRead = 0;

//    this->manager = new QNetworkAccessManager();
}

void UploadWorker::run(){

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url("http://localhost:8080/encrypt");

    this->request = new QNetworkRequest(url);

    this->fileToSend = new QFile(this->fileNameAndDirectory);
    if(!this->fileToSend->open(QIODevice::ReadOnly) || !this->fileToSend->isReadable()) {
        throw "Unable to open the file requested.";
    }

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart token;
    token.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"token\""));
    token.setBody(this->token.toUtf8());
    multiPart->append(token);

    QHttpPart fileName;
    fileName.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"fileName\""));
    QFileInfo fileInfo(this->fileToSend->fileName());
    fileName.setBody(fileInfo.fileName().toUtf8());
    multiPart->append(fileName);

    QHttpPart clearTextFile;
    clearTextFile.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"fileToEncrypt\"; filename=\""+ fileInfo.fileName().toUtf8() +"\""));
    clearTextFile.setBodyDevice(this->fileToSend);
    this->fileToSend->setParent(multiPart);
    multiPart->append(clearTextFile);

    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(erroron_filesend(QNetworkReply*)));
    QNetworkReply* reply = manager->post(*request, multiPart);
    connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(fileUploadProgress(qint64, qint64)));
}


 void UploadWorker::erroron_filesend(QNetworkReply*){
     qDebug() << "Event fired due to finishing.";
     qDebug()<<"From child thread UploadWorker::erroron_filesend(QNetworkReply*): "<< QThread::currentThreadId();
 }

 void UploadWorker::fileUploadProgress(qint64 bytesSent, qint64 bytesTotal){
    qDebug() << "bytesSent: " << bytesSent << " / " << bytesTotal;
 }
