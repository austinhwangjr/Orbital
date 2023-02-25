#include "Camera.h"
#include "WaveManager.h"

extern WaveManager wave_manager;

void Camera::init(Player player)
{
	AEVec2Zero(&velocity);
	time_to_player = 300.f;
	time_to_planet = 1000.f;
	transition_time = 0.f;
	position.x = player.position.x;
	position.y = player.position.y;
}

void Camera::update(f64 frame_time, Player player)
{
	(wave_manager.planet_adding) ? planet_transition(frame_time) : follow_player(frame_time, player);
}

void Camera::follow_player(f64 frame_time, Player player)
{
	f32 t{};
	t += frame_time * (AEVec2Distance(&player.position, &position) / time_to_player);
	AEVec2Lerp(&position, &position, &player.position, t);

	AEGfxSetCamPosition(position.x, position.y);
}

void Camera::planet_transition(f64 frame_time)
{
	transition_time += frame_time;
	f32 t{};
	t += frame_time * (AEVec2Distance(&planet_vector[planet_vector.size() - 1].position, &position) / time_to_planet);
	AEVec2Lerp(&position, &position, &planet_vector[planet_vector.size() - 1].position, t);

	AEGfxSetCamPosition(position.x, position.y);

	if (transition_time >= TRANSITION_TIME_MAX)
	{
		wave_manager.planet_adding = false;
		transition_time = 0.f;
	}
}