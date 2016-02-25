// MapMaker (c) 2016 Andrey Fidrya. MIT license. See LICENSE for more information.

#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>

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
    QMutexLocker locker_;
    QTextStream &stream_;
};

#endif // UTILS_H
