#pragma once

extern AEGfxTexture* startButtonTex;

struct Start_Button
{
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
	void load();
	void init();
	void update();
	void draw(AEGfxVertexList* pMesh1);
	void free();
	void unload();
};
