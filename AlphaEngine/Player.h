#pragma once

struct Player
{
	AEVec2 position;
	int state;
	double size;
	double dist_from_planet;
	double speed;
	double angle;
	//double free; // what is free
	double beam_str;
	double beam_width;
	double current_planet;
	double current_capacity; // probably can shorten
	double max_capacity;
	double texture;
	double velocity;


	void load();
	void init();
	void update(f64 frame_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
};

// e.g
//player.x_pos = screen / 2;
//player.y_pos = screen / 2;