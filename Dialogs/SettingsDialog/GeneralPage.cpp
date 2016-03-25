#include "GeneralPage.h"

#include <QGroupBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include "GridPage.h"
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


QGroupBox *GeneralPage::createGeneralGroupBox()
{
    QLineEdit *imagesDirectoryEdit = new QLineEdit;
    imagesDirectoryEdit->setMinimumWidth(300);
    imagesDirectoryEdit->setText(settings_->imagesDirectory());
    connect(imagesDirectoryEdit, SIGNAL(textChanged(QString)),
            settings_, SLOT(setImagesDirectory(QString)));

    QFormLayout *layout = new QFormLayout;
    layout->addRow(tr("Images Directory"), imagesDirectoryEdit);

    QGroupBox *groupBox = new QGroupBox(tr("General Settings"));
    groupBox->setLayout(layout);

    return groupBox;
}
