// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <QWidget>

class QGroupBox;
class Settings;

class DirectoryLineEdit;

class GeneralPage : public QWidget
{
    Q_OBJECT
public:
    explicit GeneralPage(QWidget *parent = 0);

signals:

public slots:

protected slots:
    void onImagesDirectoryEditingFinished();

protected:
    QGroupBox *createGeneralGroupBox();

    Settings *settings_ = nullptr;
    DirectoryLineEdit *imagesDirectoryEdit_ = nullptr;
};

#endif // GENERALPAGE_H
