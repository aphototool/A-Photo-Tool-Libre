#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <chrono>
// #include <sys/time.h>
// #include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
// using std::chrono::seconds;
using std::chrono::system_clock;

class TimeUtil
{
public:
    TimeUtil();
    static long getTimestamp();
};

#endif // TIMEUTIL_H
