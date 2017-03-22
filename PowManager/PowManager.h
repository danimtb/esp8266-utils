#ifndef POWMANAGER_H
#define POWMANAGER_H

#include <Arduino.h>

#include <cstdint>
#include <string>

#include "HLW8012.h"
#include "SimpleTimer.h"

#define SEL_PIN 5
#define CF1_PIN 13
#define CF_PIN 14

#define CURRENT_RESISTOR 0.001
#define VOLTAGE_RESISTOR_UPSTREAM ( 5 * 470000 ) // Real: 2280k
#define VOLTAGE_RESISTOR_DOWNSTREAM ( 1000 ) // Real 1.009k

class PowManager
{
private:
    HLW8012 m_hlw8012;
    SimpleTimer m_powTimer;

public:

    double getCurrent();
    uint16_t getVoltage();
    uint16_t getActivePower();
    uint16_t getApparentPower();
    double getPowerFactor();
    uint16_t getReactivePower();

    void setup();

    void loop();
};

#endif
