// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include <QObject>
#include <QImage>
#include <QPointF>
#include <QMap>

class LevelObject: public QObject
{
    Q_OBJECT
public:
    using Properties = QMap<QString, QString>;

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
    void setWidth(qreal width);
    void setHeight(qreal height);

    bool flipX() const;
    void setFlipX(bool flipX);

    bool flipY() const;
    void setFlipY(bool flipY);

    QString customProperty(const QString &key);
    void setCustomProperty(const QString &key, const QString &value);
    void resetCustomProperty(const QString &key);
    Properties customProperties();

signals:
    void modified();
    void nameChanged(const QString &name);
    void positionChanged(const QPointF &pos);
    void willChangeSize(const QSizeF &newSize);
    void sizeChanged(const QSizeF &size);
    void flipXChanged(bool flipX);
    void flipYChanged(bool flipY);
    void customPropertyChanged(const QString &key, const QString &value);
    void imageChanged();

protected:
    QString name_;
    QString filename_;
    QImage image_;
    QPointF position_;
    QSizeF size_;
    bool flipX_ = false;
    bool flipY_ = false;
    Properties customProperties_;
};

#endif // LEVELOBJECT_H
