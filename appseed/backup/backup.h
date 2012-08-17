#pragma once

#include "app-core/appseed/production/production.h"


#ifdef _CA2_FONTOPUS_BACKUP_DLL
    #define CLASS_DECL_CA2_BACKUP  _declspec(dllexport)
#else
    #define CLASS_DECL_CA2_BACKUP  _declspec(dllimport)
#endif

#include "backup_class.h"

#include "frame.h"
#include "document.h"
#include "view.h"
#include "form_callback.h"

#include "pane_view_update_hint.h"
#include "pane_view.h"

#include "application.h"