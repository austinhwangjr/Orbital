    #ifndef OPTIONS_H
    #define OPTIONS_H

    #include "AEEngine.h"

    namespace Options
    {
        void load();
        void init();
        void update(float* volume, bool* muted);
        void draw();
        void free();
        void unload();

        extern bool muteHoverState;
        extern bool draggingSlider;

        extern float muteButtonX;
        extern float muteButtonY;
        extern float sliderX;
        extern float sliderY;
        extern float sliderThumbX;
        extern float sliderThumbY;

        extern float volume;
        extern bool muted;
    }

    #endif // OPTIONS_H
