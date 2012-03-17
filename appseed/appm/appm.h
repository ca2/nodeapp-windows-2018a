#pragma once


#include "appm_renderer.h"

namespace appm
{

   ref class appm : public appm_renderer
   {
   public:
    
   
      appm();


      virtual void Initialize(Windows::UI::Core::CoreWindow^ window, Windows::UI::Xaml::Controls::SwapChainBackgroundPanel^ panel, float dpi);
      virtual void Render() override;

   };


} // namespace appm