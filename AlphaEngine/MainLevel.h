/******************************************************************************/
/*!
\file		MainLevel.h
\author 	Hwang Jin Rui, Austin (100%)
\par    	email: jinruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the main level.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#pragma once
#include "gch.h"
#include "Global.h"

namespace main_level
{
	void load();
	void init();
	void update();
	void draw();
	void free();
	void unload();
}
extern bool is_paused;