#include "framework.h"

Gdiplus::GdiplusStartupInput *   g_pgdiplusStartupInput     = NULL;
Gdiplus::GdiplusStartupOutput *  g_pgdiplusStartupOutput    = NULL;
DWORD_PTR                        g_gdiplusToken             = NULL;
DWORD_PTR                        g_gdiplusHookToken         = NULL;

BOOL WINAPI DllMain(HINSTANCE hInstance,DWORD dwReason,LPVOID lpReserved)
{

   //Sleep(30000);

   //   ASSERT(FALSE);

   xxdebug_box("draw2d_gdiplus.dll DllMain (0)","box",MB_OK);

   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(lpReserved);


   //openssl_DllMain(hInstance, dwReason, lpReserved);


   if(dwReason == DLL_PROCESS_ATTACH)
   {

      OutputDebugStringW(L"draw2d_gdiplus.dll initializing!\n");

      xxdebug_box("draw2d_gdiplus.dll DllMain","box",MB_OK);
      g_pgdiplusStartupInput     = new Gdiplus::GdiplusStartupInput();
      g_pgdiplusStartupOutput    = new Gdiplus::GdiplusStartupOutput();
      g_gdiplusToken             = NULL;
      g_gdiplusHookToken         = NULL;

      g_pgdiplusStartupInput->SuppressBackgroundThread = TRUE;

      Gdiplus::Status statusStartup = GdiplusStartup(&g_gdiplusToken,g_pgdiplusStartupInput,g_pgdiplusStartupOutput);

      if(statusStartup != Gdiplus::Ok)
      {

         simple_message_box(NULL,"Gdiplus Failed to Startup. ca cannot continue.","Gdiplus Failure",MB_ICONERROR);

         return 0;

      }




      statusStartup = g_pgdiplusStartupOutput->NotificationHook(&g_gdiplusHookToken);


      if(statusStartup != Gdiplus::Ok)
      {

      simple_message_box(NULL,"Gdiplus Failed to Hook. ca cannot continue.","Gdiplus Failure",MB_ICONERROR);

      return 0;

      }






































   }
   else if(dwReason == DLL_PROCESS_DETACH)
   {

      g_pgdiplusStartupOutput->NotificationUnhook(g_gdiplusHookToken);


       ::Gdiplus::GdiplusShutdown(g_gdiplusToken);


      OutputDebugStringW(L"draw2d_gdiplus.dll terminating!\n");


   }

   return 1;   // ok

}









int GetEncoderClsid(const WCHAR* format,CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

   Gdiplus::GetImageEncodersSize(&num,&size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num,size,pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if(wcscmp(pImageCodecInfo[j].MimeType,format) == 0)
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

