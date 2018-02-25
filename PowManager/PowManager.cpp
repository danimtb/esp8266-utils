#include "PowManager.h"

void PowManager::setup()
{
    m_powTimer.setup(RT_ON, 30000);
    m_hlw8012.begin(CF_PIN, CF1_PIN, SEL_PIN, HIGH, false);
    m_hlw8012.setResistors(CURRENT_RESISTOR, VOLTAGE_RESISTOR_UPSTREAM, VOLTAGE_RESISTOR_DOWNSTREAM);
}

void PowManager::loop()
{
    if(m_powTimer.check())
    {
        m_current = m_hlw8012.getCurrent();
        m_voltage = m_hlw8012.getVoltage();
        m_activePower = m_hlw8012.getActivePower();
        m_apparentPower = m_hlw8012.getApparentPower();
        m_powerFactor = 100 * m_hlw8012.getPowerFactor();
        m_reactivePower = m_hlw8012.getReactivePower();

        m_hlw8012.toggleMode();
        m_powTimer.start();
    }
}


double PowManager::getCurrent()
{
    return m_current;
}

uint16_t PowManager::getVoltage()
{
    return m_voltage;
}

uint16_t PowManager::getActivePower()
{
    return m_activePower;
}

uint16_t PowManager::getApparentPower()
{
    return m_apparentPower;
}

double PowManager::getPowerFactor()
{
    return m_powerFactor;
}

uint16_t PowManager::getReactivePower()
{
    return m_reactivePower;
}
