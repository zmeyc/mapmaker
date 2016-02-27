// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <ThirdParty/qtpropertybrowser/src/QtStringPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtIntPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtBoolPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtGroupPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtCheckBoxFactory>
#include <ThirdParty/qtpropertybrowser/src/QtSpinBoxFactory>
#include "PropertyBrowser.h"
#include "Controls/LevelWidget.h"
#include "Data/LevelObject.h"

PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QtTreePropertyBrowser(parent)
{
    groupManager_ = new QtGroupPropertyManager(this);
    stringManager_ = new QtStringPropertyManager(this);
    intManager_ = new QtIntPropertyManager(this);
    boolManager_ = new QtBoolPropertyManager(this);

    QtCheckBoxFactory *checkBoxFactory = new QtCheckBoxFactory(this);
    QtSpinBoxFactory *spinBoxFactory = new QtSpinBoxFactory(this);

    setFactoryForManager(boolManager_, checkBoxFactory);
    setFactoryForManager(intManager_, spinBoxFactory);

    // Standard properties
    standardGroup_ = groupManager_->addProperty("Standard");

    QtProperty *name = stringManager_->addProperty("name");
    name->setToolTip("Object name");
    name->setEnabled(false);
    standardGroup_->addSubProperty(name);

    x_ = intManager_->addProperty("x");
    x_->setToolTip("X coordinate");
    standardGroup_->addSubProperty(x_);

    y_ = intManager_->addProperty("y");
    y_->setToolTip("Y coordinate");
    standardGroup_->addSubProperty(y_);

    QtProperty *flipX = boolManager_->addProperty("flipX");
    flipX->setToolTip("Flips the object horizontally");
    standardGroup_->addSubProperty(flipX);

    QtProperty *flipY = boolManager_->addProperty("flipY");
    flipY->setToolTip("Flips the object vertically");
    standardGroup_->addSubProperty(flipY);

    // Custom properties
    customGroup_ = groupManager_->addProperty("Custom");

    // Final setup
    //addProperty(standardGroup);
    //addProperty(customGroup);
}

void PropertyBrowser::setLevelObject(LevelObject *object)
{
    if (levelObject_)
        disconnectSignals(levelObject_);

    levelObject_ = object;

    if (levelObject_) {
        addProperty(standardGroup_);
        addProperty(customGroup_);
        updatePosition(levelObject_->position());
        connectSignals(levelObject_);
    } else {
        clear();
    }
}

void PropertyBrowser::resetLevelObject()
{
    setLevelObject(nullptr);
}

void PropertyBrowser::updatePosition(const QPointF &pos)
{
    intManager_->setValue(x_, pos.x());
    intManager_->setValue(y_, pos.y());
}

void PropertyBrowser::connectSignals(LevelObject *object)
{
    connect(object, SIGNAL(positionChanged(QPointF)),
            this, SLOT(updatePosition(QPointF)));
    connect(object, SIGNAL(destroyed(QObject*)),
            this, SLOT(resetLevelObject()));
}

void PropertyBrowser::disconnectSignals(LevelObject *object)
{
    disconnect(object, nullptr,
               this, nullptr);
}
