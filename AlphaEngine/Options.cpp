#include "Options.h"
#include "Input.h"
#include "Graphics.h"
#include "AudioManager.h"

AEGfxTexture* muteButtonTexture = nullptr;
AEGfxTexture* muteButtonHoverTexture = nullptr;
AEGfxTexture* sliderTexture = nullptr;
AEGfxTexture* sliderThumbTexture = nullptr;

AEGfxVertexList* optionsMesh;

Rendering optionsMenu;

namespace Options
{
    float volume = 0.5f;
    bool muted = false;
    bool muteHoverState = false;
    bool draggingSlider = false;

    float muteButtonX = 0.0f;
    float muteButtonY = 0.0f;
    float sliderX = 0.0f;
    float sliderY = -100.0f;
    float sliderThumbX = sliderX;
    float sliderThumbY = sliderY;
}


void Options::load()
{
    muteButtonTexture           = AEGfxTextureLoad("Assets/MainMenu/Options/o_MuteButton.png");
    muteButtonHoverTexture      = AEGfxTextureLoad("Assets/MainMenu/Options/o_HoverMuteButton.png");
    sliderTexture               = AEGfxTextureLoad("Assets/MainMenu/Options/o_Slider.png");
    sliderThumbTexture          = AEGfxTextureLoad("Assets/MainMenu/Options/o_HoverSlider.png");
}

void Options::init()
{
    optionsMenu.SquareMesh(optionsMesh);

}

void Options::update(float* volume, bool* muted)
{
    muteHoverState = Input::isMouseHover(muteButtonX, muteButtonY, 50, 50, 60, 60);

    if (AEInputCheckTriggered(AEVK_LBUTTON) && muteHoverState)
    {
        *muted = !*muted;
        AudioManager::ToggleMute(*muted);
    }

    if (Input::isMouseHover(sliderThumbX, sliderThumbY, 20, 20, 20, 20))
    {
        if (AEInputCheckCurr(AEVK_LBUTTON))
        {
            draggingSlider = true;
        }
    }

    if (draggingSlider)
    {
        sliderThumbX = mouseX - AEGetWindowWidth() / 2.0f - 10;
        if (sliderThumbX < sliderX - 50) 
        {
            sliderThumbX = sliderX - 50; 
        }
        if (sliderThumbX > sliderX + 50) 
        {
            sliderThumbX = sliderX + 50; 
        }
        *volume = (sliderThumbX - (sliderX - 50)) / 100; 
        AudioManager::setVolume(*volume);
    }



    if (AEInputCheckReleased(AEVK_LBUTTON))
    {
        draggingSlider = false;
    }
}

void Options::draw()
{
    Rendering::RenderSprite(sliderTexture, sliderX, sliderY, 50, 20, optionsMesh);
    Rendering::RenderSprite(sliderThumbTexture, sliderThumbX, sliderThumbY, 20, 20, optionsMesh);

    if (muteHoverState)
    {
        Rendering::RenderSprite(muteButtonHoverTexture, muteButtonX, muteButtonY, 60, 60, optionsMesh);
    }
    else
    {
        Rendering::RenderSprite(muteButtonTexture, muteButtonX, muteButtonY, 50, 50, optionsMesh);
    }
}

void Options::free()
{
    AEGfxMeshFree(optionsMesh);

}


void Options::unload()
{
    AEGfxTextureUnload(muteButtonTexture);
    AEGfxTextureUnload(muteButtonHoverTexture);
    AEGfxTextureUnload(sliderTexture);
    AEGfxTextureUnload(sliderThumbTexture);
}


