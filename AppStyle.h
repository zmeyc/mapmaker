#ifndef APPSTYLE_H
#define APPSTYLE_H

#include <QObject>
#include <QPen>

class AppStyle : public QObject
{
    Q_OBJECT
public:
    explicit AppStyle(QObject *parent = 0);

    static AppStyle *sharedInstance();

    QPen selectedItemPen(int devicePixelRatio) const;
    QPen activeItemPen(int devicePixelRatio) const;

signals:

public slots:

protected:
    qreal selectedItemPenWidthPt_ = 1;
    mutable QPen selectedItemPen_;

    qreal activeItemPenWidthPt_ = 1;
    mutable QPen activeItemPen_;
};

#endif // APPSTYLE_H
