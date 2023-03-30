#pragma once

namespace main_menu
{
    enum MainMenuState
    {
        MENU,
        HOW_TO_PLAY
    };

    extern MainMenuState currentState;

    void load();
    void init();
    void update();
    void draw();
    void free();
    void unload();
}

extern f64 MMframe_time;
extern f64 MMtotal_time;
