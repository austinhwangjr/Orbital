#pragma once
#include "AEEngine.h"

namespace lose_menu
{
    void load();
    void init();
    void update();
    //void draw(const AEVec2& camPos);
    void draw();
    void free();
    void unload();

    struct lm_Button
    {
        float x;
        float y;
        float width;
        float height;
    };
}
