#include "Camera.h"

void Camera::init(Player player)
{
	AEVec2Zero(&velocity);
	time_to_move = 10.f;
	position.x = player.position.x;
	position.y = player.position.y;
}

void Camera::update(f64 frame_time, Player player)
{
	f32 t{};
	t += frame_time * (AEVec2Distance(&player.position, &position) / time_to_move);
	AEVec2Lerp(&position, &position, &player.position, t);

	AEGfxSetCamPosition(position.x, position.y);
}

void Camera::planet_transition(f64 frame_time, Planets planet)
{
	
}