#include "pch.h"
#include "intro.xaml.h"
#include "main.xaml.h"

using namespace appm;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

//----------------------------------------------------------------------
main::main(intro^ app)
{
    m_app = app;
}
//----------------------------------------------------------------------
void main::HideGameInfoOverlay()
{
    //VisualStateManager::GoToState(this->LayoutControl, ref new String(L"NormalState"), true);
}
//----------------------------------------------------------------------
void main::ShowGameInfoOverlay()
{
    //VisualStateManager::GoToState(this->LayoutControl, ref new String(L"GameInfoOverlayState"), true);
}
//----------------------------------------------------------------------
void main::SetAction(GameInfoOverlayCommand action)
{
    // Enable only one of the four possible commands at the bottom of the
    // Game Info Overlay.

    /*PlayAgain->Visibility = ::Visibility::Collapsed;
    PleaseWait->Visibility = ::Visibility::Collapsed;
    TapToContinue->Visibility = ::Visibility::Collapsed;

    switch (action)
    {
    case GameInfoOverlayCommand::PlayAgain:
        PlayAgain->Visibility = ::Visibility::Visible;
        break;
    case GameInfoOverlayCommand::PleaseWait:
        PleaseWait->Visibility = ::Visibility::Visible;
        break;
    case GameInfoOverlayCommand::TapToContinue:
        TapToContinue->Visibility = ::Visibility::Visible;
        break;
    case GameInfoOverlayCommand::None:
        break;
    }*/
}
//----------------------------------------------------------------------
void main::SetGameStats(
    int maxLevel, 
    int hitCount, 
    int shotCount
    )
{
/*    GameInfoOverlayTitle->Text = "Game Statistics";
    Stats->Visibility = ::Visibility::Visible;
    LevelStart->Visibility = ::Visibility::Collapsed;
    PauseData->Visibility = ::Visibility::Collapsed;

    static const int bufferLength = 20;
    static char16 wsbuffer[bufferLength];

    int length = swprintf_s(wsbuffer, bufferLength, L"%d", maxLevel);
    LevelsCompleted->Text = ref new Platform::String(wsbuffer, length);

    length = swprintf_s(wsbuffer, bufferLength, L"%d", hitCount);
    TotalPoints->Text = ref new Platform::String(wsbuffer, length);

    length = swprintf_s(wsbuffer, bufferLength, L"%d", shotCount);
    TotalShots->Text = ref new Platform::String(wsbuffer, length);

    // High Score is not used for showing Game Statistics
    HighScoreTitle->Visibility = ::Visibility::Collapsed;
    HighScoreData->Visibility  = ::Visibility::Collapsed;*/
}
//----------------------------------------------------------------------
void main::SetGameOver(
    bool win, 
    int maxLevel, 
    int hitCount, 
    int shotCount, 
    int highScore
    )
{
/*    if (win)
    {
        GameInfoOverlayTitle->Text = "You Won!";
    }
    else
    {
        GameInfoOverlayTitle->Text = "Game Over";
    }
    Stats->Visibility = ::Visibility::Visible;
    LevelStart->Visibility = ::Visibility::Collapsed;
    PauseData->Visibility = ::Visibility::Collapsed;

    static const int bufferLength = 20;
    static char16 wsbuffer[bufferLength];

    int length = swprintf_s(wsbuffer, bufferLength, L"%d", maxLevel);
    LevelsCompleted->Text = ref new Platform::String(wsbuffer, length);

    length = swprintf_s(wsbuffer, bufferLength, L"%d", hitCount);
    TotalPoints->Text = ref new Platform::String(wsbuffer, length);

    length = swprintf_s(wsbuffer, bufferLength, L"%d", shotCount);
    TotalShots->Text = ref new Platform::String(wsbuffer, length);

    // Show High Score
    HighScoreTitle->Visibility = ::Visibility::Visible;
    HighScoreData->Visibility  = ::Visibility::Visible;
    length = swprintf_s(wsbuffer, bufferLength, L"%d", highScore);
    HighScore->Text = ref new Platform::String(wsbuffer, length);*/
}
//----------------------------------------------------------------------
void main::SetLevelStart(
    int level, 
    Platform::String^ objective, 
    float timeLimit, 
    float bonusTime
    )
{
 /*   static const int bufferLength = 20;
    static char16 wsbuffer[bufferLength];

    int length = swprintf_s(wsbuffer, bufferLength, L"Level %d", level);
    GameInfoOverlayTitle->Text = ref new Platform::String(wsbuffer, length);

    Stats->Visibility = ::Visibility::Collapsed;
    LevelStart->Visibility = ::Visibility::Visible;
    PauseData->Visibility = ::Visibility::Collapsed;

    Objective->Text = objective;

    length = swprintf_s(wsbuffer, bufferLength, L"%6.1f sec", timeLimit);
    TimeLimit->Text = ref new Platform::String(wsbuffer, length);

    if (bonusTime > 0.0)
    {
        BonusTimeTitle->Visibility = ::Visibility::Visible;
        BonusTimeData->Visibility  = ::Visibility::Visible;
        length = swprintf_s(wsbuffer, bufferLength, L"%6.1f sec", bonusTime);
        BonusTime->Text = ref new Platform::String(wsbuffer, length);
    }
    else
    {
        BonusTimeTitle->Visibility = ::Visibility::Collapsed;
        BonusTimeData->Visibility  = ::Visibility::Collapsed;  
    }*/
}
//----------------------------------------------------------------------
void main::SetPause(int level, int hitCount, int shotCount, float timeRemaining)
{
/*    GameInfoOverlayTitle->Text = "Paused";
    Stats->Visibility = ::Visibility::Collapsed;
    LevelStart->Visibility = ::Visibility::Collapsed;
    PauseData->Visibility = ::Visibility::Visible;

    static const int bufferLength = 20;
    static char16 wsbuffer[bufferLength];

    int length = swprintf_s(wsbuffer, bufferLength, L"%d", level);
    PauseLevel->Text = ref new Platform::String(wsbuffer, length);

    length = swprintf_s(wsbuffer, bufferLength, L"%d", hitCount);
    PauseHits->Text = ref new Platform::String(wsbuffer, length);

    length = swprintf_s(wsbuffer, bufferLength, L"%d", shotCount);
    PauseShots->Text = ref new Platform::String(wsbuffer, length);

    length = swprintf_s(wsbuffer, bufferLength, L"%6.1f sec", timeRemaining);
    PauseTimeRemaining->Text = ref new Platform::String(wsbuffer, length);*/
}
//----------------------------------------------------------------------
void main::SetSnapped()
{
    //VisualStateManager::GoToState(this->LayoutControl, ref new String(L"SnappedState"), true);
}
//----------------------------------------------------------------------
void main::HideSnapped()
{
    //VisualStateManager::GoToState(this->LayoutControl, ref new String(L"UnsnappedState"), true);
}
//----------------------------------------------------------------------
void main::OnGameInfoOverlayTapped(Object^ sender, TappedRoutedEventArgs^ args)
{
    //m_app->PressComplete();
}
//----------------------------------------------------------------------
void main::OnPauseButtonClicked(Object^ sender, RoutedEventArgs^ args)
{
    //m_app->PauseRequested();
}
//----------------------------------------------------------------------
void main::OnPlayButtonClicked(Object^ sender, RoutedEventArgs^ args)
{
    //m_app->PressComplete();
}
//----------------------------------------------------------------------
void main::OnResetButtonClicked(Object^ sender, RoutedEventArgs^ args)
{
    //m_app->ResetGame();
}
//----------------------------------------------------------------------
