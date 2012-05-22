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
    void copyFileError(const QString &fileName);
    void readUpdateFileErrorMessage();
};
