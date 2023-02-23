#pragma once
#define MAX_PLANET 10
#define MAX_SHUTTLE 7
#define INITIAL_SHUTTLE 4
#define MAX_DEBRIS 20
#define MIN_DEBRIS 10
#define WAVE_INTERVAL_TIME 3
#define WAVE_ADD_PLANET 2

#include "AEEngine.h"
#include "Player.h"
#include "Planet.h"
#include "Shuttle.h"
#include "Camera.h"
#include "Global.h"

struct WaveManager
{
	int wave_number;
	int wave_progress;
	bool wave_completed;
	f64 wave_interval_timer;

	int shuttle_left_planet;
	int shuttle_increase_amount;
	int shuttle_randomize_amount;
	int shuttle_destroyed;
	bool shuttle_has_escaped;
	bool shuttle_has_collided;

	int planet_count;
	f64 planet_spawn_timer;
	bool planet_adding;

	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList *pMesh);
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