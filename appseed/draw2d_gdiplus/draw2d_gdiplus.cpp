#include "framework.h"


#ifndef CUBE
BOOL WINAPI DllMain(HINSTANCE hInstance,DWORD dwReason,LPVOID lpReserved)
{

   xxdebug_box("draw2d_gdiplus.dll DllMain (0)","box",MB_OK);

   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);

   if(dwReason == DLL_PROCESS_ATTACH)
   {

      output_debug_string("draw2d_gdiplus.dll initializing.\n");

   }
   else if(dwReason == DLL_PROCESS_DETACH)
   {

      output_debug_string("draw2d_gdiplus.dll terminating.\n");

   }

   return 1;

}

#endif



