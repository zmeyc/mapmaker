// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <functional>
#include <QGraphicsView>
#include <QPixmap>

class QGraphicsItem;
class Settings;
class LevelObject;
class MapScene;
class MapItem;

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MapView(QWidget *parent = 0);

    MapScene *mapScene() const;

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;

    void paintEvent(QPaintEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void resetScene();
    void selectActiveItems();
    void moveOrCloneSelectedItemsBy(int dx, int dy);

signals:
    void sceneCreated(QGraphicsScene *scene);

public slots:
    void copy();
    void paste();

protected slots:
    void updateGridPixmap();
    void onZoomChanged(double zoom);

protected:
    using MapItems = QList<MapItem *>;

    enum DragState {
        NotDragging,
        AboutToDrag,
        Dragging
    };

    friend class LevelLoader;

    QRect selectionRect() const;

    void snapToGrid(qreal firstLine, qreal secondLine, qreal gridSize, qreal *newFirstLine, qreal *newSecondLine) const;
    QRectF snapToGrid(const QRectF &rect, const QSizeF &gridSize, bool bothSides = false) const;

    QPointF snapToDockPoints(const QPointF &movedByDelta) const;

    Settings *settings_ = nullptr;
    DragState dragState_ = NotDragging;
    bool scrolling_ = false;
    bool selecting_ = false;
    QPoint startPos_;
    QPoint prevPos_;
    QPixmap gridPixmap_;
    double zoom_ = 1.0;

    MapItems draggedItems_;
    QRectF dragInitialBounds_;
    QRectF dragPrevBounds_;
};

#endif // MAPVIEW_H
