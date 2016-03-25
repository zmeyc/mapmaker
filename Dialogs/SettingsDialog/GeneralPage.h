// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <QWidget>

class QGroupBox;
class Settings;

class GeneralPage : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralPage(QWidget *parent = 0);

signals:

public slots:

protected:
    QGroupBox *createGeneralGroupBox();

    Settings *settings_;
};

#endif // GENERALPAGE_H
