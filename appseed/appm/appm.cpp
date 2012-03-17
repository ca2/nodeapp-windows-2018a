//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "appm.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Graphics::Display;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Storage;
using namespace Windows::UI::ApplicationSettings;
using namespace Windows::UI::Popups;
using namespace Windows::UI::Xaml::Controls;

namespace appm
{

   appm::appm()
   {
   }


   void appm::Initialize(CoreWindow^ window, SwapChainBackgroundPanel^ panel, float dpi)
   {
       static bool Initialized = false;
       if (!Initialized)
       {
       }

       appm_renderer::Initialize(window, panel, dpi);

       if (!Initialized)
       {
       }
   }


} // namespace appm



