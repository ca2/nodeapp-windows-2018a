#include "framework.h"


extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, uint32_t dwReason, LPVOID lpReserved)
{

   UNREFERENCED_PARAMETER(lpReserved);

   if (dwReason == DLL_PROCESS_ATTACH)
   {

      ::OutputDebugStringW(L"::core:: draw2d_gdi.dll :: initializing!\n");

   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {

      ::OutputDebugStringW(L"::core:: draw2d_gdi.dll :: terminating!\n");

   }

   return 1;   // ok

}






