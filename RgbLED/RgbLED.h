#ifndef RGBLED_H
#define RGBLED_H

#include "Arduino.h"

struct RgbColor
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class RgbLED
{
private:
    uint8_t m_redPin;
    uint8_t m_greenPin;
    uint8_t m_bluePin;
    RgbColor m_color;
    bool m_state;

    void writeColor(uint8_t red, uint8_t green, uint8_t blue);
    RgbColor readColor();

public:
    RgbLED();
    void setup(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
	void on();
	void off();
    void setColor(uint8_t red, uint8_t green, uint8_t blue);
    RgbColor getColor();
	bool getState();
	void commute();
};

#endif
