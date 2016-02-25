// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef PROPERTYBROWSER_H
#define PROPERTYBROWSER_H

#include "ThirdParty/qtpropertybrowser/src/QtTreePropertyBrowser"

class QtProperty;
class LevelWidget;
class QtGroupPropertyManager;
class QtStringPropertyManager;
class QtIntPropertyManager;
class QtBoolPropertyManager;
class QtProperty;

class PropertyBrowser : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    explicit PropertyBrowser(QWidget *parent = 0);

    LevelWidget *levelWidget() const;
    void setLevelWidget(LevelWidget *widget);

    // FIXME: connect to widget's signal directly
    void updatePosition();

signals:

public slots:

protected:
    QtGroupPropertyManager *groupManager_ = nullptr;
    QtStringPropertyManager *stringManager_ = nullptr;
    QtIntPropertyManager *intManager_ = nullptr;
    QtBoolPropertyManager *boolManager_ = nullptr;

    QtProperty *standardGroup_ = nullptr;
    QtProperty *x_ = nullptr;
    QtProperty *y_ = nullptr;

    QtProperty *customGroup_ = nullptr;
    LevelWidget *levelWidget_ = nullptr;
};

#endif // PROPERTYBROWSER_H
