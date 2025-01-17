/******************************************************************************/
/*!
\file		Options.cpp
\author 	Ong You Yang (100%)
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the options
            screen.

All content (C) 2023 DigiPen Institute of Technology Singapore. All rights reserved.
 */
/******************************************************************************/
#include "Options.h"
#include "Input.h"
#include "Graphics.h"
#include "AudioManager.h"
#include "GameStateList.h"
#include "Global.h"
#include "Easing.h"
#include <iostream>
#include "Transition.h"

AEGfxTexture* o_OptionsBackground = nullptr;
AEGfxTexture* o_ToggleMute = nullptr;
AEGfxTexture* o_VolumeSliderTEXT = nullptr;

AEGfxTexture* o_RoundedRectangle_ACTIVE = nullptr;
AEGfxTexture* o_RoundedRectangle_NOT_ACTIVE = nullptr;

AEGfxTexture* o_MuteSliderThumb_ACTIVE = nullptr;
AEGfxTexture* o_MuteSliderThumb_NOT_ACTIVE = nullptr;

AEGfxTexture* o_VolumeSlider = nullptr;
AEGfxTexture* o_VolumeSliderThumb = nullptr;

AEGfxTexture* o_returnToMMTexture = nullptr;

AEGfxVertexList* optionsMesh;

Rendering optionsMenu;

static f32 timer3 = 0.f;

static float returnToMMX11 = 0.0f;
static float returnToMMY11 = -380.0f;

// Set the dimensions of each button
static float buttonWidth11 = 100.f;
static float buttonHeight11 = 100.f;

// Define the positions and dimensions for each button
Options::Button11 buttons11[] =
{
    {returnToMMX11, returnToMMY11, buttonWidth11, buttonHeight11},   // Return to main menu button
};

namespace Options
{
    float volume = 0.5f;
    bool muted = false;
    bool muteHoverState = false;
    bool draggingSlider = false;

    float targetVolume = 0.5f;

    float oCenterX = g_windowWidth / 2;
    float oCenterY = g_windowHeight / 2;

    float toggleMuteX = -250.f;
    float toggleMuteY = 100.f;
    float toggleMuteWidth = 250.f;
    float toggleMuteHeight = 80.f;

    float VolumeSliderX = -250.f;
    float VolumeSliderY = -100.f;
    float VolumeSliderWidth = 250.f;
    float VolumeSliderHeight = 80.f;

    // mute button
    float muteButtonX = 0.0f;
    float muteButtonY = 100.0f;
    float muteButtonWidth = 300.0f;
    float muteButtonHeight = 150.0f;

    float muteSliderThumbX = 0.0f;
    float muteSliderThumbY = 100.0f;
    float muteSliderThumbWidth = 295.0f;
    float muteSliderThumbHeight = 145.0f;

    // volume slider
    float sliderX = 0.0f;
    float sliderY = -100.0f;
    float sliderWidth = 110.0f;
    float sliderHeight = 5.0f;

    // volume slider thumb
    float sliderThumbX = sliderX;
    float sliderThumbY = sliderY;
    float sliderThumbWidth = 30.0f;
    float sliderThumbHeight = 30.0f;
}


void Options::load()
{
    o_OptionsBackground             = AEGfxTextureLoad("Assets/MainMenu/Options/o_OptionsBackground.png");
    o_ToggleMute                    = AEGfxTextureLoad("Assets/MainMenu/Options/o_ToggleMute.png");
    o_VolumeSliderTEXT              = AEGfxTextureLoad("Assets/MainMenu/Options/o_VolumeSlider.png");

    o_RoundedRectangle_ACTIVE       = AEGfxTextureLoad("Assets/MainMenu/Options/o_RoundedRectangle_ACTIVE.png");
    o_RoundedRectangle_NOT_ACTIVE   = AEGfxTextureLoad("Assets/MainMenu/Options/o_RoundedRectangle_NOT_ACTIVE.png");

    o_MuteSliderThumb_ACTIVE        = AEGfxTextureLoad("Assets/MainMenu/Options/o_MuteSliderThumb_ACTIVE.png");
    o_MuteSliderThumb_NOT_ACTIVE    = AEGfxTextureLoad("Assets/MainMenu/Options/o_MuteSliderThumb_NOT_ACTIVE.png");

    o_VolumeSlider                  = AEGfxTextureLoad("Assets/MainMenu/Options/o_Slider.png");
    o_VolumeSliderThumb             = AEGfxTextureLoad("Assets/MainMenu/Options/o_SliderThumb.png");

    o_returnToMMTexture             = AEGfxTextureLoad("Assets/MainMenu/Options/o_HomeIcon.png");

    AudioManager::LoadSound("Assets/BGM/one-last-time-141289.mp3", true);
}

void Options::init()
{
    optionsMenu.MainMenuSquareMesh(optionsMesh);

    if (!AudioManager::isBGMPlaying)
    {
        AudioManager::PlayBGM("Assets/BGM/one-last-time-141289.mp3", 0.25f);
    }
}

void Options::update(float* options_volume, bool* options_muted)
{
    timer3 += g_dt;

    // Check if the left mouse button has been clicked
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        // Check which button has been clicked
        if (Input::isButtonClicked(buttons11[0].x, buttons11[0].y, buttons11[0].width, buttons11[0].height))
        {
            AudioManager::PlayOnce("Assets/BGM/hyperspace_jump.mp3", 1.0f);

            AudioManager::StopBGMIfPlaying();
            next_state = GS_MAINMENU;
            transition::isTransitionActive = true;
            transition::resetTimer();

            timer3 = 0.f;
        }
    }

    static float animationProgress = 0.0f;
    static bool lastMuteState = false;

    if (*options_muted != lastMuteState) {
        animationProgress = 0.0f;
        lastMuteState = *options_muted;
    }

    if (animationProgress < 1.0f) {
        animationProgress += 0.03f;
        muteSliderThumbX = *options_muted ? EaseOutExpo(muteButtonX, muteButtonX + muteButtonWidth * 0.5f, animationProgress)
            : EaseOutExpo(muteButtonX + muteButtonWidth * 0.5f, muteButtonX, animationProgress);
    }

    if (Input::isButtonClicked(muteButtonX, muteButtonY, muteButtonWidth, muteButtonHeight))
    {
        if (AEInputCheckTriggered(AEVK_LBUTTON))
        {
            *options_muted = !*options_muted;
            AudioManager::ToggleMute(*options_muted);
        }
    }

    if (Input::isButtonClicked(sliderThumbX, sliderThumbY, sliderThumbWidth, sliderThumbHeight))
    {
        if (AEInputCheckCurr(AEVK_LBUTTON))
        {
            draggingSlider = true;
        }
    }

    if (draggingSlider)
    {
        sliderThumbX = mouseX - g_windowWidth / 2.0f - 10;
        if (sliderThumbX < sliderX - 50)
        {
            sliderThumbX = sliderX - 50;
        }
        if (sliderThumbX > sliderX + 50)
        {
            sliderThumbX = sliderX + 50;
        }
        targetVolume = (sliderThumbX - (sliderX - 50)) / 100;
    }
    else if (next_state != GS_MAINMENU) // Add this condition to prevent updating the volume after switching states
    {
        *options_volume = EaseAudioVolume(*options_volume, targetVolume, 0.05f);
        AudioManager::setVolume(*options_volume);
    }

    if (AEInputCheckReleased(AEVK_LBUTTON))
    {
        draggingSlider = false;
    }

    if (AEInputCheckTriggered(AEVK_F11))
    {
        // If the window close button has been clicked, set the game state to quit
        Global_ToggleScreen();
        std::cout << "Toggling Screen " << std::endl;
    }
}

void Options::draw()
{
    if (timer3 >= 1.f)
    {
        Rendering::RenderSprite(o_OptionsBackground, oCenterX, oCenterY, g_windowWidth, g_windowHeight, optionsMesh);
        Rendering::RenderSprite(o_ToggleMute, toggleMuteX, toggleMuteY, toggleMuteWidth, toggleMuteHeight, optionsMesh);
        Rendering::RenderSprite(o_VolumeSliderTEXT, VolumeSliderX, VolumeSliderY, VolumeSliderWidth, VolumeSliderHeight, optionsMesh);


        Rendering::RenderSprite(o_returnToMMTexture, returnToMMX11, returnToMMY11, buttonWidth11, buttonHeight11, optionsMesh);

        Rendering::RenderSprite(o_VolumeSlider, sliderX, sliderY, sliderWidth, sliderHeight, optionsMesh);
        Rendering::RenderSprite(o_VolumeSliderThumb, sliderThumbX, sliderThumbY, sliderThumbWidth, sliderThumbHeight, optionsMesh);

        // Render the stationary rounded rectangle
        if (muted)
        {
            Rendering::RenderSprite(o_RoundedRectangle_ACTIVE, muteButtonX, muteButtonY, muteButtonWidth, muteButtonHeight, optionsMesh);
        }
        else
        {
            Rendering::RenderSprite(o_RoundedRectangle_NOT_ACTIVE, muteButtonX, muteButtonY, muteButtonWidth, muteButtonHeight, optionsMesh);
        }

        // Render the muteSliderThumb
        Rendering::RenderSprite(o_MuteSliderThumb_NOT_ACTIVE, muteSliderThumbX, muteSliderThumbY, muteSliderThumbWidth, muteSliderThumbHeight, optionsMesh);
    }
}


void Options::free()
{
    AEGfxMeshFree(optionsMesh);
}

void Options::unload()
{
    AEGfxTextureUnload(o_RoundedRectangle_ACTIVE);
    AEGfxTextureUnload(o_RoundedRectangle_NOT_ACTIVE);

    AEGfxTextureUnload(o_MuteSliderThumb_ACTIVE);
    AEGfxTextureUnload(o_MuteSliderThumb_NOT_ACTIVE);

    AEGfxTextureUnload(o_VolumeSlider);
    AEGfxTextureUnload(o_VolumeSliderThumb);

    AEGfxTextureUnload(o_returnToMMTexture);

    AEGfxTextureUnload(o_ToggleMute);
    AEGfxTextureUnload(o_VolumeSliderTEXT);
    AEGfxTextureUnload(o_OptionsBackground);        
}
