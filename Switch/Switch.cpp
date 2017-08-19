#include "Switch.h"

void singleToggleCallback() {}
void doubleToggleCallback() {}
void tripleToggleCallback() {}
void quadrupleToggleCallback() {}

Switch::Switch()
{
    m_ultraLongPressCallback = &ultraLongPressCallback;
    m_veryLongPressCallback = &veryLongPressCallback;
    m_longPressCallback = &longPressCallback;
    m_shortPressCallback = &shortPressCallback;

    m_millisSincePressed = 0;

    m_debounceTimer.setup(RT_ON, 100);
    m_overTimer.setup(RT_ON, 400);
}

void Switch::setup(uint8_t pin, ButtonType type)
{
    m_pin = pin;

    if (type == ButtonType::PULLUP)
    {
        pinMode(m_pin, INPUT);
        m_state = LOW;
    }
    else if (type == ButtonType::PULLUP_INTERNAL)
    {
        pinMode(m_pin, INPUT_PULLUP);
        m_state = LOW;
    }
    else if (type == ButtonType::PULLDOWN)
    {
        pinMode(m_pin, INPUT);
        m_state = HIGH;
    }
    else if (type == ButtonType::PULLDOWN_INTERNAL_16)
    {
        pinMode(m_pin, INPUT_PULLDOWN_16);
        m_state = HIGH;
    }
    else
    {
        pinMode(m_pin, INPUT);
        m_state = LOW;
    }
}

bool Switch::getState()
{
    return digitalRead(m_pin) == HIGH;
}

bool Switch::toggled()
{
    bool toggled;

    if (digitalRead(m_pin) != m_state)
    {
        toggled = true;
    }
    else
    {
        toggled = false;
    }

    m_state = digitalRead(m_pin);

    return toggled;
}

void Switch::loop()
{
    if (toggled())
    {
        m_debounceTimer.start();
    }
    else
    {
        if (m_debounceTimer.check())
        {
            if (!m_singleToggle)
            {
                m_singleToggle = true;
                m_overTimer.start();
            }
            else if (m_singleToggle && !m_doubleToggle)
            {
                m_doubleToggle = true;
                m_overTimer.start();
            }
            else if (m_singleToggle && m_doubleToggle && !m_tripleToggle)
            {
                m_tripleToggle = true;
                m_overTimer.start();
            }
            else if (m_singleToggle && m_doubleToggle && m_tripleToggle && !m_quadrupleToggle)
            {
                m_quadrupleToggle = true;
                m_overTimer.start();
            }
            else
            {
                // Unknown number of toggles
            }
        }

        if (m_overTimer.check())
        {
            if (m_singleToggle && m_doubleToggle && m_tripleToggle && m_quadrupleToggle)
            {
                m_quadrupleToggleCallback();
            }
            else if (m_singleToggle && m_doubleToggle && m_tripleToggle && !m_quadrupleToggle)
            {
                m_tripleToggleCallback();
            }
            else if (m_singleToggle && m_doubleToggle && !m_tripleToggle && !m_quadrupleToggle)
            {
                m_doubleToggleCallback();
            }
            else if (m_singleToggle && !m_doubleToggle && !m_tripleToggle && !m_quadrupleToggle)
            {
                m_singleToggleCallback();
            }
            else
            {
                // Unknown number of toggles
            }

            m_singleToggle = false;
            m_doubleToggle = false;
            m_tripleToggle = false;
            m_quadrupleToggle = false;
        }
    }
}

void Switch::setSingleToggleCallback(void (*callback)())
{
    m_singleToggleCallback = callback;
}

void Switch::setDoubleToggleCallback(void (*callback)())
{
    m_doubleToggleCallback = callback;
}

void Switch::setTripleToggleCallback(void (*callback)())
{
    m_tripleToggleCallback = callback;
}

void Switch::setQuadrupleToggleCallback(void (*callback)())
{
    m_quadrupleToggleCallback = callback;
}
