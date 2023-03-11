#include "AudioManager.h"
#include "AEEngine.h"
#include "fmod_errors.h"
#include <map>
#include "Global.h"
namespace AudioManager
{
	static FMOD::System* _system;
	static FMOD_RESULT _result;

	std::map<std::string, FMOD::Sound*> _soundMap;

	// For pausing and muting
	bool isMuted;
	float originalVolume;
	FMOD::Channel* bgm;

	void Init()
	{
		// Create fmodsystem
		_system = NULL;
		ErrorCheck(FMOD::System_Create(&_system));

		// Init system
		ErrorCheck(_system->init(512, FMOD_INIT_NORMAL, nullptr));
	}

	void Update()
	{
		ErrorCheck(_system->update());
	}

	void Shutdown()
	{
		UnLoadAllSounds();

		// Clean up.
		ErrorCheck(_system->release());
	}

	void ErrorCheck(FMOD_RESULT result)
	{
		AE_ASSERT(result == FMOD_OK && FMOD_ErrorString(result));
	}

	void LoadSound(const std::string& path, bool loop)
	{
		// If we alr have it in our map, return
		auto found = _soundMap.find(path);
		if (found != _soundMap.end())
			return;

		FMOD_MODE mode = FMOD_DEFAULT;
		mode |= loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

		FMOD::Sound* sound = nullptr;
		ErrorCheck(_system->createSound(path.c_str(), mode, nullptr, &sound));

		// Add to map
		if (sound)
			_soundMap[path] = sound;
	}

	void UnLoadSound(const std::string& path)
	{
		// If we have it in our map, release it
		auto found = _soundMap.find(path);
		if (found != _soundMap.end())
		{
			ErrorCheck(found->second->release());
			_soundMap.erase(found);
		}
	}

	void UnLoadAllSounds()
	{
		// Release all sounds in the map
		for (auto const& found : _soundMap)
		{
			ErrorCheck(found.second->release());
		}

		// Clear map
		_soundMap.clear();
	}

	void PlayBGM(const std::string& path, float volume)
	{
		// Clamp the volume of the one shot
		if (volume < 0.0f || volume > 1.0f)
			volume = 1.0f;

		originalVolume = volume;

		// If we DONT have it in our map, add to sound
		auto found = _soundMap.find(path);
		if (found == _soundMap.end())
		{
			LoadSound(path);
			found = _soundMap.find(path);
		}

		ErrorCheck(_system->playSound(found->second, nullptr, true, &bgm));

		if (g_isMute)
			volume = 0.0f;

		// Set volume
		ErrorCheck(bgm->setVolume(volume));

		// Unpause the track
		ErrorCheck(bgm->setPaused(false));
	}

	void PlayOneShot(const std::string& path, float volume)
	{
		// Clamp the volume of the one shot
		if (volume < 0.0f || volume > 1.0f)
			volume = 1.0f;

		// If we DONT have it in our map, add to sound
		auto found = _soundMap.find(path);
		if (found == _soundMap.end())
		{
			LoadSound(path);
			found = _soundMap.find(path);
		}

		// Play the track paused
		FMOD::Channel* c;
		ErrorCheck(_system->playSound(found->second, nullptr, true, &c));

		if (g_isMute)
			volume = 0.0f;

		// Set volume
		ErrorCheck(c->setVolume(volume));

		// Unpause the track
		ErrorCheck(c->setPaused(false));
	}

	void ToggleMute(bool mute)
	{
		g_isMute = mute;

		// Set volume
		if (bgm)
		{
			ErrorCheck(bgm->setVolume(g_isMute ? 0.0f : originalVolume));
		}

	}

	void TogglePause(bool pause)
	{
		ErrorCheck(bgm->setPaused(pause));
	}
}
