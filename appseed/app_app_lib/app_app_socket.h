#pragma once



namespace app_app
{


   class application;
   class service;


   class SPALIB_API socket :
      virtual public ::netserver::socket
   {
   public:


      bootstrap *          m_pbootstrap;


      socket(::sockets::base_socket_handler & h);
      virtual ~socket();

      virtual void on_send_response() override;


   };


} // namespace app_app

