#include "a_spalib.h"

#include "ccvotagus_ca2_spa.cpp"
#include "command_start.cpp"
#include "http1.cpp"
#include "http2.cpp"
#include "main.cpp"
#include "trace.cpp"
#include "window.cpp"


#include "axis/app_install_plugin_library_list.cpp"








string spa_version(string strVersion)
{
   static string  s_strVersion;

   if(strVersion.has_char())
   {
      s_strVersion = strVersion;
   }

   if(s_strVersion.is_empty())
   {
      if(_ca_is_basis())
      {
         s_strVersion = "basis";
      }
      else
      {
         s_strVersion = "stage";
      }
   }

   return s_strVersion;
}


string spa_title(string strTitle)
{
   static string  s_strTitle;

   if(strTitle.has_char())
   {
      s_strTitle = strTitle;
   }

   return s_strTitle;
}


a_spa * a_spa::s_pspa = NULL;

a_spa::a_spa() :
   ::aura::system(NULL, this)
{

   m_hwnd = NULL;
   m_pcanvas = NULL;
   s_pspa = this;

   m_hinstance = ::GetModuleHandleA(NULL);

}


a_spa::~a_spa()
{


}
