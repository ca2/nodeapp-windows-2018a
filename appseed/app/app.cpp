#pragma once


#include "nodeapp/appseed/app_core/app_core.h"





BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{
   if(!defer_core_init())
      return -1;

   if(::str::find_ci("session=",lpCmdLine) >= 0)
   {
    
      if(file_exists_dup("C:\\ca2\\config\\system\\wait_on_session_arg.txt"))
      {
         
         Sleep(10000);

      }

   }

   if(file_exists_dup("C:\\ca2\\config\\system\\beg_debug_box.txt"))
   {
      debug_box("zzzAPPzzz app","zzzAPPzzz app",MB_ICONINFORMATION);
   }

   int iRet = app_core_main(hinstance, hPrevInstance, (char *) (const char *) ::str::international::unicode_to_utf8(::GetCommandLineW()), nCmdShow);

   defer_core_term();

   char szTimeMessage[2048];

   sprintf(szTimeMessage,"\n\n\n\n\n\n-------------------------------\n|\n|\n|  Total Elapsed Time %d\n|\n|\n-------------------------------\n\n\n",(uint32_t) ::get_tick_count() - g_dwStartTime);

   ::OutputDebugStringA(szTimeMessage);

   printf(szTimeMessage);

   return iRet;
   
}

END_EXTERN_C