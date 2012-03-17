#include "pch.h"
#include "appm_base.h"
#include "appm_renderer.h"
#include "windows.ui.xaml.media.dxinterop.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Graphics::Display;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::ViewManagement;

using namespace Windows::UI::ApplicationSettings;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml::Controls;

appm_renderer::appm_renderer():
    m_initialized(false)
{
}

void appm_renderer::Initialize(CoreWindow^ window, SwapChainBackgroundPanel^ panel, float dpi)
{
    m_panel = panel;

    if (m_initialized)
    {
    }
    else
    {
    }

    appm_base::Initialize(window, panel, dpi);
}
void appm_renderer::UpdateForWindowSizeChange()
{
    /*if (m_window->Bounds.Width  != m_windowBounds.Width ||
        m_window->Bounds.Height != m_windowBounds.Height)
    {
        CreateWindowSizeDependentResources();
    }*/
}
//----------------------------------------------------------------------
// Helps track the DPI in the helper class.
// This is called in the dpiChanged event handler in the view class.
void appm_renderer::SetDpi(float dpi)
{
    if (dpi != m_dpi)
    {
        m_dpi = dpi;

    }
}
