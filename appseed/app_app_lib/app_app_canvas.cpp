#include "framework.h"


namespace app_app
{


   canvas::canvas(::aura::application * papp) :
      ::object(papp)
   {


   }


   canvas::~canvas()
   {


   }







   void canvas::paint_install(LPCRECT lpcrect, int iMode)
   {

      iMode = 1;

      bool bProgress = false;
      double dProgressTotal = 0.0;
      double dProgress = 0.0;
      string strHeader;
      string strBold;
      string strNormal;
      string strProgress;

      ::draw2d::graphics * pgraphics = m_pgraphics;

      pgraphics->set_alpha_mode(::draw2d::alpha_mode_blend);
      pgraphics->set_text_rendering(::draw2d::text_rendering_anti_alias_grid_fit);

      {

         string strPlatform = "x86";

         ::mutex m(NULL, false, "Global\\ca2-spa-install-" + strPlatform);

         synch_lock sl(&m);

         string strFile = dir::element() / ("install-" + strPlatform + ".log");

         int iTrace = _sopen(strFile, _O_RDONLY | _O_BINARY, _SH_DENYNO, 0);

         if (iTrace >= 0)
         {
            int iTell = _lseek(iTrace, 0, SEEK_END);
            iTell--;
            string strLine;
            int iSkip = 0;
            bool bNormal = false;
            bool bHeader = false;
            bool bBold = false;
            bool bPreNormal = true;
            bool bStart = false;
            while (iTell > 0 && !bStart && !(bNormal && bBold && bProgress && bHeader))
            {
               _lseek(iTrace, iTell, SEEK_SET);
               char ch;
               _read(iTrace, &ch, 1);
               if (ch == '\r')
               {
                  iSkip++;
               }
               else if (ch == '\n')
               {
                  iSkip++;
               }
               else if (iSkip > 0)
               {
                  iSkip = 0;
                  strLine.trim();
                  if (strLine == "--")
                  {
                     bStart = true;
                  }
                  else if (::str::begins_eat(strLine, "#----------"))
                  {
                     strHeader = L"Thank you";
                     bHeader = true;
                     strBold = u16(strLine);
                     bBold = true;
                     dProgress = 0.0;
                     bProgress = true;
                  }
                  else if (::str::begins_eat(strLine, ":::::"))
                  {
                     if (!bHeader && strLine.length() > 0 && bBold && bNormal && bPreNormal)
                     {
                        bHeader = true;
                        strHeader = u16(strLine);
                     }
                  }
                  else if (::str::begins_eat(strLine, "***"))
                  {
                     if (!bBold && strLine.length() > 0 && bNormal && bPreNormal)
                     {
                        bBold = true;
                        strBold = u16(strLine);
                     }
                  }
                  else if (::str::begins_eat(strLine, "|||"))
                  {
                     if (!bProgress && strLine.length() > 0)
                     {
                        bProgress = true;
                        long long int i = _atoi64(strLine.c_str());
                        dProgress = (double)i / 10000000.0;
                        //char sz[128];
                        //sprintf(sz,"%0.1f%%",dProgress);
                        //strProgress = u16(sz);
                        dProgress /= 100.0;
                     }
                  }
                  else if (!::str::begins_ci(strLine, "/ ") && strLine.length() > 0 && !bNormal && !bBold && !bHeader && bPreNormal)
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

      dProgressTotal += dProgress / 2.0;

      dProgress = 0.0;

      strHeader.Empty();
      strBold.Empty();
      strNormal.Empty();
      strProgress.Empty();

      {

         string strPlatform = "x64";

         ::mutex m(NULL, false, "Global\\ca2-spa-install-" + strPlatform);

         synch_lock sl(&m);

         string strFile = dir::element() / ("install-" + strPlatform + ".log");

         int iTrace = _sopen(strFile, _O_RDONLY | _O_BINARY, _SH_DENYNO, 0);

         if (iTrace >= 0)
         {
            int iTell = _lseek(iTrace, 0, SEEK_END);
            iTell--;
            string strLine;
            int iSkip = 0;
            bool bNormal = false;
            bool bHeader = false;
            bool bBold = false;
            bool bPreNormal = true;
            bool bStart = false;
            while (iTell > 0 && !bStart && !(bNormal && bBold && bProgress && bHeader))
            {
               _lseek(iTrace, iTell, SEEK_SET);
               char ch;
               _read(iTrace, &ch, 1);
               if (ch == '\r')
               {
                  iSkip++;
               }
               else if (ch == '\n')
               {
                  iSkip++;
               }
               else if (iSkip > 0)
               {
                  iSkip = 0;
                  strLine.trim();
                  if (strLine == "--")
                  {
                     bStart = true;
                  }
                  else if (::str::begins_eat(strLine, "#----------"))
                  {
                     strHeader = L"Thank you";
                     bHeader = true;
                     strBold = u16(strLine);
                     bBold = true;
                     dProgress = 0.0;
                     bProgress = true;
                  }
                  else if (::str::begins_eat(strLine, ":::::"))
                  {
                     if (!bHeader && strLine.length() > 0 && bBold && bNormal && bPreNormal)
                     {
                        bHeader = true;
                        strHeader = u16(strLine);
                     }
                  }
                  else if (::str::begins_eat(strLine, "***"))
                  {
                     if (!bBold && strLine.length() > 0 && bNormal && bPreNormal)
                     {
                        bBold = true;
                        strBold = u16(strLine);
                     }
                  }
                  else if (::str::begins_eat(strLine, "|||"))
                  {
                     if (!bProgress && strLine.length() > 0)
                     {
                        bProgress = true;
                        long long int i = _atoi64(strLine.c_str());
                        dProgress = (double)i / 10000000.0;
                        //char sz[128];
                        //sprintf(sz,"%0.1f%%",dProgress);
                        //strProgress = u16(sz);
                        dProgress /= 100.0;
                     }
                  }
                  else if (!::str::begins_ci(strLine, "/ ") && strLine.length() > 0 && !bNormal && !bBold && !bHeader && bPreNormal)
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

      dProgressTotal += dProgress / 2.0;

      dProgress = dProgressTotal;

      strHeader.Empty();
      strBold.Empty();
      strNormal.Empty();
      strProgress.Empty();


      RECT rect = *lpcrect;
      int cx = lpcrect->right - lpcrect->left;
      int cy = lpcrect->bottom - lpcrect->top;
      iMode = iMode % 5;

      if (iMode == 2 || iMode == 1 || iMode == 0)
      {

         pgraphics->DrawRect(*lpcrect, m_ppenBorder);

      }

      pgraphics->SelectFont(m_pfont);

      ::size s = pgraphics->GetTextExtent(L"CCpp");

      double cyText = MAX(s.cy, 5.0);

      int iLineCount = (rect.bottom - 30) / cyText;

      // !m_bHealingSurface => "Surgery Internals"
      {

         size_t iRefresh = 884;
         size_t iEat = 8;
         //      const wchar_t * psz = L"development message so international english file \"install.log\" excerpt  ::::::::";
         //    pgraphics->DrawString(psz,wcslen(psz) - iEat + 1 + ((::GetTickCount() / (iRefresh - 277) % iEat)),g_pfont,PointF(10,10 + cyText * 2),StringFormat::GenericTypographic(),g_ptextColor1);

         if (strHeader.length() > 0)
         {
            pgraphics->SelectFont(m_pfontHeader);
            pgraphics->SelectObject(m_ptextColor1);
            pgraphics->TextOut(10, 10+cyText * 3 + 4, strHeader);
         }
         if (strBold.length() > 0)
         {
            pgraphics->SelectFont(m_pfontBold);
            pgraphics->SelectObject(m_ptextColor1);
            pgraphics->TextOut(10, 10 + cyText * 5, strBold);
         }
         if (strNormal.length() > 0)
         {
            pgraphics->SelectFont(m_pfont);
            pgraphics->SelectObject(m_ptextColor1);
            pgraphics->TextOut(10, 10 + cyText * 6, strNormal);
         }
         //if(strProgress.length() > 0)
         //{
         //   pgraphics->DrawString(u16(strProgress),-1,m_pfont,PointF(10,10 + cyText * 7),StringFormat::GenericTypographic(),m_ptextColor1);
         //}

      }
      double cyBar = cyText * 1.2;

      {
         pgraphics->FillRectangle(m_pBarBk, RectF(10.0, (lpcrect->top + lpcrect->bottom - cyBar) / 2.0, lpcrect->right - 10.0 - 10.0, cyBar));

         if (bProgress)
         {
            double iRight = ((double)m_pcanvas->m_cx - 11.0 - 11.0) * dProgress;
            pgraphics->FillRectangle(m_pBar, RectF(11.0, (lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0, iRight, cyBar - 2.0));
         }
         else
         {
            double dPeriod = 2000.0;
            dProgress = fmod((double)GetTickCount(), dPeriod) / dPeriod;
            double iBarWidth = (lpcrect->right - 11.0 - 11.0) / 4;
            double i = ((lpcrect->right - 11.0 - 11.0) * dProgress) + 11.0;
            double iRight = i + iBarWidth;
            pgraphics->FillRectangle(m_pBar, RectF(11.0 + i, (lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0, MIN(lpcrect->right - 10.0, iRight) - 11 - i, cyBar - 2.0));
            if (iRight >= lpcrect->right - 10)
            {
               pgraphics->FillRectangle(m_pBar, RectF(11.0, (lpcrect->top + lpcrect->bottom - cyBar) / 2.0 + 1.0, iRight - lpcrect->right - 10.0 - 11.0, cyBar - 2.0));
            }
         }
         pgraphics->DrawRectangle(m_pBarBorder, RectF(10.0, (lpcrect->top + lpcrect->bottom - cyBar) / 2.0, lpcrect->right - 10.0 - 10.0, cyBar));

      }


      {

         pgraphics->DrawString(L"Thank you", -1, m_pfont, PointF(10.0, 10.0), m_ptextColor1);

      }

   }



   bool canvas::initialize(int cx, int cy)
   {


      BITMAPINFO info = {};
      info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      info.bmiHeader.biWidth = m_pcanvas->m_cx;
      info.bmiHeader.biHeight = -m_pcanvas->m_cy;
      info.bmiHeader.biPlanes = 1;
      info.bmiHeader.biBitCount = 32;
      info.bmiHeader.biCompression = BI_RGB;
      //info.bmiHeader.biSizeImage=g_cx*g_cy * 4;
      LPDWORD lpdata;
      //g_pcolorref = new COLORREF[g_cx * g_cy * sizeof(COLORREF)];
      int iScan;
      m_pcanvas->m_hbitmap = CreateDIBSection(NULL, &info, DIB_RGB_COLORS, (void **)&m_pcanvas->m_pcolorref, NULL, NULL);
      m_pcanvas->m_hdc = ::CreateCompatibleDC(NULL);
      ::SelectObject(m_pcanvas->m_hdc, m_pcanvas->m_hbitmap);
      memset(m_pcanvas->m_pcolorref, 0, m_pcanvas->m_cx*m_pcanvas->m_cy * sizeof(COLORREF));

      m_pbitmap = new Bitmap(m_pcanvas->m_cx, m_pcanvas->m_cy, m_pcanvas->m_cx * sizeof(COLORREF), PixelFormat32bppARGB, (BYTE *)m_pcanvas->m_pcolorref);
      m_pgraphics = new Graphics(m_pbitmap);
      m_pfont = ::CreatePointFont(10.0, L"Lucida Sans Unicode", false, false);
      m_pfontBold = ::CreatePointFont(10.0, L"Lucida Sans Unicode", false, true);
      m_pfontHeader = ::CreatePointFont(8.4 + 7.7, L"Lucida Sans Unicode", false, true);
      m_ppenBorder = new Pen(Gdiplus::Color(84, 84, 84, 77), 1.0f);
      m_ptextColor1 = new SolidBrush(Color(184, 84 + 49 + 49 + 49, 84 + 49 + 49 + 49, 77 + 49 + 49 + 49));
      m_pBarBorder = new Pen(Color(84, 84 + 49 + 49 + 49, 84 + 49 + 49 + 49, 77 + 49 + 49 + 49), 1.0f);
      m_pBar = new SolidBrush(Color(184, 77, 184, 84));
      m_pBarBk = new SolidBrush(Color(49, 184 + 23, 184 + 23, 184 + 23));

      return true;

   }



} // namespace app_app



