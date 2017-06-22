#pragma once


#include "aura/aura/aura.h"
#include "aura/node/windows/windows.h"


#ifdef _DLL
#if defined(SPALIB_DLL)
#define SPALIB_API CLASS_DECL_EXPORT
#else
#define SPALIB_API CLASS_DECL_IMPORT
#endif
#else
#define SPALIB_API
#endif


namespace app_app
{

   class app;


   enum e_item_type
   {

      item_type_none,
      item_type_spa,
      item_type_set,

   };


   class bootstrap;


} // namespace app_app


#undef Sys
#define Sys(pcaapp) (pcaapp->cast_app < ::app_app::app > ())



#include "resource.h"

#include "app_app_window.h"
#include "app_app_install_item.h"
#include "app_app_socket.h"
#include "app_app_socket_handler.h"
#include "app_app_socket_thread.h"
#include "app_app_trace.h"


#include "app_app_tool.h"


#include "app_app_bootstrap.h"





#include "app_app_app.h"



