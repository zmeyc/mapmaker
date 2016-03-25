#ifndef DIRECTORYLINEEDIT_H
#define DIRECTORYLINEEDIT_H

#include <QWidget>

class QLineEdit;

class DirectoryLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryLineEdit(QWidget *parent = 0);

    QString text() const;
    Q_SLOT void setText(const QString &text);

signals:
    void textChanged(const QString &);
    void editingFinished();

public slots:

protected slots:
    void onBrowseClicked();

protected:
    QLineEdit *lineEdit_ = nullptr;

};

#endif // DIRECTORYLINEEDIT_H
