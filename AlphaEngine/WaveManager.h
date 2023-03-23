#pragma once
#define PLANET_MAX				10		// Maximum number of planets
#define SHUTTLE_MAX				10		// Maximum number of shuttles per planet
#define SHUTTLE_SPAWN_MAX		8		// Maximum number of shuttles a planet can spawn with
#define SHUTTLE_SPAWN_MIN		4		// Minimum number of shuttles a planet can spawn with
#define SHUTTLE_INCREASE_AMOUNT 1		// Number of shuttles added after a wave
#define WAVE_INTERVAL_TIME		8		// Down time between waves
#define WAVE_ADD_PLANET			4		// Every nth wave planets will be added on

#include "AEEngine.h"
#include "Player.h"
#include "Planet.h"
#include "Shuttle.h"
//#include "SpaceStation.h"
#include "Camera.h"
#include "Global.h"

struct WaveManager
{
	struct Indicator
	{
		AEGfxTexture*		tex;
		AEMtx33				scale;
		AEMtx33				rotate;
		AEMtx33				translate;
		AEMtx33				transform;
		AEVec2				position;
		f32					size;
	};

	struct Outline
	{
		AEMtx33				scale;
		AEMtx33				rotate;
		AEMtx33				translate;
		AEMtx33				transform;
		AEVec2				position;
		f32					size;
	};

	struct Arrow
	{
		AEMtx33				scale;
		AEMtx33				rotate;
		AEMtx33				translate;
		AEMtx33				transform;
		AEVec2				position;
		f32					direction;
		f32					size;
		f64					blinking_timer;		// Timer for blinking indicators
		float				blinker;			// Alpha value for arrow to simulate flashing effect
		f64					urgency;			// Timer for speed of arrow indicator flashing
	};

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


// OTHER VARIABLES---------------------------
	bool	gameLost;					// Flag for when the game is lost
// OTHER VARIABLES---------------------------

	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();

	//void set_shuttle_max(std::vector<Planets> planet_vector, int max_shuttle);
	int get_total_shuttles();
	int get_current_shuttles() const;
	bool no_more_shuttles();

	void checkLoseCondition();
	void add_indicator();
	//void add_ss_indicator();
};

extern Player player;
extern Planets planet;
extern Camera camera;
extern std::vector<Planets> planet_vector;
extern std::vector<AEGfxTexture*> planet_textures;
extern std::vector<Shuttles> shuttle_vector;
//extern std::vector<SpaceStation> space_station_vector;