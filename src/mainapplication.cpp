#include "mainapplication.h"

#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QProcess>

#include "updateclient.h"

#define APPLICATION_INSTALLER_URL "http://korobka.in/"

#define UPDATE_CURRENT_XML_READ_ERROR   1
#define UPDATE_LATEST_XML_READ_ERROR    2
#define TEMP_DIR_CREATION_FAILED        3

MainApplication::MainApplication(QObject *parent): QObject(parent)
{
}

void MainApplication::readUpdateFileErrorMessage()
{
    QMessageBox::critical(NULL,
                          QObject::tr("Read file error"),
                          QObject::tr("Update file reading error"),
                          QMessageBox::Ok);
}

void MainApplication::copyFileError(const QString &fileName)
{
    QMessageBox::critical(NULL,
                          QObject::tr("Copy file error"),
                          QObject::tr("Error occured while copying file ") + fileName.toUtf8(),
                          QMessageBox::Ok);
}

void MainApplication::executeUpdate()
{
    bool updateCompleted = false;

    bool installUpdates = false;

    UpdateClient updateClient;

    UpdateFile   *updateCurrent = NULL;
    UpdateFile   *updateLatest = NULL;
    UpdateFile   *updateResulting = NULL;

    QString tempDirectory = QDir::tempPath() + QDir::separator() + "Korobka-update";
    QString tempResultDirectory = QDir::tempPath() + QDir::separator() + "Korobka-result-update";
    QString applicationDirectory = ".";

    if (!(QDir("/").mkpath(tempDirectory) && QDir("/").mkpath(tempResultDirectory)))
    {
        QMessageBox::critical(NULL,
                              QObject::tr("Directory creation error"),
                              QObject::tr("Can't create temp directory in " + tempDirectory.toUtf8() + "\n" +
                                          "Update failed."),
                              QMessageBox::Ok);
        //qApp->exit(TEMP_DIR_CREATION_FAILED);
        emit finished();
        return;
    }


    updateCurrent = updateClient.readUpdateFile("update-current.xml");
    if (!updateCurrent)
    {
        readUpdateFileErrorMessage();
        //qApp->exit(UPDATE_CURRENT_XML_READ_ERROR);
        emit finished();
        return;
    }

    //start the application
    QProcess * application = new QProcess();
    application->start(updateCurrent->mainBinary.fileName);
    if (!application->waitForStarted())
    {
        QMessageBox::warning(NULL,
                             QObject::tr("Warning"),
                             QObject::tr("Application doesn't started."),
                             QMessageBox::Ok);
    }

    while (!updateCompleted)
    {

        //1.  Read header from update-current.xml from application directory
        if (!updateCurrent)
        {
            updateCurrent = updateClient.readUpdateFile("update-current.xml");
            if (!updateCurrent)
            {
                readUpdateFileErrorMessage();
                //qApp->exit(UPDATE_CURRENT_XML_READ_ERROR);
                emit finished();
                return;
            }
        }

        //2.  Download update.xml from
        //      <update><source>/<platform>/update.xml
        //    to temp directory and say it update-latest.xml
        QString updateLatestFileName = "update.xml";
        updateClient.downloadFile(updateCurrent->source.toString() +
                                  "/" + updateCurrent->platform,
                                  tempDirectory,
                                  updateLatestFileName);

        //3.  Read header from update-latest.xml
        updateLatest = updateClient.readUpdateFile(tempDirectory + QDir::separator() + updateLatestFileName);
        if (!updateLatest)
        {
            readUpdateFileErrorMessage();
            qApp->exit(UPDATE_LATEST_XML_READ_ERROR);
        }

        //4.  Compare <currentVersion> from update-latest.xml and update-current.xml.
        if (updateLatest->currentVersion == updateCurrent->currentVersion)
        {
            //        QMessageBox::information(NULL,
            //                                 QObject::tr("No updates"),
            //                                 QObject::tr("No updates."),
            //                                 QMessageBox::Ok);

            // if updating from previous versions
            if (installUpdates && !updateCompleted)
            {
                // complete update process
                updateCompleted = true;
                continue;
            }
            // if "installUpdates" is false, it indicates about there is no updates found therefore exit
            else
            {
                //exit
                //qApp->exit();
                emit finished();
                return;
            }
        }

        //5.  Check <currentVersion> of update-current.xml and <previousVersion> from
        //    update-latest.xml file.
        //    If versions are equal say update-latest.xml as update-resulting.xml and
        //    go to Step 7.
        if (updateCurrent->currentVersion != updateLatest->previousVersion)
        {
            // put code for update from previous ( N times repeat) version

            //6.1.  If versions doesn't equal, download previous version of update.xml from
            //        <update><source>/<platform>/<previousVersion>/update.xml
            //      and say it as update-latest.xml

            //6.2.  Go to Step 3.

            QMessageBox::critical(NULL,
                                  QObject::tr("Update is not incremental"),
                                  QObject::tr("Current version is step aside from new version more then one update."
                                              "Please, download and install full version of application from <a href=\""
                                              APPLICATION_INSTALLER_URL
                                              "\">"
                                              APPLICATION_INSTALLER_URL
                                              "</a>"),
                                  QMessageBox::Ok);

            //exit
            emit finished();
            return;
        }

        if (updateResulting)
        {
            delete updateResulting;
            updateResulting = NULL;
        }
        updateResulting = updateLatest;
        updateLatest = NULL;

        //7.  If flag "installUpdates" is false, show dialog "New version found. Update?"
        //    and set flag to result of dialog choice.
        if (!installUpdates)
        {
            installUpdates = QMessageBox::Yes == QMessageBox::question(NULL,
                                                                       QObject::tr("New version found"),
                                                                       QObject::tr("New version is available. Update?"),
                                                                       QMessageBox::Yes | QMessageBox::No);

            //8.  If flag "installUpdates" is false, exit.
            if (!installUpdates)
            {
                //exit
                emit finished();
                return;
            }
        }

        //9. For each file do
        foreach(FileInfo file, updateResulting->files)
        {
            for (int i = 0; i < 3; i++)
            {
                //9.1.  Download file from
                //  <update><source>/<platform>/<currentVersion>/<install><file><name>
                // to temp directory.
                if (!updateClient.downloadFile(updateResulting->source.toString() +
                                               "/" + updateResulting->platform +
                                               "/" + updateResulting->currentVersion,
                                               tempDirectory,
                                               file.fileName))
                    continue;

                //9.2.  Check file size and file hash.
                //  If file size or hash invalid, go to Step 10.1. with the same file name.
                //  If check fails N times, show message "Update error." and exit.
                if (updateClient.isFileValid(tempDirectory, file))
                    break;
            }
        }

        //10. Rename update-resulting.xml to update-current.xml and copy to application
        //      directory.
        if (!updateClient.copyFile(tempDirectory, applicationDirectory, "update.xml", "update-current.xml"))
        {
            copyFileError("update-current.xml");
            emit finished();
            return;
        }

        //11. Move all files to temp directory for result update.
        foreach(FileInfo file, updateResulting->files)
        {
            if (!updateClient.moveFile(tempDirectory, tempResultDirectory, file.fileName))
            {
                copyFileError(file.fileName);
                emit finished();
                return;
            }
        }

        //cleanup
        if (updateCurrent)
        {
            delete updateCurrent;
            updateCurrent = NULL;
        }
        if (updateLatest)
        {
            delete updateLatest;
            updateLatest = NULL;
        }
        if (updateResulting)
        {
            delete updateResulting;
            updateResulting = NULL;
        }
        //12. Go to Step 1.
    }

    QMessageBox::information(NULL,
                             QObject::tr("Update ready"),
                             QObject::tr("Please restart application to complete update process"),
                             QMessageBox::Ok);

    //13. Wait for application exiting.
    if (application->state() == QProcess::Running)
        application->waitForFinished(-1);

    //14. Copy all files from temp directory for result update to application
    // directory.
    if (!updateClient.copyFiles(tempResultDirectory, applicationDirectory))
    {
        copyFileError("resultTempDirectory");
        emit finished();
        return;
    }


    emit finished();
}
