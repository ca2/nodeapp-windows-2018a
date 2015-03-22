#include "StdAfx.h"


class canvas_zero
{
public:
   DWORD m_dwCurZero;
   DWORD m_dwNextZero;
   int m_iMaxSize;
   int m_iLast;
   std::vector < POINT > m_pta;
   inta m_ia;
   RECT m_rect;
   HBITMAP m_hbm;
   HDC m_hdc;
   HBITMAP m_hbmZero;
   HBITMAP m_hbmZeroOld;
   HDC m_hdcZero;
   COLORREF * m_pdata;
   COLORREF * m_pdataZero;
   canvas_zero();
   void prepare(HDC hdc, LPCRECT lpcrect);
   void on_paint(HDC hdc, LPCRECT lpcrect);
   virtual void zero(HDC hdc, POINT pt, int iSize, int iStep);
};

void ca2_install_canvas_on_paint(HDC hdc, LPCRECT lpcrect, int iMode)
{
   static canvas_zero czero;
   static std::string s_strLastStatus;
   RECT rect = *lpcrect;
   int cx = lpcrect->right - lpcrect->left;
   int cy = lpcrect->bottom - lpcrect->top;
   iMode = iMode % 5;
   HFONT hfont = ::CreatePointFont(100, "Lucida Sans Unicode", hdc);
   LOGFONT lf;
   memset(&lf, 0, sizeof(LOGFONT));
   ::GetObjectA(hfont, sizeof(LOGFONT), &lf);
   lf.lfWeight = 800;
   HFONT hfontBold = ::CreateFontIndirectA(&lf);
   HFONT hfontOld = (HFONT) ::SelectObject(hdc, (HGDIOBJ) hfont);
   if(iMode == 2 || iMode == 1 || iMode == 0)
   {
      HPEN hpen = ::CreatePen(PS_SOLID, 1, RGB(84, 84, 77));
      HBRUSH hbrush = (HBRUSH) ::GetStockObject(NULL_BRUSH);
      ::SelectObject(hdc, hpen);
      ::SelectObject(hdc, hbrush);
      ::Rectangle(hdc, lpcrect->left, lpcrect->top, lpcrect->right, lpcrect->bottom);
      ::DeleteObject(hbrush);
      ::DeleteObject(hpen);
   }
   SIZE size;
   ::GetTextExtentPointW(hdc, L"CCpp", 4, &size);
   int iLineCount = (rect.bottom - 30) / size.cy;
   if(iMode == 4) // if(m_bHealingSurface)
   {
      int iCount = max(1, cx / 25);
      int jCount = max(1, cy / 25);

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

            // ca2 colors: green and yellow and black and white
            ::FillSolidRect(hdc, &rect, RGB(iR, iG, iB));
         }
      }
   }
   else if(iMode == 1) // else // !m_bHealingSurface => "Surgery Internals"
   {

      ::SetTextColor(hdc, RGB(84 + 49, 84 + 49, 77 + 49));

      size_t iRefresh = 884;
      size_t iEat = 8;
      const char * psz = "development message so international english file \"install.log\" excerpt  ::::::::";
      ::SetBkMode(hdc, TRANSPARENT);
      ::TextOutU(hdc, 10, 10 + size.cy * 2, psz, strlen(psz) - iEat + 1 + ((::GetTickCount() / (iRefresh - 277) % iEat)));

      int iLineMin = 5;
      int iLine = ((rect.bottom - 10) / size.cy) - 1;
      if(rect.bottom - rect.top >= size.cy)
      {
         int iTrace = _sopen(dir::ca2("install.log").c_str(), _O_RDONLY|_O_BINARY, _SH_DENYNO, 0);
         if(iTrace >= 0)
         {
            int iTell = _lseek(iTrace, 0, SEEK_END);
            iTell--;
            std::string strLine;
            int iSkip = 0;
            ::SetBkMode(hdc, TRANSPARENT);
            bool bNormal = false;
            bool bBold = false;
            bool bPreNormal = false;
            bool bStart = false;
            while(iTell > 0 && !bStart && !(bNormal && bBold))
            {
               _lseek(iTrace, iTell, SEEK_SET);
               char ch;
               _read(iTrace, &ch, 1);
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
                  str_trim(strLine);
                  if(strLine == "--")
                  {
                     bStart = true;
                  }
                  else if(str_begins_ci(strLine.c_str(), "***") && !bBold && strLine.length() > 0 && bNormal && bPreNormal)
                  {
                     bBold = true;
                     strLine = strLine.substr(3);
                     ::SelectObject(hdc, hfontBold);
                     ::TextOutU(hdc, 10, 10 + size.cy * 3, strLine.c_str(), strLine.length());
                  }
                  else if(!str_begins_ci(strLine.c_str(), "***") && strLine.length() > 0 && !bNormal && !bBold && bPreNormal)
                  {
                     bNormal = true;
                     ::SelectObject(hdc, hfont);
                     ::TextOutU(hdc, 10, 10 + size.cy * 4, strLine.c_str(), strLine.length());
                  }
                  else if(strLine.length() > 0 && !bPreNormal && !bBold && !bNormal)
                  {
                     bPreNormal = true;
                     //::SelectObject(hdc, hfont);
                     //::TextOutU(hdc, 10, 10 + size.cy * 4, strLine.c_str(), strLine.length());
                  }
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
   }
   else if(iMode == 2) // else // !m_bHealingSurface => "Surgery Internals"
   {

      ::SetTextColor(hdc, RGB(84 + 49, 84 + 49, 77 + 49));

      size_t iRefresh = 884;
      size_t iEat = 8;
      const char * psz = "development message so international english last lines of file \"install.log\" ::::::::";
      ::SetBkMode(hdc, TRANSPARENT);
      ::TextOutU(hdc, 10, 10 + size.cy * 2, psz, strlen(psz) - iEat + 1 + ((::GetTickCount() / (iRefresh - 277) % iEat)));
      ::SelectObject(hdc, hfontBold);
      ::TextOutU(hdc, 10, 10 + size.cy * 3, s_strLastStatus.c_str(), s_strLastStatus.length());

      int iLineMin = 5;
      int iLine = ((rect.bottom - 10) / size.cy) - 1;
      if(rect.bottom - rect.top >= size.cy)
      {
         int iTrace = _sopen(dir::ca2("install.log").c_str(), _O_RDONLY|_O_BINARY, _SH_DENYNO, 0);
         if(iTrace >= 0)
         {
            int iTell = _lseek(iTrace, 0, SEEK_END);
            iTell--;
            std::string strLine;
            int iSkip = 0;
            ::SetBkMode(hdc, TRANSPARENT);
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
                  if(str_begins_ci(strLine.c_str(), "***"))
                  {
                     strLine = strLine.substr(3);
                     if(strLine == s_strLastStatus)
                     {
                        goto skip_text_out1;
                     }
                     s_strLastStatus = strLine;
                     ::SelectObject(hdc, hfontBold);
                  }
                  else
                  {
                     int iFind = strLine.find('\t');
                     if(iFind >= 0)
                     {
                        strLine = strLine.substr(iFind + 1);
                     }
                     ::SelectObject(hdc, hfont);
                  }
                  iLine--;
                  ::TextOutU(hdc, 10, 10 + iLine * size.cy, strLine.c_str(), strLine.length());
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
               ::TextOutU(hdc, 10, 10 + iLine * size.cy, strLine.c_str(), strLine.length());
            }
         }
      }
   }
   else if(iMode == 3) // if(m_bHealingSurface)
   {
      czero.on_paint(hdc, lpcrect);
   }
   {
      const char * psz = "Thank you";
      ::SetBkMode(hdc, TRANSPARENT);
      ::SelectObject(hdc, hfont);
      ::SetTextColor(hdc, RGB(77, 77, 84));
      ::TextOutU(hdc, 10, 10, psz, strlen(psz));
      ::SelectObject(hdc, hfontBold);
      //::TextOutU(hdc, 10, 10 + size.cy, g_strTitle.c_str(), g_strTitle.length());
      ::SelectObject(hdc, hfontOld);
   }
   ::DeleteObject(hfontBold);
   ::DeleteObject(hfont);
}









HFONT CreatePointFont(int nPointSize, const char * lpszFaceName, HDC hdc)
{
   LOGFONT logFont;
   memset(&logFont, 0, sizeof(LOGFONT));
   logFont.lfCharSet = DEFAULT_CHARSET;
   logFont.lfHeight = nPointSize;
   strncpy(logFont.lfFaceName, lpszFaceName, sizeof(logFont.lfFaceName));

   return CreatePointFontIndirect(&logFont, hdc);
}

// pLogFont->nHeight is interpreted as PointSize * 10
HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdcParam)
{
   HDC hDC;
   if (hdcParam != NULL)
   {
      hDC = hdcParam;
   }
   else
      hDC = ::GetDC(NULL);

   // convert nPointSize to logical units based on pgraphics
   LOGFONT logFont = *lpLogFont;
   POINT pt;
   // 72 points/inch, 10 decipoints/point
   pt.y = ::MulDiv(::GetDeviceCaps(hDC, LOGPIXELSY), logFont.lfHeight, 720);
   pt.x = 0;
   ::DPtoLP(hDC, &pt, 1);
   POINT ptOrg = { 0, 0 };
   ::DPtoLP(hDC, &ptOrg, 1);
   logFont.lfHeight = -abs(pt.y - ptOrg.y);

   if(hdcParam == NULL)
      ReleaseDC(NULL, hDC);

   return ::CreateFontIndirect(&logFont);
}



BOOL TextOutU(HDC hdc, int x, int y, const char * pszUtf8, int iSize)
{
   std::string str;
   str.assign(pszUtf8, iSize);

   SIZE size;
   size.cx = 0;
   size.cy = 0;
   if(str.find(_unitext("✓")) != std::string::npos)
   {
      str = str_replace(str.c_str(), _unitext("✓"), "");
      ::GetTextExtentPointW(hdc, L"C", 1, &size);
      size.cx = size.cy;
      HBRUSH hbrush = ::CreateSolidBrush(RGB(0xaa, 0XCC, 0xaa));
      HBRUSH hbrushOld = (HBRUSH) ::SelectObject(hdc, (HGDIOBJ) hbrush); 
      HPEN hpen = (HPEN) ::GetStockObject(NULL_PEN);
      HPEN hpenOld = (HPEN) ::SelectObject(hdc, (HGDIOBJ) hpen); 
      ::Ellipse(hdc, x + 1, y + 1, x + size.cx - 2, y + size.cy - 2);
      ::SelectObject(hdc, (HGDIOBJ)  hbrushOld);
      ::SelectObject(hdc, (HGDIOBJ)  hpenOld);
      ::DeleteObject((HGDIOBJ) hbrush);
      size.cx += 4;
   }
   BSTR bstr = BSTRFromCStr(CP_UTF8, str.c_str());
   BOOL b = TextOutW(hdc, size.cx + x, y, bstr, wcslen(bstr));
   ::SysFreeString(bstr);
   return b;
}
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

void canvas_zero::prepare(HDC hdc, LPCRECT lpcrect)
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
   ::SelectObject(m_hdc, m_hbm);
   FillSolidRect(m_hdc, lpcrect, RGB(0, 0, 0));

   /*int iMid = (m_rect.bottom + m_rect.top) / 2;

   HPEN hpen = ::CreatePen(PS_SOLID, 1, RGB(0x7c, 0x7c, 0x7c));

   ::SelectObject(m_hdc, hpen);

   MoveToEx(m_hdc, m_rect.left, iMid, NULL);
   LineTo(m_hdc, m_rect.right, iMid);
   ::DeleteObject(hpen);*/

}

void canvas_zero::on_paint(HDC hdcPaint, LPCRECT lpcrect)
{
   HDC hdc = m_hdc;

   if(m_rect.left != lpcrect->left
   || m_rect.top != lpcrect->top
   || m_rect.right != lpcrect->right
   || m_rect.bottom != lpcrect->bottom)
   {
      prepare(hdc, lpcrect);
   }

   int cx = m_rect.right - m_rect.left;
   int cy = m_rect.bottom - m_rect.top;


   int iTimeFactor = 100; // greater faster

   if(::GetTickCount() >= m_dwNextZero)
   {
      POINT pt;
      pt.x = (rand() * cx / RAND_MAX) + lpcrect->left;
      pt.y = (rand() * cy / RAND_MAX) + lpcrect->top;
      m_pta.insert(m_pta.begin(), pt);
      m_ia.insert(m_ia.begin(), ((rand() * max(cx, cy) / 4 / RAND_MAX) + 23));
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
      iCurStep = max(2, min(iCurStep, iSize));
      zero(hdc, pt, iSize, iCurStep);
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

   ::BitBlt(hdcPaint, 0, 0, cx, cy, m_hdc, 0, 0, SRCCOPY);

   if(!bReady)
   {
      ::AlphaBlend(hdcPaint, pt.x - iSize / 2, pt.y - iSize / 2, iSize * 2 + 1, iSize * 2 + 1, m_hdcZero, 0, 0, iSize * 2 + 1, iSize * 2 + 1, bf);
      //::AlphaBlend(hdcPaint, 0, 0, iSize, iSize, m_hdcZero, 0, 0, iSize, iSize, bf);
   }

}


void canvas_zero::zero(HDC hdc, POINT pt, int iSize, int iStep)
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