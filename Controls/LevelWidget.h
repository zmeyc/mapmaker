// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef LEVELOBJECTWIDGET_H
#define LEVELOBJECTWIDGET_H

#include <QWidget>

class LevelObject;

class LevelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LevelWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

    void moveEvent(QMoveEvent *event) override;

    LevelObject *levelObject() const;
    void setLevelObject(LevelObject *obj);

    bool selected() const;
    void setSelected(bool selected);

    bool active() const;
    void setActive(bool active);

    bool flipX() const;
    void setFlipX(bool flipX);

    bool flipY() const;
    void setFlipY(bool flipY);

signals:
    void selectedChanged(bool selected);
    void positionChanged();

public slots:

protected:
    LevelObject *obj_ = nullptr;
    bool selected_ = false;
    bool active_ = false;
    bool flipX_ = false;
    bool flipY_ = false;
};

#endif // LEVELOBJECTWIDGET_H
