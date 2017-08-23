#include "framework.h"
#include <eh.h>

uint32_t g_nRedrawThreadID;
extern bool g_bAppStarted;
extern void * g_pvoidPluginSystem;


namespace ca2plugin_container
{


   ATOM register_class(HINSTANCE hInstance);


   host::host(sp(::ca2plugin_container::application) papp) :
      ::object(papp),
      ::simple_ui::style(papp),
      ::user::interaction(papp),
      hotplugin::plugin(papp),
      aura::session(papp),
      axis::session(papp),
      ::base::session(papp),
      ::aura::ipc::base(papp),
      ::aura::ipc::tx(papp),
      ::aura::ipc::ipc(papp)
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

      m_bMayProDevian         = false;


   }

   host::~host()
   {
      ::DestroyWindow(m_oswindowMessage);
   }


   void host::install_message_routing(::message::sender * pinterface)
   {

      ::hotplugin::host::install_message_routing(pinterface);

      ////IGUI_WIN_MSG_LINK(WM_TIMER,pinterface,this,&host::_001OnTimer);

   }


   void host::_001OnTimer(::timer * ptimer)
   {

      ::hotplugin::host::_001OnTimer(ptimer);

      if(ptimer->m_nIDEvent == 1984 + 77 + 3)
      {

         _001UpdateWindow();

         on_paint(NULL,m_rectClient);

         //if(!m_bMayProDevian && GetParent() == NULL)
         //{

         //   //RedrawWindow();

         //}

      }


   }


   bool host::init()
   {

      if(m_bInitialized)
         return true;

      m_bInitialized    = true;

      if(!::simple_ui::interaction::create_message_queue("ca2plugin_container::host::init create_message_queue"))
         return FALSE;

      start_plugin();

      if(!plugin_initialize())
         return FALSE;

      return TRUE;


   }


   void host::start_plugin()
   {

      ::plugin::instance * pplugin           = new ::plugin::instance(this);

      m_pplugin                              = pplugin;

      m_pplugin->m_paurasession              = m_paurasession;

      m_pplugin->m_paxissession              = m_paxissession;

      m_pplugin->m_pbasesession              = m_pbasesession;

      m_pplugin->m_pbasesystem               = m_pbasesystem;

      m_pplugin->m_paxisapp                  = m_paxisapp;

      m_pplugin->m_pbaseapp                  = m_pbaseapp;

      m_pplugin->m_pcoreapp                  = m_pcoreapp;

      m_pplugin->m_pcoresession        = m_pcoresession;

      m_pplugin->m_phost                     = this;

      m_pcontainerapp->m_psystem->m_pplugin  = pplugin;

      m_pplugin->m_strBitmapChannel          = m_strBitmapChannel;

      m_bInstalling                          = false;

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


   bool host::open_link(const string & strLink,const string & strTarget)
   {

      ensure_tx(::hotplugin::message_open_url,(void *)strLink.c_str(),(int32_t)strLink.get_length(), 1984);
      
      return true;

   }


   void host::restart_aura_ipc()
   {

      m_pcontainerapp->restart_aura_ipc();

   }



   bool host::reload_plugin()
   {

      ::TerminateProcess(::GetCurrentProcess(), 0);

      return true;

   }


   oswindow host::get_host_window()
   {
      return m_oswindow;
   }


   void host::message_handler(::message::message * pobj)
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
            // starter_start will only kick a default app_app_admin.exe if one isn't already running, cleaning file lock if any
            start_app_install(": app=session session_start=session app_type=application install");
         }
         return;
      }

      System.install().update_ca2_installed(false);

      if (System.install().is_ca2_installed())
      {

         if(!init())
            return;

         return;

      }
      else
      {

         start_app_install(": app=session session_start=session app_type=application install");

      }

   }


   void host::on_receive(::aura::ipc::rx * prx, int32_t message, void * pdata, int32_t len)
   {

      if(prx == &m_rx)
      {

         //if(message == ::hotplugin::message_init)
         //{
         //   
         //   NPWindow * pwindow = (NPWindow *) pdata;

         //   try
         //   {

         //      init(pwindow);

         //   }
         //   catch(...)
         //   {
         //   }


         //}
//         else if(message == ::hotplugin::message_set_window)
         if(message == ::hotplugin::message_set_window)
         {
            
            const RECT & rect = *((LPCRECT) pdata);

            //if(m_rect != rect)
            {

               m_rect = rect;

               m_rectClient = rect;

               m_rectClient.offset(-m_rectClient.top_left());

               m_rectWindow = rect;

               try
               {

                  SetPlacement(rect);

               }
               catch(...)
               {
               }

            }

         }
         else if(message == ::hotplugin::message_paint)
         {
            
            
            const rect & rect = *((LPCRECT) pdata);

            try
            {

               _001UpdateWindow();

               //::draw2d::graphics_sp g(allocer());

               //g->CreateCompatibleDC(NULL);

               //on_paint(g, rect);

               on_paint(NULL,rect);

            }
            catch(...)
            {
            }

         }
         else if(message == WM_APP+WM_PAINT)
         {


            try
            {

               //::draw2d::graphics_sp g(allocer());

               //g->CreateCompatibleDC(NULL);

               //on_paint(g,m_rectClient);

               on_paint(NULL,m_rectClient);

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

            if(pmsg->message == WM_ACTIVATE)
            {

               if(LOWORD(pmsg->wParam) == WA_ACTIVE)
               {
                  
                  Session.m_puiActive = this;
                  
               }
               else
               {

                  Session.m_puiActive = NULL;

               }

               return;

            }
            else if(pmsg->message == WM_SETFOCUS)
            {
               
               Session.set_keyboard_focus(this);

               return;

            }
            else if(pmsg->message == WM_KILLFOCUS)
            {
               
               Session.set_keyboard_focus(NULL);

               return;

            }
            else if(pmsg->message == WM_CLOSE)
            {

               m_pcontainerapp->post_quit();

               return;

            }

            try
            {

               message_handler(get_base(pmsg->message, pmsg->wParam, pmsg->lParam));

            }
            catch(...)
            {
            }


         }
      
      }

   }

   bool host::SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags)
   {

      bool bOk = ::hotplugin::host::SetWindowPos(z,x,y,cx,cy,nFlags);

  
      try
      {

         if(m_pcontainerapp != NULL)
         {

            if(m_pcontainerapp->m_psystem != NULL)
            {

               if(m_pcontainerapp->m_psystem->m_pplugin != NULL)
               {

                  if(m_pcontainerapp->m_psystem->m_pplugin->m_puiHost != NULL)
                  {

                     m_pcontainerapp->m_psystem->m_pplugin->m_puiHost->SetWindowPos(z,x,y,cx,cy,nFlags);

                  }

               }

            }

         }

      }
      catch(...)
      {
      }

      return bOk;

   }

   
   void host::_001Print(::draw2d::graphics * pgraphics)
   {

      if(m_pcontainerapp != NULL)
      {

         if(m_pcontainerapp->m_psystem != NULL)
         {

            if(m_pcontainerapp->m_psystem->m_pplugin != NULL)
            {

               if(m_pcontainerapp->m_psystem->m_pplugin->m_puiHost != NULL)
               {

                  m_pcontainerapp->m_psystem->m_pplugin->m_puiHost->_001Print(pgraphics);

                  {

                     static DWORD dwLast = 0;
                     static ::count c = 0;
                     static double dLast = 0.0;

                     c++;

                     DWORD dwNow = ::get_tick_count();

                     DWORD dwSpan = dwNow - dwLast;

                     if(dwSpan > 1000)
                     {

                        dLast = c * 1000.0 / dwSpan;

                        c = 0;

                        dwLast = dwNow;

                     }

#if 0

                     string str;

                     str.Format("%0.1f fps",dLast);

                     pgraphics->set_text_color(ARGB(255,255,255,0));

                     pgraphics->TextOutA(300,20,str);

#endif

                  }

               }

            }

         }

      }

   }


} // namespace ca2plugin_container



