/******************************************************************************/
/*!
\file		AudioManager.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the audio 
            manager.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "AudioManager.h"
#include "AEEngine.h"
#include "fmod_errors.h"
#include <map>
#include "Global.h"

bool isBGMPlaying = false;

namespace AudioManager
{

    // The FMOD system and error result
    static FMOD::System* _system;
    static FMOD_RESULT _result;

    // A map of loaded sounds
    std::map<std::string, FMOD::Sound*> _soundMap;

    // For pausing and muting
    float originalVolume;
    FMOD::Channel* bgm;

    // Initializes the FMOD system
    void Init()
    {
        // Create fmodsystem
        _system = NULL;
        ErrorCheck(FMOD::System_Create(&_system));

        // Init system
        ErrorCheck(_system->init(512, FMOD_INIT_NORMAL, nullptr));
    }

    // Updates the FMOD system
    void Update()
    {
        ErrorCheck(_system->update());
    }

    // Shuts down the FMOD system and unloads all sounds
    void Shutdown()
    {
        UnloadAllSounds();

        // Clean up.
        ErrorCheck(_system->release());

        isBGMPlaying = false;

    }



    // Loads a sound file into memory and stores it in a map
    void LoadSound(const std::string& path, bool loop)
    {
        // If we already have it in our map, return
        auto found = _soundMap.find(path);
        if (found != _soundMap.end())
            return;

        // Set the loop mode for the sound
        FMOD_MODE mode = FMOD_DEFAULT;
        mode |= loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

        // Load the sound into memory
        FMOD::Sound* sound = nullptr;
        ErrorCheck(_system->createSound(path.c_str(), mode, nullptr, &sound));

        // Add the sound to the map
        if (sound)
            _soundMap[path] = sound;
    }

    // Unloads a sound file from memory and removes it from the map
    void UnloadSound(const std::string& path)
    {
        // If we have it in our map, release it
        auto found = _soundMap.find(path);
        if (found != _soundMap.end())
        {
            ErrorCheck(found->second->release());
            _soundMap.erase(found);
        }
    }

    // Unloads all sound files from memory and clears the map
    void UnloadAllSounds()
    {
        // Release all sounds in the map
        for (auto const& found : _soundMap)
        {
            ErrorCheck(found.second->release());
        }

        // Clear the map
        _soundMap.clear();
    }

    // Plays a background music file
    void PlayBGM(const std::string& path, float volume)
    {
        isBGMPlaying = true;

        // Clamp the volume of the background music
        if (volume < 0.0f || volume > 1.0f)
            volume = 1.0f;

        // Remember the original volume for later use
        originalVolume = volume;

        // If no sound, load the sound
        auto found = _soundMap.find(path);
        if (found == _soundMap.end())
        {
            LoadSound(path);
            found = _soundMap.find(path);
        }

        // Play the sound in paused mode
        ErrorCheck(_system->playSound(found->second, nullptr, true, &bgm));

        // If audio is currently muted, set volume to 0
        if (g_isMute)
            volume = 0.0f;

        // Set the volume of the background music
        ErrorCheck(bgm->setVolume(volume));

        // Unpause the sound
        ErrorCheck(bgm->setPaused(false));
    }

    // Plays a one-shot sound effect
    intptr_t PlayOnce(const std::string& path, float volume)
    {
        // Clamp the volume of the one-shot sound effect
        if (volume < 0.0f || volume > 1.0f)
            volume = 1.0f;

        // If no sound, load the sound
        auto found = _soundMap.find(path);
        if (found == _soundMap.end())
        {
            LoadSound(path);
            found = _soundMap.find(path);
        }

        // Play the sound in paused mode
        FMOD::Channel* c;
        ErrorCheck(_system->playSound(found->second, nullptr, true, &c));

        // If audio is currently muted, set volume to 0
        if (g_isMute)
            volume = 0.0f;

        // Set the volume of the one-shot sound effect
        ErrorCheck(c->setVolume(volume));

        // Unpause the sound
        ErrorCheck(c->setPaused(false));

        // Return the unique sound ID (using FMOD::Channel* pointer as an ID)
        return reinterpret_cast<intptr_t>(c);
    }

    // Toggles the mute state of the audio
    void ToggleMute(bool mute)
    {
        // Set the global mute state
        g_isMute = mute;

        // Set the volume of the background music to 0 if audio is muted, else set it to the original volume
        if (bgm)
        {
            ErrorCheck(bgm->setVolume(g_isMute ? 0.0f : originalVolume));
        }
    }

    // Toggles the pause state of the audio
    void TogglePause(bool pause)
    {
        // Pause or unpause the background music
        ErrorCheck(bgm->setPaused(pause));
    }

    void setVolume(float volume)
    {
        if (volume < 0.0f)
            volume = 0.0f;
        if (volume > 1.0f)
            volume = 1.0f;

        originalVolume = volume;

        if (bgm)
        {
            ErrorCheck(bgm->setVolume(g_isMute ? 0.0f : originalVolume));
        }
    }

    void AudioManager::StopBGMIfPlaying()
    {
        if (isBGMPlaying)
        {
            ErrorCheck(bgm->stop());
            isBGMPlaying = false;
        }
    }

    void StopAll()
    {
        // Stop the background music if it's playing
        StopBGMIfPlaying();

        // Stop all sound effects
        FMOD::ChannelGroup* masterGroup = nullptr;
        _result = _system->getMasterChannelGroup(&masterGroup);
        ErrorCheck(_result);
        if (masterGroup)
        {
            _result = masterGroup->stop();
            ErrorCheck(_result);
        }
    }

    void Stop(intptr_t soundID)
    {
        FMOD::Channel* channel = reinterpret_cast<FMOD::Channel*>(soundID);
        if (channel)
        {
            ErrorCheck(channel->stop());
        }
    }

    // Helper function to check for FMOD errors
    void ErrorCheck(FMOD_RESULT result)
    {
        AE_ASSERT(result == FMOD_OK && FMOD_ErrorString(result));
    }

}
