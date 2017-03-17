#pragma once



namespace a_spa
{


   class application;
   class service;


   class SPALIB_API socket :
      virtual public ::sockets::httpd_socket
   {
   public:


      bool                             m_bSetCookie;

      socket(::sockets::base_socket_handler & h);
      ~socket();

      virtual void OnExecute();
      virtual void OnResponseComplete();

      virtual void OnSSLAccept();

      void send_response();



      virtual void simple_file_server(const char * pszPath);
      //virtual void simple_image_server(const char * pszPath, int iMaxWidth = -1, int iMaxHeight = -1);

      virtual bool http_filter_response_header(id key, stringa & straValue);

      //virtual void on_compress();


   };


} // namespace a_spa

