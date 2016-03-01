// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QGroupBox>
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

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(grid);
    layout->addStretch(1);
    setLayout(layout);
}

QGroupBox *GridPage::createGridGroupBox()
{
    QSpinBox *cellSizeSpinBox = new QSpinBox;
    cellSizeSpinBox->setValue(settings_->gridSize().width());
    connect(cellSizeSpinBox, SIGNAL(valueChanged(int)),
            settings_, SLOT(setGridSize(int)));

    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Grid Cell Size"), cellSizeSpinBox);

    QGroupBox *groupBox = new QGroupBox(tr("Grid"));
    groupBox->setLayout(layout);

    return groupBox;
}
