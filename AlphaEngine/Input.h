/* Start Header ************************************************************************/
/*!
\file		Input.h
\author		Hwang Jing Rui, Austin, jingruiaustin.hwang, 2200601
\par		jingruiaustin.hwang\@digipen.edu
\date		Jan 14, 2023
\brief		This file contains the declaration of the Input_Handle() function for the ]
			input handler.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

class Input
{
public:
    Input();

    // Member function to handle input
    void handle();

    // Static function to check if a button is clicked by the user
    static bool isButtonClicked(float center_x, float center_y, float width, float height);
};
