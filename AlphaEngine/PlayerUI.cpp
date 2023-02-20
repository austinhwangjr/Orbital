#include "AEEngine.h"
#include "PlayerUI.h"
#include <iostream>
#include <vector>

// Textures
extern AEGfxTexture* player_tex;
AEGfxTexture* shop_icon_tex;
AEGfxTexture* space_station_tex;

// Variables for shop
s8				font_id_shop;
std::string		shop_option_name;
bool			shop_triggered;

// Mouse coordinates
AEVec2 mouse_pos_world;

// Vector of buttons
std::vector<ShopOption> button_vector;

void PlayerUI::load()
{
	// Load textures
	shop_icon_tex = AEGfxTextureLoad("Assets/YellowTexture.png");
	space_station_tex = AEGfxTextureLoad("Assets/space-station.png");

	// Font for text
	font_id_shop = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
}

void PlayerUI::init()
{
	// Add shop button to vector
	ShopOption shop_open_button{};
	shop_open_button.size = 70.f;
	shop_open_button.position.x = AEGfxGetWinMaxX() - shop_open_button.size;
	shop_open_button.position.y = AEGfxGetWinMaxY() * 2 / 3;
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

	mouse_pos_world.x = mouse_x_screen - 800;
	mouse_pos_world.y = 400 - mouse_y_screen;

	if (shop_triggered)
		shop_open(player);

	else
		shop_closed();

	// =================================
	// Calculate the matrix for buttons
	// =================================

	AEMtx33 scale, rot, trans;

	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];

		if (button.button_type == SHOP_OPEN)
			AEMtx33Scale(&scale, button.size, button.size);
		else
			AEMtx33Scale(&scale, button.size * 2, button.size);

		AEMtx33Rot(&rot, 0.f);
		AEMtx33Trans(&trans, button.position.x, button.position.y);
		AEMtx33Concat(&button.transform, &rot, &scale);
		AEMtx33Concat(&button.transform, &trans, &button.transform);
	}
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

	// Shop icon test
	AEGfxTextureSet(shop_icon_tex, 0, 0);
	for (int i = 0; i < button_vector.size(); ++i) {
		ShopOption& button = button_vector[i];
		AEGfxSetTransform(button.transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Top box
	for (int i = 0; i < button_vector.size(); ++i) {
		if (button_vector[i].button_type == SHOP_OPEN) {
			shop_option_name = "SHOP";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button_vector[i].position.x - button_vector[i].size / 2) / 800, button_vector[i].position.y / 400, 1.f, 0.f, 0.f, 0.f);
		}
		else if (button_vector[i].button_type == MOVEMENT_SPEED) {
			shop_option_name = "Movement Speed";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button_vector[i].position.x - button_vector[i].size) / 800, button_vector[i].position.y / 400, 1.f, 0.f, 0.f, 0.f);
		}
		else if (button_vector[i].button_type == CAPACITY) {
			shop_option_name = "Increase Capacity";
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name.c_str()),
				(button_vector[i].position.x - button_vector[i].size) / 800, button_vector[i].position.y / 400, 1.f, 0.f, 0.f, 0.f);
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
	AEGfxDestroyFont(font_id_shop);
}

void PlayerUI::shop_open(Player& player)
{
	// ===================
	// Check player input
	// ===================

	for (int i = 0; i < button_vector.size(); ++i) {

		if ((button_vector[i].position.x - button_vector[i].size < mouse_pos_world.x) &&
			(button_vector[i].position.x + button_vector[i].size > mouse_pos_world.x)) {

			if ((button_vector[i].position.y - button_vector[i].size / 2 < mouse_pos_world.y) &&
				(button_vector[i].position.y + button_vector[i].size / 2 > mouse_pos_world.y)) {

				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					if (button_vector[i].button_type == SHOP_OPEN) {
						// remove buttons from list
						for (int i = 0; i < 4; ++i) {
							button_vector.pop_back();
						}
						shop_triggered = false;
					}
					else if (button_vector[i].button_type == CAPACITY) {
						if (player.current_capacity > 0) {
							player.current_capacity--;
							player.max_capacity++;
						}
					}
					else if (button_vector[i].button_type == MOVEMENT_SPEED) {
						if (player.current_capacity > 1) {
							player.current_capacity -= 2;
							player.speed_upgrade += 0.5;
						}
					}
					else if (button_vector[i].button_type == CREATE_DRONE || button_vector[i].button_type == SPACE_STATION) {
						if (button_vector[i].button_type == CREATE_DRONE && !placing_drone)
							placing_drone = true;

						else if (button_vector[i].button_type == SPACE_STATION && !placing_station)
							placing_station = true;

						// Remove buttons from list
						for (int i = 0; i < 4; ++i)
							button_vector.pop_back();

						// Close shop
						shop_triggered = false;
					}
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

	f32 button_left		= button_vector[0].position.x - button_vector[0].size / 2;
	f32 button_right	= button_vector[0].position.x + button_vector[0].size / 2;
	f32 button_top		= button_vector[0].position.y + button_vector[0].size / 2;
	f32 button_bottom	= button_vector[0].position.y - button_vector[0].size / 2;

	if ((button_left < mouse_pos_world.x) && (button_right > mouse_pos_world.x)) {
		if ((button_bottom < mouse_pos_world.y) && (button_top > mouse_pos_world.y)) {
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				for (int i = 0; i < 4; ++i) {
					// Populate button vector
					ShopOption player_upgrade;
					player_upgrade.size = 80.f;
					player_upgrade.position.x = button_vector[0].position.x - 150;
					player_upgrade.position.y = button_vector[0].position.y - i * 115;
					player_upgrade.button_type = SHOP_OPEN + i + 1;
					button_vector.push_back(player_upgrade);
				}
				shop_triggered = true;
			}
		}
	}
}