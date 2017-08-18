#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

enum class ButtonType
{
    PULLUP,
    PULLDOWN,
    PULLUP_INTERNAL,
    PULLDOWN_INTERNAL_16
};

class Button
{
  public:
    Button();
    void setup(uint8_t pin, ButtonType type);
    bool isPressed();
    void setShortPressCallback(void (*callback)());
    void setLongPressCallback(void (*callback)());
    void setVeryLongPressCallback(void (*callback)());
    void setUltraLongPressCallback(void (*callback)());
    void loop();

  private:
    uint8_t m_pin;
    uint8_t m_pressedState;
    uint8_t m_state;
    unsigned long m_millisSincePressed;
    bool m_shortPress;
    bool m_longPress;

    void (*m_shortPressCallback)(void);
    void (*m_longPressCallback)(void);
    void (*m_veryLongPressCallback)(void);
    void (*m_ultraLongPressCallback)(void);
};

#endif
