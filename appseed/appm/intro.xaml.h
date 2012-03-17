#pragma once


#include "main.xaml.h"
#include "appm.h"
#include "intro.g.h"


namespace appm
{


    private enum class UpdateEngineState
    {
        Uninitialized,
        WaitingForResources,
        WaitingForPress,
        Dynamics,
        Snapped,
        Suspended,
        Deactivated,
    };

    private enum class PressResultState
    {
        LoadGame,
        PlayLevel,
        ContinueLevel,
    };

    private enum class GameInfoOverlayState
    {
        GameStats,
        GameOverExpired,
        GameOverCompleted,
        LevelStart,
        Pause,
        Snapped,
    };

    public ref class intro sealed
    {
    public:
        intro();

        virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs);

        void PauseRequested() { if (m_updateState == UpdateEngineState::Dynamics) m_pauseRequested = true; };
        void PressComplete()  { if (m_updateState == UpdateEngineState::WaitingForPress) m_pressComplete = true; };
        void ResetGame();

    private:
        ~intro();

        void OnSuspending(
            _In_ Platform::Object^ sender, 
            _In_ Windows::ApplicationModel::SuspendingEventArgs^ args
            );
        void OnResuming(
            _In_ Platform::Object^ sender,
            _In_ Platform::Object^ args
            );

        void OnViewStateChanged(
            _In_ Windows::UI::ViewManagement::ApplicationView^ view,
            _In_ Windows::UI::ViewManagement::ApplicationViewStateChangedEventArgs^ args
            );

        void OnWindowActivationChanged(
            _In_ Platform::Object^ sender,
            _In_ Windows::UI::Core::WindowActivatedEventArgs^ args
            );

        void OnWindowSizeChanged(
            _In_ Windows::UI::Core::CoreWindow^ sender,
            _In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args
            );

        void OnLogicalDpiChanged(
            _In_ Platform::Object^ sender
            );

        void OnRendering(
            _In_ Object^ sender, 
            _In_ Object^ args
            );

        void InitializeGameState();
        void Update();
        void SetGameInfoOverlay(GameInfoOverlayState state);
        void SetAction (GameInfoOverlayCommand command);
        void ShowGameInfoOverlay();
        void HideGameInfoOverlay();
        void SetSnapped();
        void HideSnapped();

        Windows::Foundation::EventRegistrationToken         m_eventToken;
        bool                                                m_pauseRequested;
        bool                                                m_pressComplete;
        bool                                                m_renderNeeded;
        bool                                                m_haveFocus;

        main^                                           m_mainPage;
        appm^                                               m_game;

        UpdateEngineState                                   m_updateState;
        UpdateEngineState                                   m_updateStateNext;
        PressResultState                                    m_pressResult;
        GameInfoOverlayState                                m_gameInfoOverlayState;
    };


} // namespace appm




