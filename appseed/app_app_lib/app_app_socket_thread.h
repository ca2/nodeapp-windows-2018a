#pragma once


namespace app_app
{


   class service;
   class socket;


   class SPALIB_API socket_thread :
      virtual public ::netserver::socket_thread
   {
   public:


      socket_thread(::aura::application * papp);
      ~socket_thread();


      void install_message_handling(::message::dispatch * pinterface);

      void setup_socket(::sockets::listen_socket<socket> & ll);

      virtual ::netserver::socket_handler * create_socket_handler();
      virtual ::sockets::listen_socket_base * create_listen_socket();


   };


} // namespace app_app



