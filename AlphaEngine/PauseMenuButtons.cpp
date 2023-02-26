//#include "PauseMenuButtons.h"
//#include "Global.h"
//#include "AEEngine.h"
//#include "GameStateManager.h"
//#include "Input.h"
//#include "Graphics.h"
//#include "MainLevel.h"
//
////PauseMenuButtons::PauseMenuButtons()
////    : m_pMeshPauseButton(nullptr)
////    , m_pTexPauseButton(nullptr)
////    , m_pMeshResumeButton(nullptr)
////    , m_pTexResumeButton(nullptr)
////    , m_pMeshMainMenuButton(nullptr)
////    , m_pTexMainMenuButton(nullptr)
////    , m_pauseSelected(false)
////    , m_resumeSelected(false)
////    , m_mainMenuSelected(false)
////{
////    m_pauseButtonPos = { 300.0f, 350.0f };
////    m_resumeButtonPos = { 300.0f, 250.0f };
////    m_mainMenuButtonPos = { 300.0f, 150.0f };
////}
////
////PauseMenuButtons::~PauseMenuButtons()
////{
////    Free();
////}
//
//void PauseMenuButtons::load(const char* resumeButtonFilename,
//                            const char* restartButtonFilename,
//                            const char* exitMainMenuFilename)
//{
//    resumeTexture = AEGfxTextureLoad(resumeButtonFilename);
//    restartTexture = AEGfxTextureLoad(restartButtonFilename);
//    exitMainMenuTexture = AEGfxTextureLoad(exitMainMenuFilename);
//}
//
//void PauseMenuButtons::init()
//{
////empty
//}
//
//void PauseMenuButtons::update()
//{
//    // Get mouse position
//    float mouseX, mouseY;
//    AEInputGetCursorPosition(&mouseX, &mouseY);
//
//    // Check if the mouse is hovering over a button
//    if (AEInputCheckCurr(VK_LBUTTON))
//    {
//        // Check pause button
//        if (Input::isButtonClicked(m_pauseButtonPos.x, m_pauseButtonPos.y, 100.0f, 100.0f))
//        {
//            m_pauseSelected = true;
//        }
//
//        // Check resume button
//        if (Input::isButtonClicked(m_resumeButtonPos.x, m_resumeButtonPos.y, 100.0f, 100.0f))
//        {
//            m_resumeSelected = true;
//        }
//
//        // Check main menu button
//        if (Input::isButtonClicked(m_mainMenuButtonPos.x, m_mainMenuButtonPos.y, 100.0f, 100.0f))
//        {
//            m_mainMenuSelected = true;
//        }
//    }
//}
//
//
//void PauseMenuButtons::draw(AEGfxVertexList* pMeshP)
//{
//    // Draw pause button
//    AEVec2 pos = m_pauseButtonPos;
//    if (m_pauseSelected)
//    {
//        Rendering::RenderSprite(m_pTexPauseButton, pos.x, pos.y, 100.0f, 100.0f, m_pMeshPauseButtonSelected);
//    }
//    else
//    {
//        Rendering::RenderSprite(m_pTexPauseButton, pos.x, pos.y, 100.0f, 100.0f, m_pMeshPauseButton);
//    }
//
//    // Draw resume button
//    pos = m_resumeButtonPos;
//    if (m_resumeSelected)
//    {
//        Rendering::RenderSprite(m_pTexResumeButton, pos.x, pos.y, 100.0f, 100.0f, m_pMeshResumeButtonSelected);
//    }
//    else
//    {
//        Rendering::RenderSprite(m_pTexResumeButton, pos.x, pos.y, 100.0f, 100.0f, m_pMeshResumeButton);
//    }
//
//    // Draw main menu button
//    pos = m_mainMenuButtonPos;
//    if (m_mainMenuSelected)
//    {
//        Rendering::RenderSprite(m_pTexMainMenuButton, pos.x, pos.y, 100.0f, 100.0f, m_pMeshMainMenuButtonSelected);
//    }
//    else
//    {
//        Rendering::RenderSprite(m_pTexMainMenuButton, pos.x, pos.y, 100.0f, 100.0f, m_pMeshMainMenuButton);
//    }
//}
//
//
//void PauseMenuButtons::free()
//{
//    if (m_pMeshPauseButton)
//    {
//        AEGfxMeshFree(m_pMeshPauseButton);
//        m_pMeshPauseButton = nullptr;
//    }
//
//    if (m_pTexPauseButton)
//    {
//        AEGfxTextureUnload(m_pTexPauseButton);
//        m_pTexPauseButton = nullptr;
//    }
//
//    if (m_pMeshResumeButton)
//    {
//        AEGfxMeshFree(m_pMeshResumeButton);
//        m_pMeshResumeButton = nullptr;
//    }
//
//    if (m_pTexResumeButton)
//    {
//        AEGfxTextureUnload(m_pTexResumeButton);
//        m_pTexResumeButton = nullptr;
//    }
//
//    if (m_pMeshMainMenuButton)
//    {
//        AEGfxMeshFree(m_pMeshMainMenuButton);
//        m_pMeshMainMenuButton = nullptr;
//    }
//
//    if (m_pTexMainMenuButton)
//    {
//        AEGfxTextureUnload(m_pTexMainMenuButton);
//        m_pTexMainMenuButton = nullptr;
//    }
//}
//
//void PauseMenuButtons::unload()
//{
//    AEGfxTextureUnload(resumeTexture);
//    AEGfxTextureUnload(restartTexture);
//    AEGfxTextureUnload(exitMainMenuTexture);
//}
