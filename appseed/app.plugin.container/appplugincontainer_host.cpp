#include "framework.h"
#include <eh.h>

uint32_t g_nRedrawThreadID;
extern bool g_bAppStarted;
extern void * g_pvoidPluginSystem;


namespace ca2plugin_container
{


   ATOM register_class(HINSTANCE hInstance);


   host::host(sp(::ca2plugin_container::application) papp):
      element(papp),
      ::simple_ui::style(papp),
      ::user::interaction(papp),
      hotplugin::plugin(papp),
      base::session(papp)
   {

      m_pcontainerapp         = papp;

      m_pbasecomposer         = papp;

      m_phost                 = NULL;
      m_bInitialized          = false;
      m_bOk                   = false;

      m_bRunningSpaAdmin      = false;

      m_vssPluginName         = "ca2 plugin";
      m_vssPluginDescription  = "ca2 plugin for Firefox";

      ::ca2plugin_container::register_class((HINSTANCE) ::GetModuleHandleA("ca2plugin_container.dll"));

      m_oswindowMessage       = ::CreateWindowExA(0, "npca2_message_queue", "npca2_message_queue", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

      m_pfile                 = NULL;

      m_oswindow              = NULL;
      m_bStream               = false;


      m_pbasecomposer->m_strHostPluginLocation = calc_location_url();



   }

   host::~host()
   {
      ::DestroyWindow(m_oswindowMessage);
   }


   NPBool host::init(NPWindow* aWindow)
   {

      if(aWindow == NULL)
         return FALSE;

      RepositionWindow(aWindow->x, aWindow->y, aWindow->width, aWindow->height);

      m_oswindow = (oswindow) aWindow->window;

      if(m_oswindow == NULL)
         return FALSE;


      m_bInitialized    = true;
//      m_bReload         = false;


      //NPN_GetValue(m_instance, NPNVnetscapeWindow, &m_oswindow);

      if(!::simple_ui::interaction::create_message_queue("ca2plugin_container::host::init create_message_queue"))
         return FALSE;


      //Sleep(15 * 1000);
      start_plugin();


      if(!plugin_initialize())
         return FALSE;


      return TRUE;


   }




   bool host::is_ok()
   {

      return m_bStream && m_bOk;

   }
   

   void host::post_message(uint32_t uiMessage, WPARAM wparam, LPARAM lparam)
   {
      
      ::PostMessage(m_oswindow, uiMessage, wparam, lparam);

   }


   void host::shut()
   {

      m_bInitialized = false;
      
      ::KillTimer(m_oswindowMessage, (uint_ptr) this);

      plugin_finalize();

      m_oswindow = NULL;

   }


   bool host::open_url(const char * pszUrl)
   {

      ensure_tx(::hotplugin::message_open_url, (void *) pszUrl, (int32_t) strlen(pszUrl));

      return true;

   }


   void host::restart_small_ipc_channel()
   {

      m_pcontainerapp->restart_small_ipc_channel();

   }



   bool host::reload_plugin()
   {

      ::TerminateProcess(::GetCurrentProcess(), 0);

      return true;

   }

   string host::calc_location_url()
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
      string strUrl(varValue.value.stringValue.UTF8Characters, varValue.value.stringValue.UTF8Length);
      NPN_ReleaseVariantValue(&varValue);
      NPN_ReleaseVariantValue(&varLocation);*/
      //return strUrl;

      return "";
   }

   oswindow host::get_host_window()
   {
      return m_oswindow;
   }


   void host::message_handler(signal_details * pobj)
   {

      ::hotplugin::host::message_handler(pobj);

   }


   LRESULT CALLBACK window_proc(oswindow oswindow, uint32_t message, WPARAM wParam, LPARAM lParam)
   {
      return DefWindowProc(oswindow, message, wParam, lParam);
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
	   wcex.lpszClassName	= "npca2_message_queue";
	   wcex.hIconSm		   = NULL;

	   return RegisterClassEx(&wcex);
   }


   bool host::plugin_finalize()
   {
      
      ::KillTimer(m_oswindowMessage, (uint_ptr) this);

      return ::hotplugin::host::plugin_finalize();

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
/*string host::StringFromIdentifier(NPIdentifier identifier)
{
 //   assertMainThread();
    NPUTF8* idStr = NPN_UTF8FromIdentifier(identifier);
    string str;
    if (idStr != NULL)
        str = idStr;
    NPN_MemFree(idStr);
    return str;
}
*/
   void * host::get_system()
   {

      return m_pbaseapp->m_pbasesystem;

   }


   void host::set_system(void * pvoidSystem)
   {

      //::hotplugin::host::set_system(pvoidSystem);

      m_pbaseapp = (::base::system *) pvoidSystem;


   }



   void host::start_ca2()
   {


      if(System.install().is_installing_ca2())
      {

         if(!m_bInstalling)
         {
            m_bInstalling = true;
            // shouldn't do advanced operations using ca2
            // starter_start will only kick a default app.install.exe if one isn't already running, cleaning file lock if any
            host_starter_start(": app=session session_start=session app_type=application install");
         }
         return;
      }

      System.install().update_ca2_installed(false);

      if (System.install().is_ca2_installed())
      {
         
#ifdef WINDOWS
#ifdef X86
         //::SetDllDirectory(dir::element("stage\\x86"));
#else
         //::SetDllDirectory(dir::element("stage\\x64"));
#endif
#endif
         //Sleep(15 * 1000);

         m_pplugin = new ::plugin::instance(this);
         m_pplugin->m_pbasesession = m_pbasesession;
         m_pplugin->m_pbasesystem = m_pbasesystem;
         m_pplugin->m_pcoreapp = m_pcoreapp;
         m_pplugin->m_pplatformcomposite = m_pplatformcomposite;
         m_pplugin->m_pbaseapp = this;
         m_pplugin->m_phost = this;
         m_pplugin->m_strBitmapChannel = m_strBitmapChannel;
         m_bInstalling = false;
         start_ca2_system();
         return;
      }
      else
      {

         host_starter_start(": app=session session_start=session app_type=application install");

      }

   }


   void host::on_receive(small_ipc_rx_channel * prxchannel, int32_t message, void * pdata, int32_t len)
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
            
            const RECT & rect = (LPCRECT) pdata;

            m_rect = *lpcrect;

            if(!IsWindow())
            {

               ::simple_ui::interaction::create_message_queue("ca2plugin_container::host::init create_message_queue");

            }

            try
            {

               SetPlacement(lpcrect);

            }
            catch(...)
            {
            }

         }
         else if(message == ::hotplugin::message_paint)
         {
            
            
            LPRECT prect = (LPRECT) pdata;

            try
            {

               ::draw2d::graphics_sp g(allocer());

               g->CreateCompatibleDC(NULL);

               on_paint(g, prect);

            }
            catch(...)
            {
            }

         }
         else if(message == ::hotplugin::message_set_ready)
         {

            if(pdata != NULL)
            {

               m_memory.assign(pdata, len);

//  xxx             if(m_puchMemory != NULL)
  //                m_bStream = true;


               string str((const char *) m_memory.get_data(), len);

               xxdebug_box(str, "ca2plugincontainer::host::on_receive", 0);


            }

            m_bStream = true;

            set_ready();

         }
         else if(message == ::hotplugin::message_set_plugin_url)
         {


            string str((const char *) pdata, len);

            xxdebug_box(str, "ca2plugincontainer::host::on_receive", 0);

            m_pbasecomposer->m_strPluginUrl = str;

         }
         else if(message == ::hotplugin::message_message)
         {

            MSG * pmsg = (MSG *) pdata;

            try
            {

               message_handler(m_pplugin->get_base(pmsg));

            }
            catch(...)
            {
            }


         }
      
      }

   }

} // namespace ca2plugin_container



