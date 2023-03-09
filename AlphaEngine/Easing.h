#pragma once

#include <stdlib.h>
#include <math.h>
#define NATURAL_LOG_OF_2    0.693147181f
#define PI                3.14159265358979323846f


static float EaseOutExpo(float start, float end, float value)
{
	end -= start;
	return end * (-powf(2, -10 * value) + 1) + start;
}

static float EaseInOutBack(float start, float end, float value)
{
	float s = 1.70158f;
	end -= start;
	value /= .5f;
	if ((value) < 1)
	{
		s *= (1.525f);
		return end * 0.5f * (value * value * (((s)+1) * value - s)) + start;
	}
	value -= 2;
	s *= (1.525f);
	return end * 0.5f * ((value)*value * (((s)+1) * value + s) + 2) + start;
}