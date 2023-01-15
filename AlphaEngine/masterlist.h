/*-------------------------------------------------- Masterlist --------------------------------------------------*/
#pragma once

#include "AEEngine.h"

struct player
{
	double x_pos;
	double y_pos;
	double size;
	double dist_from_planet;
	double speed;
	double angle;
	double free; // what is free
	double beam_str;
	double beam_width;
	double current_planet;
	double current_capacity; // probably can shorten
	double max_capacity;
	double texture;
	double velocity;
};

// e.g
//player.x_pos = screen / 2;
//player.y_pos = screen / 2;

struct debris
{
	//int debris[i];
	double x_pos;
	double y_pos;
	double size;
	double dist_from_planet;
	double speed;
	double angle;
	double is_collide;
	double is_collect;
	double texture; // maybe 3-5 textures
	
};

struct shuttles
{
	double x_pos;
	double y_pos;
	double size;
	double speed;
	double vector;
	double value;
	double lifespan;
	double above_below; //random choice to shrink or increase size when passing by other planet
	double texture;
};

struct planets
{
	//double list[i];	//isnt using int btr?
	double name; // might not need
	double x_pos;
	double y_pos;
	double size;
	double shuttle_spawn_x;
	double shuttle_spawn_y;
	double shuttle_spawn_vector;
	double shuttle_timer; // shuttle countdown time displayed on planet
	double current_shuttle;
	double max_shuttle;
	double current_debris;
	double max_debris;
	double current_drones;
	double max_drones;
	double texture;
};

struct drones
{
	//double list[i];
	double x_pos;
	double y_pos;
	double size;
	double speed;
	double dist_from_planet;
	double beam_str;
	double beam_width;
	double texture;
};

struct spaceStn
{
	//double list[i];
	double x_pos;
	double y_pos;
	double size;
	double debris_value;
	double current_capacity;
	double max_capacity;
	double texture;
};

struct shop //walmart
{
	double x_pos;
	double y_pos;
	double width;
	double height;
	double button_speed_x_pos;
	double button_speed_y_pos;
	double button_strength_x_pos;
	double button_strength_y_pos;
	double button_capacity_x_pos;
	double button_capacity_y_pos;
	double upgrade_speed;
	double upgrade_strength;
	double upgrade_capacity;
};

//typedef struct audio
//{
//	bgm;
//	planet_spawn;
//	planet_selected;
//	player_thrusters_acceleration;
//	player_thrusters_deceleration;
//	player_planet_lock;
//	player_planet_orbit;
//	player_beam;
//	player_beam_levitating;
//	player_beam_cancelled;
//	drone_valid;
//	drone_invalid;
//	drone_orbit;
//	drone_beam;
//	debris_hovering;
//	shuttle_spawn;
//	shuttle_countdown;
//	shuttle_blastoff;
//	spaceStn_valid;
//	spaceStn_invalid;
//	spaceStn_selected;
//	shop_open;
//	shop_close;
//	shop_buy;
//	shop_cancel;
//
//};


/*-------------------------------------------------- General Formulas --------------------------------------------------*/
/* ---------- Player's Velocity ----------*/
//#define player.velocity = 00.0;
//#define player.speed = 00.0;
//#define shop.upgrade_speed = 0.00;
//
//#define player.velocity += player.speed * shop.upgrade_speed * dt; //  have yet to do dt
//
///* ---------- Player's Strength ----------*/
//#define player.strength = 0.00;
//#define shop.upgrade_strength = 0.00;
//
//#define player.strength = player.strength * shop.upgrade_strength;
//
///* ---------- Player's Capacity ----------*/
//#define player.capacity = 0.00;
//#define shop.upgrade_capacity = 0.00;
//
//#define player.capacity = player.capcaity + shop.upgrade_capacity; 
//
///* ---------- Player's Credit ----------*/
//#define player.credit = 0.00;
//#define shuttle.value = 0.00;
//#define drone.value = 0.00;
//#define spaceStn.value = 0.00;
//
//#define player.credit += shuttle.value + drone.value + spaceStn.value;
//
//// main menu
constexpr auto window_width = 1600;
constexpr auto window_height = 800;