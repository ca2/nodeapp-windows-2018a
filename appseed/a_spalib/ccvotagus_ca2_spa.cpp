#include "a_spalib.h"

//#include <gdiplus.h>

#include <math.h>

///using namespace Gdiplus;

#define ID_CONTROL_BOX_CLOSE 31000
#define ID_CONTROL_BOX_MINIMIZE 31001


bool g_bForceUpdatedBuild = false;

int g_cx;
int g_cy;

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

void ca2_install_canvas_init_draw();
void update_layered_window();

COLORREF * g_pcolorref = NULL;
HBITMAP g_hbitmap = NULL;
HDC g_hdc = NULL;



HBRUSH g_hbrushBk = NULL;
HBITMAP g_hbmAlpha;
HDC g_hdcAlpha;

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


HWND g_hwnd = NULL;


double g_dProgress = -1.0;
double g_dProgress1 = -1.0;
double g_dProgress2 = -1.0;
int g_iStart;

bool g_bOfflineInstall = false;
bool g_bInternetInstall = true;

bool g_bInstallSet = false;





void OnTimer(HWND hwnd, UINT nIDEvent)
{
   if(nIDEvent == 1984)
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
	switch (message)
	{
   case WM_CREATE:
      {
         
         BITMAPINFO info={};

         info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
         info.bmiHeader.biWidth=g_cx;
         info.bmiHeader.biHeight=-g_cy;
         info.bmiHeader.biPlanes=1;
         info.bmiHeader.biBitCount=32;
         info.bmiHeader.biCompression=BI_RGB;
         //info.bmiHeader.biSizeImage=g_cx*g_cy * 4;

         LPDWORD lpdata;

         //g_pcolorref = new COLORREF[g_cx * g_cy * sizeof(COLORREF)];

         int iScan;

         g_hbitmap = CreateDIBSection(NULL,&info,DIB_RGB_COLORS,(void **)&g_pcolorref, NULL,NULL);

         g_hdc = ::CreateCompatibleDC(NULL);

         ::SelectObject(g_hdc,g_hbitmap);

         

         memset(g_pcolorref,0,g_cx*g_cy*sizeof(COLORREF));

         ca2_install_canvas_init_draw();

         ::SetTimer(hWnd, TIMER_CARET, 100, NULL);

         ::SetTimer(hWnd, TIMER_ANIMATION, 33, NULL);

      }
      break;
	case WM_PAINT:
//      OnPaint(hWnd);
		break;
	case WM_ERASEBKGND:
      return TRUE;
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
      g_iHealingSurface++;
      return TRUE;
   case WM_LBUTTONDOWN:
      {
         g_iHealingSurface++;
         g_bDrag = true;
         ::GetCursorPos(&g_ptDragStart);
         ::GetWindowRect(g_hwnd, &g_rectWindowDragStart);
      };
      break;
   case WM_MOUSEMOVE:
      {
         if(g_bDrag)
         {
            DragMainWindow();
         }
      };
      break;
   case WM_LBUTTONUP:
      {
         if(g_bDrag)
         {
            DragMainWindow();
            g_bDrag = false;
         }
      };
      break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
   case WM_TIMER:
      OnTimer(hWnd, wParam);
      break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



DWORD WINAPI thread_proc_draw(LPVOID lpParam)
{
   while(g_hwnd != NULL)
   {
      update_layered_window();
      Sleep(5);
   }
   return 0;
}






string str_replace(const char * psz,const char * pszFind,const char * pszReplace)
{
   return ::str::replace(pszFind,pszReplace,psz);
}




void DragMainWindow()
{
   POINT ptCursor;
   ::GetCursorPos(&ptCursor);
   ::SetWindowPos(g_hwnd, NULL, 
      ptCursor.x - g_ptDragStart.x + g_rectWindowDragStart.left,
      ptCursor.y - g_ptDragStart.y + g_rectWindowDragStart.top,
      0, 
      0,
      SWP_NOSIZE | SWP_SHOWWINDOW);
/*   if(g_iStyle == 0)
   {
   }
   else
   {
      ::SetLayeredWindowAttributes(g_hwnd, 0, (255 * 100) / 100, LWA_ALPHA);
   }
   ::RedrawWindow(g_hwnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);*/
}






int bzuncompress(LPCTSTR lpcszUncompressed,LPCTSTR lpcszGzFileCompressed)
{
   const int iGzUncompressLen = 1024 * 1024;
   char * pchGzUncompressBuffer = NULL;
   if(pchGzUncompressBuffer == NULL)
   {
      pchGzUncompressBuffer = new char[iGzUncompressLen];
   }
   BZFILE * file = BZ2_bzopen(lpcszGzFileCompressed,"rb");
   if(file == NULL)
   {
      fprintf(stderr,"bzopen error\n");
      return -2;
   }
   string strUn(lpcszUncompressed);
   //   strUn += ".tmp";
   FILE * fileUn = fopen(strUn.c_str(),"wb+");
   if(fileUn == NULL)
   {
      BZ2_bzclose(file);
      int err;
      _get_errno(&err);
      fprintf(stderr,"fopen error\n %d",err);

      return -1;
   }
   int uncomprLen;
   while((uncomprLen = BZ2_bzread(file,pchGzUncompressBuffer,iGzUncompressLen)) > 0)
   {
      fwrite(pchGzUncompressBuffer,1,uncomprLen,fileUn);
   }
   fclose(fileUn);
   BZ2_bzclose(file);
   //   ::CopyFile(strUn.c_str(), lpcszUncompressed, FALSE);
   //   ::DeleteFile(strUn.c_str());
   return 0;
}
