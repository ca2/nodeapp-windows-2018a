#include "a_spalib.h"

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


class canvas_zero
{
public:
   DWORD m_dwCurZero;
   DWORD m_dwNextZero;
   int m_iMaxSize;
   int m_iLast;
   std::vector < POINT > m_pta;
   int_array m_ia;
   RECT m_rect;
   HBITMAP m_hbm;
   HBITMAP m_hbmOld;
   HDC m_hdc;
   HBITMAP m_hbmZero;
   HBITMAP m_hbmZeroOld;
   HDC m_hdcZero;
   COLORREF * m_pdata;
   COLORREF * m_pdataZero;
   canvas_zero();
   void prepare(Graphics * pdc,LPCRECT lpcrect);
   void on_paint(Graphics * pdc,LPCRECT lpcrect);
   virtual void zero(Graphics * pdc, POINT pt, int iSize, int iStep);
};

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

void ca2_install_canvas_on_paint(Graphics * pdc, LPCRECT lpcrect, int iMode)
{

   iMode = 1;

   bool bProgress = false;
   double dProgress = 0.0;
   wstring strHeader;
   wstring strBold;
   wstring strNormal;
   wstring strProgress;

   pdc->SetCompositingMode(CompositingModeSourceOver);
   pdc->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

   {

#if defined(_M_IX86)

      int iTrace = _sopen(dir::element() / "install-x86.log",_O_RDONLY | _O_BINARY,_SH_DENYNO,0);

#else

      int iTrace = _sopen(dir::element() / "install-x64.log",_O_RDONLY | _O_BINARY,_SH_DENYNO,0);

#endif

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

   static canvas_zero czero;
   static string s_strLastStatus;
   RECT rect = *lpcrect;
   int cx = lpcrect->right - lpcrect->left;
   int cy = lpcrect->bottom - lpcrect->top;
   iMode = iMode % 5;

   if(iMode == 2 || iMode == 1 || iMode == 0)
   {

      pdc->DrawRectangle(g_ppenBorder,make_rect(lpcrect, true));

   }

   RectF rSize;
   
   pdc->MeasureString(L"CCpp",4,g_pfont,PointF(0, 0), StringFormat::GenericTypographic(), &rSize);

   double cyText = MAX(rSize.Height,5.0);

   int iLineCount = (rect.bottom - 30) / cyText;

   if(iMode == 4) // if(m_bHealingSurface)
   {
      int iCount = MAX(1, cx / 25);
      int jCount = MAX(1,cy / 25);

      for(int i = 0; i < iCount; i++)
      {
         for(int j = 0; j < jCount; j++)
         {
            //int iPhase = 
            // 400 
            //* (0.5 + (sin(((double) ::GetTickCount() * 3.1415 * 2.0  * 0.084 / 1000.0) + ((double) (i) * 3.1415 * 2.0 / (double) (iCount))) / 2.0));
            //* (0.5 + (cos(((double) ::GetTickCount() * 3.1415 * 2.0  * 0.084 / 1000.0) + ((double) (0) * 3.1415 * 2.0 / (double) (jCount))) / 2.0));
            double dPhase = fmod((((double) ::GetTickCount() * 360  * 0.5984 / 1000.0) + (i * 360.0 / (double) iCount)  + (j * 360.0 / (double) jCount) + ((double) (sin(((double) ::GetTickCount() * 3.1415 * 2.0  *0.0484 / 1000.0)+i * 3.1415 * 2.0 * 2.0 / (double) (iCount)) * sin(((double) ::GetTickCount() * 3.1415 * 2.0  * 0.0484 / 1000.0)+j * 3.1415 * 2.0 * 2.0 / (double) (jCount)) * 360))), 360.0);
            int iR;
            int iG;
            int iB;
            double dRate = fmod(dPhase, 60.0) / 60.0;
            int iColor = (int) (dRate * 155.0);
            if(dPhase < 60)
            {
               // purple to blue
               iR = 255 - iColor;
               iG = 100;
               iB = 255;
            }
            else if(dPhase < (120))
            {
               // blue to cyan
               iR = 100;
               iG = 100 + iColor;
               iB = 255;
            }
            else if(dPhase < (180))
            {
               // cyan to green
               iR = 100;
               iG = 255;
               iB = 255 - iColor;
            }
            else if(dPhase < (240))
            {
               // green to yellow
               iR = 100 + iColor;
               iG = 255;
               iB = 100;
            }
            else if(dPhase < (300))
            {
               // yellow to red
               iR = 255;
               iG = 255 - iColor;
               iB = 100;
            }
            else
            {
               // red to purple
               iR = 255;
               iG = 100;
               iB = 100 + iColor;
            }

            double z = ((int) (360 - (dPhase - 240 + ::GetTickCount() * 360  * 0.1984 / 1000.0))) % 360;
            if(dPhase == 240)
               z = 360;

            int iInflate = (int) (z * 33.0 / 360.0);

            rect.left = cx * i / iCount + 8 - iInflate;
            rect.right = cx * (i + 1) / iCount - 8 + iInflate;
            rect.top = cy * j / jCount + 8 - iInflate;
            rect.bottom = cy * (j + 1) / jCount - 8 + iInflate;
            SolidBrush br(Color(184,iR,iG,iB));
            // ca2 colors: green and yellow and black and white
            pdc->FillRectangle(&br, make_rect(&rect));
         }
      }
   }
   else if(iMode == 1) // else // !m_bHealingSurface => "Surgery Internals"
   {

      size_t iRefresh = 884;
      size_t iEat = 8;
//      const wchar_t * psz = L"development message so international english file \"install.log\" excerpt  ::::::::";
  //    pdc->DrawString(psz,wcslen(psz) - iEat + 1 + ((::GetTickCount() / (iRefresh - 277) % iEat)),g_pfont,PointF(10,10 + cyText * 2),StringFormat::GenericTypographic(),g_ptextColor1);

      if(strHeader.length() > 0)
      {
         pdc->DrawString(strHeader.c_str(),-1,g_pfontHeader,PointF(10,10 + cyText * 3+4) ,StringFormat::GenericTypographic(),g_ptextColor1);
      }
      if(strBold.length() > 0)
      {
         pdc->DrawString(strBold.c_str(),-1,g_pfontBold,PointF(10,10 + cyText * 5),StringFormat::GenericTypographic(),g_ptextColor1);
      }
      if(strNormal.length() > 0)
      {
         pdc->DrawString(strNormal.c_str(),-1,g_pfont,PointF(10,10 + cyText * 6),StringFormat::GenericTypographic(),g_ptextColor1);
      }
      if(strProgress.length() > 0)
      {
         pdc->DrawString(strProgress.c_str(),-1,g_pfont,PointF(10,10 + cyText * 7),StringFormat::GenericTypographic(),g_ptextColor1);
      }

   }
   else if(iMode == 2) // else // !m_bHealingSurface => "Surgery Internals"
   {

      size_t iRefresh = 884;
      size_t iEat = 8;
      //const wchar_t * psz = L"development message so international english last lines of file \"install.log\" ::::::::";
      //pdc->DrawString(psz,wcslen(psz) - iEat + 1 + ((::GetTickCount() / (iRefresh - 277) % iEat)),g_pfont,PointF(10,10 + cyText * 2),StringFormat::GenericTypographic(),g_ptextColor1);
      pdc->DrawString(u16(s_strLastStatus).c_str(),-1,g_pfont,PointF(10,10 + cyText * 3),StringFormat::GenericTypographic(),g_ptextColor1);

      int iLineMin = 5;
      int iLine = ((rect.bottom - 10) / cyText) - 1;
      Font * pfont = g_pfont;
      if(rect.bottom - rect.top >= cyText)
      {

#if defined(_M_IX86)

         int iTrace = _sopen(dir::element() / "install-x86.log", _O_RDONLY|_O_BINARY, _SH_DENYNO, 0);

#else

         int iTrace = _sopen(dir::element() / "install-x64.log",_O_RDONLY | _O_BINARY,_SH_DENYNO,0);

#endif

         if(iTrace >= 0)
         {
            int iTell = _lseek(iTrace, 0, SEEK_END);
            iTell--;
            string strLine;
            int iSkip = 0;
            while(iTell > 0 && iLine >= iLineMin)
            {
               _lseek(iTrace, iTell, SEEK_SET);
               char ch;
               _read(iTrace, &ch, 1);
               if(ch == '\r')
               {
                  if(iSkip >= 2)
                  {
                     iSkip = 0;
                     iLine--;
                  }
                  iSkip++;
               }
               else if(ch == '\n')
               {
                  if(iSkip >= 2)
                  {
                     iSkip = 0;
                     iLine--;
                  }
                  iSkip++;
               }
               else if(iSkip > 0)
               {
                  iSkip = 0;
                  if(::str::begins_eat(strLine, "***"))
                  {
                     if(strLine == s_strLastStatus)
                     {
                        goto skip_text_out1;
                     }
                     s_strLastStatus = strLine;
                     pfont = g_pfontBold;
                  }
                  else
                  {
                     int iFind = strLine.find('\t');
                     if(iFind >= 0)
                     {
                        strLine = strLine.substr(iFind + 1);
                     }
                     pfont = g_pfont;
                  }
                  iLine--;
                  pdc->DrawString(u16(strLine).c_str(),-1,pfont,PointF(10,10 + iLine * 3),StringFormat::GenericTypographic(),g_ptextColor1);
                  skip_text_out1:
                  strLine = ch;
               }
               else
               {
                  strLine = ch + strLine;
               }
               iTell--;
            }
            _close(iTrace);
            if(iLine >= iLineMin && strLine.length() > 0)
            {
               iLine--;
               pdc->DrawString(u16(strLine).c_str(),-1,pfont,PointF(10,10 + iLine * 3),StringFormat::GenericTypographic(),g_ptextColor1);
            }
         }
      }
   }
   else if(iMode == 3) // if(m_bHealingSurface)
   {
      czero.on_paint(pdc, lpcrect);
   }

   double cyBar = cyText * 1.2;

   {
      pdc->FillRectangle(g_pBarBk,RectF(10.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0,lpcrect->right - 10.0 - 10.0,cyBar));

      if(bProgress)
      {
         double iRight = ((double) g_cx - 11.0 - 11.0) * dProgress;
         pdc->FillRectangle(g_pBar,RectF(11.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0,iRight,cyBar - 2.0));
      }
      else
      {
         double dPeriod = 2000.0;
         dProgress = fmod((double)GetTickCount(),dPeriod) / dPeriod;
         double iBarWidth = (lpcrect->right - 11.0 - 11.0) / 4;
         double i = ((lpcrect->right - 11.0 - 11.0) * dProgress) + 11.0;
         double iRight = i + iBarWidth;
         pdc->FillRectangle(g_pBar,RectF(11.0 + i,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0,MAX(lpcrect->right - 10.0,iRight) - 11 - i,cyBar - 2.0));
         if(iRight >= lpcrect->right - 10)
         {
            pdc->FillRectangle(g_pBar,RectF(11.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0,iRight - lpcrect->right - 10.0 - 11.0,cyBar - 2.0));
         }
      }
      pdc->DrawRectangle(g_pBarBorder,RectF(10.0,(lpcrect->top + lpcrect->bottom - cyBar) / 2.0,lpcrect->right - 10.0 - 10.0,cyBar));

   }


   {
      //const char * psz = "Thank you";
      //::SetBkMode(hdc, TRANSPARENT);
    //  HFONT hfontUnderline = ::CreatePointFont(100,"Lucida Sans Unicode",hdc,true);
      //::SelectObject(hdc,hfontUnderline);
      //::SetTextColor(hdc, RGB(77, 84, 184));
      //::TextOutU(hdc, 10, 10, psz, strlen(psz));
      //::SelectObject(hdc, hfontBold);
      //::TextOutU(hdc, 10, 10 + size.cy, g_strTitle.c_str(), g_strTitle.length());
      //::SelectObject(hdc, hfontOld);
      //::DeleteObject(hfontUnderline);
      pdc->DrawString(L"Thank you",-1,g_pfont,PointF(10.0,10.0),g_ptextColor1);
   }
   //::DeleteObject(hfontHeader);
   //::DeleteObject(hfontBold);
   //::DeleteObject(hfont);
}










Font * CreatePointFont(double dPointSize, const wchar_t * lpszFaceName, bool bUnderline, bool bBold)
{
   
   FontFamily fontFamily(lpszFaceName);
   
   return new Font(&fontFamily,dPointSize,(bBold ? FontStyleBold : 0) | (bUnderline ? FontStyleUnderline : 0),UnitPoint);

}


//BOOL TextOutU(HDC hdc, int x, int y, const char * pszUtf8, int iSize)
//{
//   string str;
//   str.assign(pszUtf8, iSize);
//
//   SIZE size;
//   size.cx = 0;
//   size.cy = 0;
//   if(str.find(_unitext("✓")) != string::npos)
//   {
//      str = str_replace(str.c_str(), _unitext("✓"), "");
//      ::GetTextExtentPointW(hdc, L"C", 1, &size);
//      size.cx = size.cy;
//      HBRUSH hbrush = ::CreateSolidBrush(RGB(0xaa, 0XCC, 0xaa));
//      HBRUSH hbrushOld = (HBRUSH) ::SelectObject(hdc, (HGDIOBJ) hbrush); 
//      HPEN hpen = (HPEN) ::GetStockObject(NULL_PEN);
//      HPEN hpenOld = (HPEN) ::SelectObject(hdc, (HGDIOBJ) hpen); 
//      ::Ellipse(hdc, x + 1, y + 1, x + size.cx - 2, y + size.cy - 2);
//      ::SelectObject(hdc, (HGDIOBJ)  hbrushOld);
//      ::SelectObject(hdc, (HGDIOBJ)  hpenOld);
//      ::DeleteObject((HGDIOBJ) hbrush);
//      size.cx += 4;
//   }
//   BSTR bstr = BSTRFromCStr(CP_UTF8, str.c_str());
//   BOOL b = TextOutW(hdc, size.cx + x, y, bstr, wcslen(bstr));
//   ::SysFreeString(bstr);
//   return b;
//}
void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
{
   ::SetBkColor(hdc, clr);
   ::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
}



canvas_zero::canvas_zero()
{
   m_rect.left = 0;
   m_rect.top = 0;
   m_rect.right = 0;
   m_rect.bottom = 0;
   m_dwCurZero = ::GetTickCount();
   m_dwNextZero = ::GetTickCount();
   m_iMaxSize = 33;
   m_iLast = 0;
}

void canvas_zero::prepare(Graphics * pdc, LPCRECT lpcrect)
{
   m_rect.left    = lpcrect->left;
   m_rect.top     = lpcrect->top;
   m_rect.right   = lpcrect->right;
   m_rect.bottom  = lpcrect->bottom;

   int cx = m_rect.right - m_rect.left;
   int cy = m_rect.bottom - m_rect.top;

   BITMAPINFO m_Info;

	m_Info.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	m_Info.bmiHeader.biWidth=cx;
	m_Info.bmiHeader.biHeight=cy;
	m_Info.bmiHeader.biPlanes=1;
	m_Info.bmiHeader.biBitCount=32; 
	m_Info.bmiHeader.biCompression=BI_RGB;
	m_Info.bmiHeader.biSizeImage=cx*cy*4;

   LPDWORD lpdata;
	
	m_hbm = CreateDIBSection ( NULL, &m_Info, DIB_RGB_COLORS, (void **)&m_pdata, NULL, NULL ); 
   m_hdc = ::CreateCompatibleDC(NULL);
   m_hbmOld = (HBITMAP) ::SelectObject(m_hdc, m_hbm);
   FillSolidRect(m_hdc, lpcrect, RGB(0, 0, 0));

   /*int iMid = (m_rect.bottom + m_rect.top) / 2;

   HPEN hpen = ::CreatePen(PS_SOLID, 1, RGB(0x7c, 0x7c, 0x7c));

   ::SelectObject(m_hdc, hpen);

   MoveToEx(m_hdc, m_rect.left, iMid, NULL);
   LineTo(m_hdc, m_rect.right, iMid);
   ::DeleteObject(hpen);*/

}

void canvas_zero::on_paint(Graphics * pdc, LPCRECT lpcrect)
{
   HDC hdc = m_hdc;

   if(m_rect.left != lpcrect->left
   || m_rect.top != lpcrect->top
   || m_rect.right != lpcrect->right
   || m_rect.bottom != lpcrect->bottom)
   {
      prepare(pdc, lpcrect);
   }

   int cx = m_rect.right - m_rect.left;
   int cy = m_rect.bottom - m_rect.top;


   int iTimeFactor = 100; // greater faster

   if(::GetTickCount() >= m_dwNextZero)
   {
      POINT pt;
      pt.x = (rand() * cx / RAND_MAX) + lpcrect->left;
      pt.y = (rand() * cy / RAND_MAX) + lpcrect->top;
      m_pta.insert_at(0, pt);
      m_ia.insert_at(0, ((rand() * MAX(cx, cy) / 4 / RAND_MAX) + 23));
      m_dwCurZero = ::GetTickCount();
      m_dwNextZero = m_dwCurZero + m_ia[m_ia.size() - 1] * 1000 / iTimeFactor + 1984 + 1977;
      m_iLast = 0;
      int iSize = m_ia[0];
      int w = iSize * 2 + 1;
      int h = iSize * 2 + 1;
      BITMAPINFO m_Info;

      ZeroMemory(&m_Info, sizeof (BITMAPINFO));

	   m_Info.bmiHeader.biSize=sizeof (BITMAPINFOHEADER);
	   m_Info.bmiHeader.biWidth=w;
	   m_Info.bmiHeader.biHeight=-h;
	   m_Info.bmiHeader.biPlanes=1;
	   m_Info.bmiHeader.biBitCount=32; 
	   m_Info.bmiHeader.biCompression=BI_RGB;
	   m_Info.bmiHeader.biSizeImage=w*h*4;

      LPDWORD lpdata;
	
	   m_hbmZero = CreateDIBSection ( NULL, &m_Info, DIB_RGB_COLORS, (void **)&m_pdataZero, NULL, NULL ); 
      m_hdcZero = ::CreateCompatibleDC(NULL);
      m_hbmZeroOld = (HBITMAP) ::SelectObject(m_hdcZero, m_hbmZero);
      FillSolidRect(m_hdcZero, lpcrect, RGB(0, 0, 0));
   }

   



   int i = 1;
/*   for(; i < m_pta.size(); i++)
   {
      POINT pt = m_pta[i];
      int iSize = m_ia[i];
      zero(hdc, pt, iSize, iSize);
   }*/

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 255;
         bf.AlphaFormat = AC_SRC_ALPHA;

   bool bReady = false;
   POINT pt;
   int iSize;
   int iCurStep;
   if(m_pta.size() > 0)
   {
      pt = m_pta[0];
      iSize = m_ia[0];
      iCurStep = (::GetTickCount() - m_dwCurZero) * iTimeFactor / 1000;
      iCurStep = MAX(2, MIN(iCurStep, iSize));
      zero(pdc, pt, iSize, iCurStep);
      if(iCurStep == iSize)
      {
         if(m_iLast <= iCurStep)
         {
            ::AlphaBlend(m_hdc, pt.x - iSize / 2, pt.y - iSize / 2, iSize * 2 + 1, iSize * 2 + 1, m_hdcZero, 0, 0, iSize * 2 + 1, iSize * 2 + 1, bf);
         }
         bReady = true;
      }
      else
      {
      }
      m_iLast = iCurStep + 1;
   }
   else
   {
   }

   {
      ::SelectObject(m_hdc,m_hbmOld);
      Bitmap b(m_hbm, NULL);
      pdc->DrawImage(&b,Rect(0,0,cx,cy));
         //::BitBlt(hdcPaint,0,0,cx,cy,m_hdc,0,0,SRCCOPY);
      ::SelectObject(m_hdc,m_hbm);
   }

   if(!bReady)
   {
      ::SelectObject(m_hdcZero,m_hbmZeroOld);
      Bitmap b(m_hbmZero, NULL);
//      pdc->DrawImage(&b,
         pdc->DrawImage(&b,Rect(pt.x - iSize / 2,pt.y - iSize / 2,iSize * 2 + 1,iSize * 2 + 1));
      ///::AlphaBlend(hdcPaint,pt.x - iSize / 2,pt.y - iSize / 2,iSize * 2 + 1,iSize * 2 + 1,m_hdcZero,0,0,iSize * 2 + 1,iSize * 2 + 1,bf);
      //::AlphaBlend(hdcPaint, 0, 0, iSize, iSize, m_hdcZero, 0, 0, iSize, iSize, bf);
      ::SelectObject(m_hdcZero,m_hbmZero);
   }

}


void canvas_zero::zero(Graphics * pdc, POINT pt, int iSize, int iStep)
{

   int cx = m_rect.right - m_rect.left;
   int cy = m_rect.bottom - m_rect.top;

   int w = iSize * 2 + 1;
   int radius = iStep - 1;
   for(int j = -iSize; j < iSize; j++)
   {
      for(int k = -iSize; k < iSize; k++)
      {
         double dj = j;
         double dk = k;
         double distance = sqrt((dj * dj + dk * dk));
         if(distance < radius)
         {
            double dLum = (double) (distance) / (double) radius;
            dLum = pow(dLum, 0.2);
            int x = j + iSize;
            int y = k + iSize;
            int iAlpha = dLum * 255.0;
            if(iAlpha > 255)
               iAlpha = 255;
            if(iAlpha < 0)
               iAlpha = 0;
            int iLum = 255 - iAlpha;
            m_pdataZero[x + y * w] = RGB(iLum, iLum, iLum) | (iLum << 24);
         }
      }
   }


}

void update_layered_window()
{

   //RECT rectWindow;
   //::GetWindowRect(hwnd, &rectWindow);


   //int cx = rectWindow.right - rectWindow.left;
   //int cy = rectWindow.bottom - rectWindow.top;

   //RECT rect;
   //rect.left         = 0;
   //rect.top          = 0;
   //rect.right        = cx;
   //rect.bottom       = cy;

   //if(lprect == NULL)
   //{
   //lprect = &rect;
   //}


   //HBITMAP hbmp      = ::CreateCompatibleBitmap(hdcWindow, cx, cy);
   //HDC hdc           = ::CreateCompatibleDC(NULL);
   //HBITMAP hbmpOld   =  (HBITMAP) ::SelectObject(hdc, (HGDIOBJ) hbmp);

   //::BitBlt(hdc, 0, 0, cx, cy, hdcWindow, 0, 0, SRCCOPY);

   //PaintBk(hdc);

   //HFONT hfontOld = NULL;
   //HFONT hfont = NULL;


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

   ca2_install_canvas_on_paint(g_pgraphics,&rect,g_iHealingSurface);


   RECT rectWindow;

   ::GetWindowRect(g_hwnd,&rectWindow);

   POINT ptCursor;

   ::GetCursorPos(&ptCursor);

   if(!PtInRect(&rectWindow,ptCursor))
   {
      //SolidBrush sb(Color(84,250,250,255));
      //g_pgraphics->FillRectangle(&sb,make_rect(&rect));
   }

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

   /*   POINT pointViewport;
   ::SetViewportOrgEx(hdc, 0, 0, &pointViewport);
   ::BitBlt(hdcWindow, lprect->left, lprect->top, lprect->right - lprect->left, lprect->bottom - lprect->top,
   hdc,       lprect->left, lprect->top, SRCCOPY);
   ::SelectObject(hdc, (HGDIOBJ) hbmpOld);
   if(hfontOld != NULL)
   {
   ::SelectObject(hdc,(HGDIOBJ)  hfontOld);
   }
   if(hfont != NULL)
   {
   ::DeleteObject(hfont);
   }
   ::DeleteObject(hbmp);
   ::DeleteDC(hdc);*/



}



//Gdiplus::GdiplusStartupInput *   g_pgdiplusStartupInput     = NULL;
//Gdiplus::GdiplusStartupOutput *  g_pgdiplusStartupOutput    = NULL;
//DWORD_PTR                        g_gdiplusToken             = NULL;
//DWORD_PTR                        g_gdiplusHookToken         = NULL;
