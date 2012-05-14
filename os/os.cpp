#include "framework.h"



extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   UNREFERENCED_PARAMETER(lpReserved);

   if (dwReason == DLL_PROCESS_ATTACH)
   {
      ::OutputDebugString("::ca2:: win.dll :: initializing!\n");
      

      ::ca::g_pfn_get_thread = &::win::get_thread;
      ::ca::g_pfn_get_thread_state = &::win::get_thread_state;

   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {
      ::OutputDebugString("::ca2:: win.dll :: terminating!\n");
   }
   return 1;   // ok
}
