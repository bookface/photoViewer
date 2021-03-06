#include <QApplication>
// #include <QCommandLineParser>

#include "mainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

// moved to mainWindow.cpp
// hide the mouse cursor
//    QApplication::setOverrideCursor(Qt::BlankCursor);

// replaced with processCommandLine.cpp
    
#if 0
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("directory"
                                 ,QCoreApplication::translate("main"
                                                              ,"Directory to open photos."));
    parser.addPositionalArgument("seconds"
                                 ,QCoreApplication::translate("main"
                                                              ,"Display time in seconds."));
    parser.addPositionalArgument("filename"
                                 ,QCoreApplication::translate("main"
                                                              ,"Display filename over image (1=yes)."));
// Process the actual command line arguments given by the user
    parser.process(app);
#endif
    
    MainWindow window(QCoreApplication::arguments());
    return app.exec();
}
