#include "AEEngine.h"
#include "PlayerUI.h"
#include <iostream>
#include <vector>

// player texture (temp)
extern AEGfxTexture* player_tex;

// Print text
s8			font_id_shop;
const char* shop_option_name_c_str;
std::string shop_option_name;
bool		shop_open		= false;

f32 mouse_x_actual, mouse_y_actual;

//Player drone;

enum BUTTON_TYPE {
	SHOP_OPEN = 0,
	MOVEMENT_SPEED,
	CAPACITY,
	CREATE_DRONE,
	SPACE_STATION,
	TRACTOR_BEAM_STRENGTH
};

struct ShopOption
{
	AEVec2 position;
	f32 size;
	int button_type;
};
AEGfxTexture* shop_icon_tex;
std::vector<ShopOption> button_list;

void PlayerUI::load()
{
	// Load textures
	shop_icon_tex = AEGfxTextureLoad("Assets/YellowTexture.png");
	player_tex = AEGfxTextureLoad("Assets/test-player.png");

	// Font for text
	//font_id_shop = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 20);
}

void PlayerUI::init()
{
	// shop icon test
	ShopOption shop_open_button{};
	shop_open_button.size = 70.f;
	shop_open_button.position.x = AEGfxGetWinMaxX() - shop_open_button.size;
	shop_open_button.position.y = AEGfxGetWinMaxY() * 2 / 3;
	button_list.push_back(shop_open_button);

	// set drone to inactive
	//drone_active	= false;
	placing_drone	= false;
	placing_station = false;
}

void PlayerUI::update(f64 frame_time, Player& player)
{
	// shop icon test
	s32 mouse_x, mouse_y;
	AEInputGetCursorPosition(&mouse_x, &mouse_y);

	mouse_x_actual = mouse_x - 800;
	mouse_y_actual = 400 - mouse_y;

	if (!shop_open) {
		if ((button_list[0].position.x - button_list[0].size / 2 < mouse_x_actual) && (button_list[0].position.x + button_list[0].size / 2 > mouse_x_actual)) {
			if ((button_list[0].position.y - button_list[0].size / 2 < mouse_y_actual) && (button_list[0].position.y + button_list[0].size / 2 > mouse_y_actual)) {
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					for (int i = 0; i < 3; ++i) {
						ShopOption player_upgrade;
						player_upgrade.size = 80.f;
						player_upgrade.position.x = button_list[0].position.x - 150;
						player_upgrade.position.y = button_list[0].position.y - i * 115;
						player_upgrade.button_type = SHOP_OPEN + i + 1;
						button_list.push_back(player_upgrade);
					}
					shop_open = true;
				}
			}
		}
	}

	else {
		for (int i = 0; i < button_list.size(); ++i) {
			if ((button_list[i].position.x - button_list[i].size < mouse_x_actual) &&
				(button_list[i].position.x + button_list[i].size > mouse_x_actual))
			{
				if ((button_list[i].position.y - button_list[i].size / 2 < mouse_y_actual) &&
					(button_list[i].position.y + button_list[i].size / 2 > mouse_y_actual))
				{
					if (AEInputCheckTriggered(AEVK_LBUTTON)) {
						if (button_list[i].button_type == SHOP_OPEN) {
							for (int i = 0; i < 3; ++i) {
								button_list.pop_back();
							}
							shop_open = false;
						}
						else if (button_list[i].button_type == CAPACITY) {
							if (player.current_capacity > 0) {
								player.current_capacity--;
								player.max_capacity++;
							}
						}
						else if (button_list[i].button_type == MOVEMENT_SPEED) {
							if (player.current_capacity > 1) {
								player.current_capacity -= 2;
								player.speed_upgrade += 0.5;
							}
						}
						else if (button_list[i].button_type == CREATE_DRONE) {
							// drag and drop
							if (!placing_drone) {
								placing_drone = true;
							}
							// left click again, place drone
							// close shop
							//shop_open = false;
						}
					}
				}
			}
		}
	}
}

void PlayerUI::draw(AEGfxVertexList* pMesh, Player& player)
{
	AEMtx33 scale{}, rotate{}, translate{}, transform{};

	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);

	// Shop icon test
	AEGfxTextureSet(shop_icon_tex, 0, 0);
	for (int i = 0; i < button_list.size(); ++i) {
		if (button_list[i].button_type == SHOP_OPEN) {
			AEMtx33Scale(&scale, button_list[i].size, button_list[i].size);
		}
		else {
			AEMtx33Scale(&scale, button_list[i].size * 2, button_list[i].size);
		}

		AEMtx33Rot(&rotate, 0.f);
		AEMtx33Trans(&translate, button_list[i].position.x, button_list[i].position.y);
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	// Top box
	for (int i = 0; i < button_list.size(); ++i) {

		if (button_list[i].button_type == SHOP_OPEN) {
			shop_option_name = "SHOP";
			shop_option_name_c_str = shop_option_name.c_str();
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name_c_str),
				(button_list[i].position.x - button_list[i].size / 2) / 800, button_list[i].position.y / 400, 1.f, 0.f, 0.f, 0.f);
		}

		else if (button_list[i].button_type == MOVEMENT_SPEED) {
			shop_option_name = "Movement Speed";
			shop_option_name_c_str = shop_option_name.c_str();
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name_c_str),
				(button_list[i].position.x - button_list[i].size) / 800, button_list[i].position.y / 400, 1.f, 0.f, 0.f, 0.f);
		}

		else if (button_list[i].button_type == CAPACITY) {
			shop_option_name = "Increase Capacity";
			shop_option_name_c_str = shop_option_name.c_str();
			AEGfxPrint(font_id_shop, const_cast<s8*>(shop_option_name_c_str),
				(button_list[i].position.x - button_list[i].size) / 800, button_list[i].position.y / 400, 1.f, 0.f, 0.f, 0.f);
		}

		else if (button_list[i].button_type == CREATE_DRONE) {
			AEGfxTextureSet(player_tex, 0, 0);
			AEMtx33Scale(&scale, 20.f, 20.f);
			AEMtx33Rot(&rotate, PI);
			AEMtx33Trans(&translate, button_list[i].position.x, button_list[i].position.y);
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	// drone drawing temp
	/*AEGfxTextureSet(player_tex, 0, 0);
	AEMtx33Scale(&scale, 20.f, 20.f);
	AEMtx33Rot(&rotate, PI);
	AEMtx33Trans(&translate, drone.position.x, drone.position.y);
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);*/

	//if (!placing_drone) {
	//	AEGfxTextureSet(player_tex, 0, 0);

	//	AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);

	//	AEMtx33Scale(&scale, 20.f, 20.f);
	//	AEMtx33Rot(&rotate, PI);
	//	AEMtx33Trans(&translate, drone.position.x, drone.position.y);
	//	AEMtx33Concat(&transform, &rotate, &scale);
	//	AEMtx33Concat(&transform, &translate, &transform);
	//	AEGfxSetTransform(transform.m);
	//	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//}
	//else {
	//	/*if (AEVec2Distance(drone.position))
	//		AEGfxSetBlendColor(0.f, 1.f, 0.f, 0.5f);
	//	else

	//		AEGfxSetBlendColor(1.f, 0.f, 0.f, 0.5f);*/

	//	AEGfxTextureSet(player_tex, 0, 0);
	//	AEMtx33Scale(&scale, 20.f, 20.f);
	//	AEMtx33Rot(&rotate, PI);
	//	AEMtx33Trans(&translate, mouse_x_actual, mouse_y_actual);
	//	AEMtx33Concat(&transform, &rotate, &scale);
	//	AEMtx33Concat(&transform, &translate, &transform);
	//	AEGfxSetTransform(transform.m);
	//	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//}
}

void PlayerUI::free()
{

}

void PlayerUI::unload()
{

	//AEGfxTextureUnload(shop_icon_tex);
	//AEGfxTextureUnload(player_tex);
	//AEGfxDestroyFont(font_id_shop);
}