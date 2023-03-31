/******************************************************************************/
/*!
\file		MenuButtons.cpp
\author 	Ong You Yang
\par    	email: youyang.o\@digipen.edu
\date   	March 28, 2023
\brief		This file contains the definition of functions for the menu buttons.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
/******************************************************************************/
#include "MenuButtons.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
float buttonClickDelay = 0.0f; 

static float normalTint[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white
static float hoverTint[4] = { 0.196f, 0.874f, 0.812f, 1.0f }; // #32dfcf
static float currentTints[6][4];

static float normalSquareSize = 15.0f;
static float hoverSquareSize = 25.0f;
static float currentSquareSizes[6];
static float currentXPositions[6];

static float hoverOffsetX = 30.0f; 

// Set the dimensions of each button
static float buttonWidth = 300.f;
static float buttonHeight = 100.f;

// Set the dimensions of each button for the hover state
static float hoverButtonWidth = 310.f;
static float hoverButtonHeight = 110.f;

static float startX = -530.0f;
static float startY = 40.f;

static float howToPlayX = -530.0f;
static float howToPlayY = -45.f;

static float highScoreX = -530.0f;
static float highScoreY = -120.f;

static float optionsX = -530.0f;
static float optionsY = -195.f;

static float creditsX = -530.0f;
static float creditsY = -270.f;

static float quitX = -530.0f;
static float quitY = -345.f;

float hoverSoundDelay = 0.0f;
float hoverSoundCooldown = 0.05f; 

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
Button buttons[] =
{
    {startX, startY, buttonWidth, buttonHeight},   // Start button
    {howToPlayX, howToPlayY, buttonWidth, buttonHeight},  // How to play button
    {highScoreX, highScoreY, buttonWidth, buttonHeight},    // high score button
    {optionsX, optionsY, buttonWidth, buttonHeight},  // Options button
    {creditsX, creditsY, buttonWidth, buttonHeight},  // Credits button
    {quitX, quitY, buttonWidth, buttonHeight}   // Quit button
};

// Define the hover dimensions for each button
HoverButton hoverButtons[] =
{
    {hoverButtonWidth, hoverButtonHeight}, // Start button
    {hoverButtonWidth, hoverButtonHeight}, // How to play button
    {hoverButtonWidth, hoverButtonHeight}, // High score button
    {hoverButtonWidth, hoverButtonHeight}, // Options button
    {hoverButtonWidth, hoverButtonHeight}, // Credits button
    {hoverButtonWidth, hoverButtonHeight}  // Quit button
};

Button currentButtonSizes[6];

/******************************************************************************/
/*!
	Load Textures and Data
*/
/******************************************************************************/
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
                        const char* exitButtonHoverFilename,
                        const char* squareTextureFilename)
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
    squareTexture = AEGfxTextureLoad(squareTextureFilename);

    AudioManager::LoadSound("Assets/BGM/hyperspace_jumping.mp3", false);
    AudioManager::LoadSound("Assets/BGM/button-124476.mp3", false);
}

/******************************************************************************/
/*!
	Initialize Variables
*/
/******************************************************************************/
void Menu_Button::init()
{
    for (int i = 0; i < 6; ++i)
    {
        hoverStates[i] = false;
        currentXPositions[i] = buttons[i].x;
        currentButtonSizes[i].width = buttons[i].width;
        currentButtonSizes[i].height = buttons[i].height;
        squareRotations[i] = 0.0f;
        currentSquareSizes[i] = normalSquareSize;

        for (int j = 0; j < 4; ++j)
        {
            currentTints[i][j] = normalTint[j];
        }
    }
}

/******************************************************************************/
/*!
	Update Menu Buttons
*/
/******************************************************************************/
void Menu_Button::update()
{
    hoverSoundDelay += static_cast<float>(g_dt);

    for (int i = 0; i < 6; ++i)
    {
        bool previousHoverState = hoverStates[i];
        hoverStates[i] = Input::isMouseHover(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, hoverButtons[i].width, hoverButtons[i].height);

        if (!previousHoverState && hoverStates[i] && hoverSoundDelay >= hoverSoundCooldown)
        {
            AudioManager::PlayOnce("Assets/BGM/button-124476.mp3", 0.2f);
            hoverSoundDelay = 0.0f;
        }

        if (AEInputCheckTriggered(AEVK_LBUTTON) && hoverStates[i])
        {
            AudioManager::StopBGMIfPlaying();
            AudioManager::PlayOnce("Assets/BGM/hyperspace_jumping.mp3", 0.3f);
            transition::isTransitionActive = true;
            transition::resetTimer();

            switch (i)
            {
            case 0: // Start button
                next_state = GS_MAINLEVEL;
                break;
            case 1: // How to play button
                main_menu::currentState = main_menu::HOW_TO_PLAY;
                break;
            case 2: // High Score button
                next_state = GS_HIGHSCORE;
                break;
            case 3: // Options button
                next_state = GS_OPTIONS;
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

            MMtotal_time = 0.f;
            std::cout << "Button " << i << " clicked" << std::endl;
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

    float easingSpeed = 0.15f; // Adjust this value to control the speed of the transition i know not suppose to be here.
    float rotationSpeed = 0.25f; // Adjust this value to control the speed of the rotation
    for (int i = 0; i < 6; ++i)
    {
        // Update hover state and square rotation
        hoverStates[i] = Input::isMouseHover(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, hoverButtons[i].width, hoverButtons[i].height);

        if (hoverStates[i])
        {
            squareRotations[i] -= rotationSpeed;
            if (squareRotations[i] < -M_PI / 2.0f)
            {
                (squareRotations[i] = static_cast<float>( - M_PI / 2.0f));
            }

            // colour easing
            for (int j = 0; j < 4; ++j)
            {
                currentTints[i][j] = Lerp(currentTints[i][j], hoverTint[j], easingSpeed);
            }

            // width & height 
            currentButtonSizes[i].width = Lerp(currentButtonSizes[i].width, hoverButtons[i].width, easingSpeed);
            currentButtonSizes[i].height = Lerp(currentButtonSizes[i].height, hoverButtons[i].height, easingSpeed);

            // sqaure size
            currentSquareSizes[i] = Lerp(currentSquareSizes[i], hoverSquareSize, easingSpeed);

            currentXPositions[i] = Lerp(currentXPositions[i], buttons[i].x + hoverOffsetX, easingSpeed);
        }

        else
        {
            squareRotations[i] += rotationSpeed;
            if (squareRotations[i] > 0.0f)
            {
                squareRotations[i] = 0.0f;
            }

            // colour easing
            for (int j = 0; j < 4; ++j)
            {
                currentTints[i][j] = Lerp(currentTints[i][j], normalTint[j], easingSpeed);
            }

            // width & height 
            currentButtonSizes[i].width = Lerp(currentButtonSizes[i].width, buttons[i].width, easingSpeed);
            currentButtonSizes[i].height = Lerp(currentButtonSizes[i].height, buttons[i].height, easingSpeed);

            currentSquareSizes[i] = Lerp(currentSquareSizes[i], normalSquareSize, easingSpeed);

            currentXPositions[i] = Lerp(currentXPositions[i], buttons[i].x, easingSpeed);
        }
    }
    AudioManager::Update();
}

/******************************************************************************/
/*!
	Draw Menu Buttons
*/
/******************************************************************************/
void Menu_Button::draw(AEGfxVertexList* pMesh)
{
    float offsetX = -180.0f;

    for (int i = 0; i < 6; ++i)
    {
        if (hoverStates[i])
        {
            Rendering::RenderSprite(hoverButtonTextures[i], currentXPositions[i], buttons[i].y, currentButtonSizes[i].width, currentButtonSizes[i].height, pMesh);
        }
        else
        {
            Rendering::RenderSprite(normalButtonTextures[i], currentXPositions[i], buttons[i].y, currentButtonSizes[i].width, currentButtonSizes[i].height, pMesh);
        }
        Rendering::RenderSpriteWithRotations(squareTexture, buttons[i].x + offsetX, buttons[i].y, currentSquareSizes[i], currentSquareSizes[i], pMesh, squareRotations[i], currentTints[i][0], currentTints[i][1], currentTints[i][2], currentTints[i][3]);
    }
}

void Menu_Button::free()
{
    AudioManager::UnloadAllSounds();
}

/******************************************************************************/
/*!
	Free Textures
*/
/******************************************************************************/
void Menu_Button::unload()
{
    for (int i = 0; i < 6; ++i)
    {
        AEGfxTextureUnload(normalButtonTextures[i]);
        AEGfxTextureUnload(hoverButtonTextures[i]);
    }
    AEGfxTextureUnload(squareTexture);
}
