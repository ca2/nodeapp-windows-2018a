#pragma once


#include "appm_base.h"


ref class appm_renderer : public appm_base
{
public:


   bool                                                m_initialized;



   appm_renderer();

   virtual void Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi);
   virtual void UpdateForWindowSizeChange() override;
   virtual void SetDpi(float dpi) override;
   virtual bool IsResourceLoadingComplete();


    

};
