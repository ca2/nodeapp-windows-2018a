#include "a_spalib.h"

void on_trace(string & str, string & str2);

stringa g_straTrace;
int g_iTrace = -1;
FILE * g_ftrace = NULL;
//string g_strLastStatus;
int g_iLastStatus = 0;


class trace_file
{
public:

   a_spa * m_paspa;
   mutex m_mutex;
   synch_lock m_sl;

   HANDLE m_hfile;

   trace_file(a_spa * paspa):
      m_paspa(paspa),
      m_mutex(paspa, false, "Global\\ca2-spa-install-" + paspa->m_strPlatform),
      m_sl(&m_mutex)
   {

      dir::mk(dir::element().c_str());

      m_hfile = ::CreateFileW(u16(dir::element() / ("install-" + m_paspa->m_strPlatform+ ".log")),GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

      ::SetFilePointer(m_hfile,0,NULL,FILE_END);

   }

   ~trace_file()
   {

      CloseHandle(m_hfile);

   }


   void print(const string & str)
   {

      if(m_hfile != NULL && str.length() > 0)
      {

         DWORD dwWritten;
         ::SetFilePointer(m_hfile,0,NULL,SEEK_END);
         WriteFile(m_hfile,str.c_str(),str.length(),&dwWritten,NULL);
         ::FlushFileBuffers(m_hfile);

      }

   }




   void print(double dRate)
   {
      if(dRate < 0.0)
         dRate = 0.0;
      if(dRate > 1.0)
         dRate = 1.0;
      dRate = dRate * 1000.0 * 1000.0 * 1000.0;
      //int32_t i = ftol(dRate);
      int i = (int)dRate;
      string str;
      str = "|||";
      char sz[20];
      str += ::_itoa(i,sz,10);
      str += "\r\n";
      print(str);
   }





};



void a_spa::trace(const string & str)
{

   trace_file(this).print(str);

}


void a_spa::trace(double dRate)
{

   trace_file(this).print(dRate);

}
