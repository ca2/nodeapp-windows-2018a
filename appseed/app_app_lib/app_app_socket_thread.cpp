#include "framework.h"


namespace app_app
{


   socket_thread::socket_thread(::aura::application * papp) :
      ::object(papp),
      thread(papp),
      ::netserver::socket_thread(papp)
   {

      m_iSsl = 1;

   }

   socket_thread::~socket_thread()
   {


   }


   void socket_thread::install_message_handling(::message::dispatch * pinterface)
   {

   }

   ::netserver::socket_handler * socket_thread::create_socket_handler()
   {

      return new ::app_app::socket_handler(get_app());


   }


   ::sockets::listen_socket_base * socket_thread::create_listen_socket()
   {

      return new ::sockets::listen_socket < socket >(*m_psockethandler);

   }


} // namespace app_app

