#include "timeutil.h"

TimeUtil::TimeUtil()
{

}

long TimeUtil::getTimestamp() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
