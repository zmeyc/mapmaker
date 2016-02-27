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

public slots:
    void setLevelObject(LevelObject *object);
    void resetLevelObject();
    void updatePosition(const QPointF &pos);

protected:
    void connectSignals(LevelObject *object);
    void disconnectSignals(LevelObject *object);

protected:
    QtGroupPropertyManager *groupManager_ = nullptr;
    QtStringPropertyManager *stringManager_ = nullptr;
    QtIntPropertyManager *intManager_ = nullptr;
    QtBoolPropertyManager *boolManager_ = nullptr;

    QtProperty *standardGroup_ = nullptr;
    QtProperty *x_ = nullptr;
    QtProperty *y_ = nullptr;

    QtProperty *customGroup_ = nullptr;
    LevelObject *levelObject_ = nullptr;
};

#endif // PROPERTYBROWSER_H
