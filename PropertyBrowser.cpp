// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <ThirdParty/qtpropertybrowser/src/QtStringPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtIntPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtBoolPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtGroupPropertyManager>
#include <ThirdParty/qtpropertybrowser/src/QtCheckBoxFactory>
#include <ThirdParty/qtpropertybrowser/src/QtSpinBoxFactory>
#include "PropertyBrowser.h"
#include "Data/LevelObject.h"

PropertyBrowser::PropertyBrowser(QWidget *parent)
    : QtTreePropertyBrowser(parent)
{
    groupManager_ = new QtGroupPropertyManager(this);
    stringManager_ = new QtStringPropertyManager(this);
    intManager_ = new QtIntPropertyManager(this);
    connect(intManager_, SIGNAL(valueChanged(QtProperty*,int)),
            this, SLOT(onIntValueChanged(QtProperty*,int)));
    boolManager_ = new QtBoolPropertyManager(this);
    connect(boolManager_, SIGNAL(valueChanged(QtProperty*,bool)),
            this, SLOT(onBoolValueChanged(QtProperty*,bool)));

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

    flipX_ = boolManager_->addProperty("flipX");
    flipX_->setToolTip("Flips the object horizontally");
    standardGroup_->addSubProperty(flipX_);

    flipY_ = boolManager_->addProperty("flipY");
    flipY_->setToolTip("Flips the object vertically");
    standardGroup_->addSubProperty(flipY_);

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

void PropertyBrowser::onIntValueChanged(QtProperty *property, int val)
{
    if (property->propertyName() == "x")
        levelObject_->setX(val);
    else if (property->propertyName() == "y")
        levelObject_->setY(val);
}

void PropertyBrowser::onBoolValueChanged(QtProperty *property, bool val)
{
    // TODO: evaluate possibility of using Q_PROPERTIES for automating this
    if (property->propertyName() == "flipX")
        levelObject_->setFlipX(val);
    else if (property->propertyName() == "flipY")
        levelObject_->setFlipY(val);
}

void PropertyBrowser::updatePosition(const QPointF &pos)
{
    intManager_->setValue(x_, pos.x());
    intManager_->setValue(y_, pos.y());
}

void PropertyBrowser::updateFlipX(bool flipX)
{
    boolManager_->setValue(flipX_, flipX);
}

void PropertyBrowser::updateFlipY(bool flipY)
{
    boolManager_->setValue(flipY_, flipY);
}

void PropertyBrowser::connectSignals(LevelObject *object)
{
    connect(object, SIGNAL(positionChanged(QPointF)),
            this, SLOT(updatePosition(QPointF)));
    connect(object, SIGNAL(flipXChanged(bool)),
            this, SLOT(updateFlipX(bool)));
    connect(object, SIGNAL(flipYChanged(bool)),
            this, SLOT(updateFlipY(bool)));
    connect(object, SIGNAL(destroyed(QObject*)),
            this, SLOT(resetLevelObject()));
}

void PropertyBrowser::disconnectSignals(LevelObject *object)
{
    disconnect(object, nullptr,
               this, nullptr);
}
