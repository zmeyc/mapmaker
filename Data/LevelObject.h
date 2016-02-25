// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include <QObject>
#include <QImage>

class LevelObject: public QObject
{
    Q_OBJECT
public:
    explicit LevelObject(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &name);

    QString filename() const;
    void setFilename(const QString &filename);

    QImage image() const;
    void setImage(const QImage &image);

    bool flipX() const;
    void setFlipX(bool flipX);

    bool flipY() const;
    void setFlipY(bool flipY);

protected:
    QString name_;
    QString filename_;
    QImage image_;
    bool flipX_ = false;
    bool flipY_ = false;
};

#endif // LEVELOBJECT_H
