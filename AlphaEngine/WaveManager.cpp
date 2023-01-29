#include "WaveManager.h"
#include <iostream>
#include <string>

WaveManager wave_manager;

// Text
s8 font_id;
const char* print_string;
std::string player_capacity;

void WaveManager::load()
{
	// Font for text
	font_id = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 50);
}

void WaveManager::init()
{
	std::cout << '\n' << "Wave Manager Initialized." << '\n' << '\n';

	wave_completed = false;
	wave_number = 1;

	planet_count = 0;
	planet_spawn_interval = 3;

	shuttle_escaped = 0;
	shuttle_increase_amount = 1;

	planet.Planets::spawn();
	planet_count++;

	std::cout << "Wave " << wave_number << " has begun." << '\n';
}

void WaveManager::update(f64 frame_time)
{
	//planet_spawn_timer += frame_time;

	if (wave_completed)
	{
		for (size_t i{}; i < planet_vector.size(); i++)
		{
			planet_vector[i].max_shuttle += shuttle_increase_amount;
			planet_vector[i].current_shuttle = 0;
			planet_vector[i].complete = false;
		}

		shuttle_escaped = 0;

		wave_completed = false;
		wave_progress = 0;
		wave_number++;

		std::cout << '\n' << "Wave " << wave_number << " has begun." << '\n';
	}

	// Add Planets at Intervals
	if (0 == (wave_number % planet_spawn_interval) && planet_count < max_planet && (planet_count * planet_spawn_interval) == wave_number)
	{
		std::cout << '\n' << "Wave " << wave_number << '\t' << "Added Planet." << '\t';

		planet.Planets::spawn();
		planet_count++;
		
		std::cout << "Planet Count: " << planet_count << '\n';
	}

	for (size_t i{}; i < shuttle_vector.size(); i++)
	{
		if (!shuttle_vector[i].active)
		{
			std::cout << "Shuttle has escaped!" << '\n';
			std::cout << get_total_shuttles() - shuttle_escaped << " Shuttle(s) left to escape." << '\n';
		}
	}

	// Check Wave Progress
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		if (!planet_vector[i].complete && planet_vector[i].current_shuttle == planet_vector[i].max_shuttle)
		{
			planet_vector[i].complete = true;
			wave_progress++;

			std::cout << '\n' << "Wave Progress: " << wave_progress << "/" << planet_count << '\n';

			// Check for completion of Wave
			if (wave_progress == planet_count)
			{
				wave_completed = true;
				std::cout << '\n' << "Wave " << wave_number << " Completed." << '\n';
			}
		}
	}

	player_capacity = std::to_string(player.current_capacity);
	print_string = player_capacity.c_str();
}

void WaveManager::draw(AEGfxVertexList *pMesh)
{
	// DRAW TEXT
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.75f, 0.75f, 1.f, 1.f, 1.f, 1.f);
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