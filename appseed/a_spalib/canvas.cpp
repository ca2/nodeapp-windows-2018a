#include "a_spalib.h"

#undef new
#define min MIN
#define max MAX
#include <gdiplus.h>
#undef min
#undef max

#include <math.h>

using namespace Gdiplus;

extern HDC g_hdc;
extern COLORREF * g_pcolorref;

Bitmap * g_pbitmap = NULL;
Graphics * g_pgraphics = NULL;


SPALIB_API Font * CreatePointFont(double dPointSize,const wchar_t * lpszFaceName,bool bUnderline,bool bBold);
Rect make_rect(LPCRECT lpcrect,bool bMinusOne = false);

Font * g_pfont = NULL;
Font * g_pfontBold = NULL;
Font * g_pfontHeader = NULL;

Pen *g_ppenBorder = NULL;
Brush * g_ptextColor1 = NULL;
Brush * g_pBar = NULL;
Brush * g_pBarBk = NULL;
Pen * g_pBarBorder = NULL;


//LONG width(LPCRECT lpcrect)
//{
//   return lpcrect->right - lpcrect->left;
//}
//
//
//LONG height(LPCRECT lpcrect)
//{
//   return lpcrect->bottom - lpcrect->top;
//}

Rect make_rect(LPCRECT lpcrect, bool bMinusOne)
{
   return Rect(lpcrect->left,lpcrect->top,width(lpcrect) - (bMinusOne ? 1 : 0),height(lpcrect) - (bMinusOne ? 1 : 0));
}



void ca2_install_canvas_init_draw()
{
   g_pbitmap = new Bitmap(g_cx,g_cy,g_cx * sizeof(COLORREF),PixelFormat32bppARGB,(BYTE *)g_pcolorref);
   g_pgraphics = new Graphics(g_pbitmap);
   g_pfont = ::CreatePointFont(10.0,L"Lucida Sans Unicode",false, false);
   g_pfontBold = ::CreatePointFont(10.0,L"Lucida Sans Unicode",false, true);
   g_pfontHeader = ::CreatePointFont(8.4 + 7.7,L"Lucida Sans Unicode",false,true);
   g_ppenBorder  = new Pen(Gdiplus::Color(84,84,84,77),1.0f);
   g_ptextColor1 = new SolidBrush(Color(184, 84 + 49 + 49 + 49,84 + 49 + 49 + 49,77 + 49 + 49 + 49));
   g_pBarBorder = new Pen(Color(84,84 + 49 + 49 + 49,84 + 49 + 49 + 49,77 + 49 + 49 + 49),1.0f);
   g_pBar = new SolidBrush(Color(184,77,184,84));
   g_pBarBk = new SolidBrush(Color(49,184 + 23,184+ 23,184 + 23));
}

void ca2_install_canvas_on_paint(Graphics * pgraphics, LPCRECT lpcrect, int iMode)
{

   iMode = 1;

   bool bProgress = false;
   double dProgress = 0.0;
   string strHeader;
   string strBold;
   string strNormal;
   string strProgress;

   pgraphics->SetCompositingMode(CompositingModeSourceOver);
   pgraphics->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

   {

      int iTrace = _sopen(dir::element() / ("install-"+process_platform_dir_name()+".log"),_O_RDONLY | _O_BINARY,_SH_DENYNO,0);

      if(iTrace >= 0)
      {
         int iTell = _lseek(iTrace,0,SEEK_END);
         iTell--;
         string strLine;
         int iSkip = 0;
         bool bNormal = false;
         bool bHeader = false;
         bool bBold = false;
         bool bPreNormal = true;
         bool bStart = false;
         while(iTell > 0 && !bStart && !(bNormal && bBold && bProgress && bHeader))
         {
            _lseek(iTrace,iTell,SEEK_SET);
            char ch;
            _read(iTrace,&ch,1);
            if(ch == '\r')
            {
               iSkip++;
            }
            else if(ch == '\n')
            {
               iSkip++;
            }
            else if(iSkip > 0)
            {
               iSkip = 0;
               strLine.trim();
               if(strLine == "--")
               {
                  bStart = true;
               }
               else if(::str::begins_eat(strLine, "#----------"))
               {
                  strHeader = L"Thank you";
                  bHeader = true;
                  strBold = u16(strLine);
                  bBold = true;
                  dProgress = 0.0;
                  bProgress = true;
               }
               else if(::str::begins_eat(strLine,":::::"))
               {
                  if(!bHeader && strLine.length() > 0 && bBold && bNormal && bPreNormal)
                  {
                     bHeader = true;
                     strHeader = u16(strLine);
                  }
               }
               else if(::str::begins_eat(strLine,"***"))
               {
                  if(!bBold && strLine.length() > 0 && bNormal && bPreNormal)
                  {
                     bBold = true;
                     strBold = u16(strLine);
                  }
               }
               else if(::str::begins_eat(strLine,"|||"))
               {
                  if(!bProgress && strLine.length() > 0)
                  {
                     bProgress = true;
                     long long int i = _atoi64(strLine.c_str());
                     dProgress = (double)i / 10000000.0;
                     char sz[128];
                     sprintf(sz,"%0.1f%%",dProgress);
                     strProgress = u16(sz);
                     dProgress /= 100.0;
                  }
               }
               else if(!::str::begins_ci(strLine,"/ ") && strLine.length() > 0 && !bNormal && !bBold && !bHeader && bPreNormal)
               {
                  bNormal = true;
                  strNormal = u16(strLine);
               }
               //else if(strLine.length() > 0 && !bPreNormal && !bBold && !bNormal && !bHeader)
               //{
               //   bPreNormal = true;
               //   //::SelectObject(hdc, hfont);
               //   //::TextOutU(hdc, 10, 10 + size.cy * 4, strLine.c_str(), strLine.length());
               //}
               strLine = ch;
            }
            else
            {
               strLine = ch + strLine;
            }
            iTell--;
         }
         _close(iTrace);
      }
   }

   static string s_strLastStatus;
   RECT rect = *lpcrect;
   int cx = lpcrect->right - lpcrect->left;
   int cy = lpcrect->bottom - lpcrect->top;
   iMode = iMode % 5;

   if(iMode == 2 || iMode == 1 || iMode == 0)
   {

      pgraphics->DrawRectangle(g_ppenBorder,make_rect(lpcrect, true));

   }

   RectF rSize;
   
   pgraphics->MeasureString(L"CCpp",4,g_pfont,PointF(0, 0), StringFormat::GenericTypographic(), &rSize);

   double cyText = MAX(rSize.Height,5.0);

   int iLineCount = (rect.bottom - 30) / cyText;

   // !m_bHealingSurface => "Surgery Internals"
   {

      size_t iRefresh = 884;
      size_t iEat = 8;
//      const wchar_t * psz = L"development message so international english file \"install.log\" excerpt  ::::::::";
  //    pgraphics->DrawString(psz,wcslen(psz) - iEat + 1 + ((::GetTickCount() / (iRefresh - 277) % iEat)),g_pfont,PointF(10,10 + cyText * 2),StringFormat::GenericTypographic(),g_ptextColor1);

      if(strHeader.length() > 0)
      {
         pgraphics->DrawString(u16(strHeader),-1,g_pfontHeader,PointF(10,10 + cyText * 3+4) ,StringFormat::GenericTypographic(),g_ptextColor1);
      }
      if(strBold.length() > 0)
      {
         pgraphics->DrawString(u16(strBold),-1,g_pfontBold,PointF(10,10 + cyText * 5),StringFormat::GenericTypographic(),g_ptextColor1);
      }
      if(strNormal.length() > 0)
      {
         pgraphics->DrawString(u16(strNormal),-1,g_pfont,PointF(10,10 + cyText * 6),StringFormat::GenericTypographic(),g_ptextColor1);
      }
      if(strProgress.length() > 0)
      {
         pgraphics->DrawString(u16(strProgress),-1,g_pfont,PointF(10,10 + cyText * 7),StringFormat::GenericTypographic(),g_ptextColor1);
      }

   }
   double cyBar = cyText * 1.2;

   {
      pgraphics->FillRectangle(g_pBarBk,RectF(10.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0,lpcrect->right - 10.0 - 10.0,cyBar));

      if(bProgress)
      {
         double iRight = ((double) g_cx - 11.0 - 11.0) * dProgress;
         pgraphics->FillRectangle(g_pBar,RectF(11.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0,iRight,cyBar - 2.0));
      }
      else
      {
         double dPeriod = 2000.0;
         dProgress = fmod((double)GetTickCount(),dPeriod) / dPeriod;
         double iBarWidth = (lpcrect->right - 11.0 - 11.0) / 4;
         double i = ((lpcrect->right - 11.0 - 11.0) * dProgress) + 11.0;
         double iRight = i + iBarWidth;
         pgraphics->FillRectangle(g_pBar,RectF(11.0 + i,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0,MIN(lpcrect->right - 10.0,iRight) - 11 - i,cyBar - 2.0));
         if(iRight >= lpcrect->right - 10)
         {
            pgraphics->FillRectangle(g_pBar,RectF(11.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0,iRight - lpcrect->right - 10.0 - 11.0,cyBar - 2.0));
         }
      }
      pgraphics->DrawRectangle(g_pBarBorder,RectF(10.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0,lpcrect->right - 10.0 - 10.0,cyBar));

   }


   {
   
      pgraphics->DrawString(L"Thank you",-1,g_pfont,PointF(10.0,10.0),g_ptextColor1);

   }

}










Font * CreatePointFont(double dPointSize, const wchar_t * lpszFaceName, bool bUnderline, bool bBold)
{
   
   FontFamily fontFamily(lpszFaceName);
   
   return new Font(&fontFamily,dPointSize,(bBold ? FontStyleBold : 0) | (bUnderline ? FontStyleUnderline : 0),UnitPoint);

}


//void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
//{
//
//   ::SetBkColor(hdc, clr);
//
//   ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
//
//}
//



void update_layered_window()
{

   RECT rect;

   rect.left = 0;
   rect.top = 0;
   rect.right = g_cx;
   rect.bottom = g_cy;

   g_pgraphics->SetCompositingMode(CompositingModeSourceCopy);
   {
      SolidBrush sb(Color(184 + 23,23,23,23));
      g_pgraphics->FillRectangle(&sb,make_rect(&rect));
   }

   ca2_install_canvas_on_paint(g_pgraphics,&rect,1);

   RECT rectWindow;

   ::GetWindowRect(g_hwnd,&rectWindow);

   //POINT ptCursor;

   //::GetCursorPos(&ptCursor);

   //if(!PtInRect(&rectWindow,ptCursor))
   //{
   //}

   HDC hdcWindow = ::GetWindowDC(g_hwnd);

   POINT pt;

   pt.x = rectWindow.left;

   pt.y = rectWindow.top;

   SIZE sz;

   sz.cx = width(&rectWindow);

   sz.cy = height(&rectWindow);

   POINT ptSrc ={0};

   BLENDFUNCTION blendPixelFunction ={AC_SRC_OVER,0,255,AC_SRC_ALPHA};

   UpdateLayeredWindow(g_hwnd,hdcWindow,&pt,&sz,g_hdc,&ptSrc,0,&blendPixelFunction,ULW_ALPHA);

   ::ReleaseDC(g_hwnd,hdcWindow);


}

