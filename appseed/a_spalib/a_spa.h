#pragma once


#include "aura/aura/aura.h"
#include "aura/node/windows/windows.h"
#include "aura/net.h"

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

namespace a_spa
{

   class simple_app;


   enum e_item_type
   {

      item_type_none,
      item_type_spa,
      item_type_set,

   };




} // namespace a_spa


#include "../a_spares/resource.h"
#include "a_spa_install_item.h"
#include "a_spa_canvas.h"
#include "a_spa_trace.h"
#include "a_spa_socket.h"
#include "a_spa_socket_handler.h"
#include "a_spa_socket_thread.h"


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


string do_install(const char * psz);


SPALIB_API int spalib_main(HINSTANCE hInstance,   HINSTANCE hPrevInstance,   LPTSTR    lpCmdLine,   int       nCmdShow);

SPALIB_API void kill_process(string strImageName);
SPALIB_API int register_spa_file_type();
SPALIB_API int is_downloading_spaadmin();
SPALIB_API void end_spa(::aura::application * papp);
SPALIB_API bool low_is_spaadmin_running(string strPlatform);
SPALIB_API void defer_start_program_files_spa_admin(string strPlatform);
SPALIB_API void get_system_locale_schema(string & strLocale, string & strSchema);
SPALIB_API void app_install_call_sync(const char * pszPlatform, const char * szParameters, const char * pszBuild);
SPALIB_API void run_vcredist(string strPlatform);
SPALIB_API string get_module_path(HMODULE hmodule);
SPALIB_API string read_resource_as_string(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);


#include "a_spa_simple_app.h"