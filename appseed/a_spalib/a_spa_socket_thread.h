#pragma once


namespace a_spa
{


   class service;
   class socket;


   class SPALIB_API socket_thread :
      public ::thread
   {
   public:


      bool                                         m_bSsl;


      socket_thread(::aura::application * papp);
      ~socket_thread();


      virtual bool initialize_thread() override;
      virtual int32_t run();


      void install_message_handling(::message::dispatch * pinterface);

      void setup_socket(::sockets::listen_socket<socket> & ll);

   };


} // namespace a_spa



