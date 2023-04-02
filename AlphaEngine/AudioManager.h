/******************************************************************************/
/*!
\file		AudioManager.h
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the declaration of functions for the player.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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
    intptr_t PlayOnce(const std::string& path, float volume = 1.0f);
    void ToggleMute(bool mute);
    void TogglePause(bool pause);
    void StopAll();
    void Stop(intptr_t soundID);


    void setVolume(float volume);

    static bool isBGMPlaying;

    void StopBGMIfPlaying();

}

