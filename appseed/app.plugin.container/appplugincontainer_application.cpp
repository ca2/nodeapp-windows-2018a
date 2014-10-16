#include "framework.h"


namespace ca2plugin_container
{


   application::application(sp(::aura::application) papp, const char * pszChannel) :
      element(papp)
   {

      m_strChannel = pszChannel;

      m_durationRunLock = millis(884);

   }

   application::~application()
   {
   }

   void application::restart_small_ipc_channel()
   {

      m_phost->m_strBitmapChannel = m_strChannel;

      string strChannel = "\\core\\app.plugin.container-";

      strChannel += m_strChannel;

      m_phost->::small_ipc_channel::open_ba(strChannel, "app.plugin.container.exe");

   }

   bool application::initialize_communication()
   {

      restart_small_ipc_channel();

      m_phost->start_ca2();

      return true;

   }


   bool application::os_native_bergedge_start()
   {

      if (!initialize_communication())
         return false;

      return true;
   }


   void application::on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage)
   {

      UNREFERENCED_PARAMETER(pszMessage);

   }

   void application::on_receive(small_ipc_rx_channel * prxchannel, int32_t message, void * pdata, int32_t len)
   {

      if(m_phost != NULL)
      {

         m_phost->on_receive(prxchannel, message, pdata, len);

      }

   }



   bool application::finalize()
   {

      return true;

   }

   int32_t application::run()
   {
      return thread::run();

/*      MSG msg;
      
      while(true)
	   {

         if(!GetMessage(&msg, NULL, 0, 0xffffffffu))
            break;

         if(msg.message == WM_QUIT)
            break;

		   TranslateMessage(&msg);
		   DispatchMessage(&msg);

	   }*/

      return 0;

   }


   ::hotplugin::host * application::create_host(sp(::base::system) psystem)
   {

      m_pbasehost = new ::ca2plugin_container::host(this);

      m_pbasehost->m_paurasession = m_paurasession;

      m_pbasehost->m_paxissession = m_paxissession;

      m_pbasehost->m_pbasesession = m_pbasesession;

      m_pbasehost->m_paxisapp = m_paxisapp;

      return m_pbasehost;


   }


   ::hotplugin::host * application::create_host()
   {

      m_phost = new host(this);

      m_phost->m_paurasession = m_paurasession;

      m_phost->m_paxissession = m_paxissession;

      m_phost->m_pbasesession = m_pbasesession;

      m_phost->m_paxisapp = m_paxisapp;

      m_phost->m_pbaseapp = this;

      m_phost->m_pcoreapp = this;

      m_phost->m_pauraapp->m_pcoreapp = this;

      return m_phost;

   }


   bool application::on_run_step()
   {

      if(!::core::application::on_run_step())
      {
       
         return false;

      }

      if(System.install().is_installing_ca2())
      {

         System.post_thread_message(WM_QUIT);

         return false;

      }

      return true;

   }



} // namespace ca2plugin_container






