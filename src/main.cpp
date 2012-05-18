#include <QApplication>
#include <QMessageBox>
#include <QDir>
#include <QProcess>

#include "updateclient.h"

#define APPLICATION_INSTALLER_URL "http://korobka.in/"

#define UPDATE_CURRENT_XML_READ_ERROR 1
#define UPDATE_LATEST_XML_READ_ERROR  2

void readUpdateFileErrorMessage()
{
    QMessageBox::critical(NULL,
                          QObject::tr("Read file error"),
                          QObject::tr("Update file reading error"),
                          QMessageBox::Ok);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    bool updateCompleted = false;

    bool installUpdates = false;

    UpdateClient updateClient;

    UpdateFile   *updateCurrent;
    UpdateFile   *updateLatest;
    UpdateFile   *updateResulting;

    QString tempDirectory = QDir::tempPath() + QDir::separator() + "Korobka-update" + QDir::separator();
    QString tempResultDirectory = QDir::tempPath() + QDir::separator() + "Korobka-result-update" + QDir::separator();
    QString applicationDirectory = "";

    updateCurrent = updateClient.readUpdateFile("update-current.xml");
    if (!updateCurrent)
    {
        readUpdateFileErrorMessage();
        return UPDATE_CURRENT_XML_READ_ERROR;
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
                return UPDATE_CURRENT_XML_READ_ERROR;
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
        updateLatest = updateClient.readUpdateFile(tempDirectory + updateLatestFileName);
        if (!updateLatest)
        {
            readUpdateFileErrorMessage();
            return UPDATE_LATEST_XML_READ_ERROR;
        }

        //4.  Compare <currentVersion> from update-latest.xml and update-current.xml.
        if (updateLatest->currentVersion == updateLatest->currentVersion)
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
                return 0;
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
            return 0;
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
                                                                       QMessageBox::Yes | QMessageBox::No,
                                                                       QMessageBox::Yes);

            //8.  If flag "installUpdates" is false, exit.
            if (!installUpdates)
            {
                //exit
                return 0;
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
                updateClient.downloadFile(updateResulting->source.toString() +
                                          "/" + updateResulting->platform +
                                          "/" + updateResulting->currentVersion,
                                          tempDirectory,
                                          file.fileName);

                //9.2.  Check file size and file hash.
                //  If file size or hash invalid, go to Step 10.1. with the same file name.
                //  If check fails N times, show message "Update error." and exit.
                if (updateClient.isFileValid(tempDirectory, file))
                    break;
            }
        }

        //10. Move all files to temp directory for result update.
        foreach(FileInfo file, updateResulting->files)
        {
            updateClient.moveFile(tempDirectory, tempResultDirectory, file.fileName);
        }

        //11. Rename update-resulting.xml to update-current.xml and copy to application
        //      directory.
        updateClient.copyFile(tempDirectory, applicationDirectory, "update.xml", "update-current.xml");

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
    updateClient.copyFiles(tempResultDirectory, applicationDirectory);

    return 0;
}
