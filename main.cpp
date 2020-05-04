#include <QApplication>

#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window(QCoreApplication::arguments());
    window.show();
    return app.exec();
}
