#include "framework.h"


//#include <psapi.h>


//#include <tlhelp32.h>



oswindow g_oswindowMessage = NULL;
stringa * g_pstraRestartCommandLine;
MSG g_msgSentinel;

ATOM sentinel_RegisterClass(HINSTANCE hInstance);

LRESULT CALLBACK sentinel_WndProc(oswindow oswindow, uint32_t message, WPARAM wParam, LPARAM lParam);


void win_factory_exchange(::aura::application * papp);





ATOM sentinel_RegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			   = 0;
	wcex.lpfnWndProc	   = sentinel_WndProc;
	wcex.cbClsExtra	   = 0;
	wcex.cbWndExtra	   = 0;
	wcex.hInstance		   = hInstance;
	wcex.hIcon			   = NULL;
	wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
   wcex.lpszMenuName	   = NULL;
	wcex.lpszClassName	= "::ca2::spaboot:callback_window";
	wcex.hIconSm		   = NULL;

	return RegisterClassEx(&wcex);
}


LRESULT CALLBACK sentinel_WndProc(oswindow oswindow, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
   case WM_COPYDATA:
      {
         COPYDATASTRUCT * pcds = (COPYDATASTRUCT *) lParam;
         if(pcds->dwData == 15111984)
         {
            g_pstraRestartCommandLine = new stringa;
            char * str = (char *) memory_alloc(pcds->cbData + 1);
            strncpy_dup(str, (const char *) pcds->lpData, pcds->cbData);
            str[pcds->cbData] = '\0';
            g_pstraRestartCommandLine->decode_v16(str);
            memory_free(str);
         }
      }
      break;
   case WM_TIMER:
      {
         if(wParam == 1245)
         {
            KillTimer(g_oswindowMessage, 12);
            throw todo(get_thread_app());
            //spa_install::sentinel::do_spa();
            SetTimer(g_oswindowMessage, 1245, (1984 + 1977) * 8, NULL);
         }
      }
      break;
   default:
      return DefWindowProc(oswindow, message, wParam, lParam);
   }
	return 0;
}






string calc_id();


//int32_t sentinel(const char * param);
int32_t APIENTRY ca2_cube_install(const char * pszId);







class sentinel :
   public ::base::app,
   public ::aura::ipc::rx::receiver
{
public:


   enum e_message
   {
      message_none,
      message_ok,
      message_failed,
      message_unknown,
   };


   e_message                  m_emessage;
   HANDLE                     m_hmutexSpabootInstall;
   ::aura::ipc::rx       m_rxchannel;

   char *                     m_modpath;
   char *                     m_pszDllEnds;
   uint32_t *                    m_dwaProcess;
   int32_t                        m_iSizeProcess;
   HMODULE *                  m_hmodulea;
   int32_t                        m_iSizeModule;
   bool                       m_bInstallerInstalling;

   sentinel();
   virtual ~sentinel();

   void install_defer_file_transfer();

   bool is_user_using(const char * pszDll);

   ATOM spaboot_message_register_class(HINSTANCE hInstance);

   void call_self_privileged_sync(const char * param);

   int32_t spaboot_registry_register(const char * lpszFile);

   void call_sync(const char * path, const char * param);

   void installer_call_sync(const char * path, const char * param);

   bool are_there_user_files_in_use();

   bool is_user_using(uint32_t processid, const char * pszDll);

   virtual void on_receive(::aura::ipc::rx * prx, const char * pszMessage);

   virtual int32_t simple_app_pre_run();

   virtual bool intro();

   virtual bool end();

   int32_t spaboot_start();


   void Ex1OnFactoryExchange()
   {
      // os.dll -> os.lib statically linked
      win_factory_exchange(this);


   }


};


// if MSVC CRT is stripped
/*extern "C" int32_t WinMainCRTStartup() \
{

ExitProcess(app::s_main < sentinel > ());

}*/

// if MSVC CRT is used
extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
__in LPTSTR lpCmdLine, int32_t nCmdShow)
{
   
   // call shared/exported WinMain

   return ::base::app_main < sentinel >(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

}


sentinel::sentinel() :
::object(this),
::aura::system(NULL, NULL),
::axis::system(NULL),
::base::system(NULL),
m_rxchannel(this)
{
   ////////////////////////////////////////////////////////////
   // configuration encryption system : with C:\\" hardware :-)
   // short login               short password  access configuration
   // |                         |               |
   // -----------------------   --       --------
   //                       |    |       |
   if (file_exists_dup("C:\\ca2\\config\\beg_debug_box.txt"))
   {
      debug_box("app.install", "app", 0);
   }

   m_hinstance = ::GetModuleHandleA(NULL);
   m_hmutexSpabootInstall = NULL;
   e_message m_emessage = message_none;
   m_modpath = NULL;
   m_pszDllEnds = NULL;
   m_dwaProcess = NULL;
   m_iSizeProcess = 0;
   m_hmodulea = NULL;
   m_iSizeModule = 0;
   m_bInstallerInstalling = false;

   construct("app.sentinel");
}

sentinel::~sentinel()
{
}

int32_t sentinel::simple_app_pre_run()
{


   return 0;

}

bool sentinel::intro()
{


   m_hmutexSpabootInstall = ::CreateMutex(NULL, FALSE, "Local\\::ca2::fontopus::ccca2_spaboot_install_sentinel::7807e510-5579-11dd-ae16-0800200c7784");


   if (::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      m_iReturnCode = -202;
      return false;
   }


   g_pstraRestartCommandLine = NULL;;

   if (!sentinel_RegisterClass(::GetModuleHandleA(NULL)))
   {

      return false;

   }


   g_oswindowMessage = ::CreateWindowExA(0, "::ca2::spaboot:callback_window", "::ca2::spaboot:callback_window", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

   if (g_oswindowMessage == NULL)
   {
      
      uint32_t dw = GetLastError();
      
      return false;

   }

   int32_t iRet = spaboot_start();

   SetTimer(g_oswindowMessage, 1245, (1977 + 1984) * 8, NULL);

   while (true)
   {

      GetMessage(&g_msgSentinel, NULL, 0, 0xffffffffu);

      TranslateMessage(&g_msgSentinel);

      DispatchMessage(&g_msgSentinel);

   }

/*   m_modpath = (char *)memory_alloc(MAX_PATH * 8);
   m_pszDllEnds = (char *)memory_alloc(MAX_PATH * 8);
   m_iSizeProcess = 1024;
   m_dwaProcess = (uint32_t *)memory_alloc(m_iSizeProcess);
   m_iSizeModule = 1024;
   m_hmodulea = (HMODULE *)memory_alloc(m_iSizeModule);

   m_rxchannel.m_preceiver = this;

   prepare_small_bell();

   if (!m_rxchannel.create("core/spaboot_install", "app.install.exe"))
   {
      m_iError = -1;
      return false;
   }*/

   return true;

}


void sentinel::install_defer_file_transfer()
{

   if (!System.install().is_installing_ca2())
   {

      System.install().update_updated();

      if (!System.install().is_updated() && !are_there_user_files_in_use())
      {

         // missing locale schema;

         throw "missing locale and schema parameters for installing";
         System.install().synch_install("starter_start: : app=session session_start=session app_type=application install in background in spa", "latest");

      }

   }

}


typedef int32_t(__cdecl * PFN_SPAADMIN_MAIN)(const char * pszCommandLine);


// non-thread safe
bool sentinel::is_user_using(uint32_t processid, const char * pszDll)
{

   HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
   MODULEENTRY32 me32;
   bool bFound = false;

   hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processid);
   if (hModuleSnap == INVALID_HANDLE_VALUE)
   {
      return false;
   }

   me32.dwSize = sizeof(MODULEENTRY32);

   if (!Module32First(hModuleSnap, &me32))
   {
      ::CloseHandle(hModuleSnap);
      return false;
   }

   strcpy_dup(m_pszDllEnds, "\\");
   strcat_dup(m_pszDllEnds, pszDll);

   do
   {
      if (str_ends_ci_dup(me32.szModule, m_pszDllEnds) || stricmp_dup(me32.szModule, pszDll) == 0)
      {
         bFound = true;
         break;
      }

   } while (Module32Next(hModuleSnap, &me32));

   ::CloseHandle(hModuleSnap);
   return bFound;

}

// non-thread safe
bool sentinel::is_user_using(const char * pszDll)
{
   HANDLE hProcessSnap;
   PROCESSENTRY32 pe32;

   hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if (hProcessSnap == INVALID_HANDLE_VALUE)
   {
      return false;
   }

   pe32.dwSize = sizeof(PROCESSENTRY32);

   if (!Process32First(hProcessSnap, &pe32))
   {
      ::CloseHandle(hProcessSnap);
      return false;
   }

   bool bUsing = false;
   do
   {
      if (is_user_using(pe32.th32ProcessID, pszDll))
      {
         bUsing = true;
         break;
      }
   } while (Process32Next(hProcessSnap, &pe32));

   ::CloseHandle(hProcessSnap);

   return bUsing;

}

void sentinel::on_receive(::aura::ipc::rx * prx, const char * pszMessage)
{
}



bool sentinel::are_there_user_files_in_use()
{
   if (is_user_using(dir::stage(process_platform_dir_name2()) / "msvcp110d.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "msvcr110d.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "ca.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "ca2.dll"))
      return true;
   if(is_user_using(dir::stage(process_platform_dir_name2()) / "ca2.dll"))
      return true;
   return false;
}


bool sentinel::end()
{

   app::end();

   memory_free(m_hmodulea);
   memory_free(m_dwaProcess);
   memory_free(m_pszDllEnds);
   memory_free(m_modpath);

   return true;

}


int32_t sentinel::spaboot_start()
{


   // todo : sentinel should install each older application with new function build get_latest_build_number



   string id = calc_id();
   if (id == NULL || id.is_empty())
      id = "spaboot_install";

   int32_t iRetry = 0;

   int32_t iRetryLimit;

   bool bBackground;

   if (!stricmp_dup(id, "spaboot_install"))
   {
      iRetryLimit = 5;
      bBackground = true;
   }
   else
   {
      iRetryLimit = -1;
      bBackground = false;
   }

   while (iRetry < iRetryLimit || iRetryLimit < 0)
   {
      if (System.install().is_ca2_installed() && System.install().is(NULL, NULL, "application", id, "_std", "_std"))
      {
         break;
      }
      string strCommandLine;

      strCommandLine = ": app=session session_start=" + id + " app_type=application install locale=_std schema=_std";

      throw todo(get_thread_app());
      System.install().synch_install(strCommandLine, "latest", bBackground);
      iRetry++;
   }

   if (!System.install().is_ca2_installed() || !System.install().is(NULL, NULL, "application", id, "_std", "_std"))
   {
      return 1;
   }

   if (stricmp_dup(id, "spaboot_install"))
   {
      string strCommandLine;

      strCommandLine = ": app=session session_start=" + id + " app_type=application locale=_std schema=_std";

      System.install().start(strCommandLine, "latest");
   }

   return 0;







}






string calc_id()
{

   char szModulePath[MAX_PATH * 3];
   ::GetModuleFileNameA((HINSTANCE)NULL, szModulePath, sizeof(szModulePath));
   string strId = read_resource_as_string_dup(::GetModuleHandleA(szModulePath), 1984, "application_id");
   if (strId == "mplite") // use veriwell_mplite
      strId = "bergedge";
   return strId;

}
