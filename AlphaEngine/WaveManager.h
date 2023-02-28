#pragma once
#define PLANET_MAX				10		// Maximum number of planets
#define SHUTTLE_MAX				10		// Maximum number of shuttles per planet
#define SHUTTLE_SPAWN_MAX		8		// Maximum number of shuttles a planet can spawn with
#define SHUTTLE_SPAWN_MIN		4		// Minimum number of shuttles a planet can spawn with
#define SHUTTLE_INCREASE_AMOUNT 1		// Number of shuttles added after a wave
#define WAVE_INTERVAL_TIME		3		// Down time between waves
#define WAVE_ADD_PLANET			4		// Every nth wave planets will be added on

#include "AEEngine.h"
#include "Player.h"
#include "Planet.h"
#include "Shuttle.h"
#include "Camera.h"
#include "Global.h"

struct WaveManager
{
// WAVE VARIABLES----------------------------
	int		wave_number;				// Current wave number
	int		wave_progress;				// Number of planets that have no more shuttles to escape
	bool	wave_completed;				// Flag for when the current wave has been completed
	f64		wave_interval_timer;		// Timer to increment towards WAVE_INTERVAL_TIME
// WAVE VARIABLES----------------------------

// SHUTTLE VARIABLES-------------------------
	int		shuttle_left_planet;		// Number of shuttles successfully escaped
	int		shuttle_destroyed;			// Number of shuttles collided with debris
	bool	shuttle_has_escaped;		// Flag for when a shuttle has successfully escaped
	bool	shuttle_has_collided;		// Flag for when a shuttle has collided with debris
// SHUTTLE VARIABLES-------------------------

// PLANET VARIABLES--------------------------
	int		planet_count;				// Number of planets
	bool	planet_adding;				// Flag for when planet adding transition is playing
// PLANET VARIABLES--------------------------

	void load();
	void init();
	void update(f64 frame_time);
	void draw();
	void free();
	void unload();

	//void set_shuttle_max(std::vector<Planets> planet_vector, int max_shuttle);
	int get_total_shuttles();
	bool no_more_shuttles();
};

extern Player player;
extern Planets planet;
extern Camera camera;
extern std::vector<Planets> planet_vector;
extern std::vector<Shuttles> shuttle_vector;