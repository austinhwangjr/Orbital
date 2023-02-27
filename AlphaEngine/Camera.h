#pragma once
#include "AEEngine.h"
#include "Player.h"

#define TRANSITION_TIME_MAX 0.f

struct Camera
{
	AEVec2 position;
	AEVec2 velocity;
	f32 time_to_player;		// Higher the number, slower the camera follows
	f32 time_to_planet;
	f32 transition_time;

	void init(Player player);
	void update(f64 frame_time, Player player);
	void follow_player(f64 frame_time, Player player);
	void planet_transition(f64 frame_time);
};