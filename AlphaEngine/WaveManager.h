#pragma once
#define PLANET_SIZE 100.0f
#define MAX_PLANET 10
#define MAX_SHUTTLE 6
#define INITIAL_SHUTTLE 4

#include "AEEngine.h"
#include "Player.h"
#include "Planet.h"
#include "Shuttle.h"

struct WaveManager
{
	int wave_number;
	int wave_progress;
	bool wave_completed;

	int shuttle_left_planet;
	int shuttle_increase_amount;
	int shuttle_randomize_amount;
	bool shuttle_has_escaped;
	bool shuttle_has_collided;

	int planet_count;
	int planet_spawn_interval;
	f64 planet_spawn_timer;

	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList *pMesh);
	void free();
	void unload();

	//void set_shuttle_max(std::vector<Planets> planet_vector, int max_shuttle);
	int get_total_shuttles();
};

extern Player player;
extern Planets planet;
extern std::vector<Planets> planet_vector;
extern std::vector<Shuttles> shuttle_vector;