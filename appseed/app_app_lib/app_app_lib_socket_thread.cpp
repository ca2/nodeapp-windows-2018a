#include "framework.h"
#include "../a_spares/resource.h"

namespace a_spa
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


//   int socket_thread::run()
//   {
//
//      while (get_run_thread())
//      {
//
//         try
//         {
//
//            socket_handler h(get_app());
//
//            h.EnablePool();
//
//            ::sockets::listen_socket<socket> ll(h);
//
//            setup_socket(ll);
//
//            ll.m_bDetach = true;
//
//            while (get_run_thread())
//            {
//
//               int iPort;
//
//               if (spa_get_admin())
//               {
//#if OSBIT == 32
//                  if (m_bSsl)
//                  {
//                     iPort = (port_t)57332;
//                  }
//                  else
//                  {
//                     iPort = (port_t)37332;
//                  }
//#else
//                  if (m_bSsl)
//                  {
//                     iPort = (port_t)57364;
//                  }
//                  else
//                  {
//                     iPort = (port_t)37364;
//                  }
//#endif
//               }
//               else
//               {
//#if OSBIT == 32
//                  if (m_bSsl)
//                  {
//                     iPort = (port_t)57232;
//                  }
//                  else
//                  {
//                     iPort = (port_t)37232;
//                  }
//#else
//                  if (m_bSsl)
//                  {
//                     iPort = (port_t)57264;
//                  }
//                  else
//                  {
//                     iPort = (port_t)37264;
//                  }
//#endif
//               }
//
//               if (ll.Bind("127.0.0.1", iPort))
//               {
//                  string strMessage;
//                  strMessage.Format("could not bind to address 127.0.0.1:" + ::str::from(iPort));
//                  TRACE(strMessage);
//                  Sleep(2000);
//                  break;
//               }
//
//               h.add(&ll);
//
//               while (get_run_thread() && h.get_count() > 0)
//               {
//
//                  h.select(8, 0);
//
//               }
//
//            }
//
//         }
//         catch (...)
//         {
//         }
//      }
//
//      return 0;
//
//   }


   //void socket_thread::setup_socket(::sockets::listen_socket<socket> & ll)
   //{

   //   if (m_bSsl)
   //   {

   //      ll.m_strCat = "cat://" + read_resource_as_string(::GetModuleHandleA("spalib.dll"), IDR_CA2SP1, "CA2SP");;

   //      ll.EnableSSL();

   //   }

   //}

   ::netserver::socket_handler * socket_thread::create_socket_handler()
   {

      return new ::a_spa::socket_handler(get_app());


   }


   ::sockets::listen_socket_base * socket_thread::create_listen_socket()
   {

      return new ::sockets::listen_socket < socket >(*m_psockethandler);

   }


} // namespace a_spa

