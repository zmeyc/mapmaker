#ifndef NEWPROPERTYDIALOG_H
#define NEWPROPERTYDIALOG_H

#include <QDialog>

class QLineEdit;

class NewPropertyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewPropertyDialog(QWidget *parent = 0);

    QString key() const;
    QString value() const;

signals:

public slots:

protected:
    QLineEdit *keyEdit_ = nullptr;
    QLineEdit *valueEdit_ = nullptr;
};

#endif // NEWPROPERTYDIALOG_H
