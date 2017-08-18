#include "TimeWatchdog.h"

void barkCallback() {}

TimeWatchdog::TimeWatchdog()
{
    m_init = false;
    m_barkCallback = &barkCallback;
    m_timer.setup(RT_ON);
}

void TimeWatchdog::setup(unsigned long time, void (*callback)())
{
    m_timer.load(time);
    m_barkCallback = callback;
}

void TimeWatchdog::init()
{
    m_init = true;
    m_timer.start();
}

void TimeWatchdog::deinit()
{
    m_init = false;
}

void TimeWatchdog::feed()
{
    if (m_init)
    {
        m_timer.start();
    }
}

void TimeWatchdog::loop()
{
    if (m_init && m_timer.check())
    {
        m_barkCallback();
    }
}
