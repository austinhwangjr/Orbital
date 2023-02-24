#include "AEEngine.h"
#include "PlayerUI.h"
#include "Global.h"
#include <iostream>
#include <vector>
#include <string>

// Textures
extern AEGfxTexture* player_tex;
AEGfxTexture* shop_icon_tex;
AEGfxTexture* space_station_tex;
AEGfxTexture* shop_background_tex;
AEGfxTexture* upgrade_level_hollow_tex;
AEGfxTexture* upgrade_level_solid_tex;

// Variables for general UI
std::string		score, credits, capacity;
extern s8		font_id;

// Variables for shop
extern s8		font_id_shop;
std::string		shop_option_name;
AEMtx33			shop_background_transform;

// Variables for camera
f32 cam_x, cam_y;

// Mouse coordinates
AEVec2 mouse_pos_world;

// Vector of buttons
std::vector<ShopOption> button_vector;

void PlayerUI::load()
{
	// Load textures
	shop_icon_tex = AEGfxTextureLoad("Assets/YellowTexture.png");
	space_station_tex = AEGfxTextureLoad("Assets/space-station.png");
	shop_background_tex = AEGfxTextureLoad("Assets/shop-background.png");
	upgrade_level_hollow_tex = AEGfxTextureLoad("Assets/upgrade_level_hollow.png");
	upgrade_level_solid_tex = AEGfxTextureLoad("Assets/upgrade_level_solid.png");
}

void PlayerUI::init()
{
	// Add shop button to vector
	ShopOption shop_open_button{};
	shop_open_button.width	= 70.f;
	shop_open_button.height = 70.f;
	shop_open_button.button_type = SHOP_OPEN;
	button_vector.push_back(shop_open_button);

	// Not in placing mode initially
	placing_drone = false;
	placing_station = false;

	// Shop is closed initially
	shop_triggered = false;
}

void PlayerUI::update(Player& player)
{
	// Get mouse coordinates (world)
	s32 mouse_x_screen, mouse_y_screen;
	AEInputGetCursorPosition(&mouse_x_screen, &mouse_y_screen);

	mouse_pos_world.x = cam_x + mouse_x_screen - 800;
	mouse_pos_world.y = cam_y + 450 - mouse_y_screen;

	if (shop_triggered)
		shop_open(player);

	else
		shop_closed();

	// ================================
	// Update open shop button position
	// ================================

	AEGfxGetCamPosition(&cam_x, &cam_y);
	button_vector[0].position.x = cam_x + static_cast<f32>(AEGetWindowWidth()) / 2.f - button_vector[0].width / 2.f;
	button_vector[0].position.y = cam_y + static_cast<f32>(AEGetWindowHeight()) / 2.f - button_vector[0].height * 2.5f;

	for (int i = 1; i < button_vector.size(); ++i) {
		/*button_vector[i].position.x = button_vector[0].position.x - 150;
		button_vector[i].position.y = button_vector[0].position.y - (i - 1) * 115;*/
		button_vector[i].position.x = cam_x - static_cast<f32>(AEGetWindowWidth() / 2) * 0.7f;
		button_vector[i].position.y = button_vector[0].position.y - (i - 1) * 115;

		for (int j = 0; j < button_vector[i].indicator_vector.size(); ++j) {
			UpgradeLevelIndicator& indicator = button_vector[i].indicator_vector[j];

			indicator.position.x = button_vector[i].position.x + button_vector[i].width + (j * indicator.width);
			indicator.position.y = button_vector[i].position.y;
		}
	}

	// =================================
	// Calculate the matrix for buttons
	// =================================

	AEMtx33 scale, rot, trans;

	// Shop buttons
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];
		AEMtx33Scale(&scale, button.width, button.height);
		AEMtx33Rot(&rot, 0.f);
		AEMtx33Trans(&trans, button.position.x, button.position.y);
		AEMtx33Concat(&button.transform, &rot, &scale);
		AEMtx33Concat(&button.transform, &trans, &button.transform);

		if (i > 0) {
			// Upgrade level
			for (int j = 0; j < button.indicator_vector.size(); ++j) {
				UpgradeLevelIndicator& indicator = button.indicator_vector[j];

				AEMtx33Scale(&scale, indicator.width, indicator.height);
				AEMtx33Rot(&rot, 0.f);
				AEMtx33Trans(&trans, indicator.position.x, indicator.position.y);
				AEMtx33Concat(&indicator.transform, &rot, &scale);
				AEMtx33Concat(&indicator.transform, &trans, &indicator.transform);
			}
		}
	}

	// Shop background
	AEMtx33Scale(&scale, static_cast<f32>(AEGetWindowWidth()) * 0.85f, static_cast<f32>(AEGetWindowHeight()) * 0.85f);
	AEMtx33Rot(&rot, 0.f);
	AEMtx33Trans(&trans, cam_x, cam_y);
	AEMtx33Concat(&shop_background_transform, &rot, &scale);
	AEMtx33Concat(&shop_background_transform, &trans, &shop_background_transform);
}

/******************************************************************************/
/*!
	Draw Player UI
*/
/******************************************************************************/
void PlayerUI::draw(AEGfxVertexList* pMesh, Player& player)
{
	AEMtx33 scale{}, rotate{}, translate{}, transform{};

	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);

	// Print score
	score = "Score: " + std::to_string(player.score);
	AEGfxPrint(font_id, const_cast<s8*>(score.c_str()),
		-0.85f, 0.75f, 1.f, 1.f, 1.f, 1.f);
	
	// Print credits
	credits = "Credits: " + std::to_string(player.credits);
	AEGfxPrint(font_id, const_cast<s8*>(credits.c_str()), 0.5f, 0.75f, 1.f, 1.f, 1.f, 1.f);

	// Print capacity
	capacity = "Capacity: " + std::to_string(player.current_capacity) + " / " + std::to_string(player.max_capacity + player.capacity_level);
	AEGfxPrint(font_id, const_cast<s8*>(capacity.c_str()), -0.25f, -0.75f, 1.f, 1.f, 1.f, 1.f);
	
	// Shop background
	if (shop_triggered) {
		// Shop background
		AEGfxTextureSet(shop_background_tex, 0, 0);
		AEGfxSetTransparency(0.5);
		AEGfxSetTransform(shop_background_transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Shop buttons
	for (int i = 0; i < button_vector.size(); ++i) {
		AEGfxTextureSet(shop_icon_tex, 0, 0);
		ShopOption& button = button_vector[i];
		AEGfxSetTransform(button.transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		if (i > 0) {
			switch (button.button_type) {
				case MOVEMENT_SPEED:
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

				case CREATE_DRONE:
					for (int j = 0; j < player.drone_count; ++j) {
						AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					for (int j = player.drone_count; j < MAX_DRONE_CNT; ++j) {
						AEGfxTextureSet(upgrade_level_hollow_tex, 0, 0);
						UpgradeLevelIndicator& indicator = button.indicator_vector[j];
						AEGfxSetTransform(indicator.transform.m);
						AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					}
					break;

				case SPACE_STATION:
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

				case TRACTOR_BEAM_STRENGTH:
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
			}
			

			/*for (int j = 0; j < button.indicator_vector.size(); ++j) {
				AEGfxTextureSet(upgrade_level_solid_tex, 0, 0);
				UpgradeLevelIndicator& indicator = button.indicator_vector[j];
				AEGfxSetTransform(indicator.transform.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}*/
		}
	}

	// Shop buttons (text/icons)
	for (int i = 0; i < button_vector.size(); ++i) {
		if (button_vector[i].button_type == SHOP_OPEN) {
			shop_option_name = "SHOP";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button_vector[i].position.x - button_vector[i].width / 2 - cam_x) / 800, (button_vector[i].position.y - button_vector[i].height / 2 - cam_y) / 400, 1.f, 0.f, 0.f, 0.f);
		}
		else if (button_vector[i].button_type == MOVEMENT_SPEED) {
			shop_option_name = "Movement Speed";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button_vector[i].position.x - button_vector[i].width / 2 - cam_x) / 800, (button_vector[i].position.y - button_vector[i].height / 2 - cam_y) / 400, 1.f, 0.f, 0.f, 0.f);
		}
		else if (button_vector[i].button_type == CAPACITY) {
			shop_option_name = "Increase Capacity";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button_vector[i].position.x - button_vector[i].width / 2 - cam_x) / 800, (button_vector[i].position.y - button_vector[i].height / 2 - cam_y) / 400, 1.f, 0.f, 0.f, 0.f);
		}
		else if (button_vector[i].button_type == CREATE_DRONE) {
			AEGfxTextureSet(player_tex, 0, 0);
			AEMtx33Scale(&scale, 20.f, 20.f);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, button_vector[i].position.x, button_vector[i].position.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
		else if (button_vector[i].button_type == SPACE_STATION) {
			AEGfxTextureSet(space_station_tex, 0, 0);
			AEMtx33Scale(&scale, 20.f, 20.f);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, button_vector[i].position.x, button_vector[i].position.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

void PlayerUI::free()
{

}

void PlayerUI::unload()
{
	AEGfxTextureUnload(shop_icon_tex);
	AEGfxTextureUnload(space_station_tex);
	AEGfxTextureUnload(shop_background_tex);
	AEGfxTextureUnload(upgrade_level_hollow_tex);
	AEGfxTextureUnload(upgrade_level_solid_tex);
}

void PlayerUI::shop_open(Player& player)
{
	// ===================
	// Check player input
	// ===================

	for (int i = 0; i < button_vector.size(); ++i) {
		if (button_clicked(button_vector[i])) {
			if (button_vector[i].button_type == SHOP_OPEN) {
				// remove buttons from list
				for (int i = 0; i < UPGRADE_COUNT; ++i) {
					button_vector.pop_back();
				}
				shop_triggered = false;
			}
			else if (button_vector[i].button_type == MOVEMENT_SPEED) {
				if (player.credits >= mov_speed_cost && player.mov_speed_level < MAX_MOV_SPEED_LVL) {
					player.credits -= mov_speed_cost;
					//player.speed_upgrade += 0.5;
					player.mov_speed_level++;
				}
			}
			else if (button_vector[i].button_type == CAPACITY) {
				if (player.credits >= capacity_cost && player.capacity_level < MAX_CAPACITY_LVL) {
					player.credits -= capacity_cost;
					//player.max_capacity++;
					player.capacity_level++;
				}
			}
			else if (button_vector[i].button_type == CREATE_DRONE || button_vector[i].button_type == SPACE_STATION) {
				if (button_vector[i].button_type == CREATE_DRONE && !placing_drone && player.credits >= drone_cost)
					placing_drone = true;

				else if (button_vector[i].button_type == SPACE_STATION && !placing_station && player.credits >= space_station_cost)
					placing_station = true;

				// Remove buttons from list
				for (int i = 0; i < UPGRADE_COUNT; ++i)
					button_vector.pop_back();

				// Close shop
				shop_triggered = false;
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
		//for (int i = 0; i < 4; ++i) {
		for (int i = 0; i < UPGRADE_COUNT; ++i) {
			// Populate button vector
			ShopOption player_upgrade{};
			player_upgrade.width = 160.f;
			player_upgrade.height = 80.f;
			player_upgrade.button_type = MOVEMENT_SPEED + i;

			for (int j = 0; j < 5; ++j) {
				UpgradeLevelIndicator indicator{};
				indicator.width = 32.0f;
				indicator.height = 16.0f;
				player_upgrade.indicator_vector.push_back(indicator);
			}

			button_vector.push_back(player_upgrade);
		}
		shop_triggered = true;
	}
}

bool PlayerUI::button_clicked(ShopOption button) 
{
	f32 button_left		= button.position.x - button.width / 2;
	f32 button_right	= button.position.x + button.width / 2;
	f32 button_top		= button.position.y + button.height / 2;
	f32 button_bottom	= button.position.y - button.height / 2;

	if ((button_left < mouse_pos_world.x) && (button_right > mouse_pos_world.x)) {
		if ((button_bottom < mouse_pos_world.y) && (button_top > mouse_pos_world.y)) {
			if (AEInputCheckTriggered(AEVK_LBUTTON))
				return true;
		}
	}

	return false;
}