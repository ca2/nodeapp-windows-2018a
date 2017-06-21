#pragma once


#include "aura/aura/aura.h"
#include "aura/node/windows/windows.h"
//#include "aura/net.h"

//extern "C" 
//{
//
//   #include "bzlib.h"
//
//}

#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <WinINet.h>
#include <shellapi.h>


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




} // namespace app_app


#undef App
#define App(pcaapp) (pcaapp->cast_app < ::app_app::app > ())



#include "../app_app_res/resource.h"

#include "app_app_canvas.h"
#include "app_app_install_item.h"
#include "app_app_socket.h"
#include "app_app_socket_handler.h"
#include "app_app_socket_thread.h"
#include "app_app_trace.h"


SPALIB_API bool spa_get_admin();
SPALIB_API void spa_set_admin(bool bSet);

SPALIB_API string spa_get_id();
SPALIB_API void spa_set_id(const char * pszVersion);

SPALIB_API string spalib_get_build();


#define TIMER_CARET 123454
#define TIMER_ANIMATION 1234508
#define CARET_TIME 1000


//SPALIB_API string str_replace(const char * psz,const char * pszFind,const char * pszReplace);


//SPALIB_API int bzuncompress(LPCTSTR lpcszUncompressed,LPCTSTR lpcszGzFileCompressed);


stringa install_get_plugin_base_library_list(const string & strPlatform, const string & strVersion);





#include "app_app_app.h"



