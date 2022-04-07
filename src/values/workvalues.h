#ifndef WORKVALUES_H
#define WORKVALUES_H

#include <QImage>
#include <QFutureWatcher>
#include <QMutex>

class WorkValues
{
public:
    WorkValues();

    QFutureWatcher<QImage> filterWatcher;
    QMutex filterMutex;
    long lastFullResTimestamp = 0;
    bool backgroundWorking = false;
};

#endif // WORKVALUES_H
