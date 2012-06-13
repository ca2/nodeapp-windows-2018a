#include "framework.h"

NPIdentifier sFoo_id;
NPIdentifier sBar_id;
NPIdentifier sDocument_id;
NPIdentifier sLocation_id;
NPIdentifier sBody_id;
NPIdentifier sCreateElement_id;
NPIdentifier sCreateTextNode_id;
NPIdentifier sAppendChild_id;
NPIdentifier sPluginType_id;
NPObject *sWindowObj;



enum e_message
{
   message_none,
   message_ok,
   message_failed,
   message_unknown,
};

HANDLE g_hinstanceNpca2Plugin = NULL; 
extern HANDLE g_hinstancePluginbase; 

int hotnpca2plugin_start();
int npca2plugin_start(const char * path);

HWND g_hwndMessage = NULL;
e_message g_emessage = message_none;

ATOM MyRegisterClass(HINSTANCE hInstance);


bool defer_check(::npca2::host * phost)
{

   MyRegisterClass((HINSTANCE) g_hinstancePluginbase);

   g_hwndMessage = ::CreateWindowExA(0, "spaboot_message_window_class", "ca2::fontopus::ccvotagus::install:callback_window", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);


   phost->start_plugin();
   phost->initialize();

   return true;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
   case WM_COPYDATA:
      {
         COPYDATASTRUCT * pcds = (COPYDATASTRUCT *) lParam;
         if(pcds->dwData == 15111984)
         {
            const char * pszMessage = strndup_dup((const char *)pcds->lpData, pcds->cbData);
            if(stricmp_dup(pszMessage, "ok"))
            {
               g_emessage = message_ok;
            }
            else if(stricmp_dup(pszMessage, "failed"))
            {
               g_emessage = message_failed;
            }
            else if(stricmp_dup(pszMessage, "unknown"))
            {
               g_emessage = message_unknown;
            }

         }
      }
      break;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
	return 0;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			   = 0;
	wcex.lpfnWndProc	   = WndProc;
	wcex.cbClsExtra	   = 0;
	wcex.cbWndExtra	   = 0;
	wcex.hInstance		   = hInstance;
	wcex.hIcon			   = NULL;
	wcex.hCursor		   = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CCVOTAGUS_CA2_SPA);
   wcex.lpszMenuName	   = NULL;
	wcex.lpszClassName	= "spaboot_message_window_class";
	wcex.hIconSm		   = NULL;

	return RegisterClassEx(&wcex);
}


DWORD CALLBACK ThreadProcDeferCheck(LPVOID param)
{
   ::npca2::host * phost = (::npca2::host *) param;
   defer_check(phost);
   return 0;
}


void start_defer_check_thread(::npca2::host * phost)
{
   //Sleep(15 * 1000);
   ::CreateThread(NULL, 0, &ThreadProcDeferCheck, (LPVOID) phost, 0, 0);
}




void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
{
   ::SetBkColor(hdc, clr);
   ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
}


bool TextOutU(HDC hdc, int x, int y, const char * pszUtf8, int iSize)
{
   SIZE size;
   size.cx = 0;
   size.cy = 0;
   bool b = TextOut(hdc, size.cx + x, y, pszUtf8, (int) strlen_dup(pszUtf8)) != FALSE;
   return b;
}





extern "C" NPError _NS_PluginInitialize()
{
   return NPERR_NO_ERROR;
}

extern "C" void _NS_PluginShutdown()
{
}

#undef new

/*extern "C" int __cdecl _purecall(void)
{
   return 0;
}*/

nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{

   if(!aCreateDataStruct)
    return NULL;

   return new ::npca2::host(aCreateDataStruct->instance);

}

void NS_DestroyPluginInstance(nsPluginInstanceBase * aPlugin)
{
   if(aPlugin)
      ca2_free(aPlugin);
}

