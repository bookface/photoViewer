#include <QApplication>
#include <QCommandLineParser>

#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("directory", QCoreApplication::translate("main", "Directory to open photos."));
    parser.addPositionalArgument("seconds", QCoreApplication::translate("main", "Display time in seconds."));
    parser.addPositionalArgument("filename", QCoreApplication::translate("main", "Display filename over image (1=yes)."));
// Process the actual command line arguments given by the user
    parser.process(app);

    MainWindow window(QCoreApplication::arguments());
    window.show();
    return app.exec();
}
