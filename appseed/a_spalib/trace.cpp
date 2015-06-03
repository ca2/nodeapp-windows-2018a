#include "a_spalib.h"

void on_trace(string & str, string & str2);

mutex g_mutexTrace;
stringa g_straTrace;
int g_iTrace = -1;
FILE * g_ftrace = NULL;
string g_strLastStatus;
int g_iLastStatus = 0;


class trace_file
{
public:

   HANDLE m_hfile;

   trace_file()
   {

      g_mutexTrace.lock();

      dir::mk(dir::element().c_str());

      m_hfile = ::CreateFileW(u16(dir::element() / ("install-" +process_platform_dir_name()+ ".log")),GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

      ::SetFilePointer(m_hfile,0,NULL,FILE_END);

   }
   ~trace_file()
   {
      g_mutexTrace.unlock();
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



void trace(const string & str)
{

   trace_file().print(str);

}


void trace(double dRate)
{

   trace_file().print(dRate);

}
