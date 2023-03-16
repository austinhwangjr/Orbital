#include "AEEngine.h"
#include "MainMenu.h"
#include "Global.h"
#include "GameStateManager.h"
#include "MenuButtons.h"
#include <iostream>
#include "input.h"
#include "Graphics.h"

//extern s8 font1;

// Set the dimensions of each button
static float buttonWidth = 200.f;
static float buttonHeight = 50.f;

// Set the dimensions of each button for the hover state
static float hoverButtonWidth = 210.f;
static float hoverButtonHeight = 60.f;

// Define the positions for each button
static float startX = 0.0f;
static float startY = -25.f;

static float howToPlayX = 0.0f;
static float howToPlayY = -100.f;

static float highScoreX = 0.0f;
static float highScoreY = -175.f;

static float optionsX = 0.0f;
static float optionsY = -250.f;

static float creditsX = 0.0f;
static float creditsY = -325.f;

static float quitX = 0.0f;
static float quitY = -400.f;

// checking input area stuff
struct Button
{
    float x;
    float y;
    float width;
    float height;
};

struct HoverButton
{
    float width;
    float height;
};


// Define the positions and dimensions for each button
Button buttons[] = {
    {startX, startY, buttonWidth, buttonHeight},   // Start button
    {howToPlayX, howToPlayY, buttonWidth, buttonHeight},  // How to play button
    {highScoreX, highScoreY, buttonWidth, buttonHeight},    // high score button
    {optionsX, optionsY, buttonWidth, buttonHeight},  // Options button
    {creditsX, creditsY, buttonWidth, buttonHeight},  // Credits button
    {quitX, quitY, buttonWidth, buttonHeight}   // Quit button
};

// Define the hover dimensions for each button
HoverButton hoverButtons[] = {
    {hoverButtonWidth, hoverButtonHeight}, // Start button
    {hoverButtonWidth, hoverButtonHeight}, // How to play button
    {hoverButtonWidth, hoverButtonHeight}, // High score button
    {hoverButtonWidth, hoverButtonHeight}, // Options button
    {hoverButtonWidth, hoverButtonHeight}, // Credits button
    {hoverButtonWidth, hoverButtonHeight}  // Quit button
};


void Menu_Button::load( const char* startButtonFilename,
                        const char* howToPlayButtonFilename,
                        const char* highScoreButtonFilename,
                        const char* optionsButtonFilename,
                        const char* creditsButtonFilename,
                        const char* exitButtonFilename,

                        const char* startButtonHoverFilename,
                        const char* howToPlayButtonHoverFilename,
                        const char* highScoreButtonHoverFilename,
                        const char* optionsButtonHoverFilename,
                        const char* creditsButtonHoverFilename,
                        const char* exitButtonHoverFilename         )
{
    // Load the normal textures for each button
    normalButtonTextures[0]     = AEGfxTextureLoad(startButtonFilename);
    normalButtonTextures[1]     = AEGfxTextureLoad(howToPlayButtonFilename);
    normalButtonTextures[2]     = AEGfxTextureLoad(highScoreButtonFilename);
    normalButtonTextures[3]     = AEGfxTextureLoad(optionsButtonFilename);
    normalButtonTextures[4]     = AEGfxTextureLoad(creditsButtonFilename);
    normalButtonTextures[5]     = AEGfxTextureLoad(exitButtonFilename);

    // Load the hover textures for each button
    hoverButtonTextures[0]      = AEGfxTextureLoad(startButtonHoverFilename);
    hoverButtonTextures[1]      = AEGfxTextureLoad(howToPlayButtonHoverFilename);
    hoverButtonTextures[2]      = AEGfxTextureLoad(highScoreButtonHoverFilename);
    hoverButtonTextures[3]      = AEGfxTextureLoad(optionsButtonHoverFilename);
    hoverButtonTextures[4]      = AEGfxTextureLoad(creditsButtonHoverFilename);
    hoverButtonTextures[5]      = AEGfxTextureLoad(exitButtonHoverFilename);
}

void Menu_Button::init()
{
    for (int i = 0; i < 6; ++i)
    {
        hoverStates[i] = false;
    }
}

void Menu_Button::update()
{
    for (int i = 0; i < 6; ++i)
    {
        hoverStates[i] = Input::isMouseHover(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, hoverButtons[i].width, hoverButtons[i].height);
        if (AEInputCheckTriggered(AEVK_LBUTTON) && hoverStates[i])
        {
            switch (i)
            {
            case 0: // Start button
                next_state = GS_MAINLEVEL;
                break;
            case 1: // How to play button
                next_state = GS_HOWTOPLAY;
                break;
            case 2: // High Score button
                next_state = GS_HIGHSCORE;
                break;
            case 3: // Options button
                // Implement options logic here
                break;
            case 4: // Credits button
                next_state = GS_CREDITS;
                break;
            case 5: // Quit button
                next_state = GS_QUIT;
                break;
            default:
                break;
            }
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

void Menu_Button::draw(AEGfxVertexList* pMesh)
{
    for (int i = 0; i < 6; ++i)
    {
        if (hoverStates[i])
        {
            Rendering::RenderSprite(hoverButtonTextures[i], buttons[i].x, buttons[i].y, hoverButtons[i].width, hoverButtons[i].height, pMesh);
        }
        else
        {
            Rendering::RenderSprite(normalButtonTextures[i], buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, pMesh);
        }
    }
}

void Menu_Button::unload()
{
    for (int i = 0; i < 6; ++i)
    {
        AEGfxTextureUnload(normalButtonTextures[i]);
        AEGfxTextureUnload(hoverButtonTextures[i]);
    }
}

