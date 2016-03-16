#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLineEdit>

#include "NewPropertyDialog.h"

NewPropertyDialog::NewPropertyDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("New Property"));

    keyEdit_ = new QLineEdit;
    valueEdit_ = new QLineEdit;

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->addRow(tr("Name"), keyEdit_);
    formLayout->addRow(tr("Value"), valueEdit_);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                Qt::Horizontal);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);
}

QString NewPropertyDialog::key() const
{
    return keyEdit_->text();
}

QString NewPropertyDialog::value() const
{
    return valueEdit_->text();
}
