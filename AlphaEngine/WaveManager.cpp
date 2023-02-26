#include "WaveManager.h"
#include <iostream>
#include <string>

WaveManager wave_manager;

// Text
extern s8 font_id;
const char* print_string;
std::string str_player_capacity; std::string str_wave_complete;

void WaveManager::load()
{
}

void WaveManager::init()
{
	srand(5);
	std::cout << '\n' << "Wave Manager Initialized." << '\n' << '\n';

	wave_completed = false;
	wave_number = 1;
	wave_interval_timer = 0;

	planet_count = 0;
	planet_adding = true;

	shuttle_left_planet = 0;
	shuttle_destroyed = 0;
	shuttle_increase_amount = 1;
	shuttle_has_escaped = false;
	shuttle_has_collided = false;

	planet.Planets::spawn(rand() % (SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN + 1) + SHUTTLE_SPAWN_MIN);
	planet_count++;

	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Wave " << wave_number << " has begun." << '\t';
	std::cout << "Planet Count: " << planet_count << '\n';
}

void WaveManager::update(f64 frame_time)
{
// Lose Condition--------------------------------------------------------
	if (shuttle_destroyed == get_total_shuttles() / 4)
	{
		//next_state = lose;
	}
// Lost Condition--------------------------------------------------------

// Track shuttles left---------------------------------------------------
	if (shuttle_has_escaped)
	{
		std::cout << '\n' << "Shuttle has escaped!" << '\n';
		std::cout << get_total_shuttles() - shuttle_left_planet << " Shuttle(s) left to escape." << '\n' << '\n';
		shuttle_has_escaped = false;
	}

	if (shuttle_has_collided)
	{
		shuttle_destroyed++;
		std::cout << '\n' << "Shuttle has been destroyed!" << '\n';
		std::cout << get_total_shuttles() - shuttle_left_planet << " Shuttle(s) left to escape." << '\n' << '\n';
		shuttle_has_collided = false;
	}
// Track shuttles left---------------------------------------------------

// Check wave progress---------------------------------------------------
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		if (!planet_vector[i].wave_complete && 0 == planet_vector[i].current_shuttle)
		{
			planet_vector[i].wave_complete = true;
			wave_progress++;
		}
	}
// Check wave progress---------------------------------------------------

// Completion of wave----------------------------------------------------
	if (wave_progress == planet_count && no_more_shuttles())
	{
		wave_completed = true;
		str_wave_complete = "Wave " + std::to_string(wave_number) + " Completed";
	}
// Completion of Wave----------------------------------------------------

// Add Planets at Intervals----------------------------------------------
	if ((planet_count < PLANET_MAX) && (planet_count * WAVE_ADD_PLANET) == wave_number)
	{
		planet.Planets::spawn(rand() % (SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN + 1) + SHUTTLE_SPAWN_MIN);
		planet_count++;
		planet_adding = true;
		//std::cout << "----------------------------------------" << std::endl;
		std::cout << '\n' << "Wave " << wave_number << '\t' << "Added Planet." << '\t';
		std::cout << "Planet Count: " << planet_count << '\n';
	}
// Add Planets at Intervals----------------------------------------------

// Wave interval timer---------------------------------------------------
	if (wave_completed && !planet_adding)
	{
		wave_interval_timer += frame_time;
	}
// Wave interval timer---------------------------------------------------

// Start of new wave-----------------------------------------------------
	if (wave_completed && wave_interval_timer >= WAVE_INTERVAL_TIME || AEInputCheckTriggered(AEVK_3))
	{
		for (size_t i{}; i < planet_vector.size(); i++)
		{
			// If not at max number of shuttles, increment max number for each planet
			if (planet_vector[i].max_shuttle <= SHUTTLE_MAX)
			{
				planet_vector[i].max_shuttle += shuttle_increase_amount;
			}
			planet_vector[i].current_shuttle = planet_vector[i].max_shuttle;	// Set current shuttles to be max at start of wave
			planet_vector[i].wave_complete = false;								// Reset planet wave complete flag
		}

		shuttle_left_planet = 0;	// Reset number of shuttles successfully escaped during wave
		shuttle_destroyed = 0;		// Reset number of shuttles that have been destroyed
		wave_completed = false;		// Reset wave complete flag
		wave_progress = 0;			// Reset wave progress to 0
		wave_number++;				// Increment wave number
		wave_interval_timer = 0;	// Reset wave interval timer

		std::cout << "----------------------------------------" << std::endl;
		std::cout << '\n' << "Wave " << wave_number << " has begun." << '\t';
		for (size_t i{}; i < planet_vector.size(); i++)
		{
			std::cout << "Planet " << planet_vector[i].id << ": " << planet_vector[i].max_shuttle << " Shuttles." << '\t';
		}
		std::cout << '\n';

	}
// Start of new wave-----------------------------------------------------
}

void WaveManager::draw(AEGfxVertexList *pMesh)
{
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		// Only draw shuttle related prints if wave is starting, not between intervals
		if (wave_completed == false && !planet_adding)
		{
			// SHUTTLE TIMERS-------------------------------------------------------------------------------------------------------------------------------------
			// Calculating the timer for each shuttle and assign to print_string for printing
			std::string timer = std::to_string(static_cast<int>(planet_vector[i].shuttle_time_to_spawn - planet_vector[i].shuttle_timer));
			print_string = timer.c_str();

			// Calculate position to draw timer (planet position to camera center)
			AEVec2 timer_pos;
			AEVec2Sub(&timer_pos, &planet_vector[i].position, &camera.position);

			// Draw timer at center of planet using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(print_string),
				2 * (timer_pos.x - 25.f / 2.f) / AEGetWindowWidth(),
				2 * (timer_pos.y - 25.f / 2.f) / AEGetWindowHeight(),
				1.f, planet_vector[i].shuttle_timer * 1.5 / planet_vector[i].shuttle_time_to_spawn, 0.f, 0.f);
			// SHUTTLE TIMERS-------------------------------------------------------------------------------------------------------------------------------------

			// SHUTTLE COUNT--------------------------------------------------------------------------------------------------------------------------------------
			// Get current shuttle count and display as Current / Max
			std::string shuttle_count = std::to_string(planet_vector[i].current_shuttle);
			shuttle_count = shuttle_count + " / " + std::to_string(planet_vector[i].max_shuttle);
			print_string = shuttle_count.c_str();

			// Calculate position to draw shuttle count (slightly below center of planet)
			AEVec2 shuttle_count_pos;
			AEVec2Sub(&shuttle_count_pos, &planet_vector[i].position, &camera.position);
			// Set offset for drawing of text because font position is bottom left of font
			AEVec2 shuttle_count_pos_offset;
			AEVec2Set(&shuttle_count_pos_offset, 50, 100);

			// Draw Current / Max using position calculated above
			AEVec2Sub(&shuttle_count_pos, &shuttle_count_pos, &shuttle_count_pos_offset);
			AEGfxPrint(font_id, const_cast<s8*>(print_string),
				2 * (shuttle_count_pos.x) / AEGetWindowWidth(),
				2 * (shuttle_count_pos.y) / AEGetWindowHeight(),
				1.f, 0.f, 0.f, 0.f);
			// SHUTTLE COUNT--------------------------------------------------------------------------------------------------------------------------------------
		}

		// DISTANCE INDICATOR-------------------------------------------------------------------------------------------------------------------------------------
		// Only draw distance indicator if the planet is off screen
		if (pow(AEVec2Distance(&planet_vector[i].position, &camera.position), 2) > (pow(AEGetWindowWidth() / 2, 2) + pow(AEGetWindowHeight() / 2, 2)))
		{
			// Calculating distance between player and planet and assign to print_string for printing
			std::string dist = std::to_string(static_cast<int>(AEVec2Distance(&player.position, &planet_vector[i].position)));
			print_string = dist.c_str();

			// Calculate position to draw indicator (planet position to camera center)
			AEVec2 dist_pos;
			AEVec2Sub(&dist_pos, &planet_vector[i].position, &camera.position);

			// Clamp position to keep indicator drawn within camera boundaries
			// Since position of AEGfxPrint is limit within -1.f to 1.f, clamp within those boundaries
			// Subtract length of indicator string from 1.f to make up for font offset since position of font is at bottom left of font
			AEGfxPrint(font_id, const_cast<s8*>(print_string),
				AEClamp(dist_pos.x / AEGetWindowWidth(), -1.f, 1.f - (static_cast<f32>(230) / static_cast<f32>(AEGetWindowWidth()))),
				AEClamp(dist_pos.y / AEGetWindowHeight(), -1.f, 1.f - (static_cast<f32>(80) / static_cast<f32>(AEGetWindowHeight()))),
				1.f, 1.f, 1.f, 1.f);
		}
		// DISTANCE INDICATOR-------------------------------------------------------------------------------------------------------------------------------------
	}

	// Wave Complete font
	if (wave_interval_timer <= WAVE_INTERVAL_TIME)
	{
		print_string = str_wave_complete.c_str();
		AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.2f, 0.85f + abs((WAVE_INTERVAL_TIME - (wave_interval_timer * 2)) / WAVE_INTERVAL_TIME) * 0.2f, 1.f, 1.f, 1.f, 1.f);
	}
}

void WaveManager::free()
{

}

void WaveManager::unload()
{

}

int WaveManager::get_total_shuttles()
{
	int total_shuttles{};
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		total_shuttles += planet_vector[i].max_shuttle;
	}
	return total_shuttles;
}

bool WaveManager::no_more_shuttles()
{
	bool check = false;
	for (size_t i{}; i < shuttle_vector.size(); i++)
	{
		check = (shuttle_vector[i].active) ? false : true;
	}
	return check;
}