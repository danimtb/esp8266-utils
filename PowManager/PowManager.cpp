#include "PowManager.h"

void PowManager::setup()
{
    m_powTimer.setup(RT_ON, 20000);
    m_hlw8012.begin(CF_PIN, CF1_PIN, SEL_PIN, HIGH, false);
    m_hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);
}

void PowManager::loop()
{
    if(m_powTimer.check())
    {
        m_hlw8012.toggleMode();
        m_powTimer.start();
    }
}


double PowManager::getCurrent()
{
    return m_hlw8012.getCurrent();
}

uint16_t PowManager::getVoltage()
{
    return m_hlw8012.getVoltage();
}

uint16_t PowManager::getActivePower()
{
    return m_hlw8012.getActivePower();
}

uint16_t PowManager::getApparentPower()
{
    return m_hlw8012.getApparentPower();
}

double PowManager::getPowerFactor()
{
    return (100 * m_hlw8012.getPowerFactor());
}

uint16_t PowManager::getReactivePower()
{
    return m_hlw8012.getReactivePower();
}
