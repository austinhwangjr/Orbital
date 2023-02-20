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
	// Font for text
	//font_id = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
}

void WaveManager::init()
{
	srand(5);
	std::cout << '\n' << "Wave Manager Initialized." << '\n' << '\n';

	wave_completed = false;
	wave_number = 1;
	wave_interval_timer = 0;

	planet_count = 0;
	planet_spawn_interval = 3;

	shuttle_left_planet = 0;
	shuttle_destroyed = 0;
	shuttle_increase_amount = 1;
	shuttle_has_escaped = false;
	shuttle_has_collided = false;

	planet.Planets::spawn(rand() % INITIAL_SHUTTLE + 1);
	planet_count++;

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
	if ((planet_count < MAX_PLANET) && (planet_count * planet_spawn_interval) == wave_number)
	{
		planet.Planets::spawn(rand() % INITIAL_SHUTTLE + 1);
		planet_count++;

		std::cout << '\n' << "Wave " << wave_number << '\t' << "Added Planet." << '\t';
		std::cout << "Planet Count: " << planet_count << '\n';
	}
// Add Planets at Intervals----------------------------------------------

// Wave interval timer---------------------------------------------------
	if (wave_completed)
	{
		wave_interval_timer += frame_time;
	}
// Wave interval timer---------------------------------------------------

// Start of new wave-----------------------------------------------------
	if (wave_completed && wave_interval_timer >= WAVE_INTERVAL_TIME)
	{
		for (size_t i{}; i < planet_vector.size(); i++)
		{
			// If not at max number of shuttles, increment max number for each planet
			if (planet_vector[i].max_shuttle <= MAX_SHUTTLE)
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
		std::string timer = std::to_string(static_cast<int>(planet_vector[i].shuttle_time_to_spawn - planet_vector[i].shuttle_timer));
		print_string = timer.c_str();
		AEVec2 pos;
		AEVec2Sub(&pos, &planet_vector[i].position, &camera.position);
		AEGfxPrint(font_id, const_cast<s8*>(print_string),
			2 * (pos.x) / AEGetWindowWidth(),
			2 * (pos.y) / AEGetWindowHeight(),
			1.f, planet_vector[i].shuttle_timer * 1.5 / planet_vector[i].shuttle_time_to_spawn, 0.f, 0.f);
	}

	if (wave_interval_timer <= WAVE_INTERVAL_TIME)
	{
		print_string = str_wave_complete.c_str();
		AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.2f, 0.85f + abs((WAVE_INTERVAL_TIME - (wave_interval_timer * 2)) / WAVE_INTERVAL_TIME) * 0.2f, 1.f, 1.f, 1.f, 1.f);
	}

	// DRAW TEXT
	str_player_capacity = "Capacity: " + std::to_string(player.current_capacity) + " / " + std::to_string(player.max_capacity);
	print_string = str_player_capacity.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.5f, 0.75f, 1.f, 1.f, 1.f, 1.f);
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