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

        struct Button11
        {
            float x;
            float y;
            float width;
            float height;
        };
    }

    #endif // OPTIONS_H
