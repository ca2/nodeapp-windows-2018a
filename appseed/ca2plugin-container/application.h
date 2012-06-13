#pragma once


namespace ca2plugin_container
{


   class application :
      virtual public small_ipc_channel
   {
   public:


      int                        m_iReturn;

      host *                     m_phost;


      application();
      virtual ~application();


      virtual void on_receive(const char * pszMessage);
      virtual void on_receive(int message, void * pdata, int len);

      virtual bool initialize(const char * pszChannel);

      virtual bool finalize();


      virtual int run();

   };



} // namespace ca2plugin_container






