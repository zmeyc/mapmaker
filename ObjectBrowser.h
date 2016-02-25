// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H

#include <QWidget>

class QListView;

class ObjectBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit ObjectBrowser(QWidget *parent = 0);

signals:

public slots:

protected slots:
    void onPressed(const QModelIndex &index);

protected:
    QListView *listView_ = nullptr;
};

#endif // OBJECTBROWSER_H
