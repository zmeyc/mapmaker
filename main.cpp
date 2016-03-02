// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QApplication>
#include <QMessageBox>

#include "MainWindow.h"
#include "Models/LevelObjectsModel.h"
#include "Controllers/LevelLoader.h"
#include "Utils/Settings.h"
#include "Utils/Utils.h"

extern void qt_set_sequence_auto_mnemonic(bool b);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setAttribute(Qt::AA_UseHighDpiPixmaps);

    qt_set_sequence_auto_mnemonic(true);

    QApplication::setOrganizationDomain("com.zabiyaka");
    QApplication::setOrganizationName("Zabiyaka");
    QApplication::setApplicationName("MapMaker");
    QApplication::setApplicationVersion("1.0");

    Settings *settings = Settings::sharedInstance();
    if (!settings->parseCommandLine())
        return 1;

    if (!settings->imagesDirectory().isEmpty()) {
        LevelObjectsModel::sharedInstance()->addImagesFromDirectory(
                    settings->imagesDirectory());
    }

    MainWindow w;
    w.show();

    return a.exec();
}
