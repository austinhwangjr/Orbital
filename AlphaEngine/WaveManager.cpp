/******************************************************************************/
/*!
\file		WaveManager.cpp
\author		Tan Rui An Ryan (100%)
\par		email: ruianryan.t\@digipen.edu
\date		March 28, 2023
\brief		This file contains the behaviour for the WaveManager game object

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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
#include"Data.h"

//Variables
static int PLANET_MAX;				// Maximum number of planets
static int SHUTTLE_MAX;				// Maximum number of shuttles per planet
static int SHUTTLE_SPAWN_MAX;		// Maximum number of shuttles a planet can spawn with
static int SHUTTLE_SPAWN_MIN;		// Minimum number of shuttles a planet can spawn with
static int SHUTTLE_INCREASE_AMOUNT;	// Number of shuttles added after a wave
static f32 WAVE_INTERVAL_TIME;		// Down time between waves
static int WAVE_ADD_PLANET;			// Every nth wave planets will be added on
int LOSE_CONDITION;					// Number of shuttles allowed to be destroyed before failure

// Text
extern s8 font_id;
const char* print_string;

AEGfxTexture* indicator_tex;
AEGfxTexture* ishuttle_tex;
AEGfxTexture* lineTexture;

// IMPORT DATA VECTOR
std::map<std::string, f32> 	WaveManagerDataMap;
std::vector<Data> 			WaveManagerData;

//extern AEGfxTexture* space_station_tex;

std::vector<WaveManager::Indicator> ss_indicator_vector;
std::vector<WaveManager::Indicator> indicator_vector;
std::vector<WaveManager::iShuttle>	ishuttle_vector;

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
void WaveManager::load()
{
	//indicator_tex = AEGfxTextureLoad("Assets/MainLevel/ml_PlanetTexture.png");
	ishuttle_tex = AEGfxTextureLoad("Assets/MainLevel/ml_Shuttle.png");
	lineTexture = AEGfxTextureLoad("Assets/MainLevel/line.png");
	ImportDataFromFile("Assets/GameObjectData/WaveManagerData.txt", WaveManagerData, WaveManagerDataMap);
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void WaveManager::init()
{
	PLANET_MAX				= static_cast<int>(WaveManagerDataMap["Maximum_Planet_To_Be_Spawn"]);
	SHUTTLE_MAX				= static_cast<int>(WaveManagerDataMap["Maximum_Shuttle_Per_Planet"]);
	SHUTTLE_SPAWN_MAX		= static_cast<int>(WaveManagerDataMap["Maximum_Shuttle_Spawn"]);
	SHUTTLE_SPAWN_MIN		= static_cast<int>(WaveManagerDataMap["Minimum_Shuttle_Spawn"]);
	SHUTTLE_INCREASE_AMOUNT = static_cast<int>(WaveManagerDataMap["Shuttle_Increase_Amount"]);
	WAVE_INTERVAL_TIME		= WaveManagerDataMap["Wave_Interval_Time"];
	WAVE_ADD_PLANET			= static_cast<int>(WaveManagerDataMap["Wave_Add_Planet"]);
	LOSE_CONDITION			= static_cast<int>(WaveManagerDataMap["Lose_Condition"]);

	std::cout << '\n' << "Wave Manager Initialized." << '\n' << '\n';

	wave_completed			= false;
	wave_number				= static_cast<int>(WaveManagerDataMap["Starting_Wave_Number"]);
	wave_progress			= static_cast<int>(WaveManagerDataMap["Wave_Progress"]);
	wave_interval_timer		= WAVE_INTERVAL_TIME * 3; // Increased time for first wave tutorial

	planet_count			= static_cast<int>(WaveManagerDataMap["Planet_Count"]);
	planet_adding			= true;

	shuttle_left_planet		= static_cast<int>(WaveManagerDataMap["Shuttle_Left_Planet"]);
	shuttle_destroyed		= static_cast<int>(WaveManagerDataMap["Shuttle_Destroyed"]);
	shuttle_has_escaped		= false;
	shuttle_has_collided	= false;

	first_contact			= false;
	capacity_full			= false;
	station_tutorial		= false;

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
void WaveManager::update()
{
	// Lose Condition--------------------------------------------------------
	if (shuttle_destroyed == LOSE_CONDITION)
	{
		player_score = player.score;
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
	// Press 2 to add a Planet
	if (planet_count < PLANET_MAX)
	{
		if ((planet_count * WAVE_ADD_PLANET - 1) == wave_number && wave_completed || AEInputCheckTriggered(AEVK_2))
		{
			planet.spawn(rand() % (SHUTTLE_SPAWN_MAX - SHUTTLE_SPAWN_MIN) + SHUTTLE_SPAWN_MIN);
			planet.add_runway(planet_vector[planet_count].position);
			add_indicator();
			planet_count++;
			planet_adding = true;

			std::cout << '\n' << "Wave " << wave_number << '\t' << "Added Planet." << '\t';
			std::cout << "Planet Count: " << planet_count << '\n';
		}
	}
	// Add Planets at Intervals----------------------------------------------


	// Wave interval timer---------------------------------------------------
	if (wave_completed && !planet_adding)
	{
		wave_interval_timer -= g_dt;
		if (AEInputCheckTriggered(AEVK_R))
		{
			wave_interval_timer = 0.f;
		}
	}
	// Wave interval timer---------------------------------------------------
	

	// Start of new wave-----------------------------------------------------
	// Press 3 to Skip a Wave
	if (wave_completed && wave_interval_timer <= 0.f || AEInputCheckTriggered(AEVK_3))
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

		shuttle_left_planet = 0;		// Reset number of shuttles successfully escaped during wave
		shuttle_destroyed = 0;			// Reset number of shuttles that have been destroyed
		wave_completed = false;			// Reset wave complete flag
		wave_progress = 0;				// Reset wave progress to 0
		wave_number++;					// Increment wave number
		wave_interval_timer = WAVE_INTERVAL_TIME * (1 + static_cast<f32>(planet_count) / 2.f);	// Reset wave interval timer, increased more planets there are
		wave_interval_timer *= (3 == wave_number) ? 1.5f : 1.f;									// Increased time for second planet tutorial

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
		// Update position of Planet image in the distance indicator
		// Clamp the image to the screen
		AEVec2Sub(&indicator_vector[i].position, &planet_vector[i].position, &camera.position);
		AEMtx33Trans(&indicator_vector[i].translate, AEClamp(indicator_vector[i].position.x * 0.5f + g_camPos.x,
																-((g_windowWidth - indicator_vector[i].size) / 2) * 0.8f + g_camPos.x,
																 ((g_windowWidth - indicator_vector[i].size) / 2) * 0.8f + g_camPos.x),
													 AEClamp(indicator_vector[i].position.y * 0.5f + g_camPos.y,
																-(g_windowHeight / 2) * 0.8f + g_camPos.y,
																 (g_windowHeight / 2) * 0.7f + g_camPos.y));
		AEMtx33Concat(&indicator_vector[i].transform, &indicator_vector[i].rotate, &indicator_vector[i].scale);
		AEMtx33Concat(&indicator_vector[i].transform, &indicator_vector[i].translate, &indicator_vector[i].transform);

		
		AEMtx33Rot(&ishuttle_vector[i].rotate, planet_vector[i].shuttle_direction + PI / 2.f);// Update the rotation accordingly
		AEVec2Sub(&ishuttle_vector[i].position, &camera.position, &planet_vector[i].position);
		AEMtx33Trans(&ishuttle_vector[i].translate, AEClamp(ishuttle_vector[i].position.x * -0.5f + g_camPos.x,
															-((g_windowWidth - indicator_vector[i].size) / 2.f) * 0.8f + g_camPos.x,
															 ((g_windowWidth - indicator_vector[i].size) / 2.f) * 0.8f + g_camPos.x),
												 AEClamp(ishuttle_vector[i].position.y * -0.5f + g_camPos.y,
															-(g_windowHeight / 2.f) * 0.8f + g_camPos.y,
															 (g_windowHeight / 2.f) * 0.7f + g_camPos.y));
		AEMtx33Concat(&ishuttle_vector[i].transform, &ishuttle_vector[i].rotate, &ishuttle_vector[i].scale);
		AEMtx33Concat(&ishuttle_vector[i].transform, &ishuttle_vector[i].translate, &ishuttle_vector[i].transform);

		ishuttle_vector[i].urgency = planet_vector[i].shuttle_timer / planet_vector[i].shuttle_time_to_spawn;

		if (ishuttle_vector[i].blinking_timer >= (1 - ishuttle_vector[i].urgency))
		{
			ishuttle_vector[i].blinker = 1 - ishuttle_vector[i].blinker;
			ishuttle_vector[i].blinking_timer = 0;
		}

		if (ishuttle_vector[i].urgency > 0.5 && ishuttle_vector[i].urgency < 0.9)
		{
			ishuttle_vector[i].blinking_timer += g_dt;
		}
		else
		{
			ishuttle_vector[i].blinker = 1;
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
		bool off_screen{ pow(AEVec2Distance(&planet_vector[i].position, &camera.position), 2) > (pow(g_windowWidth / 2, 2) + pow(g_windowHeight / 2, 2)) };
		if (off_screen && !planet_adding)
		{
			// Render planet image for distance indicator
			AEGfxTextureSet(indicator_vector[i].tex, 0, 0);
			AEGfxSetTransform(indicator_vector[i].transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// Render ishuttle image for distance indicator
			AEGfxTextureSet(ishuttle_tex, 0, 0);
			AEGfxSetTransform(ishuttle_vector[i].transform.m);
			// Flash ishuttle image to indicate shuttle leaving planet
			AEGfxSetTintColor(1.f, 1.f - ishuttle_vector[i].urgency, 1.f - ishuttle_vector[i].urgency, ishuttle_vector[i].blinker);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// Reset tint for other game objects
			AEGfxSetTintColor(1.f, 1.f, 1.f, 1.f);
		}
	}

	for (size_t i{}; i < planet_vector.size(); i++)
	{
		bool off_screen{ pow(AEVec2Distance(&planet_vector[i].position, &camera.position), 2) > (pow(g_windowWidth / 2, 2) + pow(g_windowHeight / 2, 2)) };
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
				timer_pos.x / (g_windowWidth / 2),
				(timer_pos.y - (FONT_ID_SIZE)) / (g_windowHeight / 2),
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
				(shuttle_count_pos.x - (static_cast<f32>(shuttle_count.length()) / 2.f)) / (g_windowWidth / 2),
				(shuttle_count_pos.y - (4 * FONT_ID_SIZE)) / (g_windowHeight / 2),
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
				AEClamp((dist_pos.x - (static_cast<f32>(dist.length() / 2 * FONT_ID_SIZE))) / g_windowWidth,
							-0.8f,
							 0.8f - (static_cast<f32>(dist.length() * FONT_ID_SIZE) / g_windowWidth)),
				AEClamp((dist_pos.y + (indicator_vector[i].size + FONT_ID_SIZE / 2)) / g_windowHeight,
							-0.715f,
							 0.8f - (static_cast<f32>(FONT_ID_SIZE) / g_windowHeight)),
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
		float centerX = -static_cast<f32>(str_wave_complete.length()) / 2 * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth;
		float centerY = 0.0f;
		// Render text with fade-in and fade-out effect
		AEGfxPrint(font_id, const_cast<s8*>(print_string), centerX, centerY, 1.f, 1.f, 1.f, alpha);


		// Next Wave timer display
		std::string next_wave = "Next Wave: " + std::to_string(static_cast<int>(wave_interval_timer));
		print_string = next_wave.c_str();
		AEGfxPrint(font_id, const_cast<s8*>(print_string), -((static_cast<f32>(next_wave.length() * FONT_ID_SIZE) / 2.f) / g_windowWidth), -(static_cast<f32>((4.5 * FONT_ID_SIZE) / g_windowHeight)), 1.f, 1.f, 1.f, 1.f);
		// Next Wave timer skip
		next_wave = "Skip   [R]";
		print_string = next_wave.c_str();
		AEGfxPrint(font_id, const_cast<s8*>(print_string), -((static_cast<f32>(next_wave.length() * FONT_ID_SIZE) / 2.f) / g_windowWidth), centerY - static_cast<f32>((6 * FONT_ID_SIZE) / g_windowHeight), 1.f, 1.f, 1.f, 1.f);
	}

	// Shuttles Lost counter
	std::string str_shuttle_lost = "Shuttles Lost: ";
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f - (str_shuttle_lost.length() / 1.5f * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), -0.9f, 1.5f, 1.f, 1.f, 1.f);

	str_shuttle_lost = std::to_string(shuttle_destroyed);
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f + (str_shuttle_lost.length() * 4.5f * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), -0.9f,
		1.5f + (0.2f * shuttle_destroyed),
		1.f,																						// Red
		static_cast<f32>((LOSE_CONDITION - shuttle_destroyed) / static_cast<f32>(LOSE_CONDITION)),	// Fade blue out the more shuttles destroyed
		static_cast<f32>((LOSE_CONDITION - shuttle_destroyed) / static_cast<f32>(LOSE_CONDITION)));	// Fade green out the more shuttles destroyed

	str_shuttle_lost = "/";
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f + (str_shuttle_lost.length() * 7 * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), -0.9f, 1.5f, 1.f, 1.f, 1.f);

	str_shuttle_lost = std::to_string(LOSE_CONDITION);
	print_string = str_shuttle_lost.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f + (str_shuttle_lost.length() * 8 * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), -0.9f, 2.5f, 1.f, 0.f, 0.f);

	// WAVE | SHUTTLES | PLANETS -------------------------------------------------------------------------------------------------------------------------------------
	std::string str_count; std::string str_headers;

	str_headers = "WAVE";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.2f - (static_cast<f32>(str_headers.length()) * 1.5f * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(wave_number);
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -0.2f - (static_cast<f32>(str_headers.length()) * 1.2f * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), 0.86f, 1.f, 1.f, 1.f, 1.f);

	str_headers = "SHUTTLE";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.f - (static_cast<f32>(str_headers.length()) / 2 * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(get_current_shuttles());
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), -(static_cast<f32>(str_headers.length()) / 5 * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), 0.86f, 1.f, 1.f, 1.f, 1.f);

	str_headers = "PLANET";
	print_string = str_headers.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.2f, 0.9f, 1.f, 1.f, 1.f, 1.f);

	str_count = std::to_string(planet_count);
	print_string = str_count.c_str();
	AEGfxPrint(font_id, const_cast<s8*>(print_string), 0.2f + (static_cast<f32>(str_headers.length()) / 3 * static_cast<f32>(FONT_ID_SIZE) / g_windowWidth), 0.86f, 1.f, 1.f, 1.f, 1.f);
	// WAVE | SHUTTLES | PLANETS -------------------------------------------------------------------------------------------------------------------------------------
}

/******************************************************************************/
/*!
	Clean Object Instanes
*/
/******************************************************************************/
void WaveManager::free()
{
	indicator_vector.clear();
	ishuttle_vector.clear();

	if (next_state != GS_RESTART) {
		WaveManagerData.clear();
		WaveManagerDataMap.clear();
	}
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void WaveManager::unload()
{
	
	//AEGfxTextureUnload(indicator_tex);
	AEGfxTextureUnload(ishuttle_tex);
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

// Add distance indicator
void WaveManager::add_indicator()
{
	WaveManager::Indicator new_indicator;

	new_indicator.size = WaveManagerDataMap["Indicator_Size"];
	AEVec2Set(&new_indicator.position, 0.f, 0.f);

	AEMtx33Scale(&new_indicator.scale, new_indicator.size, new_indicator.size);
	AEMtx33Rot(&new_indicator.rotate, 0.f);
	AEMtx33Trans(&new_indicator.translate, 0.f, 0.f);
	AEMtx33Concat(&new_indicator.transform, &new_indicator.rotate, &new_indicator.scale);
	AEMtx33Concat(&new_indicator.transform, &new_indicator.translate, &new_indicator.transform);

	new_indicator.tex = planet_textures[planet_vector[planet_count].texture_index];

	indicator_vector.push_back(new_indicator);


	WaveManager::iShuttle new_ishuttle;

	new_ishuttle.blinker				= WaveManagerDataMap["Arrow_Blinker"];
	new_ishuttle.blinking_timer			= WaveManagerDataMap["Blinker_Timer"];
	new_ishuttle.urgency				= WaveManagerDataMap["Blinker_Urgency"];
	new_ishuttle.size					= WaveManagerDataMap["Arrow_Size"];
	new_ishuttle.direction				= WaveManagerDataMap["Arrow_Direction"];
	AEVec2Set(&new_ishuttle.position, 0.f, 0.f);

	AEMtx33Scale(&new_ishuttle.scale, new_ishuttle.size, new_ishuttle.size);
	AEMtx33Rot(&new_ishuttle.rotate, 0.f);
	AEMtx33Trans(&new_ishuttle.translate, 0.f, 0.f);
	AEMtx33Concat(&new_ishuttle.transform, &new_ishuttle.rotate, &new_ishuttle.scale);
	AEMtx33Concat(&new_ishuttle.transform, &new_ishuttle.translate, &new_ishuttle.transform);

	ishuttle_vector.push_back(new_ishuttle);
}