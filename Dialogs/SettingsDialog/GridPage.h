// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef GRIDPAGE_H
#define GRIDPAGE_H

#include <QWidget>

class QGroupBox;
class Settings;

class GridPage : public QWidget
{
    Q_OBJECT
public:
    explicit GridPage(QWidget *parent = 0);

signals:

public slots:

protected:
    QGroupBox *createGridGroupBox();

    Settings *settings_;
};

#endif // GRIDPAGE_H
