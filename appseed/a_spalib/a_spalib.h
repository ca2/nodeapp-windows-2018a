#pragma once

#include "aura/aura/aura.h"
#include "aura/node/windows/windows.h"
extern "C" 
{
   #include "bzlib.h"
}
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <WinINet.h>
#include <shellapi.h>


#ifdef _DLL
#if defined(SPALIB_DLL)
#define SPALIB_API __declspec(dllexport)
#else
#define SPALIB_API __declspec(dllimport)
#endif
#else
#define SPALIB_API
#endif


#include "../a_spares/resource.h"
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







string do_install(const char * psz);

SPALIB_API  int spalib_main(HINSTANCE hInstance,   HINSTANCE hPrevInstance,   LPTSTR    lpCmdLine,   int       nCmdShow);

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
      ::aura::system(NULL, this)
   {
      m_hinstance             = ::GetModuleHandleA(NULL);
   };
   

   virtual ~a_spa() {}


   string spa_get_id()
   {
      return m_strId;
   }

   void spa_set_id(const char * psz)
   {
      m_strId = psz;
   }



   virtual int32_t run();


   static DWORD WINAPI spa_main_proc(LPVOID);

   virtual int spa_main();
   virtual int spa_main_start();
   virtual int spalib_main2();


   virtual int spaadmin_main();
   virtual int do_spa(const char * psz, const char * pszParams = NULL);

   virtual string get_app_id(string wstr);
   virtual int check_soon_launch();
   virtual int check_spa_installation();
   virtual int show_spa_window(bool bShow = true);
   virtual int check_soon_file_launch(string wstr);
   virtual int check_soon_app_id(string wstr);
   virtual string spa_app_id_to_app_name(string strId);

   virtual void start_app_install_in_context();
   virtual string get_latest_build_number(const char * pszVersion);

   virtual int check_berdge();
   virtual int check_vcredist();
   virtual int download_vcredist();
   virtual string download_tmp_vcredist();
   virtual int check_spa_bin();
   virtual int download_spa_bin();
   virtual string download_tmp_spa_bin();
   virtual int check_spaadmin_bin();
   virtual int download_spaadmin_bin();
   virtual string download_tmp_spaadmin_bin();
   virtual int check_install_bin_set();

   virtual bool is_file_ok(const char * path1,const char * pszTemplate,const char * pszFormatBuild);
   virtual bool is_file_ok(const stringa & straPath,const stringa & straTemplate,stringa & straMd5,const string & strFormatBuild,int iMd5Retry);

   void trace(const string & str);
   void trace(double dRate);

   bool ms_download(const char * pszUrl,const char * pszFile,bool bUrlEncode = true,int * piStatus = NULL);
   bool ms_download_progress(const char * pszUrl,const char * pszFile,bool bProgress,bool bUrlEncode = true,int * piStatus = NULL);

   string ms_get(const char * pszUrl,bool bCache = false);

} ;






