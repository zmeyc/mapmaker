// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>

class QPoint;
class QPointF;
class QSize;
class QSizeF;
class QRect;
class QRectF;

#define qout LogLocker(stdoutStream, &logMutex).stream()
#define qerr LogLocker(stderrStream, &logMutex).stream()
#define qvrb qout
#define qdbg qerr

extern QTextStream stdoutStream;
extern QTextStream stderrStream;
extern QMutex logMutex;

class LogLocker
{
public:
        LogLocker(QTextStream &stream, QMutex *mutex)
            : locker_(mutex)
            , stream_(stream)
        {
        }

        inline QTextStream &stream() const { return stream_; }

protected:
    QMutexLocker<QMutex> locker_;
    QTextStream &stream_;
};

QTextStream &operator<<(QTextStream &s, const QPoint &p);
QTextStream &operator<<(QTextStream &s, const QPointF &p);
QTextStream &operator<<(QTextStream &s, const QSize &sz);
QTextStream &operator<<(QTextStream &s, const QSizeF &sz);
QTextStream &operator<<(QTextStream &s, const QRect &r);
QTextStream &operator<<(QTextStream &s, const QRectF &r);

QString capitalize(const QString &s);

template <typename T>
QString ptrToString(T *ptr) {
    return QString("0x%1").arg((quintptr)ptr,
                               QT_POINTER_SIZE * 2, 16, QChar('0'));
}

#endif // UTILS_H
