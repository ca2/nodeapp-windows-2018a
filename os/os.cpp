#include "framework.h"



BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   UNREFERENCED_PARAMETER(lpReserved);

   if (dwReason == DLL_PROCESS_ATTACH)
   {
      ::OutputDebugString("::core:: win.dll :: initializing!\n");
      

      ::g_pfn_get_thread = &::win::get_thread;
      ::g_pfn_get_thread_state = &::win::get_thread_state;

   }
   else if (dwReason == DLL_PROCESS_DETACH)
   {

      HMODULE hmodule;
   
      try
      {
      
         if(::GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, "ca.dll", &hmodule) != FALSE)
         {

            try
            {
                  
               ::FreeLibrary(hmodule);

            }
            catch(...)
            {

            }

         }

      }
      catch(...)
      {
      }

      ::OutputDebugString("::core:: win.dll :: terminating!\n");

   }

   return 1;   // ok

}
