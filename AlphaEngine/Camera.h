#pragma once
#include "AEEngine.h"
#include "Player.h"

// Smaller the number, the faster the camera speed
#define CAM_PLAYER_SPEED		120.f	// Camera speed for player
#define CAM_PLANET_SPEED		1000.f	// Camera speed for planet transition
#define CAM_STATION_SPEED		1000.f	// Camera speed for station transition

#define TRANSITION_TIME_MAX		4.f		// Camera Transition time

struct Camera
{
	AEVec2 position;
	AEVec2 velocity;
	f32 transition_time;

	void init(Player& player);
	void update(f64 frame_time, Player& player);
	void follow_player(f64 frame_time, Player& player);
	void planet_transition(f64 frame_time);
	void station_transition(f64 frame_time);
};