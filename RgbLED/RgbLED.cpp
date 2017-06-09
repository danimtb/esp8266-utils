
#include "RgbLED.h"

RgbLED::RgbLED()
{
    // White color by default
    m_color.red = 255;
    m_color.green = 255;
    m_color.blue = 255;
}

uint16_t RgbLED::from255to1024(uint8_t value)
{
    return (uint16_t)((value / 255) * 1023);
}

uint8_t RgbLED::from1024to255(uint16_t value)
{
    return (uint8_t)((value / 1023) * 255);
}

void RgbLED::writeColor(uint8_t red, uint8_t green, uint8_t blue)
{
    analogWrite(m_redPin, this->from255to1024(red));
    analogWrite(m_greenPin, this->from255to1024(green));
    analogWrite(m_bluePin, this->from255to1024(blue));
}

RgbColor RgbLED::readColor()
{
    RgbColor color;
    color.red = this->from1024to255(analogRead(m_redPin));
    color.green = this->from1024to255(analogRead(m_greenPin));
    color.blue = this->from1024to255(analogRead(m_bluePin));
    return color;
}

void RgbLED::setup(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
    m_redPin = redPin;
    m_greenPin = greenPin;
    m_bluePin = bluePin;
}

void RgbLED::on()
{
    this->writeColor(m_color.red, m_color.green, m_color.blue);
}

void RgbLED::off()
{
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
    if (this->readColor().red == 0 && this->readColor().green == 0 && this->readColor().blue == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
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
