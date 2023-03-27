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

