// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QListView>
#include <QVBoxLayout>

#include "ObjectBrowser.h"
#include "Models/LevelObjectsModel.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Utils.h"

ObjectBrowser::ObjectBrowser(QWidget *parent) : QWidget(parent)
{
    //WidgetUtils::setBackgroundColor(this, Qt::blue);

    listView_ = new QListView;
    listView_->setModel(LevelObjectsModel::sharedInstance());
    listView_->setSpacing(5);
    listView_->setViewMode(QListView::IconMode);
    listView_->setAlternatingRowColors(false);
    listView_->setSelectionMode(QAbstractItemView::SingleSelection);
    listView_->setMovement(QListView::Static);
    listView_->setResizeMode(QListView::Adjust);
    listView_->setUniformItemSizes(false);
    listView_->setDragDropMode(QAbstractItemView::DragOnly);
    listView_->setDragEnabled(true);
    connect(listView_, SIGNAL(pressed(QModelIndex)),
            this, SLOT(onPressed(QModelIndex)));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(listView_);
    setLayout(layout);
}

void ObjectBrowser::onPressed(const QModelIndex &index)
{
    Q_UNUSED(index);
    QRect visualRect = listView_->visualRect(index);
    QPoint pos = listView_->viewport()->mapFromGlobal(QCursor::pos());
    //QPoint pos = listView_->viewport()->mapFromGlobal(listView_->cursor().pos());
    //qerr << "viewport.mar=" << listView_->viewportMargins().top() << endl;

    // Compensate 3 pixel offset ListView bug.
    // See LevelObjectsModel.cpp, SizeHintRole:
    pos.setY(pos.y() - 3);

    LevelObjectsModel::sharedInstance()->setDragOffset(pos - visualRect.topLeft());
}
