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



private:
	float width = 200.0f;
	float height = 50.0f;
	AEMtx33 scale;
	AEMtx33 rotate;
	AEMtx33 translate;
	AEMtx33 transform;
};

