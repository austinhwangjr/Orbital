#pragma once
#include "AEEngine.h"
#include "Player.h"

struct Camera
{
	AEVec2 position;
	AEVec2 velocity;
	f32 time_to_move;		// Higher the number, slower the camera follows

	void init(Player player);
	void update(f64 frame_time, Player player);
	void planet_transition(f64 frame_time, Planets planet);
};