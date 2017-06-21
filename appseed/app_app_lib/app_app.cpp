#include "framework.h"

#include "axis/app_install_plugin_library_list.cpp"


int run_file(const char * pszFile, int nCmdShow);

HANDLE g_hmutexInstall = NULL;

bool g_bStarterStart = false;
MSG g_msg;
bool g_bAdmin = false;

SPALIB_API bool spa_get_admin()
{
   return g_bAdmin;
}

SPALIB_API void spa_set_admin(bool bSet)
{
   g_bAdmin = bSet;
}


//SPALIB_API string spa_get_platform()
//{
//}



bool is_user_service_running()
{

   mutex mutex(get_thread_app(), "Local\\ca2_application_local_mutex:app-core/user_service");

   return mutex.already_exists();

}









string spa_version(string strVersion)
{
   static string  s_strVersion;

   if (strVersion.has_char())
   {
      s_strVersion = strVersion;
   }

   if (s_strVersion.is_empty())
   {
      if (_ca_is_basis())
      {
         s_strVersion = "basis";
      }
      else
      {
         s_strVersion = "stage";
      }
   }

   return s_strVersion;
}


string spa_title(string strTitle)
{
   static string  s_strTitle;

   if (strTitle.has_char())
   {
      s_strTitle = strTitle;
   }

   return s_strTitle;
}











#include "framework.h"

//#include <gdiplus.h>

#include <math.h>

///using namespace Gdiplus;

#define ID_CONTROL_BOX_CLOSE 31000
#define ID_CONTROL_BOX_MINIMIZE 31001


bool g_bForceUpdatedBuild = false;


int run_file(const char * pszFile, int nCmdShow);

bool g_bInstalling = false;
int g_iHealingSurface = 0;
//string g_strHost;
DWORD g_dwInstallStartTime;
//string g_strBuild;
//string g_strBuildResource;
HINSTANCE g_hinstance;
bool g_bShowPercentage;
extern MSG g_msg;
#ifdef WIN32



void DragMainWindow();



#endif








//machine_event g_machineevent;

bool machine_unsignalize_close_application();
bool machine_check_close_application(bool bDefault);
bool g_NeedRestartBecauseOfReservedFile;
bool g_NeedRestartFatalError;
bool g_bLoginStartup;
bool g_bMsDownload = false;

//DWORD g_dwCurFileLen;
bool m_reboot();
bool ignit_phase2();
int run_uninstall(const char * lpCmdLine, int nCmdShow);
int run_uninstall_run(const char * lpCmdLine, int nCmdShow);
int run_install(const char * lpCmdLine, int nCmdShow);
int run_file(const char * lpFile, int nCmdShow);
bool spa_exec(const char * psz);
string load_string(const char * pszId, const char * pszDefault);
string str_replace(const char * psz, const char * pszFind, const char * pszReplace);
bool is_there_application_opened();
bool is_application_opened(const char * psz);
void machine_signalize_close_application();
string read_resource_as_string(
   HINSTANCE hinst,
   UINT nID,
   LPCTSTR lpcszType);




#define MAX_LOADSTRING 100

stringa g_strSpa;
stringa g_straHost;



#define ID_START 100
#define ID_FINISH 101
#define ID_STEP_BACKWARD 102

// Global Variables:
HINSTANCE hInst;								// current instance

                                       // Forward declarations of functions included in this code module:
                                       //ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void CommandRegister();
void CommandFinish();
double g_dAnime = 0.0;
double g_dPi = asin(1.0) * 4;

bool g_bShow = true;

bool g_bDrag = false;
POINT g_ptDragStart;
RECT g_rectWindowDragStart;
int g_iScreen;
int g_iProgressMode;
int g_iTotalGzLen;
int g_iGzLen;
int g_iStyle;




double g_dProgress = -1.0;
double g_dProgress1 = -1.0;
double g_dProgress2 = -1.0;
int g_iStart;

bool g_bOfflineInstall = false;
bool g_bInternetInstall = true;

bool g_bInstallSet = false;






void OnTimer(HWND hwnd, UINT nIDEvent)
{
   if (nIDEvent == 1984)
   {
      //HDC hdc = ::GetWindowDC(hwnd);
      //OnPaint(hwnd, hdc, NULL);
      //::ReleaseDC(hwnd, hdc);



   }
}



//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

   return ::app_app::app::get()->window_proc(hWnd, message, wParam, lParam);

}






#undef new
#define min MIN
#define max MAX
#include <gdiplus.h>
#undef min
#undef max

#include <math.h>

using namespace Gdiplus;

SPALIB_API Font * CreatePointFont(double dPointSize, const wchar_t * lpszFaceName, bool bUnderline, bool bBold);
Rect make_rect(LPCRECT lpcrect, bool bMinusOne = false);


namespace app_app
{






   //void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
   //{
   //
   //   ::SetBkColor(hdc, clr);
   //
   //   ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
   //
   //}
   //

   spa_canvas::spa_canvas(app * pspa, int cx, int cy)
   {

      m_bDraw = true;

      m_cx = cx;

      m_cy = cy;

      m_pspa = pspa;

      m_pcolorref = NULL;

      init_toolbox();

   }


   spa_canvas::~spa_canvas()
   {

      ::aura::del(m_ptoolbox);

   }




   void spa_canvas::init_toolbox()
   {

      m_ptoolbox = new spa_canvas_toolbox(this);

   }


   bool spa_canvas::update_layered_window()
   {

      RECT rect;

      rect.left = 0;

      rect.top = 0;

      rect.right = m_cx;

      rect.bottom = m_cy;

      if (m_ptoolbox == NULL)
      {

         return false;

      }

      m_ptoolbox->m_pgraphics->SetCompositingMode(CompositingModeSourceCopy);

      {

         SolidBrush sb(Color(184 + 23, 23, 23, 23));

         m_ptoolbox->m_pgraphics->FillRectangle(&sb, make_rect(&rect));

      }

      m_ptoolbox->paint_install(&rect, 1);

      RECT rectWindow;

      ::GetWindowRect(m_pspa->m_hwnd, &rectWindow);

      HDC hdcWindow = ::GetWindowDC(m_pspa->m_hwnd);

      POINT pt;

      pt.x = rectWindow.left;

      pt.y = rectWindow.top;

      SIZE sz;

      sz.cx = width(&rectWindow);

      sz.cy = height(&rectWindow);

      POINT ptSrc = { 0 };

      BLENDFUNCTION blendPixelFunction = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };

      UpdateLayeredWindow(m_pspa->m_hwnd, hdcWindow, &pt, &sz, m_hdc, &ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

      ::ReleaseDC(m_pspa->m_hwnd, hdcWindow);

      return true;

   }


} // namespace app_app





Rect make_rect(LPCRECT lpcrect, bool bMinusOne)
{

   return Rect(lpcrect->left, lpcrect->top, width(lpcrect) - (bMinusOne ? 1 : 0), height(lpcrect) - (bMinusOne ? 1 : 0));

}



Font * CreatePointFont(double dPointSize, const wchar_t * lpszFaceName, bool bUnderline, bool bBold)
{

   FontFamily fontFamily(lpszFaceName);

   return new Font(&fontFamily, dPointSize, (bBold ? FontStyleBold : 0) | (bUnderline ? FontStyleUnderline : 0), UnitPoint);

}


