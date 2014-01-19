#pragma once


#include "nodeapp/appseed/production/production.h"


#ifdef _CA2_FONTOPUS_BACKUP_DLL
    #define CLASS_DECL_CA2_BACKUP  _declspec(dllexport)
#else
    #define CLASS_DECL_CA2_BACKUP  _declspec(dllimport)
#endif


#include "backup_backup.h"


#include "backup_frame.h"
#include "backup_document.h"
#include "backup_view.h"
#include "backup_form_callback.h"

#include "backup_pane_view_update_hint.h"
#include "backup_pane_view.h"


#include "backup_application.h"



