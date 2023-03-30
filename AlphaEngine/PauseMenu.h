/******************************************************************************/
/*!
\file		PauseMenu.h
\author 	
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the pause menu.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once

#include "AEEngine.h"

struct pause_menu
{
    void load();
    void init();
    void update();
    void draw(AEVec2 const& camPos);
    void free();
    void unload();
};