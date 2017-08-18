#ifndef TIMEWATCHDOG_H
#define TIMEWATCHDOGH

#include "Arduino.h"

#include "SimpleTimer.h"

class TimeWatchdog
{
private:
	bool m_init;
	SimpleTimer m_timer;
    void (*m_barkCallback)(void);

public:
    TimeWatchdog();
    void setup(unsigned long time, void (*callback)());
    void init();
    void deinit();
    void feed();
    void loop();
};

#endif
