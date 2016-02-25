// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QFileInfo>
#include "FileUtils.h"

QString nameFromFilename(const QString &filename)
{
    QFileInfo fileInfo(filename);
    return fileInfo.baseName();
}
