#pragma once

#include "ca2.h"

#ifdef _CA2_FONTOPUS_PRODUCTION_DLL
    #define CLASS_DECL_CA2_PRODUCTION  _declspec(dllexport)
#else
    #define CLASS_DECL_CA2_PRODUCTION  _declspec(dllimport)
#endif

namespace production
{
   class application;
} // namespace production

#undef App
#define App(pcaapp) (pcaapp->cast_app < ::production::application > ())

#include "twitter_authorization.h"

#include "class.h"
#include "frame.h"
#include "form_callback.h"
#include "pane_view.h"
#include "pane_view_update_hint.h"
#include "document.h"
#include "view_update_hint.h"
#include "view.h"
#include "application.h"


