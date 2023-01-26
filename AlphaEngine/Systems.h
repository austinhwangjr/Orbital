#pragma once
/* Start Header ************************************************************************/
/*!
\file		Systems.h
\author		Hwang Jing Rui, Austin, jingruiaustin.hwang, 2200601
\par		jingruiaustin.hwang\@digipen.edu
\date		Jan 14, 2023
\brief		This file contains the declaration of the Initialize() and Exit() functions
			for the System.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

namespace system_call
{
	//void load();
	void init(_In_ HINSTANCE hInstance, _In_ int nCmdShow);
	//void update();
	//void draw();
	//void free();
	void unload();
}
