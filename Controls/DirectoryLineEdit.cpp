// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include "DirectoryLineEdit.h"

DirectoryLineEdit::DirectoryLineEdit(QWidget *parent)
    : QWidget(parent)
{
    lineEdit_ = new QLineEdit;
    lineEdit_->setMinimumWidth(300);
    connect(lineEdit_, SIGNAL(textChanged(QString)),
            this, SIGNAL(textChanged(QString)));
    connect(lineEdit_, SIGNAL(editingFinished()),
            this, SIGNAL(editingFinished()));

    QPushButton *browseButton = new QPushButton(tr("Browse"));
    connect(browseButton, SIGNAL(clicked(bool)),
            this, SLOT(onBrowseClicked()));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(lineEdit_, /* stretch */ 1);
    layout->addWidget(browseButton);

    setLayout(layout);
}

QString DirectoryLineEdit::text() const
{
    return lineEdit_->text();
}

void DirectoryLineEdit::setText(const QString &text)
{
    lineEdit_->setText(text);
}

void DirectoryLineEdit::onBrowseClicked()
{
    QFileDialog dialog;
    dialog.setDirectory(lineEdit_->text());
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    //dialog.setOption(QFileDialog::DontUseNativeDialog);
    if (dialog.exec() == QDialog::Accepted) {
        QString path = dialog.directory().absolutePath();
        lineEdit_->setText(QDir::toNativeSeparators(path));
        emit editingFinished();
    }
}
