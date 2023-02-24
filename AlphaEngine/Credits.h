#pragma once
#include "Graphics.h"

class Credits : public Rendering
{
public:
    void load();
    void init();
    void update();
    void draw();
    void free();
    void unload();

    float width;
    float height;

};