#include "GeneralPage.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include "GridPage.h"
#include "Controls/DirectoryLineEdit.h"
#include "Utils/WidgetUtils.h"
#include "Utils/Settings.h"

GeneralPage::GeneralPage(QWidget *parent)
    : QWidget(parent)
    , settings_(Settings::sharedInstance())
{
    QGroupBox *general = createGeneralGroupBox();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(general);
    layout->addStretch(1);
    setLayout(layout);
}

void GeneralPage::onImagesDirectoryEditingFinished()
{
    settings_->setImagesDirectory(imagesDirectoryEdit_->text());
}

QGroupBox *GeneralPage::createGeneralGroupBox()
{
    imagesDirectoryEdit_ = new DirectoryLineEdit;
    imagesDirectoryEdit_->setText(settings_->imagesDirectory());
    connect(imagesDirectoryEdit_, SIGNAL(editingFinished()),
            this, SLOT(onImagesDirectoryEditingFinished()));

    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Images Directory"), imagesDirectoryEdit_);

    QGroupBox *groupBox = new QGroupBox(tr("General Settings"));
    groupBox->setLayout(layout);

    return groupBox;
}
