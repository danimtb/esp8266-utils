#ifndef SWITCH_H
#define SWITCH_H

#include "Arduino.h"

#include "SimpleTimer.h"

enum class SwitchType
{
    PULLUP,
    PULLDOWN,
    PULLUP_INTERNAL,
    PULLDOWN_INTERNAL_16
};

class Switch
{
  public:
    Switch();
    void setup(uint8_t pin, ButtonType type);
    bool getState();
    void setSingleToggleCallback(void (*callback)());
    void setDoubleToggleCallback(void (*callback)());
    void setTripleToggleCallback(void (*callback)());
    void setQuadrupleToggleCallback(void (*callback)());
    void loop();

  private:
    uint8_t m_pin;
    uint8_t m_state;
    bool m_singleToggle;
    bool m_doubleToggle;
    bool m_tripleToggle;
    bool m_quadrupleToggle;
    SimpleTimer m_debounceTimer;
    SimpleTimer m_overTimer;




    bool toggled();

    void (*m_singleToggleCallback)(void);
    void (*m_doubleToggleCallback)(void);
    void (*m_tripleToggleCallback)(void);
    void (*m_quadrupleToggleCallback)(void);
};

#endif
