// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QGroupBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include "GridPage.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Settings.h"

GridPage::GridPage(QWidget *parent)
    : QWidget(parent)
    , settings_(Settings::sharedInstance())
{
    //WidgetUtils::setBackgroundColor(this, Qt::blue);

    QGroupBox *grid = createGridGroupBox();
    QGroupBox *dockPoints = createDockPointsGroupBox();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(grid);
    layout->addWidget(dockPoints);
    layout->addStretch(1);
    setLayout(layout);
}

QGroupBox *GridPage::createGridGroupBox()
{
    QCheckBox *autoSizeGridCheckBox = new QCheckBox;
    QSpinBox *cellSizeSpinBox = new QSpinBox;
    cellSizeSpinBox->setMaximumWidth(50);

    autoSizeGridCheckBox->setChecked(settings_->autoSizeGrid());
    connect(autoSizeGridCheckBox, SIGNAL(clicked(bool)),
            settings_, SLOT(setAutoSizeGrid(bool)));
    connect(autoSizeGridCheckBox, SIGNAL(clicked(bool)),
            cellSizeSpinBox, SLOT(setDisabled(bool)));

    cellSizeSpinBox->setDisabled(settings_->autoSizeGrid());
    cellSizeSpinBox->setValue(settings_->gridSize().width());
    connect(cellSizeSpinBox, SIGNAL(valueChanged(int)),
            settings_, SLOT(setGridSize(int)));

    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Match selected object's size"), autoSizeGridCheckBox);
    layout->addRow(tr("Grid Cell Size"), cellSizeSpinBox);

    QGroupBox *groupBox = new QGroupBox(tr("Grid"));
    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox *GridPage::createDockPointsGroupBox()
{
    QSpinBox *dockingDistanceSpinBox = new QSpinBox;
    dockingDistanceSpinBox->setMaximumWidth(50);
    dockingDistanceSpinBox->setValue(settings_->dockingDistance());
    connect(dockingDistanceSpinBox, SIGNAL(valueChanged(int)),
            settings_, SLOT(setDockingDistance(int)));

    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Docking Distance"), dockingDistanceSpinBox);

    QGroupBox *groupBox = new QGroupBox(tr("Dock Points"));
    groupBox->setLayout(layout);

    return groupBox;
}
