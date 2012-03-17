//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "intro.xaml.h"

using namespace appm;

using namespace Platform;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::Graphics::Display;

//----------------------------------------------------------------------
intro::intro():
    m_pauseRequested(false),
    m_pressComplete(false),
    m_renderNeeded(false),
    m_haveFocus(false)
{
    InitializeComponent();
}
//----------------------------------------------------------------------
intro::~intro()
{
    CompositionTarget::Rendering::remove(m_eventToken);
}
//----------------------------------------------------------------------
void intro::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ args)
{
    m_mainPage = ref new main(this);

    Window::Current->Content = m_mainPage;
    Window::Current->Activated += ref new WindowActivatedEventHandler(this, &intro::OnWindowActivationChanged);
    Window::Current->Activate();

/*    // create the game and pass to window and root panel for swap chain setup
    m_controller = ref new MoveLookController();
    m_controller->Initialize(Window::Current->CoreWindow);*/
    
    m_game = ref new appm();
    m_game->Initialize(Window::Current->CoreWindow, m_mainPage, DisplayProperties::LogicalDpi, m_controller);

    m_eventToken = CompositionTarget::Rendering::add(ref new EventHandler<Object^>(this, &intro::OnRendering));

    ApplicationView::GetForCurrentView()->ViewStateChanged +=
        ref new TypedEventHandler<ApplicationView^, ApplicationViewStateChangedEventArgs^>(
            this,
            &intro::OnViewStateChanged
            );

    CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &intro::OnSuspending);
    CoreApplication::Resuming += ref new EventHandler<Object^>(this, &intro::OnResuming);

    Window::Current->CoreWindow->SizeChanged += 
        ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &intro::OnWindowSizeChanged);

    DisplayProperties::LogicalDpiChanged +=
        ref new DisplayPropertiesEventHandler(this, &intro::OnLogicalDpiChanged);

    InitializeGameState();
}
//----------------------------------------------------------------------
void intro::OnRendering(
    _In_ Object^ sender, 
    _In_ Object^ args
    )
{
    Update();
    if (m_updateState == UpdateEngineState::Dynamics  || m_renderNeeded)
    {
        m_game->Render();
        m_renderNeeded = false;
    }
}
//--------------------------------------------------------------------------------------
void intro::OnWindowSizeChanged(
    _In_ CoreWindow^ sender,
    _In_ WindowSizeChangedEventArgs^ args
    )
{
    m_renderNeeded = true;
    m_game->UpdateForWindowSizeChange();
}
//--------------------------------------------------------------------------------------
void intro::OnLogicalDpiChanged(
    _In_ Object^ sender
    )
{
    m_game->SetDpi(DisplayProperties::LogicalDpi);
}
//--------------------------------------------------------------------------------------
void intro::InitializeGameState()
{
    //
    // Set up the initial state machine for handling Game playing state
    //
    if (m_game->GameActive() && m_game->LevelActive())
    {
        // The last time the game terminated it was in the middle 
        // of a level. 
        // We are waiting for the user to continue the game
        m_updateState = UpdateEngineState::WaitingForResources;
        m_pressResult = PressResultState::ContinueLevel;
        SetGameInfoOverlay(GameInfoOverlayState::Pause);
    }
    else if (!m_game->GameActive() && (m_game->HighScore().totalHits > 0))
    {
        // The last time the game terminated the game had been completed.
        // Show the high score.
        // We are waiting for the user to start a new game.
        m_updateState = UpdateEngineState::WaitingForResources;
        m_pressResult = PressResultState::LoadGame;
        SetGameInfoOverlay(GameInfoOverlayState::GameStats);
    }
    else
    {
        // This is either the first time the game has run or
        // the last time the game terminated the level was completed.
        // We are waiting for the user to begin the next level.
        m_updateState = UpdateEngineState::WaitingForResources;
        m_pressResult = PressResultState::PlayLevel;
        SetGameInfoOverlay(GameInfoOverlayState::LevelStart);
    }
    SetAction(GameInfoOverlayCommand::PleaseWait);
    ShowGameInfoOverlay();
}
//--------------------------------------------------------------------------------------
void intro::Update()
{
    m_controller->Update();

    switch (m_updateState)
    {
    case UpdateEngineState::WaitingForResources:
        if (m_game->IsResourceLoadingComplete())
        {
            switch (m_pressResult)
            {
            case PressResultState::LoadGame:
                SetGameInfoOverlay(GameInfoOverlayState::GameStats);
                break;

            case PressResultState::PlayLevel:
                SetGameInfoOverlay(GameInfoOverlayState::LevelStart);
                break;

            case PressResultState::ContinueLevel:
                SetGameInfoOverlay(GameInfoOverlayState::Pause);
                break;
            }
            m_updateState = UpdateEngineState::WaitingForPress;
            SetAction(GameInfoOverlayCommand::TapToContinue);
            m_controller->WaitForPress();
            ShowGameInfoOverlay();
            m_renderNeeded = true;
        }
        break;

    case UpdateEngineState::WaitingForPress:
        if (m_controller->IsPressComplete() || m_pressComplete)
        {
            m_pressComplete = false;

            switch (m_pressResult)
            {
            case PressResultState::LoadGame:
                m_updateState = UpdateEngineState::WaitingForResources;
                m_pressResult = PressResultState::PlayLevel;
                m_controller->Active(false);
                m_game->LoadGame();
                SetAction(GameInfoOverlayCommand::PleaseWait);
                SetGameInfoOverlay(GameInfoOverlayState::LevelStart);
                ShowGameInfoOverlay();
                break;

            case PressResultState::PlayLevel:
                m_updateState = UpdateEngineState::Dynamics;
                HideGameInfoOverlay();
                m_controller->Active(true);
                m_game->StartLevel();
                break;

            case PressResultState::ContinueLevel:
                m_updateState = UpdateEngineState::Dynamics;
                HideGameInfoOverlay();
                m_controller->Active(true);
                m_game->ContinueGame();
                break;
            }
        }
        break;

    case UpdateEngineState::Dynamics:
        if (m_controller->IsPauseRequested() || m_pauseRequested)
        {
            m_pauseRequested = false;

            m_game->PauseGame();
            SetGameInfoOverlay(GameInfoOverlayState::Pause);
            SetAction(GameInfoOverlayCommand::TapToContinue);
            m_updateState = UpdateEngineState::WaitingForPress;
            m_pressResult = PressResultState::ContinueLevel;
            ShowGameInfoOverlay();
        }
        else 
        {
            GameState runState = m_game->RunGame();
            switch (runState)
            {
            case GameState::TimeExpired:
                SetAction(GameInfoOverlayCommand::TapToContinue);
                SetGameInfoOverlay(GameInfoOverlayState::GameOverExpired);
                ShowGameInfoOverlay();
                m_updateState = UpdateEngineState::WaitingForPress;
                m_pressResult = PressResultState::LoadGame;
                break;

            case GameState::LevelComplete:
                SetAction(GameInfoOverlayCommand::PleaseWait);
                SetGameInfoOverlay(GameInfoOverlayState::LevelStart);
                ShowGameInfoOverlay();
                m_updateState = UpdateEngineState::WaitingForResources;
                m_pressResult = PressResultState::PlayLevel;
                break;

            case GameState::GameComplete:
                SetAction(GameInfoOverlayCommand::TapToContinue);
                SetGameInfoOverlay(GameInfoOverlayState::GameOverCompleted);
                ShowGameInfoOverlay();
                m_updateState  = UpdateEngineState::WaitingForPress;
                m_pressResult = PressResultState::LoadGame;
                break;
            }
        }

        if (m_updateState == UpdateEngineState::WaitingForPress)
        {
            // transitioning state, so enable waiting for the press event
            m_controller->WaitForPress();
        }
        break;
    }
}
//--------------------------------------------------------------------------------------
void intro::OnWindowActivationChanged(
    _In_ Platform::Object^ sender,
    _In_ Windows::UI::Core::WindowActivatedEventArgs^ args
    )
{
    if (args->WindowActivationState == CoreWindowActivationState::Deactivated)
    {
        m_haveFocus = false;

        switch (m_updateState)
        {
        case UpdateEngineState::Dynamics:
            // From Dynamic mode, when coming out of Deactivated rather than going directly back into game play
            // go to the paused state waiting for user input to continue
            m_updateStateNext = UpdateEngineState::WaitingForPress;
            m_pressResult = PressResultState::ContinueLevel;
            SetGameInfoOverlay(GameInfoOverlayState::Pause);
            ShowGameInfoOverlay();
            m_game->PauseGame();
            m_updateState = UpdateEngineState::Deactivated;
            SetAction(GameInfoOverlayCommand::None);
            m_renderNeeded = true;
            break;

        case UpdateEngineState::WaitingForResources:
        case UpdateEngineState::WaitingForPress:
            m_updateStateNext = m_updateState;
            m_updateState = UpdateEngineState::Deactivated;
            SetAction(GameInfoOverlayCommand::None);
            ShowGameInfoOverlay();
            m_renderNeeded = true;
            break;
        }
    }
    else if (args->WindowActivationState == CoreWindowActivationState::CodeActivated 
        || args->WindowActivationState == CoreWindowActivationState::PointerActivated)
    {
        m_haveFocus = true;

        if (m_updateState == UpdateEngineState::Deactivated)
        {
            m_updateState = m_updateStateNext;

            if (m_updateState == UpdateEngineState::WaitingForPress)
            {
                SetAction(GameInfoOverlayCommand::TapToContinue);
                m_controller->WaitForPress();
            }
            else if (m_updateStateNext == UpdateEngineState::WaitingForResources)
            {
                SetAction(GameInfoOverlayCommand::PleaseWait);
            }
        }
    }
}
//--------------------------------------------------------------------------------------
void intro::OnSuspending(
    _In_ Platform::Object^ sender, 
    _In_ SuspendingEventArgs^ args
    )
{
    // Save application state.
    // If your application needs time to complete a lengthy operation, it can request a deferral.
    // The SuspendingOperation has a deadline time. Make sure all your operations are complete by that time!
    // If the app doesn't return from this handler within five seconds, it will be terminated.
    SuspendingOperation^ op = args->SuspendingOperation;
    SuspendingDeferral^ deferral = op->GetDeferral();

    switch (m_updateState)
    {
    case UpdateEngineState::Dynamics:
       // Game is in the active game play state, Stop Game Timer and Pause play and save state
        SetAction(GameInfoOverlayCommand::None);
        SetGameInfoOverlay(GameInfoOverlayState::Pause);
        m_updateStateNext = UpdateEngineState::WaitingForPress;
        m_pressResult = PressResultState::ContinueLevel;
        m_game->PauseGame();
        break;

    case UpdateEngineState::WaitingForResources:
    case UpdateEngineState::WaitingForPress:
        m_updateStateNext = m_updateState;
        break;

    default:
        // any other state don't save as next state as they are trancient states and have already set m_updateStateNext
        break;
    }
    m_updateState = UpdateEngineState::Suspended;

    m_controller->Active(false);
    m_game->OnSuspending();

    deferral->Complete();
}
//--------------------------------------------------------------------------------------
void intro::OnResuming(
    _In_ Platform::Object^ sender,
    _In_ Platform::Object^ args
    )
{
    if (m_haveFocus)
    {
        m_updateState = m_updateStateNext;
    }
    else
    {
        m_updateState = UpdateEngineState::Deactivated;
    }

    if (m_updateState == UpdateEngineState::WaitingForPress)
    {
        SetAction(GameInfoOverlayCommand::TapToContinue);
        m_controller->WaitForPress();
    }
    m_game->OnResuming();
    ShowGameInfoOverlay();
    m_renderNeeded = true;
}
//--------------------------------------------------------------------------------------
void intro::OnViewStateChanged(
     _In_ ApplicationView^ view, 
     _In_ ApplicationViewStateChangedEventArgs^ args
     )
{
    m_renderNeeded = true;

    if (args->ViewState == ApplicationViewState::Snapped)
    {
        switch (m_updateState)
        {
        case UpdateEngineState::Dynamics:
            // From Dynamic mode, when coming out of SNAPPED layout rather than going directly back into game play
            // go to the paused state waiting for user input to continue
            m_updateStateNext = UpdateEngineState::WaitingForPress;
            m_pressResult = PressResultState::ContinueLevel;
            SetGameInfoOverlay(GameInfoOverlayState::Pause);
            SetAction(GameInfoOverlayCommand::TapToContinue);
            m_game->PauseGame();
            break;

        case UpdateEngineState::WaitingForResources:
        case UpdateEngineState::WaitingForPress:
            // Avoid corrupting the m_updateStateNext on a transition from Snapped -> Snapped.
            // Otherwise just cache the current state and return to it when leaving SNAPPED layout

            m_updateStateNext = m_updateState;
            break;

        default:
            break;
        }

        m_updateState = UpdateEngineState::Snapped;
        m_controller->Active(false);
        HideGameInfoOverlay();
        SetSnapped();
    }
    else if (args->ViewState == ApplicationViewState::Filled ||
        args->ViewState == ApplicationViewState::FullScreenLandscape ||
        args->ViewState == ApplicationViewState::FullScreenPortrait)
    {
        if (m_updateState == UpdateEngineState::Snapped)
        {

            HideSnapped();
            ShowGameInfoOverlay();
            m_renderNeeded = true;

            if (m_haveFocus)
            {
                if (m_updateStateNext == UpdateEngineState::WaitingForPress)
                {
                    SetAction(GameInfoOverlayCommand::TapToContinue);
                    m_controller->WaitForPress();
                }
                else if (m_updateStateNext == UpdateEngineState::WaitingForResources)
                {
                    SetAction(GameInfoOverlayCommand::PleaseWait);
                }

                m_updateState = m_updateStateNext;
            }
            else
            {
                m_updateState = UpdateEngineState::Deactivated;
                SetAction(GameInfoOverlayCommand::None);
            }
        }
    }
}
//--------------------------------------------------------------------------------------
void intro::SetGameInfoOverlay(GameInfoOverlayState state)
{
    m_gameInfoOverlayState = state;
    switch (state)
    {
    case GameInfoOverlayState::GameStats:
        m_mainPage->SetGameStats(
            m_game->HighScore().levelCompleted + 1,
            m_game->HighScore().totalHits,
            m_game->HighScore().totalShots
            );
        break;

    case GameInfoOverlayState::LevelStart:
        m_mainPage->SetLevelStart(
            m_game->LevelCompleted() + 1,
            m_game->CurrentLevel()->Objective(),
            m_game->CurrentLevel()->TimeLimit(),
            m_game->BonusTime()
            );
        break;

    case GameInfoOverlayState::GameOverCompleted:
        m_mainPage->SetGameOver(
            true, 
            m_game->LevelCompleted() + 1,
            m_game->TotalHits(),
            m_game->TotalShots(),
            m_game->HighScore().totalHits
            );
        break;

    case GameInfoOverlayState::GameOverExpired:
        m_mainPage->SetGameOver(
            false, 
            m_game->LevelCompleted(),
            m_game->TotalHits(),
            m_game->TotalShots(),
            m_game->HighScore().totalHits
            );
        break;

    case GameInfoOverlayState::Pause:
        m_mainPage->SetPause(
            m_game->LevelCompleted() + 1,
            m_game->TotalHits(),
            m_game->TotalShots(),
            m_game->TimeRemaining()
            );
        break;
    }
}
//--------------------------------------------------------------------------------------
void intro::SetAction(GameInfoOverlayCommand command)
{
    m_mainPage->SetAction(command);
}
//--------------------------------------------------------------------------------------
void intro::ShowGameInfoOverlay()
{
    m_mainPage->ShowGameInfoOverlay();
}
//--------------------------------------------------------------------------------------
void intro::HideGameInfoOverlay()
{
    m_mainPage->HideGameInfoOverlay();
}
//--------------------------------------------------------------------------------------
void intro::SetSnapped()
{
    m_mainPage->SetSnapped();
}
//--------------------------------------------------------------------------------------
void intro::HideSnapped()
{
    m_mainPage->HideSnapped();
}
//--------------------------------------------------------------------------------------
void intro::ResetGame()
{
    m_updateState = UpdateEngineState::WaitingForResources;
    m_pressResult = PressResultState::PlayLevel;
    m_controller->Active(false);
    m_game->LoadGame();
    SetAction(GameInfoOverlayCommand::PleaseWait);
    SetGameInfoOverlay(GameInfoOverlayState::LevelStart);
    ShowGameInfoOverlay();
    m_renderNeeded = true;
}
//--------------------------------------------------------------------------------------

