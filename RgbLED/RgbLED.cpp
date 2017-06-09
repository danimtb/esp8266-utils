
#include "RgbLED.h"

RgbLED::RgbLED()
{
    // White color by default
    m_color.red = 255;
    m_color.green = 255;
    m_color.blue = 255;

    m_state = false;

    m_transitionTime = 200;
    m_transitionTimer.setup(RT_ON, m_transitionTime);
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

void RgbLED::setColor(uint8_t red, uint8_t green, uint8_t blue, uint16_t seconds)
{
    // Delete transitions if any
    m_transitions.clear();

    // Transition every 200 ms
    uint16_t numberTransitions = (seconds * 1000) / m_transitionTime;

    RgbColor temporalColor = m_color;

    int16_t redIncrement = (m_color.red - red) / numberTransitions;
    int16_t greenIncrement = (m_color.green - green) / numberTransitions;
    int16_t blueIncrement = (m_color.blue - blue) / numberTransitions;

    // Save temporal color for each increment minus last one
    for (uint16_t i = 0; i < (numberTransitions - 1); i++)
    {
        temporalColor.red = temporalColor.red - redIncrement;
        temporalColor.green = temporalColor.green - greenIncrement;
        temporalColor.blue = temporalColor.blue - blueIncrement;

        m_transitions.push_back(temporalColor);
    }

    // Add last temporal color to match final color
    temporalColor.red = red;
    temporalColor.green = green;
    temporalColor.blue = blue;

    m_transitions.push_back(temporalColor);

    // Set final color state here to avoid getting temporal color states as final
    m_color.red = red;
    m_color.green = green;
    m_color.blue = blue;

    // Set off state if final color is black
    if(red == 0 && green == 0 && blue == 0)
    {
        m_state = false;
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

void RgbLED::loop()
{
    if (m_transitionTimer.check())
    {
        if (!m_transitions.empty())
        {
            this->setColor(m_transitions.front().red, m_transitions.front().green, m_transitions.front().blue);
            m_transitions.erase(m_transitions.begin());
        }

        m_transitionTimer.start();
    }
}
