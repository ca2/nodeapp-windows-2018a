#pragma once

#include "npapi.h"

namespace ca2plugin_container
{

   class application;


   class host : 
      public ::hotplugin::host
   {
   public:

   
      HWND                          m_hwndMessage;
      HWND                          m_hwnd;
      bool                          m_bStream;

      vsstring                      m_vssPluginName;
      vsstring                      m_vssPluginDescription;

      application *                 m_papp;      


      host(application * papp);
      virtual ~host();

      virtual HWND get_host_window();
      virtual bool is_ok();
      virtual void post_message(UINT uiMessage, WPARAM wparam, LPARAM lparam);
      

      NPBool init(NPWindow* aWindow);
      void shut();

      // host
      virtual bool      open_url(const char * pszUrl);
      virtual bool      reload_plugin();


      virtual vsstring  calc_location_url();


      void on_paint(HDC hdcWindow, LPCRECT lprect);

      bool finalize();



#ifdef WINDOWS
      uint_ptr message_handler(uint_ptr uiMessage, WPARAM wparam, LPARAM lparam);
#else
      int message_handler(XEvent * pevent);
#endif

      virtual void * get_system();
      virtual void set_system(void * pvoidPluginSystem);

      virtual void on_receive(small_ipc_rx_channel * prxchannel, int message, void * pdata, int len);


      virtual void start_ca2();

   };

} // namespace ca2plugin_container

