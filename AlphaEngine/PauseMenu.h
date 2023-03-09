#pragma once

#include "AEEngine.h"

struct pause_menu
{
    void load();
    void init();
    void update();
    void draw(AEVec2 const& camPos);
    void free();
    void unload();
};