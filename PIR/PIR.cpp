#include "PIR.h"

PIR::PIR(unsigned int _pin, unsigned long secs)
{
	pin=_pin;
	pinMode(pin, INPUT);
	state=false;
	timer.setup(PULSE);
	seconds=secs;
}

bool PIR::rising_edge()
{
	if(digitalRead(pin)==LOW && state)
	{
		state=false;
	}
	
	if(digitalRead(pin)==HIGH && !state)
	{
		state=true;
		timer.load(seconds);
		return true;
	}
	else
	{
		return false;
	}
}

bool PIR::falling_edge()
{
	if(digitalRead(pin)==HIGH && !state)
	{
		state=true;
	}
	
	if(digitalRead(pin)==LOW && state)
	{
		state=false;
		return true;
	}
	else
	{
		return false;
	}
}

bool PIR::getState()
{
	return state;
}

bool PIR::timeout()
{
	if(timer.check())
	{
		state=false;
		return true;
	}
	else
		return false;
}
