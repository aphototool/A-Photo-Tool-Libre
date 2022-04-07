#ifndef BACKGROUNDCONTROL_H
#define BACKGROUNDCONTROL_H

#include "../values/imagevalues.h"
#include "../values/workvalues.h"

class BackgroundControl
{
public:
    BackgroundControl();
    static void haltBackgroundWork(Values *values, WorkValues *workValues);
    static void resumeBackgroundWork(Values *values, WorkValues *workValues);

    static const long backgroundWorkStopped = std::numeric_limits<long>::max();
};

#endif // BACKGROUNDCONTROL_H
