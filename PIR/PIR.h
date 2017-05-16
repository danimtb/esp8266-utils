#ifndef PIR_H
#define PIR_H

#include "Arduino.h"
#include "SimpleTimer.h"

class PIR
{
private:
    uint8_t m_pin;
	bool m_state;
	unsigned long m_delay;
	SimpleTimer m_timer;

	void (*m_risingEdgeCallback)(void);
    void (*m_fallingEdgeCallback)(void);

public:
    PIR(uint8_t pin, unsigned long delay);

    void setRisingEdgeCallback(void (*callback)());
    void setFallingEdgeCallback(void (*callback)());

    bool getState();

    void loop();
};

#endif
