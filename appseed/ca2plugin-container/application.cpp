#include "framework.h"


namespace ca2plugin_container
{


   application::application()
   {

   }

   application::~application()
   {
   }

   bool application::initialize(const char * pszChannel)
   {

      
      m_phost = new host(this);


      vsstring strChannel = "/ca2/ca2plugin-container-";
      strChannel += pszChannel;

      
      if(!::small_ipc_channel::open_ba(strChannel, "ca2plugin-container.exe"))
         return false;

      m_phost->start_ca2();

      if(!m_phost->initialize())
         return false;


      return true;


   }





   void application::on_receive(const char * pszMessage)
   {

      UNREFERENCED_PARAMETER(pszMessage);

   }

   void application::on_receive(int message, void * pdata, int len)
   {

      if(m_phost != NULL)
      {

         m_phost->rx(message, pdata, len);

      }

   }



   bool application::finalize()
   {

      return true;

   }

   int application::run()
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






