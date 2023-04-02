/******************************************************************************/
/*!
\file		Transition.h
\author		Ong You Yang (100%)
\par		youyang.o\@digipen.edu
\date		March 28, 2023
\brief		This file contains the declaration of functions for the transitions.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once

namespace transition
{
	void load();
	void init();
	void update(float deltaTime);
	void draw();
	void free();
	void unload();
	void resetTimer();

	extern bool isTransitionActive;
	extern float transition_alpha;
	extern float pause_duration;

}
