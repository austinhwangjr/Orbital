/******************************************************************************/
/*!
\file		MainLevel.h
\author 	Hwang Jin Rui, Austin
\par    	email: jinruiaustin.hwang\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the main level.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once
#include "pch.h"
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