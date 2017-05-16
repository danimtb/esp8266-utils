#include "PIR.h"

void risingEdge() {}

void fallingEdge() {}

PIR::PIR(uint8_t pin, unsigned long delay)
{
    m_pin = pin;
    pinMode(pin, INPUT);
    m_state = false;

    m_delay = 0;
    m_delay = delay;
    m_timer.setup(RT_ON, delay);

    m_risingEdgeCallback = &risingEdge;
    m_fallingEdgeCallback = &fallingEdge;
}

void PIR::setRisingEdgeCallback(void (*callback)())
{
    m_risingEdgeCallback = callback;
}

void PIR::setFallingEdgeCallback(void (*callback)())
{
    m_fallingEdgeCallback = callback;
}

bool PIR::getState()
{
    return m_state;
}

void PIR::loop()
{
    if (digitalRead(m_pin) == HIGH)
    {
        m_timer.start();
    }

    if (digitalRead(m_pin) == HIGH && !m_state)
    {
        m_state = true;
        m_risingEdgeCallback();
    }
    else if (m_delay == 0 && digitalRead(m_pin) == LOW && m_state)
    {
        m_state = false;
        m_fallingEdgeCallback();
    }
    else if (m_delay != 0 && m_timer.check() && m_state)
    {
        m_state = false;
        m_fallingEdgeCallback();
    }
}
