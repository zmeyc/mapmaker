// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QIcon;
class QListWidgetItem;
class QListWidget;
class QStackedWidget;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

signals:

public slots:

protected slots:
    void onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

protected:
    QListWidgetItem *createCategoryItem(const QIcon &icon, const QString &text);

    QListWidget *categories_ = nullptr;
    QStackedWidget *pages_ = nullptr;
};

#endif // SETTINGSDIALOG_H
