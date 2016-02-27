// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QDragEnterEvent>
#include <QMimeData>
#include <QByteArray>
#include <QApplication>
#include <QPainter>

#include "MapView.h"
#include "Models/LevelObjectsModel.h"
#include "Controls/LevelWidget.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Utils.h"

const char mimeType[] = "application/x-levelobject";

MapView::MapView(QWidget *parent) : QWidget(parent)
{
    WidgetUtils::setBackgroundColor(this, Qt::white);

    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
}

void MapView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (selecting_) {
        QPainter painter(this);
        painter.setPen(Qt::DashLine);
        painter.drawRect(selectionRect());
    }
}

void MapView::dragEnterEvent(QDragEnterEvent *event)
{
    qdbg << "dragEvent: mime=" << event->mimeData()->formats().first() << endl;
    if (!event->mimeData()->hasFormat(mimeType))
        return;
    event->acceptProposedAction();
}

void MapView::dragMoveEvent(QDragMoveEvent *event)
{
    Q_UNUSED(event);
}

void MapView::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
}

void MapView::dropEvent(QDropEvent *event)
{
    qdbg << "dropEvent: text=" << event->mimeData()->formats().first() << endl;
    if (!event->mimeData()->hasFormat(mimeType))
        return;
    event->acceptProposedAction();

    QPoint dragOffset;
    QByteArray nameData;

    QByteArray encodedData = event->mimeData()->data(mimeType);
    QDataStream dataStream(&encodedData, QIODevice::ReadOnly);
    dataStream >> nameData >> dragOffset;
    QString name = QString::fromUtf8(nameData);
    LevelObject *obj = LevelObjectsModel::sharedInstance()->levelObjectByName(name);
    if (!obj)
        return;

    LevelWidget *objWidget = new LevelWidget(this);
    objWidget->setLevelObject(obj);
    QPoint pos(event->pos() - dragOffset);
    objWidget->move(pos);
    objWidget->show();
    addLevelWidget(objWidget);
    modified_ = true;
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        LevelWidget *widget = qobject_cast<LevelWidget *>(childAt(event->pos()));
        if (widget) {
            if (qApp->keyboardModifiers() & Qt::ControlModifier) {
                widget->setSelected(!widget->selected());
            } else {
                if (!widget->selected()) {
                    widget->setSelected(true);
                    deselectAllExcept(widget);
                }
            }
            prevPos_ = event->pos();
            dragging_ = true;
        } else {
            if (!(qApp->keyboardModifiers() & Qt::ShiftModifier))
                deselectAllExcept(nullptr);
            startPos_ = event->pos();
            selecting_ = true;
        }
    } else if (event->button() == Qt::RightButton) {
        prevPos_ = event->pos();
        scrolling_ = true;
    }
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (event->button() == Qt::LeftButton) {
        dragging_ = false;
        if (selecting_) {
            selecting_ = false;
            selectActiveWidgets();
            update();
        }
    } else if (event->button() == Qt::RightButton) {
        scrolling_ = false;
    }
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (selecting_) {
        QRect rect = selectionRect();
        foreach (LevelWidget *widget, findChildren<LevelWidget *>()) {
            bool intersects = widget->geometry().intersects(rect);
            widget->setActive(intersects);
        }
        update();
        return;
    }

    if (!dragging_ && !scrolling_)
        return;

    // Dragging and scrolling are similar right now,
    // except scrolling moves all widgets and dragging
    // moves only selected ones
    foreach (LevelWidget *widget, findChildren<LevelWidget *>()) {
        if (dragging_ && !widget->selected())
            continue;
        widget->move(
                    widget->x() + event->pos().x() - prevPos_.x(),
                    widget->y() + event->pos().y() - prevPos_.y());
        modified_ = true;
    }
    prevPos_ = event->pos();
}

void MapView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Backspace:
        deleteSelectedWidgets();
        break;
    }
}

bool MapView::modified() const
{
    return modified_;
}

void MapView::setModified(bool modified)
{
    modified_ = modified;
}

void MapView::deselectAllExcept(LevelWidget *widget)
{
    foreach (LevelWidget *other, findChildren<LevelWidget *>()) {
        if (other == widget)
            continue;
        other->setSelected(false);
    }
}

void MapView::selectActiveWidgets()
{
    foreach (LevelWidget *widget, findChildren<LevelWidget *>()) {
        if (widget->active()) {
            widget->setActive(false);
            widget->setSelected(true);
        }
    }
}

void MapView::deleteSelectedWidgets()
{
    foreach (LevelWidget *widget, findChildren<LevelWidget *>()) {
        if (widget->selected())
            delete widget;
    }
}

void MapView::onWidgetSelectedChanged(bool selected)
{
    Q_UNUSED(selected);
    LevelWidget *widget = qobject_cast<LevelWidget *>(sender());
    if (!widget)
        return;
    emit widgetSelectionChanged(widget);
}

void MapView::onWidgetPositionChanged()
{
    LevelWidget *widget = qobject_cast<LevelWidget *>(sender());
    if (!widget)
        return;
    emit widgetPositionChanged(widget);
}

QRect MapView::selectionRect() const
{
    QPoint pos(mapFromGlobal(QCursor::pos()));
    return QRect(startPos_.x(), startPos_.y(),
                 pos.x() - startPos_.x(), pos.y() - startPos_.y());
}

void MapView::addLevelWidget(LevelWidget *widget)
{
    if (widget->parent() == nullptr)
        widget->setParent(this);
    connect(widget, SIGNAL(selectedChanged(bool)),
            this, SLOT(onWidgetSelectedChanged(bool)));
    connect(widget, SIGNAL(positionChanged()),
            this, SLOT(onWidgetPositionChanged()));
}
