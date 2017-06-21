#include "framework.h"


namespace app_app
{


   socket::socket(::sockets::base_socket_handler & h) :
      ::object(h.get_app()),
      base_socket(h),
      ::sockets::socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_base_socket(h),
      http_socket(h),
      httpd_socket(h),
      ::netserver::socket(h)
   {

   }



   socket::~socket()
   {

   }


   void socket::on_send_response()
   {

      string strReferer = inheader("referer");

      stringa stra;

      stra.add("http://store.ca2.cc/");
      stra.add("https://store.ca2.cc/");
      stra.add("http://desktop.ca2.cc/");
      stra.add("https://desktop.ca2.cc/");

      string strPrefix;

      for (auto & str : stra)
      {

         if (strReferer.begins_ci(str))
         {

            strPrefix = str;

            break;

         }

      }

      if (strPrefix.has_char())
      {

         ::str::ends_eat_ci(strPrefix, "/");

         outheader("Access-Control-Allow-Origin") = strPrefix;

         string strStart;

         if (::str::begins(m_request.m_strRequestUri, "/passthrough/"))
         {
         }
         //else if (::str::begins_get_prefix(strStart, m_request.m_strRequestUri, "/start/")
         //   || ::str::begins_get_prefix(strStart, m_request.m_strRequestUri, "/start_x86/")
         //   || ::str::begins_get_prefix(strStart, m_request.m_strRequestUri, "/start_x64/"))
         //{

         //   string strUserAgent = inheader("user-agent");

         //   string strRequest = m_request.m_strRequestUri;

         //   ::str::begins_eat_ci(strRequest, strStart);

         //   strsize iFind = strRequest.find_ci('?');

         //   string strApp = strRequest;

         //   if (iFind >= 0)
         //   {

         //      strApp = strApp.substr(0, iFind);

         //   }

         //   ::file::path path;

         //   if (strStart == "/start_x64/")
         //   {

         //      path = ::dir::program_files_x86() / "ca2/app_app/x64/app_app.exe";

         //   }
         //   else
         //   {

         //      path = ::dir::program_files_x86() / "ca2/app_app/x86/app_app.exe";

         //   }

         //   ::fork(get_app(), [=]
         //   {

         //      unsigned int uiPid;

         //      call_async(path, ": app=" + strApp, path.folder(), SW_SHOWNORMAL, false, &uiPid);

         //   });

         //   string str = "OK... soon patch applier (SPA) is working...";

         //   outheader(__id(content_type)) = "text/html; charset=UTF-8";

         //   response().file().write(str);

         //}
         else if (m_request.m_strRequestUri == "/status")
         {

            outheader(__id(content_type)) = "text/plain; charset=UTF-8";

            if (::app_app::app::s_papp->m_strSpaBootName.has_char())
            {

               response().file().write("Installing " + ::app_app::app::s_papp->m_strSpaBootName +"...");

            }
            else
            {

               response().file().write("Installing ...");

            }

         }
         else if (::str::begins(m_request.m_strRequestUri, "/matter/"))
         {
            //         outheader("Cache-control") = "public";
            //         outheader("Pragma") = "public";
            //         int32_t iPathCount;
            //         outheader("Expires") = System.http().gmdate(System.datetime().strtotime(NULL, "+1 day", 0, iPathCount));
            //#ifdef WINDOWS
            //         simple_file_server(::file::path("Z:\\") / m_request.m_strRequestUri);
            //#else
            //         simple_file_server(m_request.m_strRequestUri);
            //#endif
         }
         else if (::str::begins(m_request.m_strRequestUri, "/is_user_service_installed/"))
         {

            string strKey = inheader("sec-websocket-key");// 
                                                          //string strKey = "dGhlIHNhbXBsZSBub25jZQ==";
            strKey.trim();
            strKey += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
            memory mem;
            mem.assign(strKey);
            memory memSha1;
            System.crypto().sha1(memSha1, mem);
            strKey = System.base64().encode(memSha1);
            outheader("Sec-WebSocket-Accept") = strKey;
            outheader("Connection") = "Upgrade";
            outheader("Upgrade") = "websocket";
            outattr(__id(http_status_code)) = 101;
            outattr(__id(http_version)) = "HTTP/1.1";
            outattr(__id(http_status)) = "Switching Protocols";
            Respond();
            memory m;
            string strMessage = "yes_fontopus_com";
            m.allocate(strMessage.get_length() + 2);
            m.get_data()[0] = 0x81;
            m.get_data()[1] = strMessage.get_length();
            memcpy(&m.get_data()[2], strMessage.c_str(), strMessage.get_length());
            write(m.get_data(), m.get_size());
            return;
         }
         else
         {

         }

         if (!outheaders().has_property(__id(content_type)) && response().file().get_length() > 0)
         {

            outheader(__id(content_type)) = "text/html; charset=UTF-8";

         }
         if (IsSSLServer())
         {

            outheader("strict-transport-security") = "max-age=31536000; includeSubDomains";

         }

      }
      else
      {

         outheader(__id(content_type)) = "text/html; charset=UTF-8";

         string strH1;

         if (::app_app::app::s_papp->m_strSpaBootName.has_char())
         {

            strH1 = "Installing " + ::app_app::app::s_papp->m_strSpaBootName + " ...";

         }
         else
         {

            strH1 = "Installing ...";

         }
//         response().file().write("<html><head><meta http-equiv=\"refresh\" content=\"1\"><title>app_app.exe</title></head><body><h1>"+strH1+"</h1>app_app.exe is running.<br><br><h2>Current Install Activity:</h2>"+ ::app_app::app::s_papp->m_strHtmlLog+"</html>");

         //response().file().write("<html><head><meta http-equiv=\"refresh\" content=\"1\"><title>app_app.exe</title></head><body><h1>" + strH1 + "</h1>app_app.exe is running.</html>");

         response().file().write("<html><head><meta http-equiv=\"refresh\" content=\"1\"><title>app_app.exe</title></head><body><h1>" + strH1 + "</h1>app_app.exe is running. yOUR req:"+ m_request.m_strRequestUri +"</html>");

      }

   }


} // namespace app_app




