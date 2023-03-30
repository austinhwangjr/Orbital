/******************************************************************************/
/*!
\file		Highscore.h
\author 	Leong Wen Bin, Aceson
\par    	email: leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the highscore 
            feature.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once
#include "AEEngine.h"
#include "Player.h"

namespace HighScore
{
	void load();
	void init();
	void update();
	void draw();
	void free();
	void unload();

	struct Button3
	{
		float x;
		float y;
		float width;
		float height;
	};
};

void putHighScore(int score);