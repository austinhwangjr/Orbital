#pragma once
#include "Graphics.h"

namespace how_to_play
{
    void load();
    void init();
    void update();
    void draw();
    void free();
    void unload();

    // checking input area stuff
struct Button1
{
    float x;
    float y;
    float width;
    float height;
};


}
