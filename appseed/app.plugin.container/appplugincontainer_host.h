#pragma once



namespace ca2plugin_container
{

   class application;


   class host : 
      public ::hotplugin::host
   {
   public:

   
      oswindow                                  m_oswindowMessage;
      oswindow                                  m_oswindow;
      bool                                      m_bStream;

      string                                    m_vssPluginName;
      string                                    m_vssPluginDescription;

      ::ca2plugin_container::application *      m_pcontainerapp;


      host(sp(::ca2plugin_container::application) papp);
      virtual ~host();

      virtual oswindow get_host_window();
      virtual bool is_ok();
      virtual void post_message(uint32_t uiMessage, WPARAM wparam, LPARAM lparam);
      

      bool init();
      void shut();

      // host
      virtual bool open_link(const string & strLink,const string & strTarget);
      virtual bool reload_plugin();


      virtual void install_message_handling(::message::dispatch * pinterface);

      void start_plugin();

      bool plugin_finalize();



      void message_handler(signal_details * pobj);


      virtual void * get_system();
      virtual void set_system(void * pvoidPluginSystem);

      virtual void on_receive(::aura::ipc::rx * prx, int32_t message, void * pdata, int32_t len);
      virtual bool SetWindowPos(int_ptr z,int32_t x,int32_t y,int32_t cx,int32_t cy,UINT nFlags);

      virtual void start_ca2();

      virtual void restart_aura_ipc();

      virtual void _001OnTimer(::timer * ptimer);

      void _001Print(::draw2d::graphics * pgraphics);


   };


} // namespace ca2plugin_container



