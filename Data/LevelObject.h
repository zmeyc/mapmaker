// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include <QObject>
#include <QImage>
#include <QPointF>

class LevelObject: public QObject
{
    Q_OBJECT
public:
    explicit LevelObject(QObject *parent = nullptr);

    LevelObject *clone(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &name);

    QString filename() const;
    void setFilename(const QString &filename);

    QImage image() const;
    void setImage(const QImage &image);

    QPointF position() const;
    qreal x() const;
    qreal y() const;
    void setPosition(const QPointF &pos);
    void setPosition(qreal x, qreal y);
    void setX(qreal x);
    void setY(qreal y);

    QSizeF size() const;
    void setSize(const QSizeF &size);

    bool flipX() const;
    void setFlipX(bool flipX);

    bool flipY() const;
    void setFlipY(bool flipY);

signals:
    void modified();
    void positionChanged(const QPointF &pos);
    void willChangeSize(const QSizeF &newSize);
    void sizeChanged(const QSizeF &size);
    void flipXChanged(bool flipX);
    void flipYChanged(bool flipY);

protected:
    QString name_;
    QString filename_;
    QImage image_;
    QPointF position_;
    QSizeF size_;
    bool flipX_ = false;
    bool flipY_ = false;
};

#endif // LEVELOBJECT_H
