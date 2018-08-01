#pragma once


#include "core/core/core.h"
#include "core/user/html/html/html.h"
#include "core/filesystem/filemanager/filemanager.h"


#ifdef _CA2_FONTOPUS_WORDCARD_DLL
    #define CLASS_DECL_CA2_WORDCARD  _declspec(dllexport)
#else
    #define CLASS_DECL_CA2_WORDCARD  _declspec(dllimport)
#endif


namespace wordcard
{

   class application;

} // namespace wordcard


#undef App
#define App(papp) (*papp->cast < ::wordcard::application > ())

#include "wordcard_frame.h"
#include "wordcard_form_callback.h"
#include "wordcard_pane_view.h"
#include "wordcard_pane_view_update_hint.h"
#include "wordcard_document.h"
#include "wordcard_view_update_hint.h"
#include "wordcard_view.h"


#include "wordcard_application.h"




