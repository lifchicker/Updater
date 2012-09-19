#ifndef UPDATECLIENT_H
#define UPDATECLIENT_H

#include <QObject>

#include <QString>
#include <QUrl>
#include <QVector>

class QDir;
class QXmlStreamReader;


struct FileInfo
{
    QString     fileName;   //file name with relative path
    qint64      size;
    QByteArray  hash;
};


struct FileToCopyInfo
{
    QString srcDir;
    QString destDir;
    QString fileName;
};

struct UpdateFile
{
    QByteArray          currentVersion;
    QByteArray          previousVersion;
    QString             platform;
    QUrl                source;
    FileInfo            mainBinary;
    QVector<FileInfo>   files;
};


class UpdateClient : public QObject
{
    Q_OBJECT
public:
    explicit UpdateClient(QObject *parent = 0);
    
    bool        copyFile(const QString& srcDir, const QString& destDir, const QString& fileName);
    bool        copyFile(const QString& srcDir, const QString& destDir, const QString& srcFileName, const QString& destFileName);
    bool        copyFiles(const QDir &srcDir, const QDir &destDir);
    bool        downloadFile(const QUrl&url, const QString &destination, const QString &fileName);
    bool        isFileValid(const QString& path, const FileInfo& fileInfo);
    bool        moveFile(const QString& srcDir, const QString& destDir, const QString& fileName);
    bool        moveFile(const QString& srcDir, const QString& destDir, const QString& srcFileName, const QString& destFileName);
    UpdateFile *readUpdateFile(const QString& fileName);
    bool        removeDirectory(const QString& dir);

signals:
    void progressUpdated(int progress);

public slots:

private:
    FileInfo   readFileInfo(QXmlStreamReader &xml);
};

#endif // UPDATECLIENT_H
