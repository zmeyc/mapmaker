// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QPixmap>

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

    bool modified() const;
    Q_SLOT void setModified(bool modified = true);

    LevelObject *selectedLevelObject() const { return selectedLevelObject_; }

    void resetScene();
    void selectActiveWidgets();
    void deleteSelectedWidgets();
    void moveSelectedItemsBy(int dx, int dy);

signals:
    void selectedLevelObjectChanged(LevelObject *item);
    void sceneCreated(QGraphicsScene *scene);

public slots:

protected slots:
    void setSelectedLevelObject(LevelObject *object);
    void selectSingleItem(MapItem *item);
    void updateGridPixmap();

protected:
    using MapItems = QList<MapItem *>;

    enum DragState {
        NotDragging,
        AboutToDrag,
        Dragging
    };

    friend class LevelLoader;

    QRect selectionRect() const;
    MapItems selectedItems() const;
    QRectF itemsBoundingRect(const MapItems &items) const;

    void snapToGrid(qreal firstLine, qreal secondLine, qreal gridSize, qreal *newFirstLine, qreal *newSecondLine) const;
    QRectF snapToGrid(const QRectF &rect, const QSizeF &gridSize, bool bothSides = false) const;
    MapItems cloneItems(const MapItems &items);
    void selectItems(const MapItems &items, bool select);

    Settings *settings_ = nullptr;
    DragState dragState_ = NotDragging;
    bool scrolling_ = false;
    bool selecting_ = false;
    QPoint startPos_;
    QPoint prevPos_;
    bool modified_ = false; // TODO: move to scene?
    QPixmap gridPixmap_;

    MapItems draggedItems_;
    QRectF dragInitialBounds_;
    QRectF dragPrevBounds_;

    LevelObject *selectedLevelObject_ = nullptr;
};

#endif // MAPVIEW_H
