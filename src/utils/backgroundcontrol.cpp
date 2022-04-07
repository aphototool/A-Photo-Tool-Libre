#include "backgroundcontrol.h"

BackgroundControl::BackgroundControl()
{

}

void BackgroundControl::haltBackgroundWork(Values *values, WorkValues *workValues) {
    workValues->filterMutex.lock();
    values->filteredTime = backgroundWorkStopped;
    workValues->filterMutex.unlock();
}

void BackgroundControl::resumeBackgroundWork(Values *values, WorkValues *workValues) {
    workValues->filterMutex.lock();
    values->filteredTime = 0;
    workValues->filterMutex.unlock();
}
