#include "mainWindow.h"
#include "commandLineOptions.h"

#include <iostream>
using namespace std;
#define P(X) qPrintable(X)

void MainWindow::processCommandLine(void)
{
    CommandLineOptions options;
    options.add("c",&_hideCursor);
    options.add("d",&_directory);
    options.add("f",&_displayFileName);
    options.add("s",&_secondsToShowImage);
    options.add("sleep",&_sleepMode);
    options.parse();
#if 1
    cout << "Directory " << P(_directory.toString()) << endl;
    cout << "Display File Name " << _displayFileName.toBool() << endl;
    cout << "Seconds " << _secondsToShowImage.toInt() << endl;
    cout << "Sleep " << _sleepMode.toBool() << endl;
#endif
    
}