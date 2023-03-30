/******************************************************************************/
/*!
\file		WaveManager.cpp
\author		Tan Rui An Ryan
\par		email: ruianryan.t\@digipen.edu
\date		March 28, 2023
\brief		This file contains the behaviour for the WaveManager game object

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include "WaveManager.h"
#include "LoseMenu.h"
#include "GameStateList.h"
#include "Highscore.h"
#include "Easing.h"
#include <iostream>
#include <string>
#include <vector>

// Text
extern s8 font_id;
const char* print_string;

AEGfxTexture* indicator_tex;
AEGfxTexture* arrow_tex;
AEGfxTexture* lineTexture;

//extern AEGfxTexture* space_station_tex;

std::vector<WaveManager::Indicator> ss_indicator_vector;
std::vector<WaveManager::Indicator> indicator_vector;
std::vector<WaveManager::Arrow>	arrow_vector;

static f32 w_width{}, w_height{};

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void WaveManager::load()
{
	//indicator_tex = AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture.png");
	arrow_tex = AEGfxTextureLoad("Assets/MainLevel/ml_arrow.png");
	lineTexture = AEGfxTextureLoad("Assets/MainLevel/line.png");
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void WaveManager::init()
{
	w_width = static_cast<f32>(g_windowWidth);
	w_height = static_cast<f32>(g_windowHeight);

	std::cout << '\n' << "Wave Manager Initialized." << '\n' << '\n';

	wave_completed = false;
	wave_number = 1;
	wave_progress = 0;
	wave_interval_timer = -(WAVE_INTERVAL_TIME * 2); // Increased time for first wave tutorial

	planet_count = 0;
	planet_adding = true;

	shuttle_left_planet = 0;
	shuttle_destroyed = 0;
	shuttle_has_escaped = false;
	shuttle_has_collided = false;

	first_contact = false;
	capacity_full = false;
	station_tutorial = false;

	planet.spawn(rand() % (SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN) + SHUTTLE_SPAWN_MIN);
	planet.add_runway(planet_vector[0].position);
	add_indicator();
	planet_count++;

	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Wave " << wave_number << " has begun." << '\t';
	std::cout << "Planet Count: " << planet_count << '\n';
}

/******************************************************************************/
/*!
	Update Wave Manager
*/
/******************************************************************************/
void WaveManager::update(f32 frame_time)
{
	// Lose Condition--------------------------------------------------------
	if (shuttle_destroyed == LOSE_CONDITION)
	{
		next_state = GS_LOSEMENU;
	}
	
	
	// Tutorial - First occurrences------------------------------------------
	if (!first_contact && player.state == PLAYER_ORBIT) first_contact = true;
	if (!station_tutorial && !capacity_full && (player.current_capacity == player.max_capacity + player.capacity_level))
		capacity_full = station_tutorial = true;

	// Tutorial - First occurrences------------------------------------------


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
		planet.spawn(rand() % (SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN) + SHUTTLE_SPAWN_MIN);
		planet.add_runway(planet_vector[planet_count].position);
		add_indicator();
		planet_count++;
		planet_adding = true;

		std::cout << '\n' << "Wave " << wave_number << '\t' << "Added Planet." << '\t';
		std::cout << "Planet Count: " << planet_count << '\n';
	}
	// Add Planets at Intervals----------------------------------------------


	// Wave interval timer---------------------------------------------------
	if (wave_completed && !planet_adding)
	{
		wave_interval_timer += frame_time;
		if (AEInputCheckTriggered(AEVK_R))
		{
			wave_interval_timer = WAVE_INTERVAL_TIME * (1 + static_cast<f32>(planet_count / 2));
		}
	}
	// Wave interval timer---------------------------------------------------
	

	// Start of new wave-----------------------------------------------------
	if (wave_completed && wave_interval_timer >= (WAVE_INTERVAL_TIME * (1 + static_cast<f32>(planet_count / 2))) || AEInputCheckTriggered(AEVK_3))
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


	// Update logic for off-screen indicator-------------------------------
	for (size_t i{}; i < indicator_vector.size(); i++)
	{
		f32 cam_x{}, cam_y{};
		AEGfxGetCamPosition(&cam_x, &cam_y);

		// Update position of Planet image in the distance indicator
		// Clamp the image to the screen
		AEVec2Sub(&indicator_vector[i].position, &planet_vector[i].position, &camera.position);
		AEMtx33Trans(&indicator_vector[i].translate, AEClamp(indicator_vector[i].position.x * 0.5 + cam_x,
																-((w_width - indicator_vector[i].size) / 2) * 0.8f + cam_x,
																 ((w_width - indicator_vector[i].size) / 2) * 0.8f + cam_x),
													 AEClamp(indicator_vector[i].position.y * 0.5 + cam_y,
																-(w_height / 2) * 0.8f + cam_y,
																 (w_height / 2) * 0.7f + cam_y));
		AEMtx33Concat(&indicator_vector[i].transform, &indicator_vector[i].rotate, &indicator_vector[i].scale);
		AEMtx33Concat(&indicator_vector[i].transform, &indicator_vector[i].translate, &indicator_vector[i].transform);

		// Update the position and rotation of the arrow image ontop of the Planet in the distance indicator
		// Arrow to turn towards the direction of a planet at all times
		// Clamp the image to the screen
		arrow_vector[i].direction = static_cast<f32>(atan2(static_cast<double>(arrow_vector[i].position.y - planet_vector[i].position.y - planet_vector[i].size),
															static_cast<double>(arrow_vector[i].position.x - planet_vector[i].position.x - planet_vector[i].size)));
		arrow_vector[i].direction = AEWrap(arrow_vector[i].direction, -PI, PI); // Wraps the direction between -PI and PI
		AEMtx33Rot(&arrow_vector[i].rotate, arrow_vector[i].direction);			// Update the rotation accordingly
		AEVec2Sub(&arrow_vector[i].position, &camera.position, &planet_vector[i].position);
		AEMtx33Trans(&arrow_vector[i].translate, AEClamp(arrow_vector[i].position.x * -0.5 + cam_x,
															-((w_width - indicator_vector[i].size) / 2) * 0.8f + cam_x,
															 ((w_width - indicator_vector[i].size) / 2) * 0.8f + cam_x),
												 AEClamp(arrow_vector[i].position.y * -0.5 + cam_y,
															-(w_height / 2) * 0.8f + cam_y,
															 (w_height / 2) * 0.7f + cam_y));
		AEMtx33Concat(&arrow_vector[i].transform, &arrow_vector[i].rotate, &arrow_vector[i].scale);
		AEMtx33Concat(&arrow_vector[i].transform, &arrow_vector[i].translate, &arrow_vector[i].transform);

		arrow_vector[i].urgency = static_cast<f32>(planet_vector[i].shuttle_timer / planet_vector[i].shuttle_time_to_spawn);

		if (arrow_vector[i].blinking_timer >= (1 - arrow_vector[i].urgency))
		{
			arrow_vector[i].blinker = 1 - arrow_vector[i].blinker;
			arrow_vector[i].blinking_timer = 0;
		}

		if (arrow_vector[i].urgency > 0.5 && arrow_vector[i].urgency < 0.9)
		{
			arrow_vector[i].blinking_timer += frame_time;
		}
		else
		{
			arrow_vector[i].blinker = 1;
		}
	}
	// Update logic for off-screen indicator-------------------------------
}

/******************************************************************************/
/*!
	Draw Wave Manager (indicators)
*/
/******************************************************************************/
void WaveManager::draw(AEGfxVertexList* pMesh)
{
	for (size_t i{}; i < indicator_vector.size(); i++)
	{
		bool off_screen{ pow(AEVec2Distance(&planet_vector[i].position, &camera.position), 2) > (pow(w_width / 2, 2) + pow(w_height / 2, 2)) };
		if (off_screen && !planet_adding)
		{
			// Render planet image for distance indicator
			AEGfxTextureSet(indicator_vector[i].tex, 0, 0);
			AEGfxSetTransform(indicator_vector[i].transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// Render arrow image for distance indicator
			AEGfxTextureSet(arrow_tex, 0, 0);
			AEGfxSetTransform(arrow_vector[i].transform.m);
			// Flash arrow image to indicate shuttle leaving planet
			AEGfxSetTintColor(1.f, 1.f - arrow_vector[i].urgency, 1.f - arrow_vector[i].urgency, arrow_vector[i].blinker);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// Reset tint for other game objects
			AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);
		}
	}

	for (size_t i{}; i < planet_vector.size(); i++)
	{
		bool off_screen{ pow(AEVec2Distance(&planet_vector[i].position, &camera.position), 2) > (pow(w_width / 2, 2) + pow(w_height / 2, 2)) };
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
				timer_pos.x / (w_width / 2),
				(timer_pos.y - (FONT_ID_SIZE)) / (w_height / 2),
				2.5f, planet_vector[i].shuttle_timer * static_cast<f32>(1.5) / planet_vector[i].shuttle_time_to_spawn, 0.f, 0.f);  // Speed up red color lerp by 50%
			// SHUTTLE TIMERS-------------------------------------------------------------------------------------------------------------------------------------

			// SHUTTLE COUNT--------------------------------------------------------------------------------------------------------------------------------------
			// Get current shuttle count and display as Current / Max
			std::string shuttle_count = std::to_string(planet_vector[i].current_shuttle);
			shuttle_count = shuttle_count + " / " + std::to_string(planet_vector[i].max_shuttle);
			print_string = shuttle_count.c_str();

			// Calculate position to draw shuttle count (slightly below center of planet)
			AEVec2 shuttle_count_pos;
			AEVec2Sub(&shuttle_count_pos, &planet_vector[i].position, &camera.position);

			// Draw Current / Max using position calculated above
			AEGfxPrint(font_id, const_cast<s8*>(print_string),
				(shuttle_count_pos.x - (static_cast<f32>(shuttle_count.length()) / 2.f)) / (w_width / 2),
				(shuttle_count_pos.y - (4 * FONT_ID_SIZE)) / (w_height / 2),
				1.5f, 0.f, 0.f, 0.f);
			// SHUTTLE COUNT--------------------------------------------------------------------------------------------------------------------------------------
		}

		// DISTANCE INDICATOR-------------------------------------------------------------------------------------------------------------------------------------
		// Only draw distance indicator if the planet is off screen
		if (off_screen && !planet_adding)
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
				AEClamp((dist_pos.x - (static_cast<f32>(dist.length() / 2 * FONT_ID_SIZE))) / w_width,
							-0.8f,
							 0.8f - (static_cast<f32>(dist.length() * FONT_ID_SIZE) / w_width)),
				AEClamp((dist_pos.y + (indicator_vector[i].size + FONT_ID_SIZE / 2)) / w_height,
							-0.715f,
							 0.8f - (static_cast<f32>(FONT_ID_SIZE) / w_height)),
				1.f, 1.f, 1.f, 1.f);
		}
		// DISTANCE INDICATOR-------------------------------------------------------------------------------------------------------------------------------------
	}

	// Wave Complete font
	if (wave_completed)
	{
		// Place holder "Wave Complete"
		std::string str_wave_complete = "Wave " + std::to_string(wave_number) + " Completed";
		print_string = str_wave_complete.c_str();

		// Calculate alpha value for fade-in and fade-out effect using EaseInOutBack easing function
		float progress = static_cast<float>(wave_interval_timer / WAVE_INTERVAL_TIME);
		float easedProgress = EaseInOutBack(0, 1, progress);
		float alpha;

		if (progress < 0.5f)
		{
			alpha = easedProgress * 2;
		}
		else
		{
			alpha = 1.0f - (easedProgress - 0.5f) * 2;
		}

		// Calculate the position at the center of the screen
		float centerX = -static_cast<f32>(str_wave_complete.length()) / 2 * static_cast<f32>(FONT_ID_SIZE) / w_width;
		float centerY = 0.0f;
		// Render text with fade-in and fade-out effect
		AEGfxPrint(font_id, const_cast<s8*>(print_string), centerX, centerY, 1.f, 1.f, 1.f, alpha);


		// Next Wave timer display
		std::string next_wave = "Next Wave: " + std::to_string(static_cast<int>(abs(WAVE_INTERVAL_TIME - wave_interval_timer)));
		print_string = next_wave.c_str();
		AEGfxPrint(font_id, const_cast<s8*>(print_string), -((static_cast<f32>(next_wave.length() * FONT_ID_SIZE) / 2.f) / w_width), -(static_cast<f32>((4.5 * FONT_ID_SIZE) / w_height)), 1.f, 1.f, 1.f, 1.f);
		// Next Wave timer skip
		next_wave = "Skip   [R]";
		print_string = next_wave.c_str();
		AEGfxPrint(font_id, const_cast<s8*>(print_string), -((static_cast<f32>(next_wave.length() * FONT_ID_SIZE) / 2.f) / w_width), centerY - static_cast<f32>((6 * FONT_ID_SIZE) / w_height), 1.f, 1.f, 1.f, 1.f);
	}

	// Shuttles Lost counter
	std::string str_shuttle_lost = "Shuttles Lost: ";
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f - (str_shuttle_lost.length() / 1.5 * static_cast<f32>(FONT_ID_SIZE) / w_width), -0.9f, 1.5f, 1.f, 1.f, 1.f);

	str_shuttle_lost = std::to_string(shuttle_destroyed);
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f + (str_shuttle_lost.length() * 4.5 * static_cast<f32>(FONT_ID_SIZE) / w_width), -0.9f,
		1.5f + (0.2f * shuttle_destroyed),
		1.f,																						// Red
		static_cast<f32>((LOSE_CONDITION - shuttle_destroyed) / static_cast<f32>(LOSE_CONDITION)),	// Fade blue out the more shuttles destroyed
		static_cast<f32>((LOSE_CONDITION - shuttle_destroyed) / static_cast<f32>(LOSE_CONDITION)));	// Fade green out the more shuttles destroyed

	str_shuttle_lost = "/";
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f + (str_shuttle_lost.length() * 7 * static_cast<f32>(FONT_ID_SIZE) / w_width), -0.9f, 1.5f, 1.f, 1.f, 1.f);

	str_shuttle_lost = std::to_string(LOSE_CONDITION);
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f + (str_shuttle_lost.length() * 8 * static_cast<f32>(FONT_ID_SIZE) / w_width), -0.9f, 2.5f, 1.f, 0.f, 0.f);

	// WAVE | SHUTTLES | PLANETS -------------------------------------------------------------------------------------------------------------------------------------
	std::string str_count; std::string str_headers;

	str_headers = "WAVE";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.2f - (static_cast<f32>(str_headers.length()) * 1.5 * static_cast<f32>(FONT_ID_SIZE) / w_width), 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(wave_number);
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.2f - (static_cast<f32>(str_headers.length()) * 1.2 * static_cast<f32>(FONT_ID_SIZE) / w_width), 0.86f, 1.f, 1.f, 1.f, 1.f);

	str_headers = "SHUTTLE";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f - (static_cast<f32>(str_headers.length()) / 2 * static_cast<f32>(FONT_ID_SIZE) / w_width), 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(get_current_shuttles());
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -(static_cast<f32>(str_headers.length()) / 5 * static_cast<f32>(FONT_ID_SIZE) / w_width), 0.86f, 1.f, 1.f, 1.f, 1.f);

	str_headers = "PLANET";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.2f, 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(planet_count);
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.2f + (static_cast<f32>(str_headers.length()) / 3 * static_cast<f32>(FONT_ID_SIZE) / w_width), 0.86f, 1.f, 1.f, 1.f, 1.f);
	// WAVE | SHUTTLES | PLANETS -------------------------------------------------------------------------------------------------------------------------------------
}

/******************************************************************************/
/*!
	Clean Object Instanes
*/
/******************************************************************************/
void WaveManager::free()
{
	//ss_indicator_vector.clear();
	indicator_vector.clear();
	arrow_vector.clear();
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void WaveManager::unload()
{
	
	//AEGfxTextureUnload(indicator_tex);
	AEGfxTextureUnload(arrow_tex);
	AEGfxTextureUnload(lineTexture);
}

/******************************************************************************/
/*!
	Additional Functions
*/
/******************************************************************************/
// Return number of shuttles
int WaveManager::get_total_shuttles()
{
	int total_shuttles{};
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		total_shuttles += planet_vector[i].max_shuttle;
	}
	return total_shuttles;
}

// Return number of shuttles
int WaveManager::get_current_shuttles() const
{
	int total_shuttles{};
	for (size_t i{}; i < planet_vector.size(); i++)
	{
		total_shuttles += planet_vector[i].current_shuttle;
	}
	return total_shuttles;
}

// Check if there are still shuttles
bool WaveManager::no_more_shuttles()
{
	bool check = false;
	for (size_t i{}; i < shuttle_vector.size(); i++)
	{
		check = (shuttle_vector[i].active) ? false : true;
	}
	return check;
}

//void WaveManager::add_ss_indicator()
//{
//	WaveManager::Indicator new_indicator;
//
//	new_indicator.size = 60.f;
//	AEVec2Set(&new_indicator.position, 0.f, 0.f);
//
//	AEMtx33Scale(&new_indicator.scale, new_indicator.size, new_indicator.size);
//	AEMtx33Rot(&new_indicator.rotate, 0.f);
//	AEMtx33Trans(&new_indicator.translate, 0.f, 0.f);
//	AEMtx33Concat(&new_indicator.transform, &new_indicator.rotate, &new_indicator.scale);
//	AEMtx33Concat(&new_indicator.transform, &new_indicator.translate, &new_indicator.transform);
//
//	new_indicator.tex = space_station_tex;
//
//	ss_indicator_vector.push_back(new_indicator);
//}

// Add distance indicator
void WaveManager::add_indicator()
{
	WaveManager::Indicator new_indicator;

	new_indicator.size = 60.f;
	AEVec2Set(&new_indicator.position, 0.f, 0.f);

	AEMtx33Scale(&new_indicator.scale, new_indicator.size, new_indicator.size);
	AEMtx33Rot(&new_indicator.rotate, 0.f);
	AEMtx33Trans(&new_indicator.translate, 0.f, 0.f);
	AEMtx33Concat(&new_indicator.transform, &new_indicator.rotate, &new_indicator.scale);
	AEMtx33Concat(&new_indicator.transform, &new_indicator.translate, &new_indicator.transform);

	new_indicator.tex = planet_textures[planet_vector[planet_count].texture_index];

	indicator_vector.push_back(new_indicator);


	WaveManager::Arrow new_arrow;

	new_arrow.blinker = 1.f;
	new_arrow.blinking_timer = 0.f;
	new_arrow.urgency = 0.f;
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