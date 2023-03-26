#pragma once
#include "Graphics.h"

namespace credits
{
    void load();
    void init();
    void update();
    void draw();
    void free();
    void unload();

    struct Button2
    {
        float x;
        float y;
        float width;
        float height;
    };

    struct HoverButton2
    {
        float width;
        float height;
    };


}
