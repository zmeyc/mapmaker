// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include "QVBoxLayout"
#include "QPushButton"
#include "SettingsDialog.h"
#include "GridPage.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Properties"));

    categories_ = new QListWidget;
    categories_->setMaximumWidth(120);
    categories_->setSpacing(5);
    categories_->setFlow(QListView::TopToBottom);
    categories_->setMovement(QListView::Static);
    categories_->setIconSize(QSize(30, 30));
    connect(categories_, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(onCurrentItemChanged(QListWidgetItem*,QListWidgetItem*)));

    pages_ = new QStackedWidget;
    pages_->addWidget(new GridPage);

    categories_->addItem(createCategoryItem(QIcon(), tr("Grid")));
    categories_->setCurrentRow(0);

    QPushButton *closeButton = new QPushButton(tr("Close"));
    closeButton->setFocusPolicy(Qt::StrongFocus);
    closeButton->setDefault(true);
    connect(closeButton, SIGNAL(clicked(bool)),
            this, SLOT(accept()));

    QHBoxLayout *pagesLayout = new QHBoxLayout;
    pagesLayout->setContentsMargins(0, 0, 0, 0);
    pagesLayout->addWidget(categories_);
    pagesLayout->addWidget(pages_, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(pagesLayout);
    mainLayout->addWidget(closeButton);
    setLayout(mainLayout);
}

void SettingsDialog::onCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pages_->setCurrentIndex(categories_->row(current));
}

QListWidgetItem *SettingsDialog::createCategoryItem(const QIcon &icon, const QString &text)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QIcon(icon));
    item->setText(text);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return item;
}
