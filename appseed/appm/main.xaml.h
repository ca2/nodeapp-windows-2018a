#pragma once

#include "main.g.h"

namespace appm
{
    ref class intro;

    public enum class GameInfoOverlayCommand
    {
        None,
        TapToContinue,
        PleaseWait,
        PlayAgain,
    };

    public ref class main sealed
    {
    public:
        main(intro^ app);

        void SetGameStats(int maxLevel, int hitCount, int shotCount);
        void SetGameOver(bool win, int maxLevel, int hitCount, int shotCount, int highScore);
        void SetLevelStart(int level, Platform::String^ objective, float timeLimit, float bonusTime);
        void SetPause(int level, int hitCount, int shotCount, float timeRemaining);
        void SetSnapped();
        void HideSnapped();
        void SetAction(GameInfoOverlayCommand action);
        void HideGameInfoOverlay();
        void ShowGameInfoOverlay();

    protected:
        void OnPauseButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnPlayButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnResetButtonClicked(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void OnGameInfoOverlayTapped(Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ args);

    private:
        intro^ m_app;
    };
}
