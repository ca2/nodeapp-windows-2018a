#pragma once


ref class appm_base
{
public:


   Windows::UI::Core::CoreWindow^                                       m_window;
   Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^               m_panel;
   Windows::Foundation::Size                                            m_renderTargetSize;
   Windows::Foundation::Rect                                            m_windowBounds;
   float                                                                m_dpi;

   
   appm_base();


   virtual void Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi);
   virtual void SetDpi(float dpi);
   virtual void UpdateForWindowSizeChange();
   virtual void CreateWindowSizeDependentResources();
   virtual void Render() = 0;
   virtual void Present();


};



