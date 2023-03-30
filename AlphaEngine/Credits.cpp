/******************************************************************************/
/*!
\file		Credits.cpp
\author 	 
\par    	email: \@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the credits 
            screen.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "AEEngine.h"
#include <iostream>
#include "Global.h"
#include "GameStateManager.h"
#include "MainMenu.h"
#include "MenuButtons.h"
#include "Graphics.h"
#include "Credits.h"
#include "Input.h"
#include "Easing.h"
#include "Transition.h"
#include "AudioManager.h"

AEGfxTexture* texCreditsBackground = nullptr;
AEGfxTexture* texCreditsBackground2 = nullptr;
AEGfxTexture* texReturnToMMfromCredits = nullptr;
AEGfxTexture* texReturnToMMfromCredits2 = nullptr;

AEGfxTexture* texSlider = nullptr;
AEGfxTexture* texSliderThumb = nullptr;

AEGfxVertexList* pMeshCreditsBackground;
AEGfxVertexList* pMeshCredits;

Rendering createMeshCredits;
Rendering renderCredits;

static float timer2 = 0.0f;

const int buttonCount = 1;

bool hoverStates2[buttonCount];
float buttonScales2[buttonCount];
float targetButtonScales2[buttonCount];

// Set the dimensions of each button
static float buttonWidth2 = 50.f;
static float buttonHeight2 = 50.f;

static float hoverButtonWidth2 = 60.f;
static float hoverButtonHeight2 = 60.f;

static float creditsBGX = 0.0f;
static float creditsBGY = 0.0f;

static float creditsBGX2 = 0.0f;
static float creditsBGY2 = -900.0f;

static float returnToMMfromCreditsX = -750.0f;
static float returnToMMfromCreditsY = 400.0f;

static float sliderX = 780.0f;
static float sliderY = 0.0f;
static float sliderWidth = 10.0f;
static float sliderHeight = 800.0f;

static float sliderThumbX = sliderX;
static float sliderThumbY = 0.0f;
static float sliderThumbWidth = 20.0f;

static float contentHeight = 900.0f;
static float visibleHeight = 450.0f;
static float sliderThumbHeight = sliderHeight * (visibleHeight / contentHeight);

static float targetCreditsBGY = 0.0f;
static float easingProgress = 1.0f;
static float easingDuration = 0.5f;
credits::Button2 buttons2[] = {
    {returnToMMfromCreditsX, returnToMMfromCreditsY, buttonWidth2, buttonHeight2},
};

credits::HoverButton2 hoverButtons2[] = {
    {hoverButtonWidth2, hoverButtonHeight2}
};

void credits::load()
{
    texCreditsBackground = AEGfxTextureLoad("Assets/MainMenu/Credits/c_credits1.png");
    texCreditsBackground2 = AEGfxTextureLoad("Assets/MainMenu/Credits/c_credits2.png");
    texReturnToMMfromCredits = AEGfxTextureLoad("Assets/MainMenu/Credits/home_icon.png");
    texReturnToMMfromCredits2 = AEGfxTextureLoad("Assets/MainMenu/Credits/home_icon2.png");

    texSlider = AEGfxTextureLoad("Assets/MainMenu/Credits/c_Slider.png");
    texSliderThumb = AEGfxTextureLoad("Assets/MainMenu/Credits/c_SliderThumb.png");

    AudioManager::LoadSound("Assets/BGM/hyperspace_jump.mp3", false);
}

void credits::init()
{
    for (int i = 0; i < buttonCount; ++i)
    {
        buttonScales2[i] = 1.0f;
        targetButtonScales2[i] = 1.0f;
    }

    createMeshCredits.BackgroundMesh(pMeshCreditsBackground);
    createMeshCredits.SquareMesh(pMeshCredits);

    AE_ASSERT_MESG(pMeshCreditsBackground, "Error: Failed to create pMeshCreditsBackground in Credits.cpp!");
    AE_ASSERT_MESG(pMeshCredits, "Error: Failed to create pMeshCredits in Credits.cpp!");
}

void credits::update()
{
    timer2 += AEFrameRateControllerGetFrameTime();

    for (int i = 0; i < buttonCount; ++i)
    {
        hoverStates2[i] = Input::isMouseHover(buttons2[i].x, buttons2[i].y, buttons2[i].width, buttons2[i].height, hoverButtons2[i].width, hoverButtons2[i].height);

        if (AEInputCheckTriggered(AEVK_LBUTTON) && hoverStates2[i])
        {
            AudioManager::PlayOneShot("Assets/BGM/hyperspace_jump.mp3", 1.0f);

            next_state = GS_MAINMENU;
            transition::isTransitionActive = true;
            transition::resetTimer();
            timer2 = 0.0f;
        }

        if (hoverStates2[i])
        {
            targetButtonScales2[i] = hoverButtonWidth2 / buttonWidth2;
        }
        else
        {
            targetButtonScales2[i] = 1.0f;
        }

        if (buttonScales2[i] != targetButtonScales2[i])
        {
            easingProgress = 0.0f;
        }

        if (easingProgress < 1.0f)
        {
            // Update easing progress
            easingProgress += (1.0f / 240.0f) / easingDuration;
            if (easingProgress > 1.0f) easingProgress = 1.0f;

            // Apply easing to button scale
            buttonScales2[i] = EaseOutExpo(buttonScales2[i], targetButtonScales2[i], easingProgress);
        }
    }

    float scrollRatio = 1.0f - (creditsBGY / (contentHeight - visibleHeight));
    sliderThumbY = scrollRatio * (sliderHeight - sliderThumbHeight) + (scrollRatio) * (sliderY);
    sliderThumbX = sliderX - (sliderThumbWidth - sliderWidth) / 2.0f;

    int mouseWheelDelta = Input::GetMouseWheelDelta();
    if (mouseWheelDelta != 0)
    {
        float scrollSpeed = 70.0f;
        targetCreditsBGY -= mouseWheelDelta / (float)WHEEL_DELTA * scrollSpeed;

        // Limit the scrolling range
        if (targetCreditsBGY < 0.0f)
        {
            targetCreditsBGY = 0.0f;
        }
        if (targetCreditsBGY > 900.0f)
        {
            targetCreditsBGY = 900.0f;
        }

        // Reset easing progress
        easingProgress = 0.0f;
    }

    if (easingProgress < 1.0f)
    {
        // Update easing progress
        easingProgress += (1.0f / 240.0f) / easingDuration; //actually 60fps so 60.0f but 240.0f is smoother soo....
        if (easingProgress > 1.0f) easingProgress = 1.0f;

        // Apply easing to background position
        creditsBGY = EaseOutExpo(creditsBGY, targetCreditsBGY, easingProgress);
        creditsBGY2 = creditsBGY - 900.0f;
    }

    if (AEInputCheckTriggered(AEVK_F11))
    {
        // If the window close button has been clicked, set the game state to quit
        Global_ToggleScreen();
        std::cout << "Toggling Screen " << std::endl;
    }

    // Check if the window close button has been clicked
    if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
    {
        // If the window close button has been clicked, set the game state to quit
        next_state = GS_QUIT;
    }
}

void credits::draw()
{
    if (timer2 >= 1.f)
    {
        for (int i = 0; i < buttonCount; ++i)
        {
            float currentButtonWidth = buttonWidth2 * buttonScales2[i];
            float currentButtonHeight = buttonHeight2 * buttonScales2[i];

            if (hoverStates2[i])
            {
                renderCredits.RenderSprite(texReturnToMMfromCredits2, returnToMMfromCreditsX, returnToMMfromCreditsY, currentButtonWidth, currentButtonHeight, pMeshCredits);
            }
            else
            {
                renderCredits.RenderSprite(texReturnToMMfromCredits, returnToMMfromCreditsX, returnToMMfromCreditsY, currentButtonWidth, currentButtonHeight, pMeshCredits);
            }
        }

        renderCredits.RenderSprite(texCreditsBackground2, 0.f, creditsBGY2, 800.f, 450.f, pMeshCreditsBackground);
        renderCredits.RenderSprite(texCreditsBackground, 0.f, creditsBGY, 800.f, 450.f, pMeshCreditsBackground);

        renderCredits.RenderSprite(texSlider, sliderX, sliderY, sliderWidth, sliderHeight, pMeshCredits);
        renderCredits.RenderSprite(texSliderThumb, sliderThumbX, sliderThumbY, sliderThumbWidth, 20, pMeshCredits);
    }
}

void credits::free()
{
    AEGfxMeshFree(pMeshCreditsBackground);
    AEGfxMeshFree(pMeshCredits);

}

void credits::unload()
{
    AEGfxTextureUnload(texCreditsBackground);
    AEGfxTextureUnload(texCreditsBackground2);
    AEGfxTextureUnload(texReturnToMMfromCredits);
    AEGfxTextureUnload(texReturnToMMfromCredits2);
    AEGfxTextureUnload(texSlider);
    AEGfxTextureUnload(texSliderThumb);
}