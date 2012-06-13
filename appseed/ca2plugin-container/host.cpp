#include "framework.h"
#include <eh.h>

UINT g_nRedrawThreadID;
extern bool g_bAppStarted;
extern void * g_pvoidPluginSystem;


namespace ca2plugin_container
{

   ATOM register_class(HINSTANCE hInstance);

   VOID CALLBACK TimerProcHost(HWND hwnd,  UINT uMsg,  uint_ptr idEvent,  DWORD dwTime);


   host::host(application * papp)
   {
      
      //Sleep(15 * 1000);

      m_pvoidSystem     = g_pvoidPluginSystem; 

      m_phost           = NULL;
      m_bInitialized    = false;
      m_bOk             = false;
      m_rect.left       = 0;
      m_rect.top        = 0;
      m_rect.bottom     = 0;
      m_rect.right      = 0;

      m_bRunningSpaAdmin = false;

      m_papp = papp;

      m_vssPluginName = "ca2 plugin";
      m_vssPluginDescription = "ca2 plugin for Firefox";

      m_rxchannel.create("ca2/fontopus/ccvotagus/spaboot_install_callback", "ca2plugin_container.dll");

      ::ca2plugin_container::register_class((HINSTANCE) ::GetModuleHandleA("ca2plugin_container.dll"));

      m_hwndMessage = ::CreateWindowExA(0, "npca2_message_window", "npca2_message_window", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

      SetTimer(m_hwndMessage, (uint_ptr) this, 84, TimerProcHost); // close to 12 frames per second.

      m_pfile           = NULL;

      m_lpbMemory = NULL;
      m_iMemory = -1;


      m_hwnd = NULL;
      m_bStream = false;


      m_strHostPluginLocation = calc_location_url();



   }

   host::~host()
   {
      ::DestroyWindow(m_hwndMessage);
   }

   static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
   static WNDPROC lpOldProc = NULL;







   NPBool host::init(NPWindow* aWindow)
   {

      if(aWindow == NULL)
         return FALSE;

      m_rect.left = aWindow->x;
      m_rect.top = aWindow->y;
      m_rect.right = m_rect.left + aWindow->width;
      m_rect.bottom = m_rect.top + aWindow->height;

      m_hwnd = (HWND)aWindow->window;

      if(m_hwnd == NULL)
         return FALSE;


      m_bInitialized    = true;
//      m_bReload         = false;


      //NPN_GetValue(m_instance, NPNVnetscapeWindow, &m_hwnd);


      //Sleep(15 * 1000);
      start_plugin();


      if(!initialize())
         return FALSE;


      return TRUE;


   }




   bool host::is_ok()
   {
      return m_bStream && m_bOk;
   }
   
   void host::post_message(UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
      ::PostMessage(m_hwnd, uiMessage, wparam, lparam);
   }





   NPError host::SetWindow(NPWindow* pNPWindow) 
   {
      m_rect.left = pNPWindow->x;
      m_rect.top = pNPWindow->y;
      m_rect.right = m_rect.left + pNPWindow->width;
      m_rect.bottom = m_rect.top + pNPWindow->height;

      set_window_rect(&m_rect);

      return NPERR_NO_ERROR;
   }


   void host::shut()
   {
      m_bInitialized = false;
      
      ::KillTimer(m_hwndMessage, (uint_ptr) this);

      finalize();

      m_hwnd = NULL;
   }



   bool host::open_url(const char * pszUrl)
   {
/*      NPVariant varLocation;
      varLocation.type = NPVariantType_String;
      varLocation.value.stringValue.UTF8Characters = pszUrl;
      varLocation.value.stringValue.UTF8Length = (uint32_t) strlen_dup(pszUrl);
      NPN_SetProperty(m_instance, m_varDocument.value.objectValue, sLocation_id, &varLocation);
      return true;*/
      return true;
   }



         /* Our top-level exception filter. */
LONG WINAPI MyUnhandledExceptionFilter(EXCEPTION_POINTERS* exceptionInfo)
{
  DWORD exceptionCode = exceptionInfo->ExceptionRecord->ExceptionCode;
//  if(exceptionCode == EXCEPTION_GUARD_PAGE)
  {
    /* Handle the situation somehow... */
    /* ...and resume the exception from the place the exception was thrown. */
    return EXCEPTION_CONTINUE_EXECUTION;
  }
  //else
  {
    /* Unknown exception, let another exception filter worry about it. */
    //return EXCEPTION_CONTINUE_SEARCH;
  }
}


   bool host::reload_plugin()
   {

      //NPN_ReloadPlugins(TRUE);
      // Client-side web page javascript will reload this killed and thus not functional
      // plugin served by the web server. This behaviour should be matched, otherwise
      // this killing call will cease the reload process.
      //m_dwReload = ::GetTickCount();
      //m_bReload = true;
      ::TerminateProcess(::GetCurrentProcess(), 0);


      return true;

   }

   vsstring host::calc_location_url()
   {
/*      NPVariant varLocation;
      if(!NPN_GetProperty(m_instance, m_pobjectWindow, sLocation_id, &varLocation))
         return "";
      NPObject * plocation = varLocation.value.objectValue;
      // Create a "href" identifier.
      NPIdentifier identifier = NPN_GetStringIdentifier("href");
      // Get the location property from the location object.
      NPVariant varValue;
      if(!NPN_GetProperty(m_instance, plocation, identifier, &varValue))
      {
         NPN_ReleaseVariantValue(&varLocation);
         return "";
      }
      vsstring strUrl(varValue.value.stringValue.UTF8Characters, varValue.value.stringValue.UTF8Length);
      NPN_ReleaseVariantValue(&varValue);
      NPN_ReleaseVariantValue(&varLocation);*/
      //return strUrl;

      return "";
   }

   HWND host::get_host_window()
   {
      return m_hwnd;
   }

   uint_ptr host::message_handler(uint_ptr uiMessage, WPARAM wparam, LPARAM lparam)
   {
      switch(uiMessage)
      {
      case WM_PAINT:
         m_bOnPaint = true;
         if(m_bInitialized)
         {
            on_paint((HDC) wparam, (LPCRECT) lparam);
         }
         if(m_bReload)
         {
            m_bReload = false;
            reload_plugin();
         }
         m_bOnPaint = false;
         return 0;
         // case WM_KEYDOWN:
         // today
         // MessageBox(NULL, "key_down", "key_down", NULL);
         // TerminateProcess(::GetCurrentProcess(), 0);
         // return 0;
/*      case WM_TIMER:
         {
            switch(wparam)
            {
            case 19841977:
               {
                  NPRect invalidRect;
                  invalidRect.left     = (uint16_t) m_rect.left;
                  invalidRect.top      = (uint16_t) m_rect.top;
                  invalidRect.right    = (uint16_t) m_rect.right;
                  invalidRect.bottom   = (uint16_t) m_rect.bottom;
//                  NPN_InvalidateRect(m_instance, &invalidRect);
                  NPN_ForceRedraw(m_instance);
               }
               break;
            }
         }*/
         return 0;
      }
      return ::hotplugin::host::message_handler(uiMessage, wparam, lparam);      
   }


   VOID CALLBACK TimerProcHost(HWND hwnd,  UINT uMsg,  uint_ptr idEvent,  DWORD dwTime)
   {

      host * phost = (host *) idEvent;
      phost->redraw();

      if(phost->is_ca2_installation_ready())
      {
         phost->set_ca2_installation_ready(false);
         phost->start_ca2();
      }

   }


   LRESULT CALLBACK window_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
   {
      return DefWindowProc(hWnd, message, wParam, lParam);
   }

   ATOM register_class(HINSTANCE hInstance)
   {
	   WNDCLASSEX wcex;

	   wcex.cbSize = sizeof(WNDCLASSEX);

	   wcex.style			   = 0;
	   wcex.lpfnWndProc	   = &window_proc;
	   wcex.cbClsExtra	   = 0;
	   wcex.cbWndExtra	   = 0;
	   wcex.hInstance		   = hInstance;
	   wcex.hIcon			   = NULL;
	   wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
	   wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
      wcex.lpszMenuName	   = NULL;
	   wcex.lpszClassName	= "npca2_message_window";
	   wcex.hIconSm		   = NULL;

	   return RegisterClassEx(&wcex);
   }


   void host::on_paint(HDC hdcWindow, LPCRECT lprect)
   {
      ::hotplugin::host::on_paint(hdcWindow, lprect);

   }

   bool host::finalize()
   {
      
      ::KillTimer(m_hwndMessage, (uint_ptr) this);

      return ::hotplugin::host::finalize();

   }


/*   NPError host::GetValue(NPPVariable variable, void *value)
   {
       
      switch (variable)
      {
      case NPPVpluginNameString:
         *((const char **)value) = m_vssPluginName;
         break;
      case NPPVpluginDescriptionString:
         *((const char **)value) = m_vssPluginDescription;
         break;
      case NPPVpluginScriptableNPObject:
         *(NPObject **)value = getScriptableObject();
         break;
      default:
         return NPERR_GENERIC_ERROR;
      }
      return NPERR_NO_ERROR;
   }*/

   /*

   NPObject * host::getScriptableObject()
   {
       if(m_phostjs == NULL)
       {
           m_phostjs = host_js::NewObject(this);
       }
       /* Bugfix from Facebook: Certain older versions of webkit do a retain when
        * you return an NPObject from NPP_GetValue instead of assuming that we do
        * it before returninglike the NPAPI spec instructs; this results in a memory
        * leak if we don't fix it.   
        */
       /* //today// if (m_retainReturnedNPObject)
           m_npHost->RetainObject(m_obj);*/
    
     //  return m_phostjs;
   //}
//   */
   // This is the preferred method to get strings from NPIdentifiers, since you
// don't have to worry about cleaning it up =]
/*vsstring host::StringFromIdentifier(NPIdentifier identifier)
{
 //   assertMainThread();
    NPUTF8* idStr = NPN_UTF8FromIdentifier(identifier);
    vsstring str;
    if (idStr != NULL)
        str = idStr;
    NPN_MemFree(idStr);
    return str;
}
*/
   void * host::get_system()
   {

      if(m_pvoidSystem == NULL)
      {

         m_pvoidSystem     = g_pvoidPluginSystem; 

      }

      return m_pvoidSystem;

   }


   void host::set_system(void * pvoidSystem)
   {

      ::hotplugin::host::set_system(pvoidSystem);

      g_pvoidPluginSystem = pvoidSystem;


   }



   void host::start_ca2()
   {


      if(is_installation_lock_file_locked())
      {
         
         set_installing_ca2();

         if(!m_phost->m_bInstalling)
         {
            m_phost->m_bInstalling = true;
            // shouldn't do advanced operations using ca2
            // starter_start will only kick a default app-install.exe if one isn't already running, cleaning file lock if any
            m_phost->starter_start(": app=session session_start=session app_type=application install");
         }
         return;
      }

      update_ca2_installed(false);

      if(is_ca2_installed())
      {
         
#ifdef WINDOWS
#ifdef X86
         ::SetDllDirectory(dir::ca2("stage\\x86"));
#else
         ::SetDllDirectory(dir::ca2("stage\\x64"));
#endif
#endif
         //Sleep(15 * 1000);

         ::ca::library ca2library;
         ca2library.open("ca");
         FN_NEW_HOTPLUGIN fn_new_hotplugin = ca2library.get < FN_NEW_HOTPLUGIN >("new_hotplugin");
         m_pplugin = fn_new_hotplugin();
         m_pplugin->m_phost = m_phost;
         m_bInstalling = false;
         start_ca2_system();
         delete this;
         return;
      }
      else
      {

         char szCa2ModuleFolder[MAX_PATH];
         
         if(dir::get_ca2_module_folder_dup(szCa2ModuleFolder))
         {

            stra_dup straPrevious;

            ::process_modules(straPrevious, ::GetCurrentProcessId());

         

            vsstring strDir = dir::path(szCa2ModuleFolder, "*.*");

            stra_dup stra;

            dir::ls(stra, strDir);

            try
            {

               bool bRetry = true;

               while(bRetry)
               {

                  bRetry = false;

                  for(int i = 0; i < stra.get_count(); i++)
                  {

                     HMODULE hmodule;

                     if(stricmp_dup(stra[i], "npca2.dll") == 0)
                        continue;

                     if(::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, stra[i], &hmodule) != FALSE)
                     {

                        bRetry = true;

                        try
                        {
                  
                           ::FreeLibrary(hmodule);

                        }
                        catch(...)
                        {

                        }

                     }


                  }

               }

            }
            catch(...)
            {
            }


      
      
            stra_dup straCurrent;

            ::process_modules(straCurrent, ::GetCurrentProcessId());



      
      
            ::load_modules_diff(straPrevious, straCurrent, szCa2ModuleFolder);

      
            ::initialize_primitive_heap();

            ::reset_http();

         }

         m_phost->starter_start(": app=session session_start=session app_type=application install");
      }

   }


   void host::rx(int message, void * pdata, int len)
   {

      if(message == ::hotplugin::message_init)
      {
         NPWindow * pwindow = (NPWindow *) pdata;
         init(pwindow);
      }
      else if(message == ::hotplugin::message_set_window)
      {
         NPWindow * pwindow = (NPWindow *) pdata;
         SetWindow(pwindow);
      }
      else if(message == ::hotplugin::message_paint)
      {
         struct paint
         {
            HDC m_hdc;
            RECT m_rect;
         } * ppaint;
         
         ppaint = (struct paint *) pdata;

         on_paint(ppaint->m_hdc, &ppaint->m_rect);

      }
      else if(message == ::hotplugin::message_set_ready)
      {

         if(m_puchMemory != NULL)
         {
            try
            {
               ca2_free(m_puchMemory);
            }
            catch(...)
            {
            }
         }

         m_countMemory = len;
         m_puchMemory = (uint8_t *) ca2_alloc(len);
         memcpy(m_puchMemory, pdata, len);

         if(m_puchMemory != NULL)
            m_bStream = true;

         set_ready();

      }
      else if(message == ::hotplugin::message_message)
      {
         MSG * pmsg = (MSG *) pdata;
         message_handler(pmsg->message, pmsg->wParam, pmsg->lParam);
      }
      
      m_papp->send(message, NULL, 0);

   }

} // namespace ca2plugin_container



/*extern "C"
{
#include <ntddk.h>
};*/

/*#pragma warning (disable: 4731) // EBP register overwritten
#pragma warning (disable: 4733) // non-SAFESEH compliant

typedef void (__stdcall* CleanupFN)();

struct CleanupEntry
{
    long		m_nPrevIdx;
    CleanupFN	m_pfnCleanup;
};

struct UnwindFrame
{
    ULONG			m_sig;
    long			m_nCleanupCount;   //number of entries in unwindtable
    CleanupEntry*	m_pCleanupArr;
	// C++ try-catch data is omitted
};

struct ExcReg
{ 
    ExcReg*	m_pPrev;
    ULONG	m_handler;
    long	m_NextCleanup;
    ULONG	m_ebp;
};

void FrameUnwind(UnwindFrame* pUFrame, ULONG dwEbp, ExcReg* pExcFrame)
{
    const CleanupEntry* pCleanupArr = pUFrame->m_pCleanupArr;
    int count = pUFrame->m_nCleanupCount;

//commented on insertion 2011-05-31	ASSERT(pExcFrame->m_NextCleanup < pUFrame->m_nCleanupCount); // stack corruption test

	while (true)
	{
		long nNextID = pExcFrame->m_NextCleanup;
		if (nNextID < 0)
			break;

		const CleanupEntry& entry = pCleanupArr[nNextID];
		CleanupFN pfnCleanup = entry.m_pfnCleanup;
		pExcFrame->m_NextCleanup = entry.m_nPrevIdx;

		__try {
			_asm
			{
				mov EAX, pfnCleanup
				push EBP       //save current EBP
				mov EBP, dwEbp  //the EBP of the unwinding function
				call EAX
				pop EBP
			}
		} __finally {
			if (AbnormalTermination())
				// one of the destructor has raised an exception that resulted
				// in another unwind.
				FrameUnwind(pUFrame, dwEbp, pExcFrame); // recursively
		}
	}
}

EXCEPTION_DISPOSITION __cdecl FrameHandler(UnwindFrame* pUFrame, EXCEPTION_RECORD* pExc, ExcReg* pExcFrame)
{
	// EXCEPTION_UNWINDING = 2, EXCEPTION_EXIT_UNWIND = 4
    if (6 & pExc->ExceptionFlags)
		FrameUnwind(pUFrame, (ULONG) (ulong_ptr) &pExcFrame->m_ebp, pExcFrame);
    return ExceptionContinueSearch;
}

extern "C" __declspec (naked)
EXCEPTION_DISPOSITION __cdecl __CxxFrameHandler3(int a, int b, int c, int d)
{
	_asm {
		//prolog
        push EBP
        mov EBP, ESP

		// We don't use the last two parameters (processor and dispatcher contexts)
		// Hence - push just the 1st two parameters, plus the UnwindFrame passed in EAX
        push b
        push a
        push EAX

        call FrameHandler

		//epilog
        mov ESP, EBP
        pop EBP
        ret
	};
}
*/