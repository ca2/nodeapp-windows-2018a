#pragma once



namespace a_spa
{


   class application;
   class service;


   class SPALIB_API socket :
      virtual public ::netserver::socket
   {
   public:


      socket(::sockets::base_socket_handler & h);
      ~socket();

      virtual void on_send_response() override;


   };


} // namespace a_spa

