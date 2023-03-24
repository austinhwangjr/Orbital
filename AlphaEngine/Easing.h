#pragma once

#include <stdlib.h>
#include <math.h>
#define NATURAL_LOG_OF_2    0.693147181f



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

static float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

// Linear easing
static float easeLinear(f32 t)
{
	return t;
}

// Quadratic easing in
static float easeInQuad(f32 t)
{
	return t * t;
}

// Quadratic easing out
static float easeOutQuad(f32 t)
{
	return -(t * (t - 2));
}

static float easeInOutQuad(f32 t)
{
	return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}