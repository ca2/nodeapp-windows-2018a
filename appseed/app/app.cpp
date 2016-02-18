#pragma once

#include "core/core/core.h"
#include "nodeapp/appseed/app_core/app_core.h"

#include <sys/types.h>
#include <sys/stat.h>
#ifdef __MCRTDBG
#include <crtdbg.h>
#endif

int_bool file_exists_raw(const char * path1)
{

   uint32_t dwFileAttributes = GetFileAttributesA(path1);

   if(dwFileAttributes == INVALID_FILE_ATTRIBUTES || (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
      return FALSE;

   return TRUE;

}

void file_put_contents_raw(const char * path,const char * psz)
{

   FILE * f = fopen(path,"wb");

   if(f == NULL)
      return;

   ::count iSize = strlen(psz);

   ::count iRead = fwrite(psz,1,iSize,f);

   fclose(f);

}

void file_add_contents_raw(const char * path,const char * psz)
{

   FILE * f = fopen(path,"ab");

   if(f == NULL)
      return;

   ::count iSize = strlen(psz);

   ::count iRead = fwrite(psz,1,iSize,f);

   fclose(f);

}

void file_beg_contents_raw(const char * path,const char * psz)
{

   FILE * f = fopen(path,"rb+");

   ::count iLen = strlen(psz);

   fseek(f,iLen,SEEK_END);

   long  iEnd = ftell(f);

   int iSize = 1024 * 1024;
   char * buf = (char *)malloc(iSize);
   int iRemain = iEnd - iLen;
   while(iRemain > 0)
   {
      fseek(f,iEnd - iRemain - iLen,SEEK_SET);
      fread(buf,1,MIN(iRemain,iSize),f);
      fseek(f,iEnd - iRemain,SEEK_SET);
      fwrite(buf,1,MIN(iRemain,iSize),f);
      iRemain -= MIN(iRemain,iSize);
   }
   free(buf);
   fseek(f,0,SEEK_SET);

   ::count iRead = fwrite(psz,1,iLen,f);

   fclose(f);

}

uint64_t file_length_raw(const char * path)
{

   struct _stat st;
   ::_stat(path,&st);
   return st.st_size;

}

BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int32_t nCmdShow)
{

   app_core appcore;

   appcore.m_dwStartTime = ::get_tick_count();
   appcore.m_dwAfterApplicationFirstRequest = appcore.m_dwStartTime;

   if(!defer_core_init())
   {

      ::OutputDebugString("Failed to defer_core_init");

      return -4;

   }

   if(file_exists_dup("C:\\ca2\\config\\system\\wait_on_beg.txt"))
   {

      Sleep(10000);

   }

   if(file_exists_dup("C:\\ca2\\config\\system\\beg_debug_box.txt"))
   {
      debug_box("zzzAPPzzz app","zzzAPPzzz app",MB_ICONINFORMATION);
   }

   int iRet = app_core_main(hinstance,hPrevInstance,(char *)(const char *)  ::str::international::unicode_to_utf8(::GetCommandLineW()),nCmdShow,appcore);

   if(!defer_core_term())
   {

      ::OutputDebugString("Failed to defer_core_term");

      iRet -= 10000;

   }

   DWORD dwEnd = ::get_tick_count();

   char szTimeMessage[2048];

   ::time_t timet = ::time(NULL);

   tm t;

   errno_t err = _localtime64_s(&t,&timet);

   char szTime[2048];

   sprintf(szTime,"%04d-%02d-%02d %02d:%02d:%02d",t.tm_year + 1900,t.tm_mon + 1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);

   sprintf(szTimeMessage,"\n\n\n---------------------------------------------------------------------------------------------\n|\n|\n|  Just After First Application Request Completion %d",(uint32_t)appcore.m_dwAfterApplicationFirstRequest - appcore.m_dwStartTime);
   ::OutputDebugStringA(szTimeMessage);
   printf(szTimeMessage);

   sprintf(szTimeMessage,"\n|  Total Elapsed Time %d\n",(uint32_t)dwEnd - appcore.m_dwStartTime);
   ::OutputDebugStringA(szTimeMessage);
   printf(szTimeMessage);

   sprintf(szTimeMessage,"\n|  %s\n|\n|\n-------------------------------------------------------------------------------------------- - \n\n\n",szTime);
   ::OutputDebugStringA(szTimeMessage);
   printf(szTimeMessage);

   if(file_exists_raw("C:\\ca2\\config\\system\\show_elapsed.txt"))
   {
      char szUTCTime[2048];
//      char szLocalTime[2048];
      time_t rawtime;
      struct tm * l;
      struct tm * g;
      time(&rawtime);
      l = localtime(&rawtime);
      g = gmtime(&rawtime);
      sprintf(szUTCTime,"%04d-%02d-%02d %02d:%02d:%02d UTC",g->tm_year + 1900,g->tm_mon,g->tm_mday,g->tm_hour,g->tm_min,g->tm_sec);
      //   sprintf(szLocalTime,"%04d-%02d-%02d %02d:%02d:%02d local : ",l->tm_year + 1900,l->tm_mon,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
      char szTimeMessage1[2048];
      sprintf(szTimeMessage1," Just After First Application Request Completion %d",(uint32_t)appcore.m_dwAfterApplicationFirstRequest - appcore.m_dwStartTime);
      if(file_length_raw("C:\\ca2\\config\\system\\show_elapsed.txt") > 0)
      {
         file_add_contents_raw("C:\\ca2\\config\\system\\show_elapsed.txt","\n");
      }
      file_add_contents_raw("C:\\ca2\\config\\system\\show_elapsed.txt",szUTCTime);
      file_add_contents_raw("C:\\ca2\\config\\system\\show_elapsed.txt",szTimeMessage1);
      //file_add_contents_raw("C:\\ca2\\config\\system\\show_elapsed.txt",szLocalTime);
      file_add_contents_raw("C:\\ca2\\config\\system\\show_elapsed.txt","\n");
      char szTimeMessage2[2048];
      sprintf(szTimeMessage2," Total Elapsed Time %d",(uint32_t)dwEnd - appcore.m_dwStartTime);
      file_add_contents_raw("C:\\ca2\\config\\system\\show_elapsed.txt",szUTCTime);
      file_add_contents_raw("C:\\ca2\\config\\system\\show_elapsed.txt",szTimeMessage2);

   }
#ifdef __MCRTDBG
   _CrtDumpMemoryLeaks();
#endif
   return iRet;

}

END_EXTERN_C