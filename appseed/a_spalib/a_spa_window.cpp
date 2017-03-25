#include "framework.h"


TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

int create_spa_window();

ATOM spa_register_class();

UINT c_cdecl thread_proc_draw(LPVOID lpParam);

namespace a_spa
{


   int simple_app::show_spa_window(bool bShow)
   {

      if (m_hwnd == NULL)
      {

         if (!create_spa_window())
         {

            return 0;

         }

      }

      if (bShow)
      {

         ShowWindow(m_hwnd, SW_SHOW);

         start_a_spa_web_server();

      }
      else
      {

         ShowWindow(m_hwnd, SW_HIDE);

      }

      return TRUE;

   }


   int simple_app::create_spa_window()
   {

      // Initialize global strings

      strcpy(szTitle, "ca2 spa");

      strcpy(szWindowClass, "ca2_spa");

      spa_register_class();

      int Width = 800;

      int Height = 584;

      BITMAPINFO m_Info;

      ZeroMemory(&m_Info, sizeof(BITMAPINFO));

      m_Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      m_Info.bmiHeader.biWidth = Width;
      m_Info.bmiHeader.biHeight = -Height;
      m_Info.bmiHeader.biPlanes = 1;
      m_Info.bmiHeader.biBitCount = 32;
      m_Info.bmiHeader.biCompression = BI_RGB;
      m_Info.bmiHeader.biSizeImage = Width*Height * 4;

      LPDWORD lpdata;

      //g_hbmAlpha=CreateDIBSection(NULL,&m_Info,DIB_RGB_COLORS,(void **)&lpdata,NULL,NULL);
      //for(int y = 0; y < 584; y++)
      //{
      //   for(int x = 0; x < 800; x++)
      //   {
      //      lpdata++;
      //      /*         if(x < 23 || x > 777 || y < 23 || y > 561)
      //      {
      //      //*lpdata = (*lpdata & 0xffffff) | 0x7f888888;
      //      *lpdata = (*lpdata & 0xffffff) | 0x1fffffff;
      //      *lpdata = 0xCA888888;
      //      }
      //      else*/
      //      {
      //         *lpdata = 0x88888888;
      //      }
      //   }
      //}

   /*
      g_hdcAlpha = ::CreateCompatibleDC(NULL);
      ::SelectObject(g_hdcAlpha,g_hbmAlpha);
   */

      HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, g_hinstance, NULL);

      g_iStyle = 0;

      //g_hbrushBk = ::CreateSolidBrush(RGB(255,255,255));

      if (!hWnd)
      {
         return 0;
      }

      //if(g_iStyle == 0)
      //{
      //}
      //else
      {

         ::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

      }

      ::SetWindowLong(hWnd, GWL_STYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) & ~(WS_BORDER | WS_CAPTION));

      //g_hwnd = hWnd;

      ::__begin_thread(this, &thread_proc_draw, NULL, 50, 0, 0, NULL, NULL);



      int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);

      int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);


      int x = (cxScreen - m_pcanvas->m_cx) / 2;

      int y = (cyScreen - m_pcanvas->m_cy) / 2;

      SetWindowPos(hWnd, NULL, x, y, m_pcanvas->m_cx, m_pcanvas->m_cy, SWP_NOCOPYBITS);

      UpdateWindow(hWnd);

      SetTimer(hWnd, 1984, 5, NULL);





      return 1;

   }




   //
   //  FUNCTION: MyRegisterClass()
   //
   //  PURPOSE: Registers the window class.
   //
   //  COMMENTS:
   //
   //    This function and its usage are only necessary if you want this code
   //    to be compatible with Win32 systems prior to the 'RegisterClassEx'
   //    function that was added to Windows 95. It is important to call this function
   //    so that the application will get 'well formed' small icons associated
   //    with it.
   //


} // namespace a_spa



ATOM spa_register_class()
{

   HINSTANCE hInstance = g_hinstance;

   WNDCLASSEX wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style = 0;
   wcex.lpfnWndProc = WndProc;
   wcex.cbClsExtra = 0;
   wcex.cbWndExtra = 0;
   wcex.hInstance = hInstance;
   wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CCVOTAGUS_CA2_SPA));
   wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   //wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CCVOTAGUS_CA2_SPA);
   wcex.lpszMenuName = NULL;
   wcex.lpszClassName = szWindowClass;
   wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

   return RegisterClassEx(&wcex);

}
