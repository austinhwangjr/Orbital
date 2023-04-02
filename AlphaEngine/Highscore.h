/******************************************************************************/
/*!
\file		Highscore.h
\author 	Leong Wen Bin, Aceson (100%)
\par    	email: leong.w\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the highscore 
            feature.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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