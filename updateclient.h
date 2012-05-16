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
    
    bool        copyFile(const QString& srcDir, const QString& destDir, const QString& fileName);
    bool        copyFile(const QString& srcDir, const QString& destDir, const QString& srcFileName, const QString& destFileName);
    bool        copyFiles(const QString& srcDir, const QString& destDir);
    QString     downloadFile(const QUrl&);
    bool        moveFile(const QString& srcDir, const QString& destDir, const QString& fileName);
    bool        moveFile(const QString& srcDir, const QString& destDir, const QString& srcFileName, const QString& destFileName);
    UpdateFile  readHeader(const QString&);
    void        readListOfFiles(const UpdateFile&);
    QString     readMainBinaryName(const QString&);

signals:
    
public slots:

private:
    void parseXml();

};

#endif // UPDATECLIENT_H
