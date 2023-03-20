/* Start Header ************************************************************************/
/*!
\file		WaveManager.cpp
\author		Tan Rui An Ryan, ruianryan.t, 2200600
\par		ruianryan.t\@digipen.edu
\date		Feb 27, 2023
\brief		This file contains the behaviour for the WaveManager game object

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "AEEngine.h"
#include "WaveManager.h"
#include "LoseMenu.h"
#include "GameStateManager.h"
#include <iostream>
#include <string>
#include <vector>

// Text
extern s8 font_id;
const char* print_string;

AEGfxTexture* indicator_tex;
AEGfxTexture* outline_tex;
AEGfxTexture* arrow_tex;

std::vector<WaveManager::Indicator> indicator_vector;
std::vector<WaveManager::Outline> outline_vector;
std::vector<WaveManager::Arrow>	arrow_vector;

void WaveManager::load()
{
	indicator_tex = AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture.png");
	outline_tex = AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture.png");
	arrow_tex = AEGfxTextureLoad("Assets/MainLevel/ml_arrow.png");
	lose_menu::load();
}

void WaveManager::init()
{
	lose_menu::init();

	std::cout << '\n' << "Wave Manager Initialized." << '\n' << '\n';

	wave_completed = false;
	wave_number = 1;
	wave_interval_timer = 0;

	planet_count = 0;
	planet_adding = true;

	shuttle_left_planet = 0;
	shuttle_destroyed = 0;
	shuttle_has_escaped = false;
	shuttle_has_collided = false;

	add_indicator();
	gameLost = false;

	planet.spawn(rand() % (SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN) + SHUTTLE_SPAWN_MIN);
	planet_count++;

	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Wave " << wave_number << " has begun." << '\t';
	std::cout << "Planet Count: " << planet_count << '\n';
}

void WaveManager::update(f64 frame_time)
{
	// Lose Condition--------------------------------------------------------
	if (gameLost)
	{
		lose_menu::update();
	}
	checkLoseCondition();

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
	}
	// Completion of Wave----------------------------------------------------

	// Add Planets at Intervals----------------------------------------------
	if ((planet_count < PLANET_MAX) && (planet_count * WAVE_ADD_PLANET) == wave_number)
	{
		planet.Planets::spawn(rand() % (SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN) + SHUTTLE_SPAWN_MIN);
		planet_count++;
		planet_adding = true;

		add_indicator();

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

	// Update position of off-screen indicator-------------------------------
	for (size_t i{}; i < indicator_vector.size(); i++)
	{
		f32 cam_x{}, cam_y{};
		AEGfxGetCamPosition(&cam_x, &cam_y);


		AEVec2Sub(&indicator_vector[i].position, &planet_vector[i].position, &camera.position);
		AEMtx33Trans(&indicator_vector[i].translate, AEClamp(indicator_vector[i].position.x * 0.5 + cam_x,
																-((AEGetWindowWidth() - indicator_vector[i].size) / 2) * 0.8f + cam_x,
																 ((AEGetWindowWidth() - indicator_vector[i].size) / 2) * 0.8f + cam_x),
													 AEClamp(indicator_vector[i].position.y * 0.5 + cam_y,
																-(AEGetWindowHeight() / 2) * 0.8f + cam_y,
																 (AEGetWindowHeight() / 2) * 0.715f + cam_y));
		AEMtx33Concat(&indicator_vector[i].transform, &indicator_vector[i].rotate, &indicator_vector[i].scale);
		AEMtx33Concat(&indicator_vector[i].transform, &indicator_vector[i].translate, &indicator_vector[i].transform);


		AEVec2Sub(&outline_vector[i].position, &planet_vector[i].position, &camera.position);
		AEMtx33Trans(&outline_vector[i].translate, AEClamp(outline_vector[i].position.x * 0.5 + cam_x,
																-((AEGetWindowWidth() - outline_vector[i].size) / 2) * 0.8f + cam_x,
																 ((AEGetWindowWidth() - outline_vector[i].size) / 2) * 0.8f + cam_x),
												   AEClamp(outline_vector[i].position.y * 0.5 + cam_y,
																-(AEGetWindowHeight() / 2) * 0.8f + cam_y,
																 (AEGetWindowHeight() / 2) * 0.715f + cam_y));
		AEMtx33Concat(&outline_vector[i].transform, &outline_vector[i].rotate, &outline_vector[i].scale);
		AEMtx33Concat(&outline_vector[i].transform, &outline_vector[i].translate, &outline_vector[i].transform);


		arrow_vector[i].direction = static_cast<f32>(atan2(static_cast<double>(arrow_vector[i].position.y - planet_vector[i].position.y - planet_vector[i].size),
															static_cast<double>(arrow_vector[i].position.x - planet_vector[i].position.x - planet_vector[i].size)));
		arrow_vector[i].direction = AEWrap(arrow_vector[i].direction, -PI, PI);
		AEMtx33Rot(&arrow_vector[i].rotate, arrow_vector[i].direction);
		AEVec2Sub(&arrow_vector[i].position, &camera.position, &planet_vector[i].position);
		AEMtx33Trans(&arrow_vector[i].translate, AEClamp(arrow_vector[i].position.x * -0.5 + cam_x,
															-((AEGetWindowWidth() - outline_vector[i].size) / 2) * 0.8f + cam_x,
															 ((AEGetWindowWidth() - outline_vector[i].size) / 2) * 0.8f + cam_x),
												 AEClamp(arrow_vector[i].position.y * -0.5 + cam_y,
															-(AEGetWindowHeight() / 2) * 0.8f + cam_y,
															 (AEGetWindowHeight() / 2) * 0.715f + cam_y));
		AEMtx33Concat(&arrow_vector[i].transform, &arrow_vector[i].rotate, &arrow_vector[i].scale);
		AEMtx33Concat(&arrow_vector[i].transform, &arrow_vector[i].translate, &arrow_vector[i].transform);
	}
	// Update position of off-screen indicator-------------------------------

	// Start of new wave-----------------------------------------------------
	if (wave_completed && wave_interval_timer >= WAVE_INTERVAL_TIME || AEInputCheckTriggered(AEVK_3))
	{
		for (size_t i{}; i < planet_vector.size(); i++)
		{
			// If not at max number of shuttles, increment max number for each planet
			if (planet_vector[i].max_shuttle < SHUTTLE_MAX)
			{
				planet_vector[i].max_shuttle += SHUTTLE_INCREASE_AMOUNT;
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

void WaveManager::draw(AEGfxVertexList* pMesh)
{
	if (gameLost == true)
	{
		lose_menu::draw(camera.position);
	}

	for (size_t i{}; i < planet_vector.size(); i++)
	{
		bool off_screen{ pow(AEVec2Distance(&planet_vector[i].position, &camera.position), 2) > (pow(AEGetWindowWidth() / 2, 2) + pow(AEGetWindowHeight() / 2, 2)) };
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
				2 * (timer_pos.x - FONT_ID_SIZE / 4.f) / AEGetWindowWidth(),
				2 * (timer_pos.y - FONT_ID_SIZE / 4.f) / AEGetWindowHeight(),
				1.f, planet_vector[i].shuttle_timer * static_cast<f32>(1.5) / planet_vector[i].shuttle_time_to_spawn, 0.f, 0.f);  // Speed up red color lerp by 50%
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
			AEVec2Set(&shuttle_count_pos_offset, FONT_ID_SIZE, FONT_ID_SIZE * 2);

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
		if (off_screen)
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
				AEClamp((dist_pos.x - (static_cast<f32>(dist.length() / 2 * FONT_ID_SIZE))) / AEGetWindowWidth(), -0.8f, 0.8f - (static_cast<f32>(dist.length() * FONT_ID_SIZE) / static_cast<f32>(AEGetWindowWidth()))),
				AEClamp((dist_pos.y + 50) / AEGetWindowHeight(), -0.75f, 0.8f - (static_cast<f32>(1.5 * FONT_ID_SIZE) / static_cast<f32>(AEGetWindowHeight()))),
				1.f, 1.f, 1.f, 1.f);
		}
		// DISTANCE INDICATOR-------------------------------------------------------------------------------------------------------------------------------------
	}

	for (size_t i{}; i < indicator_vector.size(); i++)
	{
		bool off_screen{ pow(AEVec2Distance(&planet_vector[i].position, &camera.position), 2) > (pow(AEGetWindowWidth() / 2, 2) + pow(AEGetWindowHeight() / 2, 2)) };
		if (off_screen)
		{
			AEGfxTextureSet(indicator_tex, 0, 0);
			AEGfxSetTransform(indicator_vector[i].transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);


			AEGfxTextureSet(outline_tex, 0, 0);
			AEGfxSetTransform(outline_vector[i].transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);


			AEGfxTextureSet(arrow_tex, 0, 0);
			AEGfxSetTransform(arrow_vector[i].transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	// Wave Complete font
	if (wave_interval_timer <= WAVE_INTERVAL_TIME)
	{
		// Place holder "Wave Complete"
		std::string str_wave_complete = "Wave " + std::to_string(wave_number) + " Completed";
		print_string = str_wave_complete.c_str();
		AEGfxPrint(font_id, const_cast<s8*>(print_string), static_cast<f32>(-0.2f), static_cast<f32>(0.85f + abs((WAVE_INTERVAL_TIME - (wave_interval_timer * 2)) / WAVE_INTERVAL_TIME) * 0.2f), 1.f, 1.f, 1.f, 1.f);
	}


	// Place holder "Shuttles Lost" counter
	std::string str_shuttle_lost = "Shuttles Lost: " + std::to_string(shuttle_destroyed);
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f - (str_shuttle_lost.length() / 2 * static_cast<f32>(FONT_ID_SIZE) / static_cast<f32>(AEGetWindowWidth())), -0.65f, 1.f, 1.f, 1.f, 1.f);

	// WAVE | SHUTTLES | PLANETS -------------------------------------------------------------------------------------------------------------------------------------
	std::string str_count; std::string str_headers;

	str_headers = "WAVE";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.25f, 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(wave_number);
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.25 + ((f32)str_headers.length() / 2 * static_cast<f32>(FONT_ID_SIZE) / static_cast<f32>(AEGetWindowWidth())), 0.86f, 1.f, 1.f, 1.f, 1.f);

	str_headers = "SHUTTLE";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f - ((f32)str_headers.length() / 2 * static_cast<f32>(FONT_ID_SIZE) / static_cast<f32>(AEGetWindowWidth())), 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(get_current_shuttles());
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f, 0.86f, 1.f, 1.f, 1.f, 1.f);

	str_headers = "PLANET";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.25f - ((f32)str_headers.length() * static_cast<f32>(FONT_ID_SIZE) / static_cast<f32>(AEGetWindowWidth())), 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(planet_count);
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.25f - ((f32)str_headers.length() / 2 * static_cast<f32>(FONT_ID_SIZE) / static_cast<f32>(AEGetWindowWidth())), 0.86f, 1.f, 1.f, 1.f, 1.f);
	// WAVE | SHUTTLES | PLANETS -------------------------------------------------------------------------------------------------------------------------------------
}

void WaveManager::free()
{
	indicator_vector.clear();
	outline_vector.clear();
	arrow_vector.clear();
	lose_menu::free();
}

void WaveManager::unload()
{
	AEGfxTextureUnload(indicator_tex);
	AEGfxTextureUnload(outline_tex);
	AEGfxTextureUnload(arrow_tex);
	lose_menu::unload();

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

int WaveManager::get_current_shuttles()
{
	int total_shuttles{};
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		total_shuttles += planet_vector[i].current_shuttle;
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

void WaveManager::add_indicator()
{
	WaveManager::Indicator new_indicator;

	new_indicator.size = 50.f;
	new_indicator.blinking_timer = 0.f;
	AEVec2Set(&new_indicator.position, 0.f, 0.f);

	AEMtx33Scale(&new_indicator.scale, new_indicator.size, new_indicator.size);
	AEMtx33Rot(&new_indicator.rotate, 0.f);
	AEMtx33Trans(&new_indicator.translate, 0.f, 0.f);
	AEMtx33Concat(&new_indicator.transform, &new_indicator.rotate, &new_indicator.scale);
	AEMtx33Concat(&new_indicator.transform, &new_indicator.translate, &new_indicator.transform);

	indicator_vector.push_back(new_indicator);

	WaveManager::Outline new_outline;

	new_outline.size = 60.f;
	AEVec2Set(&new_outline.position, 0.f, 0.f);

	AEMtx33Scale(&new_outline.scale, new_outline.size, new_outline.size);
	AEMtx33Rot(&new_outline.rotate, 0.f);
	AEMtx33Trans(&new_outline.translate, 0.f, 0.f);
	AEMtx33Concat(&new_outline.transform, &new_outline.rotate, &new_outline.scale);
	AEMtx33Concat(&new_outline.transform, &new_outline.translate, &new_outline.transform);

	outline_vector.push_back(new_outline);

	WaveManager::Arrow new_arrow;

	new_arrow.size = 25.f;
	new_arrow.direction = 0.f;
	AEVec2Set(&new_arrow.position, 0.f, 0.f);

	AEMtx33Scale(&new_arrow.scale, new_arrow.size, new_arrow.size);
	AEMtx33Rot(&new_arrow.rotate, 0.f);
	AEMtx33Trans(&new_arrow.translate, 0.f, 0.f);
	AEMtx33Concat(&new_arrow.transform, &new_arrow.rotate, &new_arrow.scale);
	AEMtx33Concat(&new_arrow.transform, &new_arrow.translate, &new_arrow.transform);

	arrow_vector.push_back(new_arrow);
}

void WaveManager::checkLoseCondition()
{
	if (shuttle_destroyed == 5)
	{
		gameLost = true;
	}
}