#include "ToggleSwitch.h"

void singleToggleCallback() {}
void doubleToggleCallback() {}
void tripleToggleCallback() {}
void quadrupleToggleCallback() {}

ToggleSwitch::ToggleSwitch()
{
    m_singleToggleCallback = &singleToggleCallback;
    m_doubleToggleCallback = &doubleToggleCallback;
    m_tripleToggleCallback = &tripleToggleCallback;
    m_quadrupleToggleCallback = &quadrupleToggleCallback;

    m_toggle = false;
    m_singleToggle = false;
    m_doubleToggle = false;
    m_tripleToggle = false;
    m_quadrupleToggle = false;

    m_debounceTimer.setup(RT_ON, 150);
    m_debounceTimer.start();
    m_overTimer.setup(PULSE, 100);
}

void ToggleSwitch::setup(uint8_t pin, ToggleSwitchType type)
{
    m_pin = pin;

    if (type == ToggleSwitchType::PULLUP)
    {
        pinMode(m_pin, INPUT);
    }
    else if (type == ToggleSwitchType::PULLUP_INTERNAL)
    {
        pinMode(m_pin, INPUT_PULLUP);
    }
    else if (type == ToggleSwitchType::PULLDOWN)
    {
        pinMode(m_pin, INPUT);
    }
    else if (type == ToggleSwitchType::PULLDOWN_INTERNAL_16)
    {
        pinMode(m_pin, INPUT_PULLDOWN_16);
    }
    else
    {
        pinMode(m_pin, INPUT);
    }

    m_state = digitalRead(m_pin);
}

bool ToggleSwitch::getState()
{
    return digitalRead(m_pin) == HIGH;
}

bool ToggleSwitch::toggled()
{
    bool toggled;
    int digRead = digitalRead(m_pin);

    if (digRead != m_toggledState)
    {
        toggled = true;
    }
    else
    {
        toggled = false;
    }

    m_toggledState = digRead;

    return toggled;
}

void ToggleSwitch::loop()
{
    if (toggled())
    {
        if (m_debounceTimer.check())
        {
            m_tempToggleState = m_toggledState;
            m_debounceTimer.start();
        }
    }

    if (m_debounceTimer.check() && m_tempToggleState != m_state)
    {
        if (m_tempToggleState == m_toggledState)
        {
            m_state = m_toggledState;
            m_toggle = true;
        }
    }
    else
    {
        m_toggle = false;
    }

    if (m_toggle)
    {
        m_toggle = false;

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

void ToggleSwitch::setSingleToggleCallback(void (*callback)())
{
    m_singleToggleCallback = callback;
}

void ToggleSwitch::setDoubleToggleCallback(void (*callback)())
{
    m_doubleToggleCallback = callback;
}

void ToggleSwitch::setTripleToggleCallback(void (*callback)())
{
    m_tripleToggleCallback = callback;
}

void ToggleSwitch::setQuadrupleToggleCallback(void (*callback)())
{
    m_quadrupleToggleCallback = callback;
}
