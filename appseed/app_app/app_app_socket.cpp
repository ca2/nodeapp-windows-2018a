#include "framework.h"


namespace user_service
{


   static ::count s_countNetnodeSocket = 0;


   socket::socket(::sockets::base_socket_handler & h) :
      ::object(h.get_app()),
      base_socket(h),
      ::sockets::socket(h),
      stream_socket(h),
      tcp_socket(h),
      http_base_socket(h),
      http_socket(h),
      httpd_socket(h)
   {
      m_bSetCookie = true;
      s_countNetnodeSocket++;
   }



   socket::~socket()
   {
      s_countNetnodeSocket--;
   }

   void socket::send_response()
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
         else if (::str::begins_get_prefix(strStart, m_request.m_strRequestUri, "/start/")
            || ::str::begins_get_prefix(strStart, m_request.m_strRequestUri, "/start_x86/")
            || ::str::begins_get_prefix(strStart, m_request.m_strRequestUri, "/start_x64/"))
         {

            string strUserAgent = inheader("user-agent");

            string strRequest = m_request.m_strRequestUri;

            ::str::begins_eat_ci(strRequest, strStart);

            strsize iFind = strRequest.find_ci('?');

            string strApp = strRequest;

            if (iFind >= 0)
            {

               strApp = strApp.substr(0, iFind);

            }

            ::file::path path;

            if (strStart == "/start_x64/")
            {

               path = ::dir::program_files_x86() / "ca2/app_app/x64/app_app.exe";

            }
            else
            {

               path = ::dir::program_files_x86() / "ca2/app_app/x86/app_app.exe";

            }

            ::fork(get_app(), [=]
            {

               unsigned int uiPid;

               call_async(path, ": app=" + strApp, path.folder(), SW_SHOWNORMAL, false, &uiPid);

            });

            string str = "OK... soon patch applier (SPA) is working...";

            outheader(__id(content_type)) = "text/html; charset=UTF-8";

            response().file().write(str);

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


      int32_t iStatusCode;

      string strStatus;

      if (outattr(__id(http_status_code)).is_new() || outattr(__id(http_status)).is_new())
      {

         if (outheaders().has_property(__id(location)))
         {

            iStatusCode = 303; // 303 (See Other Location)

            strStatus = "See Other";

         }
         else
         {

            iStatusCode = 200;

            strStatus = "OK";

         }

         outattr(__id(http_status_code)) = iStatusCode;

         outattr(__id(http_status)) = strStatus;

      }

      outattr(__id(http_version)) = "HTTP/1.1";

      if (::str::begins(outheader(__id(content_type)), "image/"))
      {

         m_bSetCookie = false;

      }
      else if (outheader(__id(content_type)) == "application/javascript")
      {

         m_bSetCookie = false;

      }
      else if (outheader(__id(content_type)) == "text/css")
      {

         m_bSetCookie = false;

      }

      Respond();

   }




   void socket::OnExecute()
   {
      string strUrl = m_request.attr("http_protocol").get_string() + "://" + m_request.header("host").get_string() + m_request.attr("request_uri").get_string();
      TRACE("netnode::socket::OnExecute Url ----**START**====> %s", strUrl);
      DWORD dwExecuteBeg = ::get_tick_count();
      m_bEnd = false;
      send_response();
      DWORD dwExecuteEnd = ::get_tick_count();
      TRACE("netnode::socket::OnExecute Url ----**END****====> %s", strUrl);
      TRACE("netnode::socket::OnExecute Ms -----**END****====> %d", dwExecuteEnd - dwExecuteBeg);
   }

   void socket::OnResponseComplete()
   {
   }


   bool socket::http_filter_response_header(id key, stringa & straValue)
   {
      if (key == __id(location) && straValue.get_count() >= 1)
      {
         for (int i = 0; i < straValue.get_size(); i++)
         {
            url_domain domain;
            domain.create(System.url().get_server(straValue[i]));
            if (domain.m_strName == "account.ca2.cc")
            {
               //straValue[i] = "https://" + Application.m_strFontopusServer + System.url().get_object(straValue[i]);
            }
         }
      }
      else if (!m_bSetCookie && key == __id(set_cookie))
      {
         return false;
      }
      return true;
   }

   void socket::OnSSLAccept()
   {

      ::sockets::httpd_socket::OnSSLAccept();
   }

   void socket::simple_file_server(const char * pszPath)
   {

      smart_pointer_array < int_array > rangea;

      if (strlen(inheader("range")) > 0)
      {
         stringa straItem;
         straItem.explode("=", inheader("range"));
         if (straItem.get_count() == 2)
         {
            string strUnit = straItem[0];
            stringa stra;
            stra.explode(",", straItem[1]);
            for (int32_t i = 0; i < stra.get_count(); i++)
            {
               stringa straRange;
               straRange.explode("-", stra[i]);
               if (straRange.get_count() == 2)
               {
                  rangea.add(new int_array());
                  rangea.last_ptr()->add(atoi(straRange[0]));
                  straRange[1].trim();
                  if (strlen(straRange[1]) == 0)
                     rangea.last_ptr()->add(-1);
                  else
                     rangea.last_ptr()->add(atoi(straRange[1]));
               }
            }
         }
      }

      read_file(pszPath, &rangea);

   }

   void socket::simple_image_server(const char * pszPath, int iMaxWidth, int iMaxHeight)
   {

      if (iMaxWidth <= 0 && iMaxHeight <= 0)
      {
         simple_file_server(pszPath);
      }
      else
      {
         ::visual::dib_sp spdib(allocer());

         spdib.load_from_file(pszPath);

         double dRateW = 1.0;

         if (iMaxWidth > 0)
         {
            if (spdib->m_size.cx > iMaxWidth)
            {
               dRateW = (double)iMaxWidth / (double)spdib->m_size.cx;
            }
         }

         double dRateH = 1.0;

         if (iMaxHeight > 0)
         {
            if (spdib->m_size.cy > iMaxHeight)
            {
               dRateH = (double)iMaxHeight / (double)spdib->m_size.cx;
            }
         }

         double dRate = MIN(dRateW, dRateH);

         ::visual::dib_sp dib(allocer());

         dib->create((int32_t)(spdib->m_size.cx * dRate), (int32_t)(spdib->m_size.cy * dRate));

         dib->stretch_dib(spdib);

         ::visual::save_image saveimage;

         saveimage.m_eformat = ::visual::image::format_jpeg;
         saveimage.m_iQuality = 50;

         outheader(__id(content_type)) = "image/jpeg";


         dib.save_to_file(&response().file(), &saveimage);


      }

   }






} // namespace user_service

