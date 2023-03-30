/******************************************************************************/
/*!
\file		AudioManager.h
\author 	 
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the player.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#pragma once

#include "fmod.hpp"
#include <string>

namespace AudioManager
{
    void Init();
    void Update();
    void Shutdown();
    void ErrorCheck(FMOD_RESULT result);

    void LoadSound(const std::string& path, bool loop = false);
    void UnloadSound(const std::string& path);
    void UnloadAllSounds();
    void PlayBGM(const std::string& path, float volume = 1.0f);
    void PlayOneShot(const std::string& path, float volume = 1.0f);
    void ToggleMute(bool mute);
    void TogglePause(bool pause);

    void setVolume(float volume);

    static bool isBGMPlaying;

    void StopBGMIfPlaying();

}

