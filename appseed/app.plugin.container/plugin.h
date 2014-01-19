#pragma once


int spaboot_start(const char * pszVersion, const char * pszId);


namespace npca2
{

   class host;

   enum e_check
   {
      check_before_auth,
      check_before_start,
   };


   class plugin :
      virtual public ::hotplugin::plugin
   {
   public:



      plugin();
      virtual ~plugin();



      virtual bool hist(const char * pszUrl);
      virtual void run_start_install(const char * pszRun);


      virtual void start_ca2();


      //int starter_start(const char * pszId);

      virtual bool initialize();

      //virtual void on_bare_paint(HDC hdc, LPCRECT lprect);
      virtual void on_paint(HDC hdcWindow, LPCRECT lprect);

      virtual void on_prepare_memory();

      virtual UINT_PTR message_handler(UINT_PTR uiMessage, WPARAM wparam, LPARAM lparam);


   };

   ATOM MyRegisterClass(HINSTANCE hInstance);

} // namespace npca2