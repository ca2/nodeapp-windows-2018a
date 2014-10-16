#pragma once


namespace ca2plugin_container
{


   class application :
      virtual public ::core::application,
      virtual public ::hotplugin::composer
   {
   public:


      int32_t                    m_iReturn;

      string                     m_strChannel;

      host *                     m_phost;


      application(sp(::aura::application) papp, const char * pszChannel);
      virtual ~application();


      virtual void on_receive(small_ipc_rx_channel * prxchannel, const char * pszMessage);
      virtual void on_receive(small_ipc_rx_channel * prxchannel, int32_t message, void * pdata, int32_t len);

      virtual bool initialize_communication();

      virtual bool finalize();

      virtual bool os_native_bergedge_start();


      virtual int32_t run();


      virtual void restart_small_ipc_channel();


      virtual ::hotplugin::host * create_host(sp(::base::system) psystem);

      virtual ::hotplugin::host * create_host();


      virtual bool on_run_step();


   };



} // namespace ca2plugin_container






