#pragma once


#include "core/core/core.h"
#include "core/user/html/html/html.h"
#include "core/filesystem/filemanager/filemanager.h"


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
#define App(papp) (*papp->cast < ::production::application > ())


//#include "hi5_facebook.h"

#include "production_production.h"


#include "production_frame.h"
#include "production_form_callback.h"
#include "production_pane_view.h"
#include "production_pane_view_update_hint.h"
#include "production_document.h"
#include "production_view_update_hint.h"
#include "production_view.h"


#include "production_application.h"




