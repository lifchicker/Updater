#ifndef UPDATECLIENT_H
#define UPDATECLIENT_H

#include <QObject>

#include <QString>
#include <QUrl>
#include <QVector>

struct FileInfo
{
    QString     fileName;   //file name with relative path
    qint64      size;
    QByteArray  hash;
};


struct UpdateFile
{
    QByteArray          currentVersion;
    QByteArray          previousVersion;
    QString             platform;
    QUrl                source;
    QVector<FileInfo>   files;
};


class UpdateClient : public QObject
{
    Q_OBJECT
public:
    explicit UpdateClient(QObject *parent = nullptr);
    
    bool        copyFile(const QString& locationFrom, const QString&locationTo, const QString& fileName);
    bool        copyFile(const QString& locationFrom, const QString&locationTo, const QString& originalFileName, const QString& resultFileName);
    bool        copyFiles(const QString& source, const QString& destination);
    QString     downloadFile(const QUrl&);
    bool        moveFile(const QString& locationFrom, const QString&locationTo, const QString& fileName);
    bool        moveFile(const QString& locationFrom, const QString&locationTo, const QString& originalFileName, const QString& resultFileName);
    UpdateFile  readHeader(const QString&);
    void        readListOfFiles(const UpdateFile&);
    QString     readMainBinaryName(const QString&);

signals:
    
public slots:

private:
    void parseXml();

};

#endif // UPDATECLIENT_H
