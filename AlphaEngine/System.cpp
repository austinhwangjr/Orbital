/* Start Header ************************************************************************/
/*!
\file		System.cpp
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
#include "System.h"

// ----------------------------------------------------------------------------
// This function initializes the system
// ----------------------------------------------------------------------------
void system_initialize(_In_ HINSTANCE hInstance, _In_ int nCmdShow)
{
	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("Orbital");

	// reset the system modules
	AESysReset();
}

// ----------------------------------------------------------------------------
// This function terminates the system
// ----------------------------------------------------------------------------
void system_exit()
{
	// free the system
	AESysExit();
}