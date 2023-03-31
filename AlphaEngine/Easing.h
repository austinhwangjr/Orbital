/******************************************************************************/
/*!
\file		Easing.h
\author 	Hwang Jing Rui, Austin
\co-author	Ong You Yang
\par    	email: jingruiaustin.hwang\@digipen.edu
				   youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for Easing 
			transitions.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once

#include <stdlib.h>
#include <math.h>
#define NATURAL_LOG_OF_2    0.693147181f

namespace {

float EaseOutExpo(float start, float end, float value)
{
	end -= start;
	return end * (-powf(2, -10 * value) + 1) + start;
}

float EaseInOutBack(float start, float end, float value)
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

float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

// Linear easing
float easeLinear(f32 t)
{
	return t;
}

// Quadratic easing in
float EaseInQuad(f32 t)
{
	return t * t;
}

// Quadratic easing out
float easeOutQuad(f32 t)
{
	return -(t * (t - 2));
}

float easeInOutQuad(f32 t)
{
	return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

float EaseAudioVolume(float startVolume, float targetVolume, float progress)
{
	float easedProgress = EaseOutExpo(0, 1, progress);
	return Lerp(startVolume, targetVolume, easedProgress);
}

}
