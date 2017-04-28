#pragma once

#include "axis/axis/axis.h"
#include "nodeapp/appseed/app_core/app_core.h"





BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   DWORD dwStartTime = ::get_tick_count();

   if(!defer_axis_init())
      return -1;

   if(file_exists_dup("C:\\ca2\\config\\system\\wait_on_beg.txt"))
   {
         
      Sleep(10000);

   }

   if(file_exists_dup("C:\\ca2\\config\\system\\beg_debug_box.txt"))
   {
      debug_box("zzzAPPzzz app","zzzAPPzzz app",MB_ICONINFORMATION);
   }

   int iRet = app_core_main(hinstance, hPrevInstance, (char *) (const char *) ::str::international::unicode_to_utf8(::GetCommandLineW()), nCmdShow);

   defer_axis_term();

   char szTimeMessage[2048];

   sprintf(szTimeMessage,"\n\n\n\n\n\n-------------------------------\n|\n|\n|  Total Elapsed Time %d\n|\n|\n-------------------------------\n\n\n",(uint32_t) ::get_tick_count() - dwStartTime);

   ::OutputDebugStringA(szTimeMessage);

   printf(szTimeMessage);

   return iRet;
   
}

END_EXTERN_C