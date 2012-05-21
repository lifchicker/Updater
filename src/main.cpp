#include "mainapplication.h"

#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setQuitOnLastWindowClosed(false);

    MainApplication * mainApp = new MainApplication(&app);

    QObject::connect(mainApp, SIGNAL(finished()), &app, SLOT(quit()));

    QTimer::singleShot(0, mainApp, SLOT(executeUpdate()));

    return app.exec();
}
