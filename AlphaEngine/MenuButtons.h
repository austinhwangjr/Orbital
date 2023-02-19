#pragma once

extern AEGfxTexture* startButtonTex;

class Menu_Button
{
public:
	void load();
	void init();
	void update();
	void draw(AEGfxVertexList* pMesh1);
	void free();
	void unload();

	float width;
	float height;

private:
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
};