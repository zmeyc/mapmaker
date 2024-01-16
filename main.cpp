// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QApplication>
#include <QMessageBox>

#include "MainWindow.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"

extern void qt_set_sequence_auto_mnemonic(bool b);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qt_set_sequence_auto_mnemonic(true);

    QApplication::setOrganizationDomain("com.zabiyaka");
    QApplication::setOrganizationName("Zabiyaka");
    QApplication::setApplicationName("MapMaker");
    QApplication::setApplicationVersion("1.0");

    Settings *settings = Settings::sharedInstance();
    if (!settings->parseCommandLine())
        return 1;

    MainWindow w;
    w.show();

    return a.exec();
}
