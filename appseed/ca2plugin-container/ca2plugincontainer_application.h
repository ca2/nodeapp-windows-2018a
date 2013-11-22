#pragma once


namespace ca2plugin_container
{


   class application :
      virtual public ::base_application
   {
   public:


      int32_t                        m_iReturn;

      string                     m_strChannel;

      host *                     m_phost;


      application();
      virtual ~application();


      virtual void on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage);
      virtual void on_receive(small_ipc_rx_channel * prxchannel, int32_t message, void * pdata, int32_t len);

      virtual bool initialize(const char * pszChannel);

      virtual bool finalize();


      virtual int32_t run();


      virtual void restart_small_ipc_channel();

   };



} // namespace ca2plugin_container






