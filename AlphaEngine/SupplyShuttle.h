#pragma once
struct Shuttles
{
	double x_pos;
	double y_pos;
	double size;
	double speed;
	double vector;
	double value;
	double lifespan;
	double above_below; //random choice to shrink or increase size when passing by other planet
	double texture;
};

namespace shuttles
{
	void load();
	void init();
	void update();
	void draw();
	void free();
	void unload();
}