#include "Camera.h"
#include "WaveManager.h"
#include "SpaceStation.h"

extern WaveManager wave_manager;
extern std::vector<SpaceStation> space_station_vector;

void Camera::init(Player& player)
{
	AEVec2Zero(&velocity);
	transition_time = 0.f;
	position.x = player.position.x;
	position.y = player.position.y;
}

void Camera::update(f64 frame_time, Player& player)
{
	if (wave_manager.planet_adding)
	{
		planet_transition(frame_time);
	}
	else if (wave_manager.capacity_full)
	{
		station_transition(frame_time);
	}
	else
	{
		follow_player(frame_time, player);
	}
}

void Camera::follow_player(f64 frame_time, Player& player)
{
	f32 t{};
	t += static_cast<f32>(frame_time) * (AEVec2Distance(&player.position, &position) / CAM_PLAYER_SPEED);
	AEVec2Lerp(&position, &position, &player.position, t);

	AEGfxSetCamPosition(position.x, position.y);
}

void Camera::planet_transition(f64 frame_time)
{
	transition_time += static_cast<f32>(frame_time);
	f32 t{};
	t += static_cast<f32>(frame_time) * (AEVec2Distance(&planet_vector[planet_vector.size() - 1].position, &position) / CAM_PLANET_SPEED);
	AEVec2Lerp(&position, &position, &planet_vector[planet_vector.size() - 1].position, t);

	AEGfxSetCamPosition(position.x, position.y);

	if (transition_time >= TRANSITION_TIME_MAX)
	{
		wave_manager.planet_adding = false;
		transition_time = 0.f;
	}
}

void Camera::station_transition(f64 frame_time)
{
	transition_time += static_cast<f32>(frame_time);
	f32 t{};
	t += static_cast<f32>(frame_time) * (AEVec2Distance(&space_station_vector[0].position, &position) / CAM_STATION_SPEED);
	AEVec2Lerp(&position, &position, &space_station_vector[0].position, t);

	AEGfxSetCamPosition(position.x, position.y);

	if (transition_time >= TRANSITION_TIME_MAX)
	{
		wave_manager.capacity_full = false;
		transition_time = 0.f;
	}
}