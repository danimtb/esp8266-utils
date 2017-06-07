#ifndef PIR_H
#define PIR_H

#include "Arduino.h"
#include "SimpleTimer.h"

class TEMT6000
{
private:
    uint8_t m_pin;
    float m_rawValue;
    float m_vccVoltage;
    unsigned long m_sampleTimeMillis;
	SimpleTimer m_timer;

    void (*m_onChangeCallback)(float lux);

public:
    TEMT6000();
    void setup(uint8_t pin, float vccVoltage);
    void setOnChangeCallback(void (*callback)(), unsigned long sampleTimeMillis);

    int16_t getRawValue();
    float getVoltage();
    float getCurrent();
    float getLux();

    void loop();
};

#endif
