#pragma once

#include "npapi.h"

namespace ca2plugin_container
{

   class application;


   class host : 
      public ::hotplugin::host
   {
   public:

   
      oswindow                      m_oswindowMessage;
      oswindow                      m_oswindow;
      bool                          m_bStream;

      string                        m_vssPluginName;
      string                        m_vssPluginDescription;

      sp(::ca2plugin_container::application)               m_pcontainerapp;


      host(sp(::ca2plugin_container::application) papp);
      virtual ~host();

      virtual oswindow get_host_window();
      virtual bool is_ok();
      virtual void post_message(uint32_t uiMessage, WPARAM wparam, LPARAM lparam);
      

      NPBool init(NPWindow* aWindow);
      void shut();

      // host
      virtual bool      open_url(const char * pszUrl);
      virtual bool      reload_plugin();


      virtual string  calc_location_url();


      bool plugin_finalize();



      void message_handler(signal_details * pobj);


      virtual void * get_system();
      virtual void set_system(void * pvoidPluginSystem);

      virtual void on_receive(small_ipc_rx_channel * prxchannel, int32_t message, void * pdata, int32_t len);


      virtual void start_ca2();

      virtual void restart_small_ipc_channel();


   };


} // namespace ca2plugin_container



