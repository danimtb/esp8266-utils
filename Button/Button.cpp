#include "Button.h"

void shortPressCallback() {}
void longPressCallback() {}
void longlongPressCallback() {}

Button::Button()
{
    m_longlongPressCallback = &longlongPressCallback;
    m_longPressCallback = &longPressCallback;
    m_shortPressCallback = &shortPressCallback;

    m_state = false;
    m_millisSincePressed = 0;
}

void Button::setup(uint8_t pin, ButtonType type)
{
    m_pin = pin;

    if (type == ButtonType::PULLUP)
    {
        pinMode(m_pin, INPUT);
        m_pressedState = LOW;
    }
    else if (type == ButtonType::PULLUP_INTERNAL)
    {
        pinMode(m_pin, INPUT_PULLUP);
        m_pressedState = LOW;
    }
    else if (type == ButtonType::PULLDOWN)
    {
        pinMode(m_pin, INPUT);
        m_pressedState = HIGH;
    }
    else if (type == ButtonType::PULLDOWN_INTERNAL_16)
    {
        pinMode(m_pin, INPUT_PULLDOWN_16);
        m_pressedState = HIGH;
    }
    else
    {
        pinMode(m_pin, INPUT);
        m_pressedState = LOW;
    }
}

bool Button::isPressed()
{
    return digitalRead(m_pin) == m_pressedState;
}

void Button::loop()
{
    if (this->isPressed() && !m_state)
    {
        //Button has been pressed, store time of press
        m_state = true;
        m_millisSincePressed = millis();
    }
    else if (!this->isPressed() && m_state)
    {
        //Button has been released, trigger one of the possible options
        m_state = false;

        if (millis() - m_millisSincePressed > 10000)
        {
            m_longlongPressCallback();
        }
        else if (millis() - m_millisSincePressed > 500)
        {
            m_longPressCallback();
        }
        else if (millis() - m_millisSincePressed > 100)
        {
            m_shortPressCallback();
        }
        else
        {
            //Too short to register as a press
        }

        m_millisSincePressed = 0;
    }
}

void Button::setShortPressCallback(void (*callback)())
{
    m_shortPressCallback = callback;
}

void Button::setLongPressCallback(void (*callback)())
{
    m_longPressCallback = callback;
}

void Button::setLongLongPressCallback(void (*callback)())
{
    m_longlongPressCallback = callback;
}
