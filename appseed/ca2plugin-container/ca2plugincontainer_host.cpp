#include "framework.h"
#include <eh.h>

UINT g_nRedrawThreadID;
extern bool g_bAppStarted;
extern void * g_pvoidPluginSystem;


namespace ca2plugin_container
{


   ATOM register_class(HINSTANCE hInstance);


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

      ::ca2plugin_container::register_class((HINSTANCE) ::GetModuleHandleA("ca2plugin_container.dll"));

      m_hwndMessage = ::CreateWindowExA(0, "npca2_message_window", "npca2_message_window", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

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


   void host::shut()
   {

      m_bInitialized = false;
      
      ::KillTimer(m_hwndMessage, (uint_ptr) this);

      finalize();

      m_hwnd = NULL;

   }


   bool host::open_url(const char * pszUrl)
   {

      ensure_tx(::hotplugin::message_open_url, (void *) pszUrl, strlen(pszUrl));

      return true;

   }



   bool host::reload_plugin()
   {

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
         }
         return 0;*/
      }
      return ::hotplugin::host::message_handler(uiMessage, wparam, lparam);      
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

         if(!m_bInstalling)
         {
            m_bInstalling = true;
            // shouldn't do advanced operations using ca2
            // starter_start will only kick a default app-install.exe if one isn't already running, cleaning file lock if any
            starter_start(": app=session session_start=session app_type=application install");
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

         m_pplugin = new ::plugin::instance();
         m_pplugin->m_phost = this;
         m_pplugin->m_strBitmapChannel = m_strBitmapChannel;
         m_bInstalling = false;
         start_ca2_system();
         return;
      }
      else
      {

         starter_start(": app=session session_start=session app_type=application install");

      }

   }


   void host::on_receive(small_ipc_rx_channel * prxchannel, int message, void * pdata, int len)
   {

      if(prxchannel == &m_rxchannel)
      {

         if(message == ::hotplugin::message_init)
         {
            
            NPWindow * pwindow = (NPWindow *) pdata;

            try
            {

               init(pwindow);

            }
            catch(...)
            {
            }


         }
         else if(message == ::hotplugin::message_set_window)
         {
            
            LPCRECT lpcrect = (LPCRECT) pdata;

            try
            {

               set_window_rect(lpcrect);

            }
            catch(...)
            {
            }

         }
         else if(message == ::hotplugin::message_paint)
         {
            
            struct paint
            {
               HDC      m_hdc;
               RECT     m_rect;
            } * ppaint;
         
            ppaint = (struct paint *) pdata;

            try
            {

               on_paint(ppaint->m_hdc, &ppaint->m_rect);

            }
            catch(...)
            {
            }

         }
         else if(message == ::hotplugin::message_set_ready)
         {

            if(pdata != NULL)
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


               vsstring str((const char *) m_puchMemory, len);

               // debug_box(str, "ca2plugincontainer::host::on_receive", 0);


               set_ready();

            }

         }
         else if(message == ::hotplugin::message_message)
         {

            MSG * pmsg = (MSG *) pdata;

            try
            {

               message_handler(pmsg->message, pmsg->wParam, pmsg->lParam);

            }
            catch(...)
            {
            }


         }
      
      }

   }

} // namespace ca2plugin_container



