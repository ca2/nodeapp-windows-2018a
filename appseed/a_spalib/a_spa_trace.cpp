#include "framework.h"

void on_trace(string & str, string & str2);

stringa g_straTrace;
int g_iTrace = -1;
FILE * g_ftrace = NULL;
//string g_strLastStatus;
int g_iLastStatus = 0;

namespace a_spa
{


   trace_file::trace_file(simple_app * papp) :
      m_papp(papp),
      m_mutex(papp, false, "Global\\ca2-spa-install-" + papp->m_strPlatform),
      m_sl(&m_mutex)
   {

      dir::mk(dir::element().c_str());

      m_hfile = ::CreateFileW(u16(dir::element() / ("install-" + m_papp->m_strPlatform + ".log")), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

      ::SetFilePointer(m_hfile, 0, NULL, FILE_END);

   }

   trace_file::~trace_file()
   {

      CloseHandle(m_hfile);

   }


   void trace_file::print(const string & str)
   {

      if (m_hfile != NULL && str.length() > 0)
      {

         DWORD dwWritten;
         ::SetFilePointer(m_hfile, 0, NULL, SEEK_END);
         WriteFile(m_hfile, str.c_str(), str.length(), &dwWritten, NULL);
         ::FlushFileBuffers(m_hfile);

      }

   }


   void trace_file::print(double dRate)
   {
      if (dRate < 0.0)
         dRate = 0.0;
      if (dRate > 1.0)
         dRate = 1.0;
      dRate = dRate * 1000.0 * 1000.0 * 1000.0;
      //int32_t i = ftol(dRate);
      int i = (int)dRate;
      string str;
      str = "|||";
      char sz[20];
      str += ::_itoa(i, sz, 10);
      str += "\r\n";
      print(str);
   }


} // namespace a_spa

