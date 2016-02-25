// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef MAPLAYER_H
#define MAPLAYER_H

#include <QWidget>

class LevelWidget;

class MapLayer : public QWidget
{
    Q_OBJECT
public:
    explicit MapLayer(QWidget *parent = 0);

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
    void setModified(bool modified);

    void deselectAllExcept(LevelWidget *widget);
    void selectActiveWidgets();
    void deleteSelectedWidgets();

signals:
    void widgetSelectionChanged(LevelWidget *widget);
    void widgetPositionChanged(LevelWidget *widget);

public slots:

protected slots:
    void onWidgetSelectedChanged(bool selected);
    void onWidgetPositionChanged();

protected:
    friend class LevelLoader;

    QRect selectionRect() const;
    void addLevelWidget(LevelWidget *widget);

    bool dragging_ = false;
    bool scrolling_ = false;
    bool selecting_ = false;
    QPoint startPos_;
    QPoint prevPos_;
    bool modified_ = false;
};

#endif // MAPLAYER_H
