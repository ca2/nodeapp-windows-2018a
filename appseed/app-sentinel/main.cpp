#include "framework.h"


int32_t spaboot_start();


oswindow g_oswindowMessage = NULL;
stringa * g_pstraRestartCommandLine;
MSG g_msgSentinel;
HANDLE g_hmutexSpabootInstall;

ATOM sentinel_RegisterClass(HINSTANCE hInstance);

LRESULT CALLBACK sentinel_WndProc(oswindow oswindow, uint32_t message, WPARAM wParam, LPARAM lParam);


extern "C" int32_t WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int32_t nCmdShow)
{

//   initialize_primitive_heap();

   if(!os_initialize())
      return -1;

   if(!main_initialize())
      return -1;

   
   g_hmutexSpabootInstall = ::CreateMutex(NULL, FALSE, "Local\\::ca2::fontopus::ccca2_spaboot_install_sentinel::7807e510-5579-11dd-ae16-0800200c7784");
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      return false;
   }

   g_pstraRestartCommandLine = NULL;;

   if(!sentinel_RegisterClass(::GetModuleHandleA(NULL)))
      return -1;


   g_oswindowMessage = ::CreateWindowExA(0, "::ca2::spaboot:callback_window", "::ca2::spaboot:callback_window", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

   if(g_oswindowMessage == NULL)
   {
      uint32_t dw = GetLastError();
      return -1;
   }


   int32_t iRet = spaboot_start();


   SetTimer(g_oswindowMessage, 1245, (1977 + 1984) * 8, NULL);


	while(true)
	{
      GetMessage(&g_msgSentinel, NULL, 0, 0xffffffffu);
		TranslateMessage(&g_msgSentinel);
		DispatchMessage(&g_msgSentinel);
	}

   main_finalize();

   os_finalize();

//   finalize_primitive_heap();


   return iRet;

}





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
            spa_install::installer::do_spa();
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


int32_t installer(const char * param);
int32_t APIENTRY ca2_cube_install(const char * pszId);

int32_t spaboot_start()
{


   // todo : sentinel should install each older application with new function build get_latest_build_number

   

   string id = calc_id();
   if(id == NULL || id.is_empty())
      id = "spaboot_install";

   int32_t iRetry = 0;

   int32_t iRetryLimit;

   bool bBackground;

   if(!stricmp_dup(id, "spaboot_install"))
   {
      iRetryLimit = 5;
      bBackground = true;
   }
   else
   {
      iRetryLimit = -1;
      bBackground = false;
   }

   while(iRetry < iRetryLimit || iRetryLimit < 0)
   {
      update_ca2_installed(true);
      if(is_ca2_installed() && is_installed(NULL, NULL, "application", id, "_std", "_std"))
      {  
         break;
      }
      string strCommandLine;

      strCommandLine = ": app=session session_start=" + id + " app_type=application install locale=_std schema=_std";

      ca2_cube_install(strCommandLine, bBackground);
      iRetry++;
   }

   if(!is_ca2_installed() || !is_installed(NULL, NULL, "application", id, "_std", "_std"))
   {  
      return 1;
   }

   if(stricmp_dup(id, "spaboot_install"))
   {
      cube_run(id);
   }

   return 0;

}



string calc_id()
{

   char szModulePath[MAX_PATH * 3];
   ::GetModuleFileNameA((HINSTANCE) NULL, szModulePath, sizeof(szModulePath));
   string strId = read_resource_as_string_dup(::GetModuleHandleA(szModulePath), 1984, "CGCL");
   if(strId == "mplite") // use veriwell_mplite
      strId = "bergedge";
   return strId;

}


