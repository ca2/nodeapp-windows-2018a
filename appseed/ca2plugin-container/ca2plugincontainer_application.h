#pragma once


namespace ca2plugin_container
{


   class application :
      virtual public ::plane::session
   {
   public:


      int32_t                        m_iReturn;

      string                     m_strChannel;

      host *                     m_phost;


      application(sp(base_application) papp, const char * pszChannel);
      virtual ~application();


      virtual void on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage);
      virtual void on_receive(small_ipc_rx_channel * prxchannel, int32_t message, void * pdata, int32_t len);

      virtual bool initialize_communication();

      virtual bool finalize();

      virtual bool os_native_bergedge_start();


      virtual int32_t run();


      virtual void restart_small_ipc_channel();

   };



} // namespace ca2plugin_container






