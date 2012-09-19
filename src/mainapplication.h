#include <QObject>

class MainApplication:
        public QObject
{
    Q_OBJECT

public:
    MainApplication(QObject * parent = 0);

public slots:
    void executeUpdate();

signals:
    void finished();

private:
    void showCopyFileError(const QString &fileName);
    void showReadUpdateFileErrorMessage();
    void showRemoveDirectoryError(const QString &directoryName);
};
