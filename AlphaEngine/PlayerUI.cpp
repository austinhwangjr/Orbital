#include "AEEngine.h"
#include "PlayerUI.h"
#include "Global.h"
#include "Easing.h"
#include <iostream>
#include <vector>
#include <string>

// Textures
extern AEGfxTexture* player_tex;
AEGfxTexture* shop_icon_tex;
AEGfxTexture* shop_open_tex;
AEGfxTexture* shop_close_tex;
AEGfxTexture* space_station_tex;
AEGfxTexture* shop_background_tex;
AEGfxTexture* tutorial_open_tex;
AEGfxTexture* tutorial_background_tex;
AEGfxTexture* upgrade_level_hollow_tex;
AEGfxTexture* upgrade_level_solid_tex;
AEGfxTexture* player_hud_tex;

// Upgrade preview textures
AEGfxTexture* speed_hover_tex;
AEGfxTexture* capacity_hover_tex;
AEGfxTexture* strength_hover_tex;

// Shop button textures
AEGfxTexture* mov_speed_button_tex;
AEGfxTexture* capacity_button_tex;
AEGfxTexture* strength_button_tex;

// Variables for general UI
std::string		score, credits, capacity;
extern s8		font_id;

// Variables for shop
extern s8		font_id_shop;
std::string		shop_option_name;
std::string		shop_upgrade_cost;

// Variables for camera
f32 cam_x, cam_y;

// Mouse coordinates
AEVec2 mouse_pos_world;

// Vector of buttons
std::vector<ShopOption> button_vector;

void PlayerUI::load()
{
	// Load textures
	shop_icon_tex				= AEGfxTextureLoad("Assets/MainLevel/ml_YellowButtonBackground.png");
	shop_open_tex				= AEGfxTextureLoad("Assets/MainLevel/ml_ShopOpenButton.png");
	shop_close_tex				= AEGfxTextureLoad("Assets/MainLevel/ml_ShopCloseButton.png");
	space_station_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_SpaceStation.png");
	shop_background_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_ShopBackground.png");
	tutorial_open_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_TutorialBackground.png");
	tutorial_background_tex		= AEGfxTextureLoad("Assets/MainLevel/ml_TutorialBackground.png");
	upgrade_level_hollow_tex	= AEGfxTextureLoad("Assets/MainLevel/ml_UpgradeLevelHollow.png");
	upgrade_level_solid_tex		= AEGfxTextureLoad("Assets/MainLevel/ml_UpgradeLevelSolid.png");
	player_hud_tex				= AEGfxTextureLoad("Assets/MainLevel/ml_HeadsUpDisplay.png");

	// Upgrade preview textures
	speed_hover_tex				= AEGfxTextureLoad("Assets/MainLevel/ml_MovSpeedUpgradePreview.png");
	capacity_hover_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_CapacityUpgradePreview.png");
	strength_hover_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_BeamStrengthUpgradePreview.png");

	// Shop button textures
	mov_speed_button_tex		= AEGfxTextureLoad("Assets/MainLevel/ml_MovSpeedUpgradeButton.png");
	capacity_button_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_CapacityUpgradeButton.png");
	strength_button_tex			= AEGfxTextureLoad("Assets/MainLevel/ml_BeamStrengthUpgradeButton.png");
}

void PlayerUI::init()
{
	// Player UI
	player_hud_width	= static_cast<f32>(AEGetWindowWidth());
	player_hud_height	= static_cast<f32>(AEGetWindowHeight());

	// Add buttons to vector
	ShopOption shop_open_button{};
	shop_open_button.width	= 70.f;
	shop_open_button.height = 70.f;
	shop_open_button.button_type = SHOP_OPEN;
	button_vector.push_back(shop_open_button);

	for (int i = 0; i < UPGRADE_COUNT; ++i) {
		// Populate button vector
		ShopOption player_upgrade{};
		player_upgrade.width = 160.f;
		player_upgrade.height = 80.f;
		player_upgrade.button_type = MOVEMENT_SPEED + i;

		// Max upgrade levels for each upgrade type
		int count{};
		switch (player_upgrade.button_type) {
		case MOVEMENT_SPEED:
			count = MAX_MOV_SPEED_LVL;
			break;
		case CAPACITY:
			count = MAX_CAPACITY_LVL;
			break;
		case TRACTOR_BEAM_STRENGTH:
			count = MAX_BEAM_STRENGTH_LVL;
			break;
		case SPACE_STATION:
			count = MAX_SPACE_STATION_CNT;
			break;
		}

		// Add indicators (upgrade levels)
		for (int j = 0; j < count; ++j) {
			UpgradeLevelIndicator indicator{};
			indicator.width = 32.0f;
			indicator.height = 16.0f;
			player_upgrade.indicator_vector.push_back(indicator);
		}

		button_vector.push_back(player_upgrade);
	}

	ShopOption tutorial_button{};
	tutorial_button.width = 70.f;
	tutorial_button.height = 70.f;
	tutorial_button.button_type = TUTORIAL_OPEN;
	button_vector.push_back(tutorial_button);

	// Not in placing mode initially
	placing_drone			= false;
	placing_station			= false;
	drone_placement_flag	= false;
	station_placement_flag	= false;

	// Shop is closed initially
	shop_triggered			= false;
	shop_transition			= false;
	upgrade_preview_display = false;

	// Shop background
	shop_bg_width			= static_cast<f32>(AEGetWindowWidth()) * 0.85f;
	shop_bg_height			= static_cast<f32>(AEGetWindowHeight()) * 0.85f;

	// Tutorial is opened initially
	tutorial_triggered = true;
	tutorial_transition = false;

	// Tutorial background
	tutorial_bg_width = static_cast<f32>(12 * FONT_ID_SIZE);
	tutorial_bg_height = static_cast<f32>(10 * FONT_ID_SIZE);

	// Set the offset of the shop
	shop_offset				= static_cast<f32>(AEGetWindowWidth());

	// Set the offset of the tutorial
	tutorial_offset = 0;

	// Icons in shop
	icon_size				= 20.f;

	// Set the size of the upgrade preview
	upgrade_preview_size	= 400.f;

	// Timer for shop transition
	shop_trans_timer = 0.f;
	shop_trans_duration = 1.f;

	// Timer for tutorial transition
	tutorial_trans_timer = 0.f;
	tutorial_trans_duration = 1.f;
}

void PlayerUI::update(f64 frame_time, Player& player)
{
	// Get mouse coordinates (world)
	s32 mouse_x_screen, mouse_y_screen;
	AEInputGetCursorPosition(&mouse_x_screen, &mouse_y_screen);

	mouse_pos_world.x = cam_x + mouse_x_screen - static_cast<f32>(AEGetWindowWidth() / 2);
	mouse_pos_world.y = cam_y + static_cast<f32>(AEGetWindowHeight() / 2.f) - mouse_y_screen;

	// Shop transitions
	if (shop_triggered) {
		// Start transition
		if (shop_transition) {
			// Adjust shop offset
			shop_trans_timer += frame_time;
			shop_offset = EaseOutExpo(static_cast<f32>(AEGetWindowWidth()), 0, shop_trans_timer / shop_trans_duration);

			// Stop transition once duration is met
			if (shop_trans_timer >= shop_trans_duration) {
				shop_trans_timer = 0.f;
				shop_transition = false;
			}
		}

		// Transition done
		else
			shop_open(player);
	}
	else {
		// Start transition
		if (shop_transition) {
			// Adjust shop offset
			shop_trans_timer += static_cast<f32>(frame_time);
			shop_offset = EaseInOutBack(0, static_cast<f32>(AEGetWindowWidth()), shop_trans_timer / shop_trans_duration);

			// Stop transition once duration is met
			if (shop_trans_timer >= shop_trans_duration) {
				shop_trans_timer = 0.f;
				shop_transition = false;
			}
		}

		// Transition done
		else
			shop_closed();
	}

	// Tutorial transitions
	if (tutorial_triggered) {
		// Start transition
		if (tutorial_transition) {
			// Adjust shop offset
			tutorial_trans_timer += frame_time;
			tutorial_offset = EaseOutExpo(-tutorial_bg_width, 0, tutorial_trans_timer / tutorial_trans_duration);

			// Stop transition once duration is met
			if (tutorial_trans_timer >= tutorial_trans_duration) {
				tutorial_trans_timer = 0.f;
				tutorial_transition = false;
			}
		}

		// Transition done
		else
			tutorial_open();
	}
	else {
		// Start transition
		if (tutorial_transition) {
			// Adjust tutorial offset
			tutorial_trans_timer += static_cast<f32>(frame_time);
			tutorial_offset = EaseInOutBack(0, -tutorial_bg_width, tutorial_trans_timer / tutorial_trans_duration);

			// Stop transition once duration is met
			if (tutorial_trans_timer >= tutorial_trans_duration) {
				tutorial_trans_timer = 0.f;
				tutorial_transition = false;
			}
		}

		// Transition done
		else
			tutorial_closed();
	}

	// =================
	// Update positions
	// =================

	AEGfxGetCamPosition(&cam_x, &cam_y);
	// Player HUD to follow camera
	player_hud_position.x = cam_x;
	player_hud_position.y = cam_y;

	// Shop background
	shop_bg_position.x = cam_x + shop_offset;
	shop_bg_position.y = cam_y;

	// Tutorial background
	//tutorial_bg_position.x = cam_x - static_cast<f32>(AEGetWindowWidth()) / 2.f - button_vector[6].width / 2.f - tutorial_offset;
	tutorial_bg_position.x = cam_x - static_cast<f32>(AEGetWindowWidth()) / 2.f + tutorial_bg_width / 2 + tutorial_offset;
	tutorial_bg_position.y = cam_y + static_cast<f32>(AEGetWindowHeight()) / 2.f - button_vector[6].height * 3.f;

	// Button to open shop
	button_vector[0].position.x = cam_x + static_cast<f32>(AEGetWindowWidth()) / 2.f - button_vector[0].width / 2.f;
	button_vector[0].position.y = cam_y + static_cast<f32>(AEGetWindowHeight()) / 2.f - button_vector[0].height * 2.5f;

	// Button to open tutorial
	button_vector[6].position.x = cam_x - static_cast<f32>(AEGetWindowWidth()) / 2.f + button_vector[6].width / 2 + tutorial_bg_width + tutorial_offset;
	button_vector[6].position.y = cam_y + static_cast<f32>(AEGetWindowHeight()) / 2.f - button_vector[6].height * 2.5f;

	// Upgrade preview on left half of shop
	upgrade_preview_position.x = cam_x - shop_bg_width / 4.f + shop_offset;
	upgrade_preview_position.y = cam_y + shop_bg_width / 8.f;

	// Movement speed, capacity and beam strength buttons on the left half
	for (int i = 1; i < 4; ++i) {
		ShopOption& button = button_vector[i];
		button.position.x = cam_x - shop_bg_width / 4.f - button.width * 1.3f * (2 - i) + shop_offset;
		//button.position.y = button_vector[0].position.y - button.height * 4.f;
		button.position.y = cam_y - shop_bg_height / 8.f;

		// Level indicators for upgrades
		for (int j = 0; j < button.indicator_vector.size(); ++j) {
			UpgradeLevelIndicator& indicator = button.indicator_vector[j];
			indicator.position.x = button.position.x - indicator.width * (2 - j);
			indicator.position.y = button.position.y - button.height;
		}
	}

	// Drone and space station buttons on the right half
	for (int i = 4; i < 6; ++i) {
		ShopOption& button = button_vector[i];
		button.position.x = cam_x + shop_bg_width / 4.f + shop_offset;
		button.position.y = button_vector[0].position.y - (i - 4) * button.height * 4.f;

		// Level indicators for upgrades
		for (int j = 0; j < button.indicator_vector.size(); ++j) {
			UpgradeLevelIndicator& indicator = button.indicator_vector[j];
			indicator.position.x = button.position.x - indicator.width * (2 - j);
			indicator.position.y = button.position.y - button.height;
		}
	}

	// =======================
	// Calculate the matrices
	// =======================

	AEMtx33 scale, rot, trans;

	// Player HUD
	AEMtx33Scale(&scale, player_hud_width, player_hud_height);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, player_hud_position.x, player_hud_position.y);
	AEMtx33Concat(&player_hud_transform, &rot, &scale);
	AEMtx33Concat(&player_hud_transform, &trans, &player_hud_transform);

	// Shop background
	AEMtx33Scale(&scale, shop_bg_width, shop_bg_height);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, shop_bg_position.x, shop_bg_position.y);
	AEMtx33Concat(&shop_bg_transform, &rot, &scale);
	AEMtx33Concat(&shop_bg_transform, &trans, &shop_bg_transform);

	// Tutorial background
	AEMtx33Scale(&scale, tutorial_bg_width, tutorial_bg_height);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, tutorial_bg_position.x, tutorial_bg_position.y);
	AEMtx33Concat(&tutorial_bg_transform, &rot, &scale);
	AEMtx33Concat(&tutorial_bg_transform, &trans, &tutorial_bg_transform);

	// Upgrade preview
	AEMtx33Scale(&scale, upgrade_preview_size, upgrade_preview_size);
	AEMtx33Rot(&rot, PI);
	AEMtx33Trans(&trans, upgrade_preview_position.x, upgrade_preview_position.y);
	AEMtx33Concat(&upgrade_preview_transform, &rot, &scale);
	AEMtx33Concat(&upgrade_preview_transform, &trans, &upgrade_preview_transform);
	
	// Shop buttons
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];
		AEMtx33Scale(&scale, button.width, button.height);
		AEMtx33Rot(&rot, PI);
		AEMtx33Trans(&trans, button.position.x, button.position.y);
		AEMtx33Concat(&button.transform, &rot, &scale);
		AEMtx33Concat(&button.transform, &trans, &button.transform);

		// Level indicators for upgrades
		if (i > 0) {
			for (int j = 0; j < button.indicator_vector.size(); ++j) {
				UpgradeLevelIndicator& indicator = button.indicator_vector[j];
				AEMtx33Scale(&scale, indicator.width, indicator.height);
				AEMtx33Rot(&rot, PI);
				AEMtx33Trans(&trans, indicator.position.x, indicator.position.y);
				AEMtx33Concat(&indicator.transform, &rot, &scale);
				AEMtx33Concat(&indicator.transform, &trans, &indicator.transform);
			}
		}

		// Shop icons (drone/space station)
		if (button_vector[i].button_type == CREATE_DRONE) {			
			AEMtx33Scale(&scale, icon_size * 4, icon_size * 4);
			AEMtx33Rot(&rot, PI);
			AEMtx33Trans(&trans, button.position.x, button.position.y);
			AEMtx33Concat(&drone_icon_transform, &rot, &scale);
			AEMtx33Concat(&drone_icon_transform, &trans, &drone_icon_transform);
		}
		else if (button_vector[i].button_type == SPACE_STATION) {
			AEMtx33Scale(&scale, icon_size * 4, icon_size * 4);
			AEMtx33Rot(&rot, PI);
			AEMtx33Trans(&trans, button.position.x, button.position.y);
			AEMtx33Concat(&space_station_icon_transform, &rot, &scale);
			AEMtx33Concat(&space_station_icon_transform, &trans, &space_station_icon_transform);
		}
	}
}

/******************************************************************************/
/*!
	Draw Player UI
*/
/******************************************************************************/
void PlayerUI::draw(AEGfxVertexList* pMesh, Player player)
{
	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);

	// Player HUD
	AEGfxTextureSet(player_hud_tex, 0, 0);
	AEGfxSetTransform(player_hud_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Print score
	score = "Score: " + std::to_string(player.score);
	AEGfxPrint(font_id, const_cast<s8*>(score.c_str()), -0.85f, 0.75f, 1.f, 1.f, 1.f, 1.f);
	
	// Print credits
	credits = "Credits: " + std::to_string(player.credits);
	AEGfxPrint(font_id, const_cast<s8*>(credits.c_str()), 0.5f, 0.75f, 1.f, 1.f, 1.f, 1.f);

	// Print capacity
	capacity = "Capacity: " + std::to_string(player.current_capacity) + " / " + std::to_string(player.max_capacity + player.capacity_level);
	AEGfxPrint(font_id, const_cast<s8*>(capacity.c_str()), -0.25f, -0.75f, 1.f, 1.f, 1.f, 1.f);

	// Shop background
	AEGfxTextureSet(shop_background_tex, 0, 0);
	AEGfxSetTransparency(0.5f);
	AEGfxSetTransform(shop_bg_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Tutorial background
	AEGfxTextureSet(tutorial_background_tex, 0, 0);
	AEGfxSetTransparency(0.5f);
	AEGfxSetTransform(tutorial_bg_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Reset transparency
	AEGfxSetTransparency(1.f);

	// Upgrade preview
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];
		if (hover_over_button(button)) {
			if (button.button_type == MOVEMENT_SPEED) {
				AEGfxTextureSet(speed_hover_tex, 0, 0);
				upgrade_preview_display = true;
				break;
			}

			else if (button.button_type == CAPACITY) {
				AEGfxTextureSet(capacity_hover_tex, 0, 0);
				upgrade_preview_display = true;
				break;
			}

			else if (button.button_type == TRACTOR_BEAM_STRENGTH) {
				AEGfxTextureSet(strength_hover_tex, 0, 0);
				upgrade_preview_display = true;
				break;
			}
			
		}
		else
			AEGfxTextureSet(player_tex, 0, 0);
	}
	
	AEGfxSetTransform(upgrade_preview_transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Shop buttons
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];

		if (i == 0) {
			if (shop_triggered)
				AEGfxTextureSet(shop_close_tex, 0, 0);
			else
				AEGfxTextureSet(shop_open_tex, 0, 0);

			AEGfxSetTransform(button.transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}

		// Shop upgrades
		if (i > 0) {
			switch (button.button_type) {
				case MOVEMENT_SPEED:
					AEGfxTextureSet(mov_speed_button_tex, 0, 0);
					AEGfxSetTransform(button.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

					for (int j = 0; j < player.mov_speed_level; ++j) {
						AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					for (int j = player.mov_speed_level; j < MAX_MOV_SPEED_LVL; ++j) {
						AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					break;

				case CAPACITY:
					AEGfxTextureSet(capacity_button_tex, 0, 0);
					AEGfxSetTransform(button.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

					for (int j = 0; j < player.capacity_level; ++j) {
						AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					for (int j = player.capacity_level; j < MAX_CAPACITY_LVL; ++j) {
						AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					break;

				case TRACTOR_BEAM_STRENGTH:
					AEGfxTextureSet(strength_button_tex, 0, 0);
					AEGfxSetTransform(button.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

					for (int j = 0; j < player.beam_level; ++j) {
						AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					for (int j = player.beam_level; j < MAX_BEAM_STRENGTH_LVL; ++j) {
						AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					break;

				case SPACE_STATION:
					AEGfxTextureSet(mov_speed_button_tex, 0, 0);
					AEGfxSetTransform(button.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

					for (int j = 0; j < player.space_station_count; ++j) {
						AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					for (int j = player.space_station_count; j < MAX_SPACE_STATION_CNT; ++j) {
						AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					break;

				case TUTORIAL_OPEN:
					AEGfxTextureSet(tutorial_open_tex, 0, 0);
					AEGfxSetTransform(button.transform.m);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					break;
			}
		}
	}

	// Shop buttons (text/icons)
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];

		if (button.button_type == SHOP_OPEN) {
			/*shop_option_name = "SHOP";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button.position.x - button.width / 2 - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - cam_y) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);*/
		}
		else if (button.button_type == MOVEMENT_SPEED) {
			// Print upgrade name
			shop_option_name = "Movement Speed";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - button.height / 2.f - cam_y) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);
		
			// Print upgrade cost
			/*shop_upgrade_cost = "Cost: " + std::to_string(mov_speed_cost);
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_upgrade_cost.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - cam_y - FONT_ID_SHOP_SIZE) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);*/
		}
		else if (button.button_type == CAPACITY) {
			// Print upgrade name
			shop_option_name = "Increase Capacity";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - button.height / 2.f - cam_y) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);
		
			// Print upgrade cost
			/*shop_upgrade_cost = "Cost: " + std::to_string(capacity_cost);
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_upgrade_cost.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - cam_y - FONT_ID_SHOP_SIZE) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);*/
		}
		else if (button.button_type == TRACTOR_BEAM_STRENGTH) {
			// Print upgrade name
			shop_option_name = "Beam Strength";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - button.height / 2.f - cam_y) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);
		
			// Print upgrade cost
			/*shop_upgrade_cost = "Cost: " + std::to_string(beam_strength_cost);
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_upgrade_cost.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - cam_y - FONT_ID_SHOP_SIZE) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);*/
		}
		else if (button.button_type == CREATE_DRONE) {
			// Draw icon
			AEGfxTextureSet(player_tex, 0, 0);
			AEGfxSetTransform(drone_icon_transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// Print upgrade cost
			shop_upgrade_cost = "Cost: " + std::to_string(drone_cost);
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_upgrade_cost.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - cam_y - FONT_ID_SHOP_SIZE) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);
		}
		else if (button.button_type == SPACE_STATION) {
			// Draw icon
			AEGfxTextureSet(space_station_tex, 0, 0);
			AEGfxSetTransform(space_station_icon_transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

			// Print upgrade cost
			shop_upgrade_cost = "Cost: " + std::to_string(space_station_cost);
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_upgrade_cost.c_str()),
				(button.position.x - button.width / 2.f - cam_x) / static_cast<f32>(AEGetWindowWidth() / 2),
				(button.position.y - cam_y - FONT_ID_SHOP_SIZE) / static_cast<f32>(AEGetWindowHeight() / 2),
				1.f, 0.f, 0.f, 0.f);
		}
	}
	
	// Tutorial text
	if (PLAYER_FLY == player.state)
	{
		// Tutorial
		std::string tutorial = "W          Accelerate";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.65f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "S           Decelerate";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.6f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "A           Rotate Left";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.55f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "D           Rotate Right";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.5f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "LMB      Release Debris";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.42f, 1.f, 1.f, 1.f, 1.f);
	}
	else
	{
		// Tutorial
		std::string tutorial = "W          Leave Planet";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.65f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "A           Rotate Left";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.6f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "D           Rotate Right";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.55f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "Space   Collect Debris";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.48f, 1.f, 1.f, 1.f, 1.f);

		tutorial = "LMB      Release Debris";
		AEGfxPrint(font_id, const_cast<s8*>(tutorial.c_str()), -0.98f + tutorial_offset / (AEGetWindowWidth() / 2), 0.43f, 1.f, 1.f, 1.f, 1.f);
	}
}

void PlayerUI::free()
{
	button_vector.clear();
}

void PlayerUI::unload()
{
	AEGfxTextureUnload(shop_icon_tex);
	AEGfxTextureUnload(shop_open_tex);
	AEGfxTextureUnload(shop_close_tex);
	AEGfxTextureUnload(space_station_tex);
	AEGfxTextureUnload(shop_background_tex);
	AEGfxTextureUnload(tutorial_open_tex);
	AEGfxTextureUnload(tutorial_background_tex);
	AEGfxTextureUnload(upgrade_level_hollow_tex);
	AEGfxTextureUnload(upgrade_level_solid_tex);
	AEGfxTextureUnload(player_hud_tex);
	AEGfxTextureUnload(speed_hover_tex);
	AEGfxTextureUnload(capacity_hover_tex);
	AEGfxTextureUnload(strength_hover_tex);
	AEGfxTextureUnload(mov_speed_button_tex);
	AEGfxTextureUnload(capacity_button_tex);
	AEGfxTextureUnload(strength_button_tex);
}

void PlayerUI::shop_open(Player& player)
{
	// ===================
	// Check player input
	// ===================

	// Player clicks outside shop
	if (click_outside_shop() || AEInputCheckTriggered(AEVK_ESCAPE))
		close_shop();

	else {
		// =================
		// Shop interaction
		// =================

		for (int i = 0; i < button_vector.size(); ++i) {
			ShopOption& button = button_vector[i];

			if (button_clicked(button)) {
				if (button.button_type == MOVEMENT_SPEED) {
					if (player.credits >= mov_speed_cost && player.mov_speed_level < MAX_MOV_SPEED_LVL) {
						player.credits -= mov_speed_cost;
						player.mov_speed_level++;
					}
				}
				else if (button.button_type == CAPACITY) {
					if (player.credits >= capacity_cost && player.capacity_level < MAX_CAPACITY_LVL) {
						player.credits -= capacity_cost;
						player.capacity_level++;
					}
				}
				else if (button.button_type == TRACTOR_BEAM_STRENGTH) {
					if (player.credits >= beam_strength_cost && player.beam_level < MAX_BEAM_STRENGTH_LVL) {
						player.credits -= beam_strength_cost;
						player.beam_level++;
					}
				}
				else if (button.button_type == CREATE_DRONE || button.button_type == SPACE_STATION) {
					if (button.button_type == CREATE_DRONE && !placing_drone && player.credits >= drone_cost)
						placing_drone = true;

					else if (button.button_type == SPACE_STATION && !placing_station && player.credits >= space_station_cost)
						placing_station = true;

					close_shop();
				}
			}
		}
	}
}

void PlayerUI::shop_closed()
{
	// ================
	// Check for input
	// ================

	if (button_clicked(button_vector[0])) {
		shop_triggered = true;
		shop_transition = true;
	}
}

void PlayerUI::close_shop()
{
	// Close shop
	shop_triggered = false;
	shop_transition = true;
}

bool PlayerUI::button_clicked(ShopOption button) 
{
	// Get position of each side of button
	f32 button_left		= button.position.x - button.width / 2.f;
	f32 button_right	= button.position.x + button.width / 2.f;
	f32 button_top		= button.position.y + button.height / 2.f;
	f32 button_bottom	= button.position.y - button.height / 2.f;

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if ((button_left < mouse_pos_world.x) && (button_right > mouse_pos_world.x)) {
			if ((button_bottom < mouse_pos_world.y) && (button_top > mouse_pos_world.y)) {
				return true;
			}
		}
	}

	return false;
}

bool PlayerUI::click_outside_shop()
{
	// Get position of each side of shop background
	f32 shop_background_left	= cam_x - shop_bg_width / 2.f;
	f32 shop_background_right	= cam_x + shop_bg_width / 2.f;
	f32 shop_background_top		= cam_y + shop_bg_height / 2.f;
	f32 shop_background_bottom	= cam_y - shop_bg_height / 2.f;

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		// Left/right/top/bottom border
		if ((mouse_pos_world.x > (cam_x - static_cast<f32>(AEGetWindowWidth()) / 2.f) && mouse_pos_world.x < shop_background_left) ||
			(mouse_pos_world.x > shop_background_right && mouse_pos_world.x < (cam_x + static_cast<f32>(AEGetWindowWidth()) / 2.f)) ||
			(mouse_pos_world.y > (cam_y - static_cast<f32>(AEGetWindowHeight()) / 2.f) && mouse_pos_world.y < shop_background_bottom) ||
			(mouse_pos_world.y > shop_background_top && mouse_pos_world.y < (cam_y + static_cast<f32>(AEGetWindowHeight()) / 2.f)))

			return true;
	}

	return false;
}

bool PlayerUI::hover_over_button(ShopOption button)
{
	// Get position of each side of button
	f32 button_left = button.position.x - button.width / 2.f;
	f32 button_right = button.position.x + button.width / 2.f;
	f32 button_top = button.position.y + button.height / 2.f;
	f32 button_bottom = button.position.y - button.height / 2.f;

	if ((button_left < mouse_pos_world.x) && (button_right > mouse_pos_world.x)) {
		if ((button_bottom < mouse_pos_world.y) && (button_top > mouse_pos_world.y)) {
			return true;
		}
	}

	return false;
}

void PlayerUI::tutorial_open()
{
	// ===================
	// Check player input
	// ===================

	// Player clicks outside tutorial
	if (button_clicked(button_vector[6]) || AEInputCheckTriggered(AEVK_ESCAPE))
		close_tutorial();
}

void PlayerUI::tutorial_closed()
{
	// ================
	// Check for input
	// ================

	if (button_clicked(button_vector[6])) {
		tutorial_triggered = true;
		tutorial_transition = true;
	}
}

void PlayerUI::close_tutorial()
{
	// Close tutorial
	tutorial_triggered = false;
	tutorial_transition = true;
}