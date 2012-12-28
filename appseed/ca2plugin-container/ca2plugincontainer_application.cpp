#include "framework.h"


namespace ca2plugin_container
{


   application::application()
   {

      m_phost = new host(this);

   }

   application::~application()
   {
   }

   void application::restart_small_ipc_channel()
   {

      m_phost->m_strBitmapChannel = m_strChannel;

      vsstring strChannel = "\\ca2\\ca2plugin-container-";

      strChannel += m_strChannel;

      m_phost->::small_ipc_channel::open_ba(strChannel, "ca2plugin-container.exe");

   }

   bool application::initialize(const char * pszChannel)
   {

      m_strChannel = pszChannel;

      restart_small_ipc_channel();

      m_phost->start_ca2();

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

      MSG msg;
      
      while(true)
	   {

         if(!GetMessage(&msg, NULL, 0, 0xffffffffu))
            break;

         if(msg.message == WM_QUIT)
            break;

		   TranslateMessage(&msg);
		   DispatchMessage(&msg);

	   }

      return 0;

   }


} // namespace ca2plugin_container






