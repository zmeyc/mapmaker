// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H

#include "ThirdParty/qtpropertybrowser/src/QtTreePropertyBrowser"

class QtProperty;
class QtGroupPropertyManager;
class QtStringPropertyManager;
class QtIntPropertyManager;
class QtBoolPropertyManager;
class QtProperty;

class LevelObject;

class PropertyBrowser : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    explicit PropertyBrowser(QWidget *parent = 0);

    LevelObject *levelObject() const { return levelObject_; }

signals:
    void hasLevelObject(bool has);

public slots:
    void setLevelObject(LevelObject *object);
    void resetLevelObject();

protected slots:
    void onAddProperty();
    void onIntValueChanged(QtProperty *property, int val);
    void onBoolValueChanged(QtProperty *property, bool val);
    void onStringValueChanged(QtProperty *property, const QString &val);
    void onCustomPropertyValueChanged(QtProperty *property, const QString &val);
    void updateName(const QString &name);
    void updatePosition(const QPointF &pos);
    void updatePosition(LevelObject *levelObject, const QPointF &pos);
    void updateSize(const QSizeF &size);
    void updateFlipX(bool flipX);
    void updateFlipY(bool flipY);
    void onCustomPropertyChanged(const QString &key, const QString &value);

protected:
    void connectToPropertiesOf(LevelObject *object);
    void disconnectFromPropertiesOf(LevelObject *object);

protected:
    QtGroupPropertyManager *groupManager_ = nullptr;
    QtIntPropertyManager *intManager_ = nullptr;
    QtBoolPropertyManager *boolManager_ = nullptr;
    QtStringPropertyManager *stringManager_ = nullptr;
    QtStringPropertyManager *customPropertyManager_ = nullptr;

    QtProperty *standardGroup_ = nullptr;
    QtProperty *name_ = nullptr;
    QtProperty *x_ = nullptr;
    QtProperty *y_ = nullptr;
    QtProperty *width_ = nullptr;
    QtProperty *height_ = nullptr;
    QtProperty *flipX_ = nullptr;
    QtProperty *flipY_ = nullptr;

    QtProperty *customGroup_ = nullptr;
    LevelObject *levelObject_ = nullptr;
};

#endif // PROPERTYBROWSER_H
