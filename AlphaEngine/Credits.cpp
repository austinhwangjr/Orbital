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


AEGfxTexture* texCreditsBackground = nullptr;
AEGfxTexture* texCreditsBackground2 = nullptr;
AEGfxTexture* texReturnToMMfromCredits = nullptr;
AEGfxTexture* texSlider = nullptr;
AEGfxTexture* texSliderThumb = nullptr;

// Set the dimensions of each button
static float buttonWidth2 = 200.f;
static float buttonHeight2 = 50.f;

AEGfxVertexList* pMeshCreditsBackground;
AEGfxVertexList* pMeshCredits;

Rendering createMeshCredits;
Rendering renderCredits;

static float creditsBGX = 0.0f;
static float creditsBGY = 0.0f;

static float creditsBGX2 = 0.0f;
static float creditsBGY2 = -900.0f;

static float returnToMMfromCreditsX = 0.0f;
static float returnToMMfromCreditsY = -400.0f;

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
// Define the positions and dimensions for each button
credits::Button2 buttons2[] = {
	{returnToMMfromCreditsX, returnToMMfromCreditsY, buttonWidth2, buttonHeight2},
};

void credits::load()
{
	texCreditsBackground = AEGfxTextureLoad("Assets/MainMenu/Credits/c_credits1.png");
    texCreditsBackground2 = AEGfxTextureLoad("Assets/MainMenu/Credits/c_credits2.png");
	texReturnToMMfromCredits = AEGfxTextureLoad("Assets/MainMenu/Credits/c_ExitButtonCredits.png");
    texSlider = AEGfxTextureLoad("Assets/MainMenu/Credits/c_Slider.png");
    texSliderThumb = AEGfxTextureLoad("Assets/MainMenu/Credits/c_SliderThumb.png");
}

void credits::init()
{

	createMeshCredits.BackgroundMesh(pMeshCreditsBackground);
	createMeshCredits.SquareMesh(pMeshCredits);

	AE_ASSERT_MESG(pMeshCreditsBackground, "Error: Failed to create pMeshCreditsBackground in Credits.cpp!");
	AE_ASSERT_MESG(pMeshCredits, "Error: Failed to create pMeshCredits in Credits.cpp!");
}

void credits::update()
{
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

    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        if (Input::isButtonClicked(buttons2[0].x, buttons2[0].y, buttons2[0].width, buttons2[0].height))
        {

            next_state = GS_MAINMENU;
        }
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
    renderCredits.RenderSprite(texCreditsBackground2, 0.f, creditsBGY2, 800.f, 450.f, pMeshCreditsBackground);
    renderCredits.RenderSprite(texCreditsBackground, 0.f, creditsBGY, 800.f, 450.f, pMeshCreditsBackground);

    renderCredits.RenderSprite(texReturnToMMfromCredits, returnToMMfromCreditsX, returnToMMfromCreditsY, buttonWidth2, buttonHeight2, pMeshCredits);
    renderCredits.RenderSprite(texSlider, sliderX, sliderY, sliderWidth, sliderHeight, pMeshCredits);
    renderCredits.RenderSprite(texSliderThumb, sliderThumbX, sliderThumbY, sliderThumbWidth, 20, pMeshCredits);
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
    AEGfxTextureUnload(texSlider);
    AEGfxTextureUnload(texSliderThumb);
}
