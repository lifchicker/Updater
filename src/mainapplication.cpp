#include "mainapplication.h"

#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QProcess>

#include "updateclient.h"
#include "progressdialog.h"

#define APPLICATION_INSTALLER_URL       "http://korobka.in/"

#define UPDATER_TEMP_DIR_NAME           "korobka-update"

#define UPDATE_CURRENT_XML_READ_ERROR   1
#define UPDATE_LATEST_XML_READ_ERROR    2
#define TEMP_DIR_CREATION_FAILED        3

MainApplication::MainApplication(QObject *parent): QObject(parent)
{
}

void MainApplication::showReadUpdateFileErrorMessage()
{
    QMessageBox::critical(NULL,
                          QString::fromUtf8("Ошибка чтения файла"),
                          QString::fromUtf8("Ошибка чтения файла обновлений."),
                          QMessageBox::Ok);
}

void MainApplication::showCopyFileError(const QString &fileName)
{
    QMessageBox::critical(NULL,
                          QString::fromUtf8("Ошибка копирования файла"),
                          QString::fromUtf8("Ошибка при копировании файла ") + fileName.toUtf8(),
                          QMessageBox::Ok);
}

void MainApplication::showRemoveDirectoryError(const QString &directoryName)
{
    QMessageBox::critical(NULL,
                          QString::fromUtf8("Ошибка удаления директории"),
                          QString::fromUtf8("Ошибка при удалении директории ") + directoryName.toUtf8(),
                          QMessageBox::Ok);
}

void MainApplication::executeUpdate()
{
    bool updateCompleted = false;
    int  updatesCount = 0;

    bool installUpdates = false;

    UpdateClient updateClient;

    UpdateFile   *updateCurrent = NULL;
    UpdateFile   *updateLatest = NULL;
    UpdateFile   *updateResulting = NULL;

    QString tempDirectory = QDir::tempPath() + QDir::separator() + UPDATER_TEMP_DIR_NAME;
    QString tempResultDirectory = QDir::tempPath() + QDir::separator() + UPDATER_TEMP_DIR_NAME + "-result";
    QString updaterDirectory = QDir::currentPath();

    if (!(QDir("/").mkpath(tempDirectory) && QDir("/").mkpath(tempResultDirectory)))
    {
        QMessageBox::critical(NULL,
                              QString::fromUtf8("Ошибка создания директории"),
                              QString::fromUtf8("Ошибка при создании временной директории " + tempDirectory.toUtf8() + "\n" +
                                          "Программа не будет обновлена."),
                              QMessageBox::Ok);
        //qApp->exit(TEMP_DIR_CREATION_FAILED);
        emit finished();
        return;
    }


    updateCurrent = updateClient.readUpdateFile("update-current.xml");
    if (!updateCurrent)
    {
        showReadUpdateFileErrorMessage();
        //qApp->exit(UPDATE_CURRENT_XML_READ_ERROR);
        emit finished();
        return;
    }

    //start the application
    QProcess * application = new QProcess();
    QDir applicationDirectory(updaterDirectory);
    applicationDirectory.cdUp();
    application->setWorkingDirectory(applicationDirectory.path());
    application->start(applicationDirectory.path() + QDir::separator() + updateCurrent->mainBinary.fileName);
    if (!application->waitForStarted())
    {
        QMessageBox::warning(NULL,
                             QString::fromUtf8("Внимание"),
                             QString::fromUtf8("Приложение НЕ запущено."),
                             QMessageBox::Ok);
        emit finished();
        return;
    }

    while (!updateCompleted)
    {

        //1.  Read header from update-current.xml from application directory
        if (!updateCurrent)
        {
            updateCurrent = updateClient.readUpdateFile("update-current.xml");
            if (!updateCurrent)
            {
                showReadUpdateFileErrorMessage();
                //qApp->exit(UPDATE_CURRENT_XML_READ_ERROR);
                emit finished();
                return;
            }
        }

        //2.  Download update.xml from
        //      <update><source>/<platform>/update.xml
        //    to temp directory and say it update-latest.xml
        QString updateLatestFileName = "update.xml";
        QString updateFileUrl = updateCurrent->source.toString() + "/" + updateCurrent->platform;

        do
        {
            updateClient.downloadFile(updateFileUrl,
                                      tempDirectory,
                                      updateLatestFileName);

            //3.  Read header from update-latest.xml
            updateLatest = updateClient.readUpdateFile(tempDirectory + QDir::separator() + updateLatestFileName);
            if (!updateLatest)
            {
                showReadUpdateFileErrorMessage();
                emit finished();
                return;
            }

            //4.  Compare <currentVersion> from update-latest.xml and update-current.xml.
            if (updateLatest->currentVersion == updateCurrent->currentVersion)
            {
                // if updating from previous versions
                if (installUpdates && !updateCompleted)
                {
                    // complete update process
                    updateCompleted = true;
                }
                // if "installUpdates" is false, it indicates about there is no updates found therefore exit
                else
                {
                    //exit
                    emit finished();
                    return;
                }
            }

            //5.  Check <currentVersion> of update-current.xml and <previousVersion> from
            //    update-latest.xml file.
            //    If versions are equal say update-latest.xml as update-resulting.xml and
            //    go to Step 7.
            //6.1.  If versions doesn't equal, download previous version of update.xml from
            //        <update><source>/<platform>/<previousVersion>/update.xml
            //      and say it as update-latest.xml
            updateFileUrl = updateLatest->source.toString() + "/" + updateCurrent->platform + "/" + updateLatest->previousVersion;

            // if version older than 5 versions odd
            // exit updater
            updatesCount++;
            if (updatesCount > 5)
            {
                QMessageBox::critical(NULL,
                                      QString::fromUtf8("Версия сильно устарела"),
                                      QString::fromUtf8("Версия вашего программного обеспечения сильно устарела.\n") +
                                      QString::fromUtf8("Пожалуйста, зайдите на сайт вашего ПО и скачайте последнюю версию."),
                                      QMessageBox::Ok);
                //exit
                emit finished();
                return;
            }
            //6.2.  Go to Step 3.
        }
        while ((updateCurrent->currentVersion != updateLatest->previousVersion) && !updateCompleted);

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
                                                                       QString::fromUtf8("Найдена новая версия"),
                                                                       QString::fromUtf8("Доступна новая версия. Обновить?"),
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
        if (!updateClient.copyFile(tempDirectory, updaterDirectory, "update.xml", "update-current.xml"))
        {
            showCopyFileError("update-current.xml");
            emit finished();
            return;
        }

        //11. Move all files to temp directory for result update.
        foreach(FileInfo file, updateResulting->files)
        {
            if (!updateClient.moveFile(tempDirectory, tempResultDirectory, file.fileName))
            {
                showCopyFileError(file.fileName);
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
                             QString::fromUtf8("Обновления готовы"),
                             QString::fromUtf8("Пожалуйста, перезапустите приложение для применения обновлений."),
                             QMessageBox::Ok);

    //13. Wait for application exiting.
    if (application->state() == QProcess::Running)
        application->waitForFinished(-1);

    //14. Copy all files from temp directory for result update to application
    // directory.
    ProgressDialog progressDialog;
    connect(&updateClient, SIGNAL(progressUpdated(int)), progressDialog.getProgressBar(), SLOT(setValue(int)));
    progressDialog.show();

    if (!updateClient.copyFiles(tempResultDirectory, applicationDirectory.path()))
    {
        showCopyFileError("resultTempDirectory");
        emit finished();
        return;
    }

    if (!updateClient.removeDirectory(tempDirectory))
    {
        showRemoveDirectoryError(tempDirectory);
    }

    if (!updateClient.removeDirectory(tempResultDirectory))
    {
        showRemoveDirectoryError(tempResultDirectory);
    }

    progressDialog.close();


    emit finished();
}
