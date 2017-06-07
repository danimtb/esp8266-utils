#include "TEMT6000.h"

void onChange(float lux) {}

TEMT6000::TEMT6000()
{
    m_rawValue = 0;
    m_sampleTimeMillis; = 0;
    m_onChangeCallback = &onChange;
}

TEMT6000::setup(uint8_t pin, float vccVoltage)
{
    m_pin = pin;
    m_vccVoltage = vccVoltage;
    m_rawValue = this->getRawValue();
}

void TEMT6000::setOnChangeCallback(void (*callback)(),  unsigned long sampleTimeMillis)
{
    m_sampleTimeMillis = sampleTimeMillis;
    m_timer.setup(RT_ON, m_sampleTimeMillis);
    m_fallingEdgeCallback = callback;
}

int TEMT6000::getRawValue()
{
    return analogRead(m_pin);
}

float TEMT6000::getVoltage()
{
    return this->getRawValue() * m_vccVoltage / 1024.0;
}

float TEMT6000::getCurrent()
{
    return this->getVoltage() / 10000.0; // 10.000 Ohms resistor
}

float TEMT6000::getLux()
{
    // Convert current to microamps and apply formula
    // Formula: Lux = 2.0 * Microamps
    return this->getCurrent() * 1000000 * 2.0;
}

void TEMT6000::loop()
{
    if (m_timer.check())
    {
        if (m_rawValue != this->getRawValue())
        {
            m_rawValue = this->getRawValue();
            m_onChangeCallback(this->getLux());
        }
    }
}
