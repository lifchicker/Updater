#include "updateclient.h"

#include <QFile>
#include <QDir>
#include <QCryptographicHash>
#include <QHttp>
#include <QXmlStreamReader>

#include <unistd.h>

UpdateClient::UpdateClient(QObject *parent) :
    QObject(parent)
{
}

inline bool UpdateClient::copyFile(const QString &srcDir, const QString &destDir, const QString &fileName)
{
    return copyFile(srcDir, destDir, fileName, fileName);
}


bool UpdateClient::copyFile(const QString &srcDir, const QString &destDir, const QString &srcFileName, const QString &destFileName)
{
    QString sourceFile(QDir::cleanPath(srcDir + QDir::separator() + srcFileName));
    QString destFile(QDir::cleanPath(destDir + QDir::separator() + destFileName));

    //if source file doesn't exists
    if (!QFile::exists(sourceFile))
    {
        return false;
    }

    // check if dest directory not exists
    if (!QDir(QDir::cleanPath(destDir)).exists())
    {
        // try to create dest dir
        if (!QDir(QDir::rootPath()).mkpath(destDir))
            return false;
    }

    // if dest file exists
    if (QFile::exists(destFile))
    {
        // remove it (analog rewrite functionality)
        QFile::remove(destFile);
    }

    return QFile::copy(sourceFile, destFile);
}


bool UpdateClient::copyFiles(const QDir &srcDir, const QDir &destDir)
{
    bool withoutErrors = true;

    if (!srcDir.exists())
        return false;

    // list all directories
    QStringList entries = srcDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    foreach (QString entry, entries)
    {
        QFileInfo entryInfo(QString("%1/%2").arg(srcDir.path()).arg(entry));

        if (entryInfo.isSymLink())
            continue;

        if (entryInfo.isDir())
        {
            QDir nextSrcDir(srcDir);
            QDir nextDestDir(destDir);

            //change directory deeper
            nextSrcDir.cd(entryInfo.fileName());

            //if can't change destination directory (probably doesn't exists)
            if (!nextDestDir.cd(entryInfo.fileName()))
            {
                // create directory
                nextDestDir.mkdir(entryInfo.fileName());

                //try change directory
                if (!nextDestDir.cd(entryInfo.fileName()))
                {
                    //stop copy and return
                    return false;
                }

            }

            // go deeper
            withoutErrors &= copyFiles(nextSrcDir, nextDestDir);
        }
        else
        {
            withoutErrors &= copyFile(srcDir.path(), destDir.path(), entryInfo.fileName());
        }
    }


    return withoutErrors;
}


bool UpdateClient::downloadFile(const QUrl &url, const QString &destination, const QString &fileName)
{
    QHttp   http;
    QString fullFileName = destination + "/" + fileName;
    QFile   *file        = new QFile(fullFileName);

    QUrl    fullUrl(url);
    fullUrl.setPath(url.path() + fileName);

    http.setHost(fullUrl.host(), fullUrl.port(80));
    http.get(fullUrl.path(), file);

    while (http.currentId() != 0)
    {
        //qApp->processEvents();
        sleep(10);
    }

    file->close();
    delete file;

    if (http.error() != QHttp::NoError)
    {
        return false;
    }

    return true;
}


bool UpdateClient::isFileValid(const QString &path, const FileInfo &fileInfo)
{
    QFile file(QDir::cleanPath(path + QDir::separator() + fileInfo.fileName));
    file.open(QFile::ReadOnly);

    // are files sizes equal ?
    if (file.size() != fileInfo.size)
    {
        return false;
    }

    QCryptographicHash hasher(QCryptographicHash::Md5);

    while (!file.atEnd())
    {
        hasher.addData(file.read(1024));
    }

    // are files hashes equal?
    return hasher.result() == fileInfo.hash;
}


inline bool UpdateClient::moveFile(const QString &srcDir, const QString &destDir, const QString &fileName)
{
    return moveFile(srcDir, destDir, fileName, fileName);
}


bool UpdateClient::moveFile(const QString &srcDir, const QString &destDir, const QString &srcFileName, const QString &destFileName)
{
    // copy file
    if (!copyFile(srcDir, destDir, srcFileName, destFileName))
    {
        return false;
    }

    // remove source file
    return QFile::remove(QDir::cleanPath(srcDir + QDir::separator() + srcFileName));
}


UpdateFile * UpdateClient::readUpdateFile(const QString &fileName)
{
    UpdateFile * updateFile = new UpdateFile();

    // open file
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return NULL;
    }

    // create and open xml stream
    QXmlStreamReader xml;
    xml.setDevice(&file);

    // read update file
    if (xml.readNextStartElement())
    {
        if (xml.name() == "update")
        {
            // read content
            while (xml.readNextStartElement())
            {
                if (xml.name()      == "currentVersion")
                {
                    updateFile->currentVersion = xml.readElementText().toUtf8();
                }
                else if (xml.name() == "previousVersion")
                {
                    updateFile->previousVersion = xml.readElementText().toUtf8();
                }
                else if (xml.name() == "platform")
                {
                    updateFile->platform = xml.readElementText();
                }
                else if (xml.name() == "source")
                {
                    updateFile->source = QUrl(xml.readElementText());
                }
                else if (xml.name() == "application-main-binary")
                {
                    while (xml.readNextStartElement())
                    {
                        if (xml.name() == "file")
                        {
                            updateFile->mainBinary = readFileInfo(xml);
                        }
                        else
                            xml.skipCurrentElement();
                    }
                }
                else if (xml.name() == "install")
                {
                    while(xml.readNextStartElement())
                    {
                        if (xml.name() == "file")
                        {
                            updateFile->files.append(readFileInfo(xml));
                        }
                        else
                            xml.skipCurrentElement();
                    }
                }
                else
                    xml.skipCurrentElement();
            }
        }
        else
            return NULL;
        //xml.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
    }

    return updateFile;
}

FileInfo UpdateClient::readFileInfo(QXmlStreamReader &xml)
{
    FileInfo fileInfo;

    while (xml.readNextStartElement())
    {
        if (xml.name()      == "name")
        {
            fileInfo.fileName = xml.readElementText();
        }
        else if (xml.name() == "size")
        {
            fileInfo.size = xml.readElementText().toLongLong();
        }
        else if (xml.name() == "hash")
        {
            fileInfo.hash = xml.readElementText().toUtf8();
        }
        else
            xml.skipCurrentElement();
    }

    return fileInfo;
}
