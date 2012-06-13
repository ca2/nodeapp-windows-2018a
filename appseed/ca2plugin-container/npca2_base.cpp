#include "framework.h"



HANDLE g_hinstancePluginbase = NULL; 
void * g_pvoidPluginSystem = NULL;


int __stdcall DllMain(HINSTANCE hinstance, DWORD dwReason, LPVOID lpReserved)
{
	
   UNREFERENCED_PARAMETER(lpReserved);
	
   if(dwReason == DLL_PROCESS_ATTACH)
	{

      initialize_primitive_heap();

      if(!os_initialize())
         return FALSE;


      //MessageBox(NULL, "boxmain", "box", MB_OK);
      
      //Sleep(15 * 1000);

      g_hinstancePluginbase = hinstance;

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{

      os_finalize();

      finalize_primitive_heap();

	}

	return 1;   // ok

}


