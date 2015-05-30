#pragma once


#define _WIN32_WINNT 0x0502

#define _CRT_SECURE_NO_WARNINGS

#pragma warning(disable:4275)

#include "targetversion.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include "aura/version.config.h"


#ifdef _DLL
#if defined(SPALIB_DLL)
#define SPALIB_API __declspec(dllexport)
#else
#define SPALIB_API __declspec(dllimport)
#endif
#else
#define SPALIB_API
#endif

#ifdef CLASS_DECL_MACHINE_EVENT
#undef CLASS_DECL_MACHINE_EVENT
#endif

#define CLASS_DECL_MACHINE_EVENT

#ifdef Application
#undef Application
#endif

#define Application
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>

extern "C"
{
#include "app/axis/bzip2/bzlib.h"
}

#include <map>
#include <string>
#include <vector>
#include <shlobj.h>
#include <process.h>

#include "../spares/resource.h"
#include <shellapi.h>
#include "machine_event_data.h"
#include <math.h>
//#include "se_exception.h"
//#include "se_translator.h"


#include "keep_true.h"
#include "md5.h"
#include "str.h"
#include "stringa.h"
#include "inta.h"
#include "doublea.h"
#include "dir.h"
#include "file.h"
#include "XMLite.h"
#include "focus.h"
#include "machine_event_data.h"
#include "machine_event.h"
#include "http1.h"
#include "http2.h"
#include "ca2_install_canvas.h"
#include "simple_mutex.h"
#include "mutex_lock.h"

using namespace std;


SPALIB_API bool spa_get_admin();
SPALIB_API void spa_set_admin(bool bSet);

SPALIB_API std::string spa_get_id();
SPALIB_API void spa_set_id(const char * pszVersion);

SPALIB_API std::string spa_get_platform();

SPALIB_API std::string spalib_get_build();

extern int g_cx;
extern int g_cy;
extern HWND g_hwnd;



#define TIMER_CARET 123454
#define TIMER_ANIMATION 1234508
#define CARET_TIME 1000

extern HINSTANCE hInst;								// current instance
extern std::string g_strStart;
extern std::string g_strInstallFilter;
extern stringa g_straTerminateProcesses;
extern stringa g_straRestartCommandLine;


std::string url_query_param(int & iParam, const char * pszParam);


std::string load_string(const char * pszId, const char * pszDefault);

#ifdef WIN32
void config_session_proxy(HINTERNET hSession, WCHAR * pwzUrl);
BOOL AfxInternalPumpMessage();
wstring u16(const char * psz);
string u8(const wchar_t * pwsz);
wstring u16(const char * psz);
string u8(const wchar_t * pwsz);
#endif
void CommandStart();

SPALIB_API bool is_installed(const char * psz);

int LangFromOS();

class Lang
{
public:
   int            m_iCommand;
   std::string    m_strCode;
};


extern Lang g_langa[];

void add_spa_start(const char * pszId);
void remove_spa_start(const char * pszId);

extern bool g_bInstalling;

extern HINTERNET  g_hSession;
extern std::string g_strHost;
extern DWORD g_dwInstallStartTime;
extern std::string g_strBuild;
extern std::string g_strBuildResource;
extern HINSTANCE g_hinstance;
extern bool g_bShowPercentage;
extern MSG g_msg;
extern bool g_bShow;







#ifdef WIN32
void config_session_proxy(HINTERNET hSession, WCHAR * pwzUrl);



BOOL AfxInternalPumpMessage();

std::string ms_post(const char * pszUrl, const char * pszPost);
#endif


extern machine_event g_machineevent;

extern bool machine_unsignalize_close_application();
extern bool machine_check_close_application(bool bDefault);
extern bool g_NeedRestartBecauseOfReservedFile;
extern bool g_NeedRestartFatalError;
extern bool g_bLoginStartup;
extern bool g_bMsDownload;
extern std::string g_strTitle;
extern DWORD g_dwDownloadLen;
extern DWORD g_dwCurFileLen;
bool m_reboot();
bool ignit_phase2();
int run_uninstall(const char * lpCmdLine, int nCmdShow);
int run_install(const char * lpCmdLine, int nCmdShow);
bool spa_exec(const char * psz);
std::string load_string(const char * pszId, const char * pszDefault);
SPALIB_API std::string str_replace(const char * psz, const char * pszFind, const char * pszReplace);
bool is_there_application_opened();
bool is_application_opened(const char * psz);
void machine_signalize_close_application();
SPALIB_API std::string read_resource_as_string(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);


wchar_t * wfy(const char * pszUtf8);


void wfree(wchar_t * push);

SPALIB_API int bzuncompress(LPCTSTR lpcszUncompressed, LPCTSTR lpcszGzFileCompressed);

int CopyFileList(stringa & stringa);

std::string ca2_get_dir(LPCTSTR lpcszUrl);
std::string ca2bz_get_dir(LPCTSTR lpcszUrl);
std::string ca2unbz_get_dir(LPCTSTR lpcszUrl);
std::string ca2_get_file(LPCTSTR lpcszUrl);
std::string ca2bz_get_file(LPCTSTR lpcszUrl, const char * pszMd5);
std::string ca2unbz_get_file(LPCTSTR lpcszUrl);
bool ca2_fy_url(std::string & str, LPCTSTR lpcszUrl, bool bExist, int iLength, const char * pszMd5, int iGzLen, bool bIndex = false);


extern int g_iScreen;
extern int g_iProgressMode;
extern int g_iTotalGzLen;
extern int g_iGzLen;
extern int g_iStyle;

extern stringa g_strSpa;
extern stringa g_straHost;

extern bool g_bOfflineInstall;
extern bool g_bInternetInstall;
extern std::string g_strIndex;
extern std::string g_strIndexGz;
extern std::string g_strInstall;
extern std::string g_strInstallGz;

extern bool g_bInstallSet;
extern FILE * g_ftrace;


//extern bool Get(const std::string& url_in, std::string & doc);
//extern bool Get(const std::string& url_in, bool bExist, int iLength, const char * pszMd5, int iGzLen);
//extern bool DownloadFile(const std::string& url_in, bool bExist, int iLength, const char * pszMd5, int iGzLen);
//extern int GetFileList(stringa & stringa, LPCTSTR lpcszUrl, strintmap & mapLen, strintmap & mapGzLen, strstrmap & mapMd5);
//extern int GetLocalFileList(stringa & stringa, LPCTSTR lpcszUrl);
//extern int DownloadFileList(stringa & stringa, strintmap & mapLen, strstrmap & mapMd5, strintmap & mapGzLen);
//extern void ParseIndexFile(const char * psz, strintmap & mapLen, strstrmap & mapMd5, strintmap & mapGzLen);
//extern int UncompressFileList(stringa & stringa, strstrmap & strmapMd5);
//extern void ParseSpaIndex(XNode & node);
//extern void CommandLang(int iLang);
//extern std::string Login();


stringa install_get_plugin_base_library_list(const string & strVersion);



extern double g_dProgress;
extern double g_dProgress1;
extern double g_dProgress2;
extern XNode g_nodeStringTable;
extern int g_iStart;
extern doublea g_daDownloadRate;
extern DWORD g_dwDownloadTick;
extern DWORD g_dwDownload;
extern int g_iDownloadRate;
extern double g_dDownloadRate;
extern DWORD g_dwDownloadZeroRateTick;
extern DWORD g_dwDownloadZeroRateRemain;
extern DWORD g_dwDownloadRemain;




void trace(const string & psz);
void trace(double dRate);



std::string install(const char * psz);

SPALIB_API  std::string get_temp_file_name(const char * pszName, const char * pszExtension);

//void trace_add(const char * psz);

SPALIB_API  int spalib_main(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow);


extern stringa g_straTrace;
extern stringa g_strSpa;
extern stringa g_straHost;
extern int g_iTrace;
extern FILE * g_ftrace;
extern bool g_bForceUpdatedBuild;
extern bool g_bStarterStart;
extern int g_iHealingSurface;

SPALIB_API void prepare_small_bell();
SPALIB_API void play_small_bell();
SPALIB_API void defer_play_small_bell();

SPALIB_API void reg_delete_tree(HKEY hkey, const char * name);


#include <string>
#include "../spares/resource.h"
#include <shellapi.h>
#include <psapi.h>
#include <string>
#include "../spares/resource.h"
#include "spalib.h"
#include "dir.h"
#include "file.h"
#include "http1.h"
#include "md5.h"
#include "str.h"
#include "XMLite.h"
#include <shellapi.h>

#include <string>
#include "../spares/resource.h"
#include "spalib.h"
#include "dir.h"
#include "file.h"
#include "http1.h"
#include "md5.h"
#include "str.h"
#include "XMLite.h"
#include <shellapi.h>
#include "StdAfx.h"
#include <string>
#include "../spares/resource.h"
#include "spalib.h"
#include "dir.h"
#include "file.h"
#include "http1.h"
#include "md5.h"
#include "str.h"
#include "XMLite.h"
#include <shellapi.h>
#include <string>
#include "../spares/resource.h"
#include "spalib.h"
#include "dir.h"
#include "file.h"
#include "http1.h"
#include "md5.h"
#include "str.h"
#include <shellapi.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include "../spares/resource.h"
#include "spalib.h"
#include "dir.h"
#include "file.h"
#include "http1.h"
#include "md5.h"
#include "str.h"
#include "XMLite.h"
#include <shellapi.h>
#include <Mmsystem.h>
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include "md5.h"
#include "StdAfx.h"
#include <string>
#include "../spares/resource.h"
#include "spalib.h"
#include "dir.h"
#include "file.h"
#include "http1.h"
#include "md5.h"
#include "str.h"
#include "XMLite.h"
#include <shellapi.h>
