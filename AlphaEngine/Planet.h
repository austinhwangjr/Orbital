#pragma once

#define max_planet 10

namespace planet
{
	void load();
	void init();
	void update(f64 elapsed_time);
	void draw(AEGfxVertexList* pMesh);
	void free();
	void unload();
}
