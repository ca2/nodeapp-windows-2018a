#include "pch.h"
#include "appm_base.h"

using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;

appm_base::appm_base() :
    m_dpi(-1.0f)
{
}

void appm_base::Initialize(CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi)
{
    m_window = window;

    SetDpi(dpi);
}

// Helps track the DPI in the helper class.
// This is called in the dpiChanged event handler in the view class.
void appm_base::SetDpi(float dpi)
{
    if (dpi != m_dpi)
    {
        // Often a DPI change implies a window size change. In some cases Windows will issues
        // both a size changed event and a DPI changed event. In this case, the resulting bounds 
        // will not change, and the window resize code will only be executed once.
        UpdateForWindowSizeChange();
    }
}

// This routine is called in the event handler for the view SizeChanged event.
void appm_base::UpdateForWindowSizeChange()
{
    if (m_window->Bounds.Width  != m_windowBounds.Width ||
        m_window->Bounds.Height != m_windowBounds.Height)
    {
        CreateWindowSizeDependentResources();
    }
}

// Allocate all memory resources that change on a window SizeChanged event.
void appm_base::CreateWindowSizeDependentResources()
{
    // Store the window bounds so the next time we get a SizeChanged event we can
    // avoid rebuilding everything if the size is identical.
    m_windowBounds = m_window->Bounds;

}

// Method to deliver the final image to the display.
void appm_base::Present()
{
}
