#pragma once

#include "aura/aura/aura.h"
#include "aura/node/windows/windows.h"

#ifdef _DLL
#if defined(SPALIB_DLL)
#define SPALIB_API __declspec(dllexport)
#else
#define SPALIB_API __declspec(dllimport)
#endif
#else
#define SPALIB_API
#endif

#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/types.h>

extern "C"
{
#include "app/axis/bzip2/bzlib.h"
}

#include "../a_spares/resource.h"
#include <shellapi.h>
#include "http1.h"
#include "http2.h"
#include "canvas.h"


SPALIB_API bool spa_get_admin();
SPALIB_API void spa_set_admin(bool bSet);

SPALIB_API string spa_get_id();
SPALIB_API void spa_set_id(const char * pszVersion);

//SPALIB_API ::file::path spa_path();
//SPALIB_API ::file::path spa_admin_path();

SPALIB_API string spalib_get_build();



extern int g_cx;
extern int g_cy;
extern HWND g_hwnd;



#define TIMER_CARET 123454
#define TIMER_ANIMATION 1234508
#define CARET_TIME 1000









SPALIB_API string str_replace(const char * psz,const char * pszFind,const char * pszReplace);

SPALIB_API int bzuncompress(LPCTSTR lpcszUncompressed,LPCTSTR lpcszGzFileCompressed);



stringa install_get_plugin_base_library_list(const string & strVersion);




void trace(const string & psz);
void trace(double dRate);



string do_install(const char * psz);

SPALIB_API  int spalib_main(HINSTANCE hInstance,   HINSTANCE hPrevInstance,   LPTSTR    lpCmdLine,   int       nCmdShow);

#include "../a_spares/resource.h"
#include "http1.h"



//SPALIB_API string spa_version(string strVersion= "");
//SPALIB_API string spa_title(string strTitle= "");

class SPALIB_API a_spa :
   public ::aura::simple_app
{
public:

   //string m_strLogin;
   //string m_strSessid;
   //string m_strStart;
   //string m_strInstallFilter;
   //string m_strLocale;
   //string m_strLoginFailed;
   //string m_strFile;
   //string m_strIndex;
   //string m_strIndexGz;
   //string m_strInstall;
   //string m_strInstallGz;
   //string m_strLastHost;
   //string m_strCurrentHost;
   string m_strId;
   //string m_strPlatform;
   string m_strVersion;
   mutex m_mutexTrace;


   a_spa() :
      ::aura::system(this)
   {
      m_hinstance             = ::GetModuleHandleA(NULL);
   };
   

   virtual ~a_spa() {}

   virtual int32_t run();

} ;


SPALIB_API a_spa & aspa();




