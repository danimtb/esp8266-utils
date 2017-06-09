
#include "RgbLED.h"

RgbLED::RgbLED()
{
    // White color by default
    m_color.red = 255;
    m_color.green = 255;
    m_color.blue = 255;

    m_state = false;
}

void RgbLED::writeColor(uint8_t red, uint8_t green, uint8_t blue)
{
    analogWrite(m_redPin, red);
    analogWrite(m_greenPin, green);
    analogWrite(m_bluePin, blue);
}

RgbColor RgbLED::readColor()
{
    RgbColor color;
    color.red = analogRead(m_redPin);
    color.green = analogRead(m_greenPin);
    color.blue = analogRead(m_bluePin);
    return color;
}

void RgbLED::setup(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
    m_redPin = redPin;
    m_greenPin = greenPin;
    m_bluePin = bluePin;

    analogWriteRange(255);

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}

void RgbLED::on()
{
    m_state = true;
    this->writeColor(m_color.red, m_color.green, m_color.blue);
}

void RgbLED::off()
{
    m_state = false;
    this->writeColor(0, 0, 0);
}

void RgbLED::setColor(uint8_t red, uint8_t green, uint8_t blue)
{
    if(red == 0 && green == 0 && blue == 0)
    {
        this->off();
    }
    else
    {
        m_color.red = red;
        m_color.green = green;
        m_color.blue = blue;

        this->on();
    }
}

RgbColor RgbLED::getColor()
{
    return m_color;
}

bool RgbLED::getState()
{
    return m_state;
}

void RgbLED::commute()
{
    if (this->getState())
	{
        this->off();
	}
	else
	{
        this->on();
	}
}
