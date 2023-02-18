/* Start Header ************************************************************************/
/*!
\file		Systems.cpp
\author		Hwang Jing Rui, Austin, jingruiaustin.hwang, 2200601
\par		jingruiaustin.hwang\@digipen.edu
\date		Jan 14, 2023
\brief		This file contains the definition of the Initialize() and Exit() functions 
			for the System.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "pch.h"
#include "Systems.h"
#include "Global.h"
#include <iostream>

// ----------------------------------------------------------------------------
// This function initializes the system
// ----------------------------------------------------------------------------
void system_call::init(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
	AESysInit(hInstance, nCmdShow, g_windowWidth, g_windowHeight, g_consoleOn, 60, true, NULL);										// Using custom window procedure
	AESysSetWindowTitle("Orbital");																										// Changing the window title
	AESysReset();																														// reset the system modules
	std::cout << "--------------------Initializing system--------------------" << std::endl;

}

// ----------------------------------------------------------------------------
// This function terminates the system
// ----------------------------------------------------------------------------
void system_call::unload()
{
	// free the system
	AESysExit();
}

