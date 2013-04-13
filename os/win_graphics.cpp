#include "framework.h"
#include <math.h>


#undef new


namespace win
{


   graphics::graphics(sp(::ca::application) papp) :
      ca(papp)
   {

      m_bPrinting       = FALSE;
      m_pdibAlphaBlend  = ::null();
      m_pgraphics       = ::null();
      m_hdc             = ::null();
      m_hdcGraphics     = ::null();
      m_ppath           = ::null();
      m_ppathPaint      = ::null();
      m_etextrendering  = ::ca::text_rendering_anti_alias_grid_fit;
      m_dFontFactor     = 1.0;

   }

   graphics::graphics()
   {

      m_bPrinting       = FALSE;
      m_pgraphics       = ::null();
      m_pgraphics       = ::null();
      m_hdc             = ::null();
      m_ppath           = ::null();
      m_ppathPaint      = ::null();
      m_etextrendering  = ::ca::text_rendering_anti_alias_grid_fit;
;

   }


   void graphics::assert_valid() const
   {
      ::ca::object::assert_valid();
   }

   void graphics::dump(dump_context & dumpcontext) const
   {
      ::ca::object::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << get_handle1();
      dumpcontext << "\nm_hAttribDC = " << get_handle2();
      dumpcontext << "\nm_bPrinting = " << m_bPrinting;

      dumpcontext << "\n";
   }


   graphics::~graphics()
   {
   
      DeleteDC();
   
   }


   sp(::ca::window) graphics::GetWindow() const
   { 
      ASSERT(get_handle1() != ::null()); return ::win::window::from_handle(::WindowFromDC(get_handle1())); 
   }
   
   bool graphics::IsPrinting() const
   {
      return m_bPrinting; 
   }
   
   bool graphics::CreateDC(const char * lpszDriverName,
      const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   {
      return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData)); 
   }
   
   bool graphics::CreateIC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   { 
      return Attach(::CreateIC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*) lpInitData)); 
   }


   bool graphics::CreateCompatibleDC(::ca::graphics * pgraphics)
   { 

      HDC hdc = Detach();
      
      if(hdc != ::null())
      {

         bool bDeleted = ::DeleteDC(hdc) != FALSE;

         if(!bDeleted)
         {
         
            TRACE("Failed to delete GDI device context");

         }

      }

      if(m_hdcGraphics != ::null())
      {
         
         m_pgraphics->ReleaseHDC(m_hdcGraphics);

         m_hdcGraphics = ::null();

      }

      if(m_pgraphics != ::null())
      {

         try
         {
            
            delete m_pgraphics;

         }
         catch(...)
         {
            
            TRACE("graphics::CreateCompatibleDC(::ca::graphics * ) : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = ::null();

      }

      if(pgraphics == ::null())
      {

         hdc = ::CreateCompatibleDC(::null());

      }
      else if(WIN_DC(pgraphics)->m_hdc != ::null())
      {
         
         hdc = ::CreateCompatibleDC(WIN_DC(pgraphics)->m_hdc);

      }
      else if(WIN_DC(pgraphics)->m_hdcGraphics != ::null())
      {
         
         hdc = ::CreateCompatibleDC(WIN_DC(pgraphics)->m_hdcGraphics);

      }
      else
      {

         HDC hdcTemplate = ((Gdiplus::Graphics *) pgraphics->get_os_data())->GetHDC();

         hdc = ::CreateCompatibleDC(hdcTemplate);

         ((Gdiplus::Graphics *) pgraphics->get_os_data())->ReleaseHDC(hdc);

      }

      if(hdc == ::null())
         return false;

      m_hdc = hdc;

      m_pgraphics = new Gdiplus::Graphics(m_hdc);

      return true;

   }


   int32_t graphics::ExcludeUpdateRgn(sp(::ca::window) pWnd)
   { 
      ASSERT(get_handle1() != ::null()); 
      return ::ExcludeUpdateRgn(get_handle1(), WIN_WINDOW(pWnd)->get_handle()); 
   }

   int32_t graphics::GetDeviceCaps(int32_t nIndex) const
   { 
      ASSERT(get_handle2() != ::null()); 
      return ::GetDeviceCaps(get_handle2(), nIndex); 
   }

   point graphics::GetBrushOrg() const
   {
      ASSERT(get_handle1() != ::null());
      POINT point;
      VERIFY(::GetBrushOrgEx(get_handle1(), &point));
      return point;
   }
   
   point graphics::SetBrushOrg(int32_t x, int32_t y)
   {
      ASSERT(get_handle1() != ::null());
      POINT point;
      VERIFY(::SetBrushOrgEx(get_handle1(), x, y, &point));
      return point;
   }
   
   point graphics::SetBrushOrg(POINT point)
   {
      ASSERT(get_handle1() != ::null());
      VERIFY(::SetBrushOrgEx(get_handle1(), point.x, point.y, &point));
      return point;
   }

   int32_t graphics::EnumObjects(int32_t nObjectType, int32_t (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   {
      ASSERT(get_handle2() != ::null());
      return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData); 
   }

   ::ca::bitmap* graphics::SelectObject(::ca::bitmap * pBitmap)
   { 

      
      if(pBitmap == ::null())
         return ::null();

/*      if(get_handle1() == ::null())
         return ::null();
      if(pBitmap == ::null())
         return ::null();
      return dynamic_cast < ::ca::bitmap* > (SelectGdiObject(get_app(), get_handle1(), pBitmap->get_os_data()));*/
      if(m_pgraphics != ::null())
      {
         try
         {
   
            delete m_pgraphics;

         }
         catch(...)
         {

            TRACE("graphics::SelectObject(::ca::bitmap *) : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = ::null();

      }

      m_pgraphics = new Gdiplus::Graphics((Gdiplus::Bitmap *) pBitmap->get_os_data());

      set_text_rendering(::ca::text_rendering_anti_alias_grid_fit);

      m_bitmap = pBitmap;

      return m_bitmap;
   }


   ::ca::graphics_object* graphics::SelectObject(::ca::graphics_object* pObject)
   {
   /*      ASSERT(get_handle1() != ::null()); 
      if(pObject == ::null())
         return ::null();
      return SelectGdiObject(get_app(), get_handle1(), pObject->get_os_data()); */
      return ::null();
   }

   HGDIOBJ graphics::SelectObject(HGDIOBJ hObject) // Safe for ::null() handles
   {

      UINT uiType = GetObjectType(hObject);

      if(uiType == OBJ_BITMAP)
      {
         
         HBITMAP hbitmap = (HBITMAP) hObject;

         if(m_bitmap.is_null())
            m_bitmap.create(allocer());

         if(m_bitmap.is_null())
            return ::null();

         (dynamic_cast < ::win::bitmap * > (m_bitmap.m_p))->m_pbitmap = new Gdiplus::Bitmap(hbitmap, ::null());

         if(m_pgraphics != ::null())
         {
            
            try
            {
               
               delete m_pgraphics;

            }
            catch(...)
            {

               TRACE("graphics::SelectObject(HGDIOBJ) OBJ_BITMAP : Failed to delete Gdiplus::Graphics");

            }

            m_pgraphics = ::null();

         }

         m_pgraphics = new Gdiplus::Graphics((Gdiplus::Bitmap *) m_bitmap->get_os_data());

         set_text_rendering(::ca::text_rendering_anti_alias_grid_fit);

         return hbitmap;

      }

      //*ASSERT(get_handle1() == get_handle2()); // ASSERT a simple graphics object
      //return (hObject != ::null()) ? ::SelectObject(get_handle1(), hObject) : ::null(); */
      return ::null();
   }

   COLORREF graphics::GetNearestColor(COLORREF crColor) const
   { 
      return ::GetNearestColor(get_handle2(), crColor); 
   }

   UINT graphics::RealizePalette()
   { 
      return ::RealizePalette(get_handle1()); 
   }
   
   void graphics::UpdateColors()
   { 
      ::UpdateColors(get_handle1()); 
   }
   
   COLORREF graphics::GetBkColor() const
   { 
      return ::GetBkColor(get_handle2()); 
   }
   
   int32_t graphics::GetBkMode() const
   { 
      return ::GetBkMode(get_handle2()); 
   }
   
   int32_t graphics::GetPolyFillMode() const
   { 
      return ::GetPolyFillMode(get_handle2()); 
   }
   
   int32_t graphics::GetROP2() const
   { 
      return ::GetROP2(get_handle2()); 
   }
   
   int32_t graphics::GetStretchBltMode() const
   { 
      return ::GetStretchBltMode(get_handle2()); 
   }
   
   COLORREF graphics::GetTextColor() const
   { 
      return ::GetTextColor(get_handle2()); 
   }
   
   int32_t graphics::GetMapMode() const
   { 
      return ::GetMapMode(get_handle2()); 
   }
   
   int32_t graphics::GetGraphicsMode() const
   { 
      return ::GetGraphicsMode(get_handle2()); 
   }
   
   bool graphics::GetWorldTransform(XFORM* pXform) const
   { 

      return ::GetWorldTransform(get_handle2(),pXform) != FALSE; 

   }

   size graphics::GetViewportExt() const
   {
      SIZE size;
      ::GetViewportExtEx(get_handle2(), &size);
      return size;
   }

   point graphics::GetWindowOrg() const
   {
      POINT point;
      ::GetWindowOrgEx(get_handle2(), &point);
      return point;
   }

   size graphics::GetWindowExt() const
   {
      SIZE size;
      ::GetWindowExtEx(get_handle2(), &size);
      return size;
   }

   // non-virtual helpers calling virtual mapping functions
   point graphics::SetViewportOrg(POINT point)
   { 
      return SetViewportOrg(point.x, point.y); 
   }

   size graphics::SetViewportExt(SIZE size)
   {
      return SetViewportExt(size.cx, size.cy); 
   }

   point graphics::SetWindowOrg(POINT point)
   {
      return SetWindowOrg(point.x, point.y); 
   }

   size graphics::SetWindowExt(SIZE size)
   {
      return SetWindowExt(size.cx, size.cy); 
   }

   void graphics::DPtoLP(LPPOINT lpPoints, int32_t nCount) const
   { 
      ::DPtoLP(get_handle2(), lpPoints, nCount); 
   }

   void graphics::DPtoLP(LPRECT lpRect) const
   { 
      ::DPtoLP(get_handle2(), (LPPOINT)lpRect, 2); 
   }

   void graphics::LPtoDP(LPPOINT lpPoints, int32_t nCount) const
   {
      ::LPtoDP(get_handle2(), lpPoints, nCount); 
   }

   void graphics::LPtoDP(LPRECT lpRect) const
   {
      ::LPtoDP(get_handle2(), (LPPOINT)lpRect, 2);
   }

   bool graphics::FillRgn(::ca::region* pRgn, ::ca::brush* pBrush)
   { 

      return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != FALSE;

   }

   bool graphics::FrameRgn(::ca::region* pRgn, ::ca::brush* pBrush, int32_t nWidth, int32_t nHeight)
   { 

      return ::FrameRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight) != FALSE; 

   }

   bool graphics::InvertRgn(::ca::region* pRgn)
   {
      
      ASSERT(get_handle1() != ::null());
      
      return ::InvertRgn(get_handle1(), (HRGN)pRgn->get_os_data()) != FALSE;
   
   }

   bool graphics::PaintRgn(::ca::region* pRgn)
   {
      
      ASSERT(get_handle1() != ::null());
      
      return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data())  != FALSE;
   
   }

   bool graphics::PtVisible(int32_t x, int32_t y) const
   {
      
      ASSERT(get_handle1() != ::null()); 

      return ::PtVisible(get_handle1(), x, y) != FALSE;

   }

   bool graphics::PtVisible(POINT point) const
   { ASSERT(get_handle1() != ::null()); return PtVisible(point.x, point.y); } // call virtual

   bool graphics::RectVisible(LPCRECT lpRect) const
   {
      
      ASSERT(get_handle1() != ::null());
      
      return ::RectVisible(get_handle1(), lpRect) != FALSE;
   
   }

   point graphics::GetCurrentPosition() const
   {
      ASSERT(get_handle2() != ::null());
      POINT point;
      VERIFY(::GetCurrentPositionEx(get_handle2(), &point));
      return point;
   }

   bool graphics::Arc(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4)
   { 

      ::Gdiplus::RectF rectf((Gdiplus::REAL) x1, (Gdiplus::REAL) y1, (Gdiplus::REAL) x2, (Gdiplus::REAL) y2);

      double centerx    = (x2 + x1) / 2.0;
      double centery    = (y2 + y1) / 2.0;
      
      double start      = atan2(y3 - centery, x3 - centerx);
      double end        = atan2(y4 - centery, x4 - centerx);

   
      return m_pgraphics->DrawArc(gdiplus_pen(), rectf, (Gdiplus::REAL) start, (Gdiplus::REAL) end) == Gdiplus::Status::Ok;
      
   }

   bool graphics::Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   {
   
      ASSERT(get_handle1() != ::null());
      
      return ::Arc(get_handle1(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y)  != FALSE; 
   
   }

   bool graphics::Polyline(const POINT* lpPoints, int32_t nCount)
   {
   
      ASSERT(get_handle1() != ::null());
      
      return ::Polyline(get_handle1(), lpPoints, nCount) != FALSE;
   
   }

   void graphics::FillRect(LPCRECT lpRect, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != ::null()); ::FillRect(get_handle1(), lpRect, (HBRUSH)pBrush->get_os_data()); }
   void graphics::FrameRect(LPCRECT lpRect, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != ::null()); ::FrameRect(get_handle1(), lpRect, (HBRUSH)pBrush->get_os_data()); }
   void graphics::InvertRect(LPCRECT lpRect)
   { ASSERT(get_handle1() != ::null()); ::InvertRect(get_handle1(), lpRect); }

   bool graphics::DrawIcon(int32_t x, int32_t y, ::visual::icon * picon)
   {
      
      ASSERT(get_handle1() != ::null()); 
      
      if(picon == ::null())
         return false;

      return ::DrawIcon(get_handle1(), x, y, (HICON) picon->m_picon) != FALSE; 

   }

   bool graphics::DrawIcon(POINT point, ::visual::icon * picon)
   { 
      
      ASSERT(get_handle1() != ::null()); 
      
      if(picon == ::null())
         return false;

      return ::DrawIcon(get_handle1(), point.x, point.y, (HICON) picon->m_picon) != FALSE; 

   }

   bool graphics::DrawIcon(int32_t x, int32_t y, ::visual::icon * picon, int32_t cx, int32_t cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
   { 

      try
      {
      
         if(picon == ::null())
            return FALSE;

         if(m_pgraphics == ::null())
            return FALSE;

         bool bOk = FALSE;

         BITMAPINFO info;
         COLORREF * pcolorref;

         ZeroMemory(&info, sizeof (BITMAPINFO));

         info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
         info.bmiHeader.biWidth         = cx;
         info.bmiHeader.biHeight        = - cy;
         info.bmiHeader.biPlanes        = 1;
         info.bmiHeader.biBitCount      = 32; 
         info.bmiHeader.biCompression   = BI_RGB;
         info.bmiHeader.biSizeImage     = cx * cy * 4;

         HBITMAP hbitmap = ::CreateDIBSection(::null(), &info, DIB_RGB_COLORS, (void **) &pcolorref, ::null(), 0);

         HDC hdc = ::CreateCompatibleDC(::null());

         HBITMAP hbitmapOld = (HBITMAP) ::SelectObject(hdc, hbitmap);

         if(::DrawIconEx(hdc, 0, 0, (HICON) picon->m_picon, cx, cy, istepIfAniCur, ::null(), DI_IMAGE | DI_MASK))
         {

            ::SelectObject(hdc, hbitmapOld);

            try
            {

               Gdiplus::Bitmap b(cx, cy, cx * 4 , PixelFormat32bppARGB, (BYTE *) pcolorref);

               bOk = m_pgraphics->DrawImage(&b, x, y, 0, 0, cx, cy, Gdiplus::UnitPixel) == Gdiplus::Ok;

            }
            catch(...)
            {
            }

         }

         ::DeleteDC(hdc);

         ::DeleteObject(hbitmap);

         return bOk;

      }
      catch(...)
      {
         return FALSE;
      }

      //return ::DrawIconEx(get_handle1(), x, y, picon->m_hicon, cx, cy, istepIfAniCur, hbrFlickerFreeDraw, diFlags); 

   }

   bool graphics::DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush)
   { 
      
      ASSERT(get_handle1() != ::null()); 
      return ::DrawState(get_handle1(), hBrush, ::null(), (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;
   
   }
   
   bool graphics::DrawState(point pt, size size, ::ca::bitmap* pBitmap, UINT nFlags, ::ca::brush* pBrush)
   {
      
      ASSERT(get_handle1() != ::null());
      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), ::null(), (LPARAM)pBitmap->get_os_data(), 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;
   
   }
   
   bool graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, HBRUSH hBrush)
   {
      
      ASSERT(get_handle1() != ::null()); 
      return ::DrawState(get_handle1(), hBrush, ::null(), (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE; 

   }
   

   bool graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, ::ca::brush* pBrush)
   {

      ASSERT(get_handle1() != ::null());
      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), ::null(), (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE;
   
   }
   
   bool graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, bool bPrefixText, int32_t nTextLen, HBRUSH hBrush)
   {
   
      ASSERT(get_handle1() != ::null()); 
      return ::DrawState(get_handle1(), hBrush,  ::null(), (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }
   
   bool graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, bool bPrefixText, int32_t nTextLen, ::ca::brush* pBrush)
   {

      ASSERT(get_handle1() != ::null()); 
      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), ::null(), (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush)
   { ASSERT(get_handle1() != ::null()); return ::DrawState(get_handle1(), hBrush,
   lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE; }
   bool graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, ::ca::brush* pBrush)
   { ASSERT(get_handle1() != ::null()); return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
   lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE; }
   bool graphics::DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags)
   { ASSERT(get_handle1() != ::null()); return ::DrawEdge(get_handle1(), lpRect, nEdge, nFlags) != FALSE; }
   bool graphics::DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState)
   { ASSERT(get_handle1() != ::null()); return ::DrawFrameControl(get_handle1(), lpRect, nType, nState) != FALSE; }

   bool graphics::Chord(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
      int32_t x4, int32_t y4)
   { ASSERT(get_handle1() != ::null()); return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE; }
   bool graphics::Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { ASSERT(get_handle1() != ::null()); return ::Chord(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
   ptEnd.x, ptEnd.y) != FALSE; }
   void graphics::DrawFocusRect(LPCRECT lpRect)
   { ASSERT(get_handle1() != ::null()); ::DrawFocusRect(get_handle1(), lpRect); }
   
   bool graphics::DrawEllipse(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      
      return (m_pgraphics->DrawEllipse(gdiplus_pen(), x1, y1, x2 - x1, y2 - y1)) == Gdiplus::Status::Ok;

   }

   bool graphics::DrawEllipse(LPCRECT lpRect)
   { 
   
      /*return ::Ellipse(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom); */

      return (m_pgraphics->DrawEllipse(gdiplus_pen(), lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top)) == Gdiplus::Status::Ok;
   
   }

   bool graphics::FillEllipse(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      
      return (m_pgraphics->FillEllipse(gdiplus_brush(), x1, y1, x2 - x1, y2 - y1)) == Gdiplus::Status::Ok;

   }

   bool graphics::FillEllipse(LPCRECT lpRect)
   { 
   
      /*return ::Ellipse(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom); */

      return (m_pgraphics->FillEllipse(gdiplus_brush(), lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top)) == Gdiplus::Status::Ok;
   
   }

   bool graphics::Pie(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4)
   { ASSERT(get_handle1() != ::null()); return ::Pie(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE; }
   bool graphics::Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { ASSERT(get_handle1() != ::null()); return ::Pie(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
   ptEnd.x, ptEnd.y) != FALSE; }

   bool graphics::fill_polygon(const POINTD * lpPoints, int32_t nCount)
   {

      if(nCount <= 0)
         return TRUE;

      bool bOk1 = FALSE;

      Gdiplus::Point * ppoints = new Gdiplus::Point[nCount];

      try
      {

         for(int32_t i = 0; i < nCount; i++)
         {
            ppoints[i].X = (INT) lpPoints[i].x;
            ppoints[i].Y = (INT) lpPoints[i].y;
         }
   
         bOk1 = m_pgraphics->FillPolygon(gdiplus_brush(), ppoints, nCount, gdiplus_get_fill_mode()) == Gdiplus::Status::Ok;

      }
      catch(...)
      {
      }
      
      try
      {
         delete ppoints;         
      }
      catch(...)
      {
      }


      return bOk1;
   }

   bool graphics::fill_polygon(const POINT* lpPoints, int32_t nCount)
   {

      if(nCount <= 0)
         return TRUE;

      bool bOk1 = FALSE;

      Gdiplus::Point * ppoints = new Gdiplus::Point[nCount];

      try
      {

         for(int32_t i = 0; i < nCount; i++)
         {
            ppoints[i].X = lpPoints[i].x;
            ppoints[i].Y = lpPoints[i].y;
         }
   
         bOk1 = m_pgraphics->FillPolygon(gdiplus_brush(), ppoints, nCount, gdiplus_get_fill_mode()) == Gdiplus::Status::Ok;

      }
      catch(...)
      {
      }
      
      try
      {
         delete ppoints;         
      }
      catch(...)
      {
      }


      return bOk1;
   }


   bool graphics::Polygon(const POINT* lpPoints, int32_t nCount)
   {

      if(nCount <= 0)
         return TRUE;

      bool bOk1 = FALSE;

      bool bOk2 = FALSE;

      Gdiplus::Point * ppoints = new Gdiplus::Point[nCount];

      try
      {

         for(int32_t i = 0; i < nCount; i++)
         {
            ppoints[i].X = lpPoints[i].x;
            ppoints[i].Y = lpPoints[i].y;
         }
   
         bOk1 = m_pgraphics->FillPolygon(gdiplus_brush(), ppoints, nCount, gdiplus_get_fill_mode()) == Gdiplus::Status::Ok;

         bOk2 = m_pgraphics->DrawPolygon(gdiplus_pen(), ppoints, nCount) == Gdiplus::Status::Ok;

      }
      catch(...)
      {
      }
      
      try
      {
         delete ppoints;         
      }
      catch(...)
      {
      }


      return bOk1 && bOk2;
   
   }
   bool graphics::PolyPolygon(const POINT* lpPoints, const INT* lpPolyCounts, int32_t nCount)
   { ASSERT(get_handle1() != ::null()); return ::PolyPolygon(get_handle1(), lpPoints, lpPolyCounts, nCount) != FALSE; }
   
   bool graphics::Rectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   { 
      
      Gdiplus::RectF rectf((Gdiplus::REAL) x1, (Gdiplus::REAL) y1, (Gdiplus::REAL) (x2 - x1), (Gdiplus::REAL) (y2 - y1));

      bool bOk1 = m_pgraphics->FillRectangle(gdiplus_brush(), rectf) == Gdiplus::Status::Ok;

      bool bOk2 = m_pgraphics->DrawRectangle(gdiplus_pen(), rectf) == Gdiplus::Status::Ok;

      return bOk1 && bOk2;

   }
   
   bool graphics::Rectangle(LPCRECT lpRect)
   { 

      return Rectangle(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);

   }

   bool graphics::DrawRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   { 
      
      Gdiplus::RectF rectf((Gdiplus::REAL) x1, (Gdiplus::REAL) y1, (Gdiplus::REAL) (x2 - x1), (Gdiplus::REAL) (y2 - y1));

      bool bOk = m_pgraphics->DrawRectangle(gdiplus_pen(), rectf) == Gdiplus::Status::Ok;

      return bOk;

   }
   
   bool graphics::DrawRectangle(LPCRECT lpRect)
   { 

      return DrawRectangle(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);

   }

   bool graphics::FillRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   { 
      
      Gdiplus::RectF rectf((Gdiplus::REAL) x1, (Gdiplus::REAL) y1, (Gdiplus::REAL) (x2 - x1), (Gdiplus::REAL) (y2 - y1));

      bool bOk = m_pgraphics->FillRectangle(gdiplus_brush(), rectf) == Gdiplus::Status::Ok;

      return bOk;

   }
   
   bool graphics::FillRectangle(LPCRECT lpRect)
   { 

      return FillRectangle(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);

   }

   bool graphics::RoundRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
   { ASSERT(get_handle1() != ::null()); return ::RoundRect(get_handle1(), x1, y1, x2, y2, x3, y3) != FALSE; }
   bool graphics::RoundRect(LPCRECT lpRect, POINT point)
   { ASSERT(get_handle1() != ::null()); return ::RoundRect(get_handle1(), lpRect->left, lpRect->top,
   lpRect->right, lpRect->bottom, point.x, point.y) != FALSE; }
   bool graphics::PatBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, uint32_t dwRop)
   { ASSERT(get_handle1() != ::null()); return ::PatBlt(get_handle1(), x, y, nWidth, nHeight, dwRop) != FALSE; }
   
   
   bool graphics::BitBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::ca::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, uint32_t dwRop)
   { 
      

      if(m_pdibAlphaBlend != ::null())
      {


         rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());


         ::ca::dib * pdibWork = ::null();
         ::ca::dib * pdibWork2 = ::null();
//         ::ca::dib * pdibWork3 = ::null();
         ::ca::dib * pdibWork4 = ::null();

            
         class point ptSrc(xSrc, ySrc);
         class point ptDest(x, y);
         class size size(nWidth, nHeight);



         ::ca::dib_sp spdib;
         if(pdibWork == ::null())
         {
            spdib.create(allocer());
            pdibWork = spdib;
         }
         if(pdibWork == ::null())
            return false;
         if(!pdibWork->create(size))
            return false;

         pdibWork->Fill(0, 0, 0, 0);

         pdibWork->get_graphics()->set_alpha_mode(::ca::alpha_mode_set);

         if(!pdibWork->from(null_point(), pgraphicsSrc, ptSrc, size))
            return false;




         ::ca::dib_sp spdib2;
         if(pdibWork2 == ::null())
         {
            spdib2.create(allocer());
            pdibWork2 = spdib2;
         }


         ::ca::dib_sp spdib4;
         if(pdibWork4 == ::null())
         {
            spdib4.create(allocer());
            pdibWork4 = spdib4;
         }
         if(pdibWork4 == ::null())
            return false;
         if(!pdibWork4->create(size))
            return false;


         pdibWork4->Fill(255, 0, 0, 0);

         pdibWork4->get_graphics()->set_alpha_mode(::ca::alpha_mode_set);
            
         pdibWork4->from(point(max(0, m_ptAlphaBlend.x - x), max(0, m_ptAlphaBlend.y - y)),
            m_pdibAlphaBlend->get_graphics(), point(max(0, x - m_ptAlphaBlend.x), max(0, y - m_ptAlphaBlend.y)), 
                               class size(max(0, m_pdibAlphaBlend->cx - max(0, x - m_ptAlphaBlend.x)), max(0, m_pdibAlphaBlend->cy - max(0, y - m_ptAlphaBlend.y))));
   
         pdibWork->channel_multiply(visual::rgba::channel_alpha, pdibWork4);

         /*pdibWork->get_graphics()->set_alpha_mode(::ca::alpha_mode_blend);

         pdibWork->from(point(max(0, m_ptAlphaBlend.x - x), max(0, m_ptAlphaBlend.y - y)),
            m_pdibAlphaBlend->get_graphics(), point(max(0, x - m_ptAlphaBlend.x), max(0, y - m_ptAlphaBlend.y)), 
                               class size(max(0, size.cx - max(0, x - m_ptAlphaBlend.x)), max(0, size.cy - max(0, y - m_ptAlphaBlend.y))));*/
   
         //keeper < ::ca::dib * > keep(&m_pdibAlphaBlend, ::null(), m_pdibAlphaBlend, true);

         Gdiplus::CompositingMode mode = m_pgraphics->GetCompositingMode();

         //m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);

         bool bOk = m_pgraphics->DrawImage(
            (Gdiplus::Bitmap *) pdibWork->get_graphics()->GetCurrentBitmap().get_os_data(),
            x, y , 0, 0, nWidth, nHeight, Gdiplus::UnitPixel) == Gdiplus::Status::Ok;


         m_pgraphics->SetCompositingMode(mode);

         return bOk;

         //return System.visual().imaging().true_blend(this, ptDest, size, pdibWork->get_graphics(), null_point()); 


      }


      try
      {

         if(pgraphicsSrc == ::null())
            return FALSE;

         if(&pgraphicsSrc->GetCurrentBitmap() == ::null())
            goto gdi_fallback;

         if(pgraphicsSrc->GetCurrentBitmap().get_os_data() == ::null())
            goto gdi_fallback;


         return m_pgraphics->DrawImage(
            (Gdiplus::Bitmap *) pgraphicsSrc->GetCurrentBitmap().get_os_data(),
            x, y , xSrc, ySrc, nWidth, nHeight, Gdiplus::UnitPixel) == Gdiplus::Status::Ok;

      }
      catch(...)
      {
         return FALSE;
      }

      //return ::BitBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, dwRop); 

gdi_fallback:
      
      HDC hdcDst = get_hdc();

      if(hdcDst == ::null())
         return false;

      HDC hdcSrc = WIN_DC(pgraphicsSrc)->get_hdc();

      if(hdcSrc == ::null())
      {

         release_hdc(hdcDst);

         return false;

      }

      bool bOk = ::BitBlt(hdcDst, x, y, nWidth, nHeight, hdcSrc, x, y, dwRop) != FALSE;

      WIN_DC(pgraphicsSrc)->release_hdc(hdcSrc);

      release_hdc(hdcDst);

      return bOk;

   }


   bool graphics::StretchBlt(int32_t xDst, int32_t yDst, int32_t nDstWidth, int32_t nDstHeight, ::ca::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, uint32_t dwRop)
   { 

      if(pgraphicsSrc == ::null())
         return FALSE;

      Gdiplus::RectF dstRect((Gdiplus::REAL) xDst, (Gdiplus::REAL) yDst, (Gdiplus::REAL) nDstWidth, (Gdiplus::REAL) nDstHeight);

      Gdiplus::RectF srcRect((Gdiplus::REAL) xSrc, (Gdiplus::REAL) ySrc, (Gdiplus::REAL) nSrcWidth, (Gdiplus::REAL) nSrcHeight);

      try
      {
         return m_pgraphics->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->GetCurrentBitmap().get_os_data(),  dstRect, srcRect, Gdiplus::UnitPixel) == Gdiplus::Status::Ok;
      }
      catch(...)
      {
         
      }

      return FALSE;

      //return ::StretchBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop); 
   
   }


   COLORREF graphics::GetPixel(int32_t x, int32_t y) const
   { ASSERT(get_handle1() != ::null()); return ::GetPixel(get_handle1(), x, y); }
   COLORREF graphics::GetPixel(POINT point) const
   { ASSERT(get_handle1() != ::null()); return ::GetPixel(get_handle1(), point.x, point.y); }
   COLORREF graphics::SetPixel(int32_t x, int32_t y, COLORREF crColor)
   { ASSERT(get_handle1() != ::null()); return ::SetPixel(get_handle1(), x, y, crColor); }
   COLORREF graphics::SetPixel(POINT point, COLORREF crColor)
   { ASSERT(get_handle1() != ::null()); return ::SetPixel(get_handle1(), point.x, point.y, crColor); }
   bool graphics::FloodFill(int32_t x, int32_t y, COLORREF crColor)
   { ASSERT(get_handle1() != ::null()); return ::FloodFill(get_handle1(), x, y, crColor) != FALSE; }
   bool graphics::ExtFloodFill(int32_t x, int32_t y, COLORREF crColor, UINT nFillType)
   { ASSERT(get_handle1() != ::null()); return ::ExtFloodFill(get_handle1(), x, y, crColor, nFillType) != FALSE; }
   

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   //// COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   bool graphics::TextOut(int32_t x, int32_t y, const string & str)
   { 
      if(m_pdibAlphaBlend != ::null())
      {
         //if(GetBkMode() == TRANSPARENT)
         {
         //   return TRUE;
            rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());
            rect rectText(point(x, y), GetTextExtent(str));
            if(rectIntersect.intersect(rectIntersect, rectText))
            {
               /*::ca::dib_sp dib0(get_app());
               dib0->create(rectText.size());
               dib0->Fill(0, 0, 0, 0);
               dib0->get_graphics()->SetTextColor(ARGB(255, 255, 255, 255));
               dib0->get_graphics()->SelectObject(&GetCurrentFont());
               dib0->get_graphics()->SetBkMode(TRANSPARENT);
               dib0->get_graphics()->TextOut(0, 0, str);
               dib0->ToAlpha(0);*/
               ::ca::dib_sp dib1(allocer());
               dib1->create(rectText.size());
               dib1->Fill(0, 0, 0, 0);
               dib1->get_graphics()->set_color(m_crColor);
               dib1->get_graphics()->SelectObject(&GetCurrentFont());
               dib1->get_graphics()->SetBkMode(TRANSPARENT);
               dib1->get_graphics()->TextOut(0, 0, str);
               //dib1->channel_from(visual::rgba::channel_alpha, dib0);
               ::ca::dib_sp dib2(allocer());
               dib2->create(rectText.size());
               dib2->Fill(255, 0, 0, 0);
               dib2->get_graphics()->set_alpha_mode(::ca::alpha_mode_set);
               dib2->from(point(max(0, m_ptAlphaBlend.x - x), max(0, m_ptAlphaBlend.y - y)),
                  m_pdibAlphaBlend->get_graphics(), point(max(0, x - m_ptAlphaBlend.x), max(0, y - m_ptAlphaBlend.y)), 
                  size(max(0, m_pdibAlphaBlend->cx-max(0, x - m_ptAlphaBlend.x)),
                        max(0, m_pdibAlphaBlend->cy-max(0, y - m_ptAlphaBlend.y))));
               dib1->channel_multiply(visual::rgba::channel_alpha, dib2);
               /*::ca::dib_sp dib3(get_app());
               dib1->mult_alpha(dib3);*/

               keeper < ::ca::dib * > keep(&m_pdibAlphaBlend, ::null(), m_pdibAlphaBlend, true);

               return System.visual().imaging().true_blend(this, point(x, y), rectText.size(), dib1->get_graphics(), null_point());

               /*BLENDFUNCTION bf;
               bf.BlendOp     = AC_SRC_OVER;
               bf.BlendFlags  = 0;
               bf.SourceConstantAlpha = 0xFF;
               bf.AlphaFormat = AC_SRC_ALPHA;
               return ::AlphaBlend(get_handle1(), x, y, 
                  rectText.width(), rectText.height(), WIN_HDC(dib1->get_graphics()), 0, 0, rectText.width(), 
                  rectText.height(), bf) != FALSE; */
            }
         }
      }
      
      //ASSERT(get_handle1() != ::null()); 
      //wstring wstr = ::ca::international::utf8_to_unicode(str);
      return TextOut(x, y, str, (int32_t) str.get_length()); 
   
   } // call virtual

   bool graphics::TextOut(double x, double y, const string & str)
   { 
      if(m_pdibAlphaBlend != ::null())
      {
         if(GetBkMode() == TRANSPARENT)
         {
         //   return TRUE;
            rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());
            rect rectText(point((int64_t) x, (int64_t) y), GetTextExtent(str));
            if(rectIntersect.intersect(rectIntersect, rectText))
            {
               ::ca::dib_sp dib0(allocer());
               dib0->create(rectText.size());
               dib0->get_graphics()->SetTextColor(RGB(255, 255, 255));
               dib0->get_graphics()->SelectObject(&GetCurrentFont());
               dib0->get_graphics()->SetBkMode(TRANSPARENT);
               dib0->get_graphics()->TextOut(0, 0, str);
               dib0->ToAlpha(0);
               ::ca::dib_sp dib1(allocer());
               dib1->create(rectText.size());
               dib1->get_graphics()->SetTextColor(GetTextColor());
               dib1->get_graphics()->SelectObject(&GetCurrentFont());
               dib1->get_graphics()->SetBkMode(TRANSPARENT);
               dib1->get_graphics()->TextOut(0, 0, str);
               dib1->channel_from(visual::rgba::channel_alpha, dib0);
               ::ca::dib_sp dib2(allocer());
               dib2->create(rectText.size());
               dib2->Fill(255, 0, 0, 0);
               dib2->from(point((int64_t) max(0, m_ptAlphaBlend.x - x), (int64_t) max(0, m_ptAlphaBlend.y - y)),
               m_pdibAlphaBlend->get_graphics(), point((int64_t) max(0, x - m_ptAlphaBlend.x), (int64_t) max(0, y - m_ptAlphaBlend.y)), rectText.size());
               dib1->channel_multiply(visual::rgba::channel_alpha, dib2);
               /*::ca::dib_sp dib3(get_app());
               dib1->mult_alpha(dib3);*/

               keeper < ::ca::dib * > keep(&m_pdibAlphaBlend, ::null(), m_pdibAlphaBlend, true);

               return System.visual().imaging().true_blend(this, point((int64_t) x, (int64_t) y), rectText.size(), dib1->get_graphics(), null_point());

               /*BLENDFUNCTION bf;
               bf.BlendOp     = AC_SRC_OVER;
               bf.BlendFlags  = 0;
               bf.SourceConstantAlpha = 0xFF;
               bf.AlphaFormat = AC_SRC_ALPHA;
               return ::AlphaBlend(get_handle1(), x, y, 
                  rectText.width(), rectText.height(), WIN_HDC(dib1->get_graphics()), 0, 0, rectText.width(), 
                  rectText.height(), bf) != FALSE; */
            }
         }
      }
      
      //ASSERT(get_handle1() != ::null()); 
      //wstring wstr = ::ca::international::utf8_to_unicode(str);
      return TextOut(x, y, str, (int32_t) str.get_length()); 
   
   } // call virtual

   bool graphics::ExtTextOut(int32_t x, int32_t y, UINT nOptions, LPCRECT lpRect, const char * lpszString, UINT nCount, LPINT lpDxWidths)
   { 
      ASSERT(get_handle1() != ::null()); 
      return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths) != FALSE; 
   }

   bool graphics::ExtTextOut(int32_t x, int32_t y, UINT nOptions, LPCRECT lpRect,
      const string & str, LPINT lpDxWidths)
   { ASSERT(get_handle1() != ::null()); return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect,
   str, (UINT)str.get_length(), lpDxWidths) != FALSE; }
   size graphics::TabbedTextOut(int32_t x, int32_t y, const char * lpszString, int32_t nCount,
      int32_t nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin)
   { ASSERT(get_handle1() != ::null()); return ::TabbedTextOut(get_handle1(), x, y, lpszString, nCount,
   nTabPositions, lpnTabStopPositions, nTabOrigin); }
   size graphics::TabbedTextOut(int32_t x, int32_t y, const string & str,
      int32_t nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin)
   { ASSERT(get_handle1() != ::null()); return ::TabbedTextOut(get_handle1(), x, y, str, (int32_t)str.get_length(),
   nTabPositions, lpnTabStopPositions, nTabOrigin); }


   size graphics::GetTabbedTextExtent(const char * lpszString, strsize nCount, int32_t nTabPositions, LPINT lpnTabStopPositions) const
   { 
      ASSERT(get_handle2() != ::null()); 
      return ::GetTabbedTextExtent(get_handle2(), lpszString, (int32_t) nCount, nTabPositions, lpnTabStopPositions);
   }

   size graphics::GetTabbedTextExtent(const string & str, int32_t nTabPositions, LPINT lpnTabStopPositions) const
   {
      ASSERT(get_handle2() != ::null()); 
      return ::GetTabbedTextExtent(get_handle2(), str, (int32_t) str.get_length(), nTabPositions, lpnTabStopPositions);
   }

   size graphics::GetOutputTabbedTextExtent(const char * lpszString, strsize nCount, int32_t nTabPositions, LPINT lpnTabStopPositions) const
   {
      ASSERT(get_handle1() != ::null());
      return ::GetTabbedTextExtent(get_handle1(), lpszString, (int32_t) nCount, nTabPositions, lpnTabStopPositions);
   }

   size graphics::GetOutputTabbedTextExtent(const string & str, int32_t nTabPositions, LPINT lpnTabStopPositions) const
   {
      ASSERT(get_handle1() != ::null());
      return ::GetTabbedTextExtent(get_handle1(), str, (int32_t) str.get_length(), nTabPositions, lpnTabStopPositions);
   }

   bool graphics::GrayString(::ca::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int32_t), LPARAM lpData, int32_t nCount,int32_t x, int32_t y, int32_t nWidth, int32_t nHeight)
   {
      ASSERT(get_handle1() != ::null()); 
      return ::GrayString(get_handle1(), (HBRUSH)pBrush->get_os_data(),(GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight) != FALSE;
   }

   UINT graphics::GetTextAlign() const
   { ASSERT(get_handle2() != ::null()); return ::GetTextAlign(get_handle2()); }
   int32_t graphics::GetTextFace(__in int32_t nCount, __out_ecount_part_z(nCount, return + 1) LPTSTR lpszFacename) const
   { ASSERT(get_handle2() != ::null()); return ::GetTextFace(get_handle2(), nCount, lpszFacename); }
   int32_t graphics::GetTextFace(string & rString) const
   { ASSERT(get_handle2() != ::null()); int32_t nResult = ::GetTextFace(get_handle2(),
   256, rString.GetBuffer(256)); rString.ReleaseBuffer();
   return nResult; }

   bool graphics::get_text_metrics(LPTEXTMETRIC lpMetrics) const
   { 
      //ASSERT(get_handle2() != ::null()); return ::GetTextMetrics(get_handle2(), lpMetrics); 
      /*wstring wstr(L"123AWZwmc");
      Gdiplus::RectF rect;
      Gdiplus::RectF rect2;
      Gdiplus::PointF origin(0, 0);
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, (Gdiplus::Font *) m_font->get_os_data(), origin, &rect);

      wstr = L"123AWZwmcpQçg";
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, (Gdiplus::Font *) m_font->get_os_data(), origin, &rect2);

      lpMetrics->tmAveCharWidth = rect.width / (double) wstr.get_length();
      lpMetrics->tmAscent = rect.height;
      lpMetrics->tmDescent = rect2.height - rect.height;*/


      //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

      Gdiplus::FontFamily family;


      if(((graphics * )this)->gdiplus_font() == ::null())
         return FALSE;

      ((graphics * )this)->gdiplus_font()->GetFamily(&family);

      double dHeight = family.GetEmHeight(((graphics * )this)->gdiplus_font()->GetStyle());

      lpMetrics->tmAscent              = (LONG) (((graphics * )this)->gdiplus_font()->GetSize() * family.GetCellAscent(((graphics * )this)->gdiplus_font()->GetStyle()) / dHeight);
      lpMetrics->tmDescent             = (LONG) (((graphics * )this)->gdiplus_font()->GetSize() * family.GetCellDescent(((graphics * )this)->gdiplus_font()->GetStyle()) / dHeight);
      lpMetrics->tmHeight              = (LONG) (((graphics * )this)->gdiplus_font()->GetSize());

      lpMetrics->tmInternalLeading     = (LONG) lpMetrics->tmAscent + lpMetrics->tmDescent - lpMetrics->tmHeight;
      lpMetrics->tmExternalLeading     = (LONG) (((graphics * )this)->gdiplus_font()->GetSize() * 
                                                (family.GetLineSpacing(((graphics * )this)->gdiplus_font()->GetStyle()) 
                                                - family.GetCellAscent(((graphics * )this)->gdiplus_font()->GetStyle())
                                                - family.GetCellDescent(((graphics * )this)->gdiplus_font()->GetStyle())) / dHeight);
      
      const Gdiplus::FontFamily * pfamilyMono = family.GenericMonospace();

      ::Gdiplus::Font font2(pfamilyMono, pfamilyMono->GetEmHeight(((graphics * )this)->gdiplus_font()->GetStyle()));
      
      wstring wstr(L"123AWZwmc123AWZwmcpQçg");
      Gdiplus::RectF rect(0.f, 0.f, 1024.f * 1024.f, 1024.f * 1024.f);
      Gdiplus::RectF rect2;
      Gdiplus::PointF origin(0, 0);

      m_pgraphics->MeasureString(wstr, (INT) wstr.get_length(), ((graphics * )this)->gdiplus_font(), origin, &rect);


      /*wstr = L"";
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, (Gdiplus::Font *) m_font->get_os_data(), origin, &rect2);*/

      lpMetrics->tmAveCharWidth        = (LONG) (rect.Width * GetCurrentFont().m_dFontWidth / (double) wstr.get_length());


      return TRUE;
   }


   bool graphics::get_output_text_metrics(LPTEXTMETRIC lpMetrics) const
   {
   
      ASSERT(get_handle1() != ::null());
      
      return ::GetTextMetrics(get_handle1(), lpMetrics) != FALSE;
   
   }


   int32_t graphics::GetTextCharacterExtra() const
   { ASSERT(get_handle2() != ::null()); return ::GetTextCharacterExtra(get_handle2()); }
   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   { ASSERT(get_handle2() != ::null()); return ::GetCharWidth(get_handle2(), nFirstChar, nLastChar, lpBuffer) != FALSE; }
   bool graphics::GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   { ASSERT(get_handle1() != ::null()); return ::GetCharWidth(get_handle1(), nFirstChar, nLastChar, lpBuffer) != FALSE; }
   uint32_t graphics::GetFontLanguageInfo() const
   { ASSERT(get_handle1() != ::null()); return ::GetFontLanguageInfo(get_handle1()); }

   uint32_t graphics::GetCharacterPlacement(const char * lpString, int32_t nCount, int32_t nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const
   { ASSERT(get_handle1() != ::null()); return ::GetCharacterPlacement(get_handle1(), lpString, nCount, nMaxExtent, lpResults, dwFlags); }
   
   uint32_t graphics::GetCharacterPlacement(string & str, int32_t nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const
   { 
      
      ASSERT(get_handle1() != ::null()); 
      
      return ::GetCharacterPlacement(get_handle1(), (const char *)str, (int32_t) str.get_length(), nMaxExtent, lpResults, dwFlags);
   
   }


   size graphics::GetAspectRatioFilter() const
   {
      ASSERT(get_handle2() != ::null());
      SIZE size;
      VERIFY(::GetAspectRatioFilterEx(get_handle2(), &size));
      return size;
   }
   bool graphics::ScrollDC(int32_t dx, int32_t dy,
      LPCRECT lpRectScroll, LPCRECT lpRectClip,
      ::ca::region* pRgnUpdate, LPRECT lpRectUpdate)
   { ASSERT(get_handle1() != ::null()); return ::ScrollDC(get_handle1(), dx, dy, lpRectScroll,
   lpRectClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate) != FALSE; }

   // Printer Escape Functions
   int32_t graphics::Escape(int32_t nEscape, int32_t nCount, const char * lpszInData, LPVOID lpOutData)
   { ASSERT(get_handle1() != ::null()); return ::Escape(get_handle1(), nEscape, nCount, lpszInData, lpOutData);}

   // graphics 3.1 Specific functions
   UINT graphics::SetBoundsRect(LPCRECT lpRectBounds, UINT flags)
   { ASSERT(get_handle1() != ::null()); return ::SetBoundsRect(get_handle1(), lpRectBounds, flags); }
   UINT graphics::GetBoundsRect(LPRECT lpRectBounds, UINT flags)
   { ASSERT(get_handle2() != ::null()); return ::GetBoundsRect(get_handle2(), lpRectBounds, flags); }
   bool graphics::ResetDC(const DEVMODE* lpDevMode)
   { ASSERT(get_handle2() != ::null()); return ::ResetDC(get_handle2(), lpDevMode) != ::null(); }
   UINT graphics::GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const
   { ASSERT(get_handle2() != ::null()); return ::GetOutlineTextMetrics(get_handle2(), cbData, lpotm); }
   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const
   { ASSERT(get_handle2() != ::null()); return ::GetCharABCWidths(get_handle2(), nFirstChar, nLastChar, lpabc) != FALSE; }
   uint32_t graphics::GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData,
      uint32_t cbData) const
   { ASSERT(get_handle2() != ::null()); return ::GetFontData(get_handle2(), dwTable, dwOffset, lpData, cbData); }
   int32_t graphics::GetKerningPairs(int32_t nPairs, LPKERNINGPAIR lpkrnpair) const
   { ASSERT(get_handle2() != ::null()); return ::GetKerningPairs(get_handle2(), nPairs, lpkrnpair); }
   uint32_t graphics::GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
      uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const
   { ASSERT(get_handle2() != ::null()); return ::GetGlyphOutline(get_handle2(), nChar, nFormat,
   lpgm, cbBuffer, lpBuffer, lpmat2); }

   // ::user::document handling functions
   int32_t graphics::StartDoc(LPDOCINFO lpDocInfo)
   { 

      ASSERT(m_hdc != ::null());

      return ::StartDoc(m_hdc, lpDocInfo);
   
   }

   int32_t graphics::StartPage()
   { ASSERT(get_handle1() != ::null()); return ::StartPage(get_handle1()); }
   int32_t graphics::EndPage()
   { ASSERT(get_handle1() != ::null()); return ::EndPage(get_handle1()); }
   int32_t graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, int32_t))
   { ASSERT(get_handle1() != ::null()); return ::SetAbortProc(get_handle1(), (ABORTPROC)lpfn); }
   int32_t graphics::AbortDoc()
   { ASSERT(get_handle1() != ::null()); return ::AbortDoc(get_handle1()); }
   int32_t graphics::EndDoc()
   { ASSERT(get_handle1() != ::null()); return ::EndDoc(get_handle1()); }

   bool graphics::MaskBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::ca::graphics * pgraphicsSrc,
      int32_t xSrc, int32_t ySrc, ::ca::bitmap& maskBitmap, int32_t xMask, int32_t yMask, uint32_t dwRop)
   { ASSERT(get_handle1() != ::null()); return ::MaskBlt(get_handle1(), x, y, nWidth, nHeight, WIN_HDC(pgraphicsSrc),
   xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop) != FALSE; }
   bool graphics::PlgBlt(LPPOINT lpPoint, ::ca::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc,
      int32_t nWidth, int32_t nHeight, ::ca::bitmap& maskBitmap, int32_t xMask, int32_t yMask)
   { ASSERT(get_handle1() != ::null()); 
   return ::PlgBlt(get_handle1(), lpPoint, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nWidth,
   nHeight, (HBITMAP)maskBitmap.get_os_data(), xMask, yMask) != FALSE; }
   bool graphics::SetPixelV(int32_t x, int32_t y, COLORREF crColor)
   { ASSERT(get_handle1() != ::null()); 
   return ::SetPixelV(get_handle1(), x, y, crColor) != FALSE; }
   bool graphics::SetPixelV(POINT point, COLORREF crColor)
   { ASSERT(get_handle1() != ::null()); 
   return ::SetPixelV(get_handle1(), point.x, point.y, crColor) != FALSE; }
   bool graphics::AngleArc(int32_t x, int32_t y, int32_t nRadius,
      float fStartAngle, float fSweepAngle)
   { ASSERT(get_handle1() != ::null()); 
   return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle) != FALSE; }
   bool graphics::ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { ASSERT(get_handle1() != ::null()); 
   return ArcTo(lpRect->left, lpRect->top, lpRect->right,
   lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); }
   int32_t graphics::GetArcDirection() const
   { ASSERT(get_handle2() != ::null());
   return ::GetArcDirection(get_handle2()); }
   bool graphics::PolyPolyline(const POINT* lpPoints, const uint32_t* lpPolyPoints,
      int32_t nCount)
   { ASSERT(get_handle1() != ::null()); 
   return ::PolyPolyline(get_handle1(), lpPoints, (LPDWORD) lpPolyPoints, nCount) != FALSE; }
   bool graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
   { ASSERT(get_handle2() != ::null()); 
   return ::GetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE; }

   ::ca::pen & graphics::GetCurrentPen() const
   {

      return *m_sppen.m_p;

   }

   ::ca::brush & graphics::GetCurrentBrush() const
   {
      
      return *m_spbrush.m_p;

   }
   
   ::ca::palette & graphics::GetCurrentPalette() const
   {

      return *(::ca::palette *)::null();

   }

   ::ca::font & graphics::GetCurrentFont() const
   {

      return *m_spfont.m_p;

   }

   ::ca::bitmap & graphics::GetCurrentBitmap() const
   {

      return *m_bitmap.m_p;

   }

   bool graphics::PolyBezier(const POINT* lpPoints, int32_t nCount)
   {
      ASSERT(get_handle1() != ::null()); 
      return ::PolyBezier(get_handle1(), lpPoints, nCount) != FALSE; 
   }

   int32_t graphics::DrawEscape(int32_t nEscape, int32_t nInputSize, const char * lpszInputData)
   {
      ASSERT(get_handle1() != ::null()); 
      return ::DrawEscape(get_handle1(), nEscape, nInputSize, lpszInputData); 
   }
   
   int32_t graphics::Escape(__in int32_t nEscape, __in int32_t nInputSize, __in_bcount(nInputSize) const char * lpszInputData,  __in int32_t nOutputSize, __out_bcount(nOutputSize) char * lpszOutputData)
   {
       ASSERT(get_handle1() != ::null()); 
       return ::ExtEscape(get_handle1(), nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData); 
   }

   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
      LPABCFLOAT lpABCF) const
   { 
      ASSERT(get_handle2() != ::null()); 
      return ::GetCharABCWidthsFloat(get_handle2(), nFirstChar, nLastChar, lpABCF) != FALSE; 
   }
   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, float* lpFloatBuffer) const
   {
      ASSERT(get_handle2() != ::null()); 
      return ::GetCharWidthFloat(get_handle2(), nFirstChar, nLastChar, lpFloatBuffer) != FALSE; 
   }

   bool graphics::AbortPath()
   {
      if(m_ppath != ::null())
      {
         delete m_ppath;
         m_ppath = ::null();
      }
      return true;
   }

   bool graphics::BeginPath()
   { 
      
      if(m_ppath != ::null())
         delete m_ppath;
      
      m_ppath = new Gdiplus::GraphicsPath;

      return m_ppath != ::null();

   }

   bool graphics::CloseFigure()
   {
      ASSERT(m_ppath != ::null()); 
      return m_ppath->CloseFigure() == Gdiplus::Status::Ok;
   }

   bool graphics::EndPath()
   { 

      if(m_ppath == ::null())
         return FALSE;

      m_ppathPaint   = m_ppath;
      m_ppath        = ::null();
      
      return true;

   }

   bool graphics::FillPath()
   { 

      return m_pgraphics->FillPath(gdiplus_brush(), m_ppath) == Gdiplus::Status::Ok;

   }

   bool graphics::FlattenPath()
   {
      return m_ppath->Flatten() == Gdiplus::Status::Ok;
   }

   float graphics::GetMiterLimit() const
   {
      ASSERT(get_handle1() != ::null()); 
      float fMiterLimit;
      VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit)); 
      return fMiterLimit; 
   }

   int32_t graphics::GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int32_t nCount) const
   { 
      ASSERT(get_handle1() != ::null()); 
      return ::GetPath(get_handle1(), lpPoints, lpTypes, nCount); 
   }
   bool graphics::SetMiterLimit(float fMiterLimit)
   {
      ASSERT(get_handle1() != ::null()); 
      return ::SetMiterLimit(get_handle1(), fMiterLimit, ::null()) != FALSE; 
   }
   
   bool graphics::StrokeAndFillPath()
   { 

      bool bOk1 = m_pgraphics->FillPath(gdiplus_brush(), m_ppathPaint) == Gdiplus::Status::Ok;

      bool bOk2 = m_pgraphics->DrawPath(gdiplus_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      return bOk1 && bOk2;

   }

   bool graphics::StrokePath()
   {

      return m_pgraphics->DrawPath(gdiplus_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

   }

   bool graphics::WidenPath()
   {

      return m_ppath->Widen(gdiplus_pen()) == Gdiplus::Status::Ok;

   }


   bool graphics::draw_path(::ca::graphics_path * ppath)
   {

      return m_pgraphics->DrawPath(gdiplus_pen(), (Gdiplus::GraphicsPath *) ppath->get_os_data()) == Gdiplus::Status::Ok;

   }

   bool graphics::fill_path(::ca::graphics_path * ppath)
   {

      return m_pgraphics->FillPath(gdiplus_brush(), (Gdiplus::GraphicsPath *) ppath->get_os_data()) == Gdiplus::Status::Ok;

   }


   bool graphics::AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData)
   { 
      ASSERT(get_handle1() != ::null()); 
      return ::GdiComment(get_handle1(), nDataSize, pCommentData) != FALSE; 
   }
   

/*bool CALLBACK metaCallback(
   EmfPlusRecordType recordType, 
   uint32_t flags, 
   uint32_t dataSize, 
   const uchar* pStr, 
   void* callbackData)
{ 
   // Play only EmfPlusRecordTypeFillEllipse records.
   if (recordType == EmfPlusRecordTypeFillEllipse)
   {
   // Explicitly cast callbackData as a metafile pointer, and use it to call
   // the PlayRecord method.
   static_cast < Metafile* > (callbackData)->PlayRecord(recordType, flags, dataSize, pStr);
   }
   return TRUE; 
}

VOID Example_EnumerateMetafile9(HDC hdc)
{   
   Graphics graphics(hdc);
   // Create a Metafile object from an existing disk metafile.
   Metafile* pMeta = new Metafile(L"SampleMetafile.emf", hdc);
   {
      // Fill a rectangle and an ellipse in pMeta.
      Graphics metaGraphics(pMeta);
      metaGraphics.FillRectangle(&SolidBrush(Color(255, 0, 0, 0)), 0, 0, 100, 100);
  metaGraphics.FillEllipse(&SolidBrush(Color(255, 255, 0, 0)), 100, 0, 200, 100);
   }
   // Enumerate pMeta to the destination rectangle, passing pMeta as the callback data. 
   graphics.EnumerateMetafile(
   pMeta,
   Rect(0, 0, 300, 50),
   metaCallback,
   pMeta);
   // Draw pMeta as an image.
   graphics.DrawImage(pMeta, Point(0, 150));
   delete pMeta;;
}*/
   bool graphics::PlayMetaFile(HENHMETAFILE hEnhMF, LPCRECT lpBounds)
   { 

      Gdiplus::RectF rect((Gdiplus::REAL) lpBounds->left, (Gdiplus::REAL) lpBounds->top, (Gdiplus::REAL) width(lpBounds), (Gdiplus::REAL) height(lpBounds));

      Gdiplus::Metafile* pMeta = new Gdiplus::Metafile(hEnhMF, false);

      //m_pgraphcis->EnumerateMetafile(pMeta, rect, metaCallback, PMETAHEADER);

      bool bOk = m_pgraphics->DrawImage(pMeta, rect) == Gdiplus::Status::Ok;

      delete pMeta;

      return bOk ? TRUE : FALSE;
      //return ::PlayEnhMetaFile(get_handle1(), hEnhMF, lpBounds); 
   
   }

   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

// Thank you
// Jiju George T
// Web Developer
// India India
// Member

   bool graphics::alpha_blend(int32_t xDest, int32_t yDest, int32_t nDestWidth, int32_t nDestHeight, ::ca::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, double dRate)
   {
      
      if(m_pdibAlphaBlend != ::null())
      {


         rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());


         ::ca::dib * pdibWork = ::null();
         ::ca::dib * pdibWork2 = ::null();
//         ::ca::dib * pdibWork3 = ::null();
         ::ca::dib * pdibWork4 = ::null();

            
         class point ptSrc(xSrc, ySrc);
         class point ptDest(xDest, yDest);
         class size size(nDestWidth, nDestHeight);



         ::ca::dib_sp spdib;
         if(pdibWork == ::null())
         {
            spdib.create(allocer());
            pdibWork = spdib;
         }
         if(pdibWork == ::null())
            return false;
         if(!pdibWork->create(size))
            return false;
         if(!pdibWork->from(null_point(), pgraphicsSrc, ptSrc, size))
            return false;




         ::ca::dib_sp spdib2;
         if(pdibWork2 == ::null())
         {
            spdib2.create(allocer());
            pdibWork2 = spdib2;
         }


         ::ca::dib_sp spdib4;
         if(pdibWork4 == ::null())
         {
            spdib4.create(allocer());
            pdibWork4 = spdib4;
         }
         if(pdibWork4 == ::null())
            return false;
         if(!pdibWork4->create(size))
            return false;


         pdibWork4->Fill(255, 0, 0, 0);
            
         pdibWork4->from(point(max(0, m_ptAlphaBlend.x - xDest), max(0, m_ptAlphaBlend.y - yDest)),
            m_pdibAlphaBlend->get_graphics(), point(max(0, xDest - m_ptAlphaBlend.x), max(0, yDest - m_ptAlphaBlend.y)), size);
   
         pdibWork->channel_multiply(visual::rgba::channel_alpha, pdibWork4);


         keeper < ::ca::dib * > keep(&m_pdibAlphaBlend, ::null(), m_pdibAlphaBlend, true);


         return System.visual().imaging().true_blend(this, ptDest, size, pdibWork->get_graphics(), ptSrc); 


      }


      float fA = (float) dRate;

      Gdiplus::ColorMatrix matrix = { 
         1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f, fA  , 0.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 1.0f
      };

      Gdiplus::ImageAttributes attributes;

      attributes.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

      Gdiplus::RectF dstRect((Gdiplus::REAL) xDest, (Gdiplus::REAL) yDest, (Gdiplus::REAL) nDestWidth, (Gdiplus::REAL) nDestHeight);

      m_pgraphics->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->GetCurrentBitmap().get_os_data(), dstRect, 
         (Gdiplus::REAL) xSrc, (Gdiplus::REAL) ySrc, (Gdiplus::REAL) nSrcWidth, (Gdiplus::REAL) nSrcHeight, Gdiplus::UnitPixel, &attributes);

      return true;

   }


   /*bool graphics::alpha_blend(int32_t xDest, int32_t yDest, int32_t nDestWidth, int32_t nDestHeight,
      ::ca::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, BLENDFUNCTION blend)
   { 
      
      throw not_implemented(get_app());
      //if(get_handle1() == ::null())
        // return false;

      
      if(m_pdibAlphaBlend != ::null())
      {


         rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());


         ::ca::dib * pdibWork = ::null();
         ::ca::dib * pdibWork2 = ::null();
//         ::ca::dib * pdibWork3 = ::null();
         ::ca::dib * pdibWork4 = ::null();

            
         class point ptSrc(xSrc, ySrc);
         class point ptDest(xDest, yDest);
         class size size(nDestWidth, nDestHeight);



         ::ca::dib_sp spdib;
         if(pdibWork == ::null())
         {
            spdib.create(allocer());
            pdibWork = spdib;
         }
         if(pdibWork == ::null())
            return false;
         if(!pdibWork->create(size))
            return false;
         if(!pdibWork->from(null_point(), pgraphicsSrc, ptSrc, size))
            return false;




         ::ca::dib_sp spdib2;
         if(pdibWork2 == ::null())
         {
            spdib2.create(allocer());
            pdibWork2 = spdib2;
         }


         ::ca::dib_sp spdib4;
         if(pdibWork4 == ::null())
         {
            spdib4.create(allocer());
            pdibWork4 = spdib4;
         }
         if(pdibWork4 == ::null())
            return false;
         if(!pdibWork4->create(size))
            return false;


         pdibWork4->Fill(255, 0, 0, 0);
            
         pdibWork4->from(point(max(0, m_ptAlphaBlend.x - xDest), max(0, m_ptAlphaBlend.y - yDest)),
            m_pdibAlphaBlend->get_graphics(), point(max(0, xDest - m_ptAlphaBlend.x), max(0, yDest - m_ptAlphaBlend.y)), size);
   
         pdibWork->channel_multiply(visual::rgba::channel_alpha, pdibWork4);


         keeper < ::ca::dib * > keep(&m_pdibAlphaBlend, ::null(), m_pdibAlphaBlend, true);


         return System.visual().imaging().true_blend(this, ptDest, size, pdibWork->get_graphics(), ptSrc); 


      }

      return ::AlphaBlend(get_handle1(), xDest, yDest, 
         nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
         nSrcHeight, blend) != FALSE; 
   }*/


   bool graphics::TransparentBlt(int32_t xDest, int32_t yDest, int32_t nDestWidth, 
      int32_t nDestHeight, ::ca::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, 
      int32_t nSrcHeight, UINT crTransparent)
   { ASSERT(get_handle1() != ::null()); return ::TransparentBlt(get_handle1(), xDest, yDest, 
   nDestWidth, nDestHeight, WIN_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
   nSrcHeight, crTransparent) != FALSE; }
   
   bool graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices, void * pMesh, ULONG nMeshElements, uint32_t dwMode)
   {

      ASSERT(get_handle1() != ::null()); return ::GradientFill(get_handle1(), pVertices, nVertices, pMesh, nMeshElements, dwMode) != FALSE;

   }

   // This is ca API library.
   // 
   // 
   //
   // 
   // 
   // 
   // 
   // 

   // Always Inline. Functions only in Win98/Win2K or later

   inline COLORREF graphics::GetDCBrushColor() const
   { ASSERT(get_handle1() != ::null()); return ::GetDCBrushColor(get_handle1()); }
   inline COLORREF graphics::SetDCBrushColor(COLORREF crColor)
   { ASSERT(get_handle1() != ::null()); return ::SetDCBrushColor(get_handle1(), crColor); }

   inline COLORREF graphics::GetDCPenColor() const
   { ASSERT(get_handle1() != ::null()); return ::GetDCPenColor(get_handle1()); }
   inline COLORREF graphics::SetDCPenColor(COLORREF crColor)
   { ASSERT(get_handle1() != ::null()); return ::SetDCPenColor(get_handle1(), crColor); }

   inline bool graphics::GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const
   { ASSERT(get_handle1() != ::null()); return ::GetCharABCWidthsI(get_handle1(), giFirst, cgi, pgi, lpabc) != FALSE; }
   inline bool graphics::GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const
   { ASSERT(get_handle1() != ::null()); return ::GetCharWidthI(get_handle1(), giFirst, cgi, pgi, lpBuffer) != FALSE; }

   inline bool graphics::GetTextExtentExPointI(LPWORD pgiIn, int32_t cgi, int32_t nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize) const
   { 
      ENSURE(lpSize != ::null()); 
      ASSERT(get_handle1() != ::null()); 
      return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, lpSize) != FALSE; 
   }
   inline bool graphics::GetTextExtentPointI(LPWORD pgiIn, int32_t cgi, LPSIZE lpSize) const
   {
      ENSURE(lpSize != ::null()); 
      ASSERT(get_handle1() != ::null()); 
      return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, lpSize) != FALSE; 
   }






   __STATIC_DATA HBRUSH gen_HalftoneBrush = 0;

   void c_cdecl __win_gdi_x_term()
   {
      __delete_object((HGDIOBJ*)&gen_HalftoneBrush);
   }
   char gen_WingdixTerm = 0;

   /////////////////////////////////////////////////////////////////////////////
   // More coordinate transforms (in separate file to avoid transitive refs)

#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      int32_t nMapMode;
      if (this != ::null() && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained map mode, map against physical inch
         ((::ca::graphics *)this)->SetMapMode(MM_HIMETRIC);
         DPtoLP(lpSize);
         ((::ca::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // map against logical inch for non-constrained mapping modes
         int32_t cxPerInch, cyPerInch;
         if (this != ::null())
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != ::null());  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);
         }
         else
         {
//            cxPerInch = afxData.cxPixelsPerInch;
  //          cyPerInch = afxData.cyPixelsPerInch;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
         lpSize->cy = MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);
      }
   }

   void graphics::HIMETRICtoDP(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      int32_t nMapMode;
      if (this != ::null() && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained map mode, map against physical inch
         ((::ca::graphics *)this)->SetMapMode(MM_HIMETRIC);
         LPtoDP(lpSize);
         ((::ca::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // map against logical inch for non-constrained mapping modes
         int32_t cxPerInch, cyPerInch;
         if (this != ::null())
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != ::null());  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);
         }
         else
         {
//            cxPerInch = afxData.cxPixelsPerInch;
  //          cyPerInch = afxData.cyPixelsPerInch;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
         lpSize->cy = MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
      }
   }

   void graphics::LPtoHIMETRIC(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      LPtoDP(lpSize);
      DPtoHIMETRIC(lpSize);
   }

   void graphics::HIMETRICtoLP(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      HIMETRICtoDP(lpSize);
      DPtoLP(lpSize);
   }

   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing primitives/helpers

   ::ca::brush* graphics::GetHalftoneBrush(sp(::ca::application) papp)
   {
/*      ::ca::LockGlobals(CRIT_HALFTONEBRUSH);
      if (gen_HalftoneBrush == ::null())
      {
         WORD grayPattern[8];
         for (int32_t i = 0; i < 8; i++)
            grayPattern[i] = (WORD)(0x5555 << (i & 1));
         HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
         if (grayBitmap != ::null())
         {
            gen_HalftoneBrush = ::CreatePatternBrush(grayBitmap);
            ::DeleteObject(grayBitmap);
         }
      }
      if (!gen_WingdixTerm)
         gen_WingdixTerm = (char)!atexit(&__win_gdi_x_term);
      ::ca::UnlockGlobals(CRIT_HALFTONEBRUSH);

//      return ::win::brush::from_handle(papp, gen_HalftoneBrush);*/
      return ::null();
   }

   void graphics::DrawDragRect(LPCRECT lpRect, SIZE size, LPCRECT lpRectLast, SIZE sizeLast, ::ca::brush* pBrush, ::ca::brush* pBrushLast)
   {

      throw not_implemented(get_app());

/*

      ASSERT(__is_valid_address(lpRect, sizeof(RECT), FALSE));
      ASSERT(lpRectLast == ::null() ||
         __is_valid_address(lpRectLast, sizeof(RECT), FALSE));

      // first, determine the update region and select it
      ::ca::region rgnNew;
      ::ca::region rgnOutside, rgnInside;
      rgnOutside.create_rect(lpRect);
      rect rect = *lpRect;
      rect.inflate(-size.cx, -size.cy);
      rect.intersect(rect, lpRect);
      rgnInside.create_rect(rect);
      rgnNew.create_rect(0, 0, 0, 0);
      rgnNew.combine(&rgnOutside, &rgnInside, RGN_XOR);

      ::ca::brush* pBrushOld = ::null();
      if (pBrush == ::null())
      {
         pBrush = graphics::GetHalftoneBrush(get_app());
      }

      ENSURE(pBrush);

      if (pBrushLast == ::null())
      {
         pBrushLast = pBrush;
      }

      ::ca::region rgnLast, rgnUpdate;
      if (lpRectLast != ::null())
      {
         // find difference between new region and old region
         rgnLast.create_rect(0, 0, 0, 0);
         rgnOutside.SetRectRgn(lpRectLast);
         rect = *lpRectLast;
         rect.inflate(-sizeLast.cx, -sizeLast.cy);
         rect.intersect(rect, lpRectLast);
         rgnInside.SetRectRgn(rect);
         rgnLast.CombineRgn(&rgnOutside, &rgnInside, RGN_XOR);

         // only diff them if brushes are the same
         if (pBrush->get_os_data() == pBrushLast->get_os_data())
         {
            rgnUpdate.create_rect(0, 0, 0, 0);
            rgnUpdate.CombineRgn(&rgnLast, &rgnNew, RGN_XOR);
         }
      }
      if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != ::null())
      {
         // brushes are different -- erase old region first
         SelectClipRgn(&rgnLast);
         GetClipBox(&rect);
         pBrushOld = SelectObject(pBrushLast);
         PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);
         SelectObject(pBrushOld);
         pBrushOld = ::null();
      }

      // draw into the update/new region
      SelectClipRgn(rgnUpdate.get_os_data() != ::null() ? &rgnUpdate : &rgnNew);
      GetClipBox(&rect);
      pBrushOld = SelectObject(pBrush);
      PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);

      // cleanup DC
      if (pBrushOld != ::null())
         SelectObject(pBrushOld);
      SelectClipRgn(::null());

*/
   }

   /*void graphics::FillSolidRect(LPCRECT lpRect, COLORREF clr)
   {
      ::SetBkColor(get_handle1(), clr);
      ::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, lpRect, ::null(), 0, ::null());
   }*/

   void graphics::FillSolidRect(const __rect64 * lpRect, COLORREF clr)
   {
      rect rect32;
      ::copy(rect32, lpRect);
      FillSolidRect(rect32, clr);
   }


   /*
    
   void graphics::FillSolidRect(int32_t x, int32_t y, int32_t cx, int32_t cy, COLORREF clr)
   {
      ::SetBkColor(get_handle1(), clr);
      rect rect(x, y, x + cx, y + cy);
      ::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, &rect, ::null(), 0, ::null());
   }

   */

   void graphics::Draw3dRect(LPCRECT lpRect,
      COLORREF clrTopLeft, COLORREF clrBottomRight)
   {
      Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left,
         lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
   }

   void graphics::Draw3dRect(int32_t x, int32_t y, int32_t cx, int32_t cy,
      COLORREF clrTopLeft, COLORREF clrBottomRight)
   {
      FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
      FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
      FillSolidRect(x + cx - 1, y, 1, cy, clrBottomRight);
      FillSolidRect(x, y + cy - 1, cx, 1, clrBottomRight);
   }





   //::ca::graphics * ::win::graphics::from_handle(HDC hDC)
   //{
      //hdc_map* pMap = afxMapHDC(TRUE); //create map if not exist
      //ASSERT(pMap != ::null());
//      ::ca::graphics * pgraphics = (::ca::graphics *)pMap->from_handle(hDC);
  //    ASSERT(pgraphics == ::null() || (dynamic_cast<::win::graphics * >(pgraphics))->get_handle1() == hDC);
    //  return pgraphics;
     // return ::null();
   //}

   bool graphics::Attach(HDC hdc)
   {

      if(hdc == ::null())
         return FALSE;

      if(m_hdc == hdc)
      {

         if(m_pgraphics != ::null())
            return TRUE;

      }
      
      if(hdc != ::null())
      {
         
         m_pgraphics = new ::Gdiplus::Graphics((HDC) hdc);

         set_text_rendering(::ca::text_rendering_anti_alias_grid_fit);

         m_hdc = (HDC) hdc;

      }

      return m_pgraphics != ::null();

      /*ASSERT(get_handle1() == ::null());      // only attach once, detach on destroy
      ASSERT(get_handle2() == ::null());    // only attach to an is_empty DC

      if (hDC == ::null())
      {
         return FALSE;
      }
      // remember early to avoid leak
      set_handle1(hDC);
      hdc_map* pMap = afxMapHDC(TRUE); // create map if not exist
      ASSERT(pMap != ::null());
      pMap->set_permanent(get_handle1(), this);

      SetAttribDC(get_handle1());     // Default to same as output
      return TRUE;*/
   }

   HDC graphics::Detach()
   {

      if(m_hdc == ::null())
         return ::null();

      if(m_pgraphics == ::null())
         return ::null();

      try
      {

         delete m_pgraphics;

      }
      catch(...)
      {

         TRACE("graphics::Detach : Failed to delete Gdiplus::Graphics");

      }

      m_pgraphics = ::null();

      HDC hdc = m_hdc;
      m_hdc = ::null();

      return hdc;
      
   }

   bool graphics::DeleteDC()
   {

      HDC hdc = Detach();
      
      if(hdc != ::null())
      {

         bool bDeleted = ::DeleteDC(hdc) != FALSE;

         if(!bDeleted)
         {
         
            TRACE("graphics::DeleteDC : Failed to delete GDI device context");

         }

      }

      if(m_pgraphics != ::null())
      {

         try
         {
            
            delete m_pgraphics;

         }
         catch(...)
         {

            TRACE("graphics::DeleteDC : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = ::null();

      }

      if(m_ppath != ::null())
      {

         try
         {
            
            delete m_ppath;

         }
         catch(...)
         {

         }

         m_ppath = ::null();

      }

      if(m_ppathPaint != ::null())
      {

         try
         {
            
            delete m_ppathPaint;

         }
         catch(...)
         {

         }

         m_ppathPaint = ::null();

      }

      return true;

   }



   void graphics::SetAttribDC(HDC hDC)  // set the Attribute DC
   {
//      set_handle2(hDC);
   }

   void graphics::SetOutputDC(HDC hDC)  // set the Output DC
   {
#ifdef DEBUG
/*      hdc_map* pMap = afxMapHDC();
      if (pMap != ::null() && pMap->lookup_permanent(get_handle1()) == this)
      {
         TRACE(::ca::trace::category_AppMsg, 0, "Cannot set Output hDC on Attached graphics.\n");
         ASSERT(FALSE);
      }*/
#endif
  //    set_handle1(hDC);
   }

   void graphics::ReleaseAttribDC()     // Release the Attribute DC
   {
//      set_handle2(::null());
   }

   void graphics::ReleaseOutputDC()     // Release the Output DC
   {
#ifdef DEBUG
/*      hdc_map* pMap = afxMapHDC();
      if (pMap != ::null() && pMap->lookup_permanent(get_handle1()) == this)
      {
         TRACE(::ca::trace::category_AppMsg, 0, "Cannot Release Output hDC on Attached graphics.\n");
         ASSERT(FALSE);
      }*/
#endif
      //set_handle1(::null());
   }

   /////////////////////////////////////////////////////////////////////////////
   // Out-of-line routines

   int32_t graphics::StartDoc(const char * lpszDocName)
   {
      DOCINFO di;
      memset(&di, 0, sizeof(DOCINFO));
      di.cbSize = sizeof(DOCINFO);
      di.lpszDocName = lpszDocName;
      return StartDoc(&di);
   }

   int32_t graphics::SaveDC()
   {
      int32_t nRetVal = 0;
      if(get_handle2() != ::null())
         nRetVal = ::SaveDC(get_handle2());
      if(get_handle1() != ::null() && get_handle1() != get_handle2() && ::SaveDC(get_handle1()) != 0)
         nRetVal = -1;   // -1 is the only valid restore value for complex DCs
      return nRetVal;
   }

   bool graphics::RestoreDC(int32_t nSavedDC)
   {
      bool bRetVal = TRUE;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         bRetVal = ::RestoreDC(get_handle1(), nSavedDC) != FALSE;
      if(get_handle2() != ::null())
         bRetVal = (bRetVal && ::RestoreDC(get_handle2(), nSavedDC) != FALSE);
      return bRetVal;
   }

//   ::ca::graphics_object* graphics::SelectGdiObject(sp(::ca::application) papp, HDC hDC, HGDIOBJ h)
  // {
//      return ::win::graphics_object::from_handle(papp, ::SelectObject(hDC, h));
   //}

   ::ca::graphics_object* graphics::SelectStockObject(int32_t nIndex)
   {
/*      HGDIOBJ hObject = ::GetStockObject(nIndex);
      HGDIOBJ hOldObj = ::null();
      ASSERT(hObject != ::null());
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), hObject);
      if(get_handle2() != ::null())
         hOldObj = ::SelectObject(get_handle2(), hObject);
      return ::win::graphics_object::from_handle(get_app(), hOldObj);*/

      return ::null();
   }

   ::ca::pen* graphics::SelectObject(::ca::pen* pPen)
   {
      /*HGDIOBJ hOldObj = ::null();
      if(pPen == ::null())
         return ::null();
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), pPen->get_os_data());
      if(get_handle2() != ::null())
         hOldObj = ::SelectObject(get_handle2(), pPen->get_os_data());
      return dynamic_cast < pen * > (::win::graphics_object::from_handle(get_app(), hOldObj));*/
      m_penxyz = *pPen;
      return &m_penxyz;
   }

   ::ca::brush* graphics::SelectObject(::ca::brush* pBrush)
   {
/*      HGDIOBJ hOldObj = ::null();
      if(pBrush == ::null())
         return ::null();
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), pBrush->get_os_data());
      if(get_handle2() != ::null())
         hOldObj = ::SelectObject(get_handle2(), pBrush->get_os_data());
      return dynamic_cast < ::ca::brush * > (::win::graphics_object::from_handle(get_app(), hOldObj));*/
      m_brushxyz = *pBrush;
      return &m_brushxyz;

   }

   ::ca::font* graphics::SelectObject(::ca::font* pfont)
   {
/*      HGDIOBJ hOldObj = ::null();
      if(pFont == ::null())
         return ::null();
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), pFont->get_os_data());
      if(get_handle2() != ::null())
         hOldObj = ::SelectObject(get_handle2(), pFont->get_os_data());
      return dynamic_cast < ::ca::font * > (::win::graphics_object::from_handle(get_app(), hOldObj));*/

      /*ASSERT(pFont != ::null());

      if(pFont == ::null())
         return ::null();

      m_fontxyz = *pFont;
      return &m_fontxyz;*/
      
      if(!select_font(pfont))
         return ::null();

      return &m_fontxyz;

   }

   int32_t graphics::SelectObject(::ca::region* pRgn)
   {
      int32_t nRetVal = GDI_ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = (int32_t)(int_ptr)::SelectObject(get_handle1(), (HGDIOBJ) pRgn->get_os_data());
      if(get_handle2() != ::null())
         nRetVal = (int32_t)(int_ptr)::SelectObject(get_handle2(), (HGDIOBJ) pRgn->get_os_data());
      return nRetVal;
   }

   ::ca::palette* graphics::SelectPalette(::ca::palette* pPalette, bool bForceBackground)
   {
      return ::null();
//      return dynamic_cast < ::ca::palette * > (::win::graphics_object::from_handle(get_app(), ::SelectPalette(get_handle1(), (HPALETTE)pPalette->get_os_data(), bForceBackground)));
   }

   COLORREF graphics::SetBkColor(COLORREF crColor)
   {
      COLORREF crRetVal = CLR_INVALID;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         crRetVal = ::SetBkColor(get_handle1(), crColor);
      if(get_handle2() != ::null())
         crRetVal = ::SetBkColor(get_handle2(), crColor);
      return crRetVal;
   }

   int32_t graphics::SetBkMode(int32_t nBkMode)
   {

      return 0;

/*      int32_t nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SetBkMode(get_handle1(), nBkMode);
      if(get_handle2() != ::null())
         nRetVal = ::SetBkMode(get_handle2(), nBkMode);
      return nRetVal;*/
   }

   int32_t graphics::SetPolyFillMode(int32_t nPolyFillMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SetPolyFillMode(get_handle1(), nPolyFillMode);
      if(get_handle2() != ::null())
         nRetVal = ::SetPolyFillMode(get_handle2(), nPolyFillMode);
      return nRetVal;
   }

   int32_t graphics::SetROP2(int32_t nDrawMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SetROP2(get_handle1(), nDrawMode);
      if(get_handle2() != ::null())
         nRetVal = ::SetROP2(get_handle2(), nDrawMode);
      return nRetVal;
   }

   int32_t graphics::SetStretchBltMode(int32_t nStretchMode)
   {
      if(nStretchMode == 0)
      {
         m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
      }
      else if(nStretchMode == HALFTONE)
      {
         m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
      }
      else
      {
         m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeLowQuality);
      }
      return 1;
      /*int32_t nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SetStretchBltMode(get_handle1(), nStretchMode);
      if(get_handle2() != ::null())
         nRetVal = ::SetStretchBltMode(get_handle2(), nStretchMode);
      return nRetVal;*/
   }

   COLORREF graphics::SetTextColor(COLORREF crColor)
   {
      return set_color(crColor);
      //COLORREF crRetVal = m_crColor;
      //m_crColor = crColor;
/*      COLORREF crRetVal = CLR_INVALID;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         crRetVal = ::SetTextColor(get_handle1(), crColor);
      if(get_handle2() != ::null())
         crRetVal = ::SetTextColor(get_handle2(), crColor);*/
      //return crRetVal;
   }

   int32_t graphics::SetGraphicsMode(int32_t iMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
      {
         nRetVal = ::SetGraphicsMode(get_handle1(), iMode);
      }
      if(get_handle2() != ::null())
      {
         nRetVal = ::SetGraphicsMode(get_handle2(), iMode);
      }
      return nRetVal;
   }

   bool graphics::SetWorldTransform(const XFORM* pXform)
   {
      bool nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
      {
         nRetVal = ::SetWorldTransform(get_handle1(), pXform) != FALSE;
      }
      if(get_handle2() != ::null())
      {
         nRetVal = ::SetWorldTransform(get_handle2(), pXform) != FALSE;
      }
      return nRetVal;
   }

   bool graphics::ModifyWorldTransform(const XFORM* pXform,uint32_t iMode)
   {
      bool nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
      {
         nRetVal = ::ModifyWorldTransform(get_handle1(), pXform, iMode) != FALSE;
      }
      if(get_handle2() != ::null())
      {
         nRetVal = ::ModifyWorldTransform(get_handle2(), pXform, iMode) != FALSE;
      }
      return nRetVal;
   }

   int32_t graphics::SetMapMode(int32_t nMapMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SetMapMode(get_handle1(), nMapMode);
      if(get_handle2() != ::null())
         nRetVal = ::SetMapMode(get_handle2(), nMapMode);
      return nRetVal;
   }

   point graphics::GetViewportOrg() const
   {
      //POINT point;
      //::GetViewportOrgEx(get_handle2(), &point);

      Gdiplus::Matrix m;

      m_pgraphics->GetTransform(&m);

      Gdiplus::PointF origin(0, 0);

      m.TransformPoints(&origin);

      return point((int64_t) origin.X, (int64_t) origin.Y);
   }



   point graphics::SetViewportOrg(int32_t x, int32_t y)
   {
      /*point point(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::SetViewportOrgEx(get_handle1(), x, y, &point);
      if(get_handle2() != ::null())
         ::SetViewportOrgEx(get_handle2(), x, y, &point);*/
      Gdiplus::Matrix m;
      m.Translate((Gdiplus::REAL) x, (Gdiplus::REAL) y);
      g().SetTransform(&m);
      //return point;
      return point(x, y);
   }

   point graphics::OffsetViewportOrg(int32_t nWidth, int32_t nHeight)
   {
      
      point point = GetViewportOrg();

      point.offset(nWidth, nHeight);

      return SetViewportOrg(point.x, point.y);

   }

   size graphics::SetViewportExt(int32_t x, int32_t y)
   {
      size size(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::SetViewportExtEx(get_handle1(), x, y, &size);
      if(get_handle2() != ::null())
         ::SetViewportExtEx(get_handle2(), x, y, &size);
      return size;
   }

   size graphics::ScaleViewportExt(int32_t xNum, int32_t xDenom, int32_t yNum, int32_t yDenom)
   {
      size size(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::ScaleViewportExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      if(get_handle2() != ::null())
         ::ScaleViewportExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      return size;
   }

   point graphics::SetWindowOrg(int32_t x, int32_t y)
   {
      point point(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::SetWindowOrgEx(get_handle1(), x, y, &point);
      if(get_handle2() != ::null())
         ::SetWindowOrgEx(get_handle2(), x, y, &point);
      return point;
   }

   point graphics::OffsetWindowOrg(int32_t nWidth, int32_t nHeight)
   {
      point point(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::OffsetWindowOrgEx(get_handle1(), nWidth, nHeight, &point);
      if(get_handle2() != ::null())
         ::OffsetWindowOrgEx(get_handle2(), nWidth, nHeight, &point);
      return point;
   }

   size graphics::SetWindowExt(int32_t x, int32_t y)
   {
      size size(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::SetWindowExtEx(get_handle1(), x, y, &size);
      if(get_handle2() != ::null())
         ::SetWindowExtEx(get_handle2(), x, y, &size);
      return size;
   }

   size graphics::ScaleWindowExt(int32_t xNum, int32_t xDenom, int32_t yNum, int32_t yDenom)
   {
      size size(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::ScaleWindowExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      if(get_handle2() != ::null())
         ::ScaleWindowExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      return size;
   }

   int32_t graphics::GetClipBox(LPRECT lpRect) const
   {
      return ::GetClipBox(get_handle1(), lpRect);
   }

   int32_t graphics::SelectClipRgn(::ca::region * pregion)
   {

      if(pregion == ::null())
      {
         m_pgraphics->ResetClip();
      }
      else
      {
         m_pgraphics->SetClip((Gdiplus::Region *) pregion->get_os_data());
      }

      return 0;

/*      int32_t nRetVal = ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SelectClipRgn(get_handle1(), pRgn == ::null() ? ::null() : (HRGN) pRgn->get_os_data());
      if(get_handle2() != ::null())
         nRetVal = ::SelectClipRgn(get_handle2(), pRgn == ::null() ? ::null() : (HRGN) pRgn->get_os_data());
      return nRetVal;*/
   }

   int32_t graphics::ExcludeClipRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      int32_t nRetVal = ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::ExcludeClipRect(get_handle1(), x1, y1, x2, y2);
      if(get_handle2() != ::null())
         nRetVal = ::ExcludeClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int32_t graphics::ExcludeClipRect(LPCRECT lpRect)
   {
      int32_t nRetVal = ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::ExcludeClipRect(get_handle1(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom);
      if(get_handle2() != ::null())
         nRetVal = ::ExcludeClipRect(get_handle2(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom);
      return nRetVal;
   }

   int32_t graphics::IntersectClipRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      int32_t nRetVal = ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::IntersectClipRect(get_handle1(), x1, y1, x2, y2);
      if(get_handle2() != ::null())
         nRetVal = ::IntersectClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int32_t graphics::IntersectClipRect(LPCRECT lpRect)
   {
      int32_t nRetVal = ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::IntersectClipRect(get_handle1(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
      if(get_handle2() != ::null())
         nRetVal = ::IntersectClipRect(get_handle2(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
      return nRetVal;
   }

   int32_t graphics::OffsetClipRgn(int32_t x, int32_t y)
   {
      int32_t nRetVal = ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::OffsetClipRgn(get_handle1(), x, y);
      if(get_handle2() != ::null())
         nRetVal = ::OffsetClipRgn(get_handle2(), x, y);
      return nRetVal;
   }

   int32_t graphics::OffsetClipRgn(SIZE size)
   {
      int32_t nRetVal = ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::OffsetClipRgn(get_handle1(), size.cx, size.cy);
      if(get_handle2() != ::null())
         nRetVal = ::OffsetClipRgn(get_handle2(), size.cx, size.cy);
      return nRetVal;
   }

   /*point graphics::MoveTo(int32_t x, int32_t y)
   {
      point point(0, 0);
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::MoveToEx(get_handle1(), x, y, &point);
      if(get_handle2() != ::null())
         ::MoveToEx(get_handle2(), x, y, &point);
      return point;
   }*/

   UINT graphics::SetTextAlign(UINT nFlags)
   {
      UINT nRetVal = GDI_ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         ::SetTextAlign(get_handle1(), nFlags);
      if(get_handle2() != ::null())
         nRetVal = ::SetTextAlign(get_handle2(), nFlags);
      return nRetVal;
   }

   int32_t graphics::SetTextJustification(int32_t nBreakExtra, int32_t nBreakCount)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SetTextJustification(get_handle1(), nBreakExtra, nBreakCount);
      if(get_handle2() != ::null())
         nRetVal = ::SetTextJustification(get_handle2(), nBreakExtra, nBreakCount);
      return nRetVal;
   }

   int32_t graphics::SetTextCharacterExtra(int32_t nCharExtra)
   {
      ASSERT(get_handle1() != ::null());
      int32_t nRetVal = 0x8000000;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         nRetVal = ::SetTextCharacterExtra(get_handle1(), nCharExtra);
      if(get_handle2() != ::null())
         nRetVal = ::SetTextCharacterExtra(get_handle2(), nCharExtra);
      return nRetVal;
   }

   uint32_t graphics::SetMapperFlags(uint32_t dwFlag)
   {
      ASSERT(get_handle1() != ::null());
      uint32_t dwRetVal = GDI_ERROR;
      if(get_handle1() != ::null() && get_handle1() != get_handle2())
         dwRetVal = ::SetMapperFlags(get_handle1(), dwFlag);
      if(get_handle2() != ::null())
         dwRetVal = ::SetMapperFlags(get_handle2(), dwFlag);
      return dwRetVal;
   }

   typedef uint32_t (CALLBACK* __GDIGETLAYOUTPROC)(HDC);
   typedef uint32_t (CALLBACK* __GDISETLAYOUTPROC)(HDC, uint32_t);

   uint32_t graphics::GetLayout() const
   {
      HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != ::null());
      uint32_t dwGetLayout = LAYOUT_LTR;
      __GDIGETLAYOUTPROC pfn;
      pfn = (__GDIGETLAYOUTPROC) GetProcAddress(hInst, "GetLayout");
      // if they API is available, just call it. If it is not
      // available, indicate an error.
      if (pfn != ::null())
         dwGetLayout = (*pfn)(get_handle1());
      else
      {
         dwGetLayout = GDI_ERROR;
         SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
      }
      return dwGetLayout;
   }

   uint32_t graphics::SetLayout(uint32_t dwSetLayout)
   {
      HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != ::null());
      uint32_t dwGetLayout = LAYOUT_LTR;
      __GDISETLAYOUTPROC pfn;
      pfn = (__GDISETLAYOUTPROC) GetProcAddress(hInst, "SetLayout");
      // If the API is availalbe, just call it. If it's not available,
      // setting anything other than LAYOUT_LTR is an error.
      if (pfn != ::null())
         dwGetLayout = (*pfn)(get_handle1(), dwSetLayout);
      else if (dwSetLayout != LAYOUT_LTR)
      {
         dwGetLayout = GDI_ERROR;
         SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
      }
      return dwGetLayout;
   }
   /*
   void window::ScreenToClient(LPRECT lpRect)
   {
   ASSERT(::IsWindow(get_handle1()));
   ::ScreenToClient(get_handle1(), (LPPOINT)lpRect);
   ::ScreenToClient(get_handle1(), ((LPPOINT)lpRect)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   rect::swap_left_right(lpRect);
   }

   void window::ClientToScreen(LPRECT lpRect)
   {
   ASSERT(::IsWindow(get_handle1()));
   ::ClientToScreen(get_handle1(), (LPPOINT)lpRect);
   ::ClientToScreen(get_handle1(), ((LPPOINT)lpRect)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   rect::swap_left_right(lpRect);
   }*/


   /////////////////////////////////////////////////////////////////////////////
   // Advanced Win32 GDI functions

   bool graphics::ArcTo(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4)
   {
      ASSERT(get_handle1() != ::null());
      bool bResult = ::ArcTo(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, ::null()));
      }
      return bResult;
   }

   int32_t graphics::SetArcDirection(int32_t nArcDirection)
   {
      ASSERT(get_handle1() != ::null());
      int32_t nResult = 0;
      if (get_handle1() != get_handle2())
         nResult = ::SetArcDirection(get_handle1(), nArcDirection);
      if (get_handle2() != ::null())
         nResult = ::SetArcDirection(get_handle2(), nArcDirection);
      return nResult;
   }

   bool graphics::PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int32_t nCount)
   {
      ASSERT(get_handle1() != ::null());
      bool bResult = ::PolyDraw(get_handle1(), lpPoints, lpTypes, nCount) != FALSE;
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, ::null()));
      }
      return bResult;
   }

   bool graphics::PolylineTo(const POINT* lpPoints, int32_t nCount)
   {
      ASSERT(get_handle1() != ::null());
      bool bResult = ::PolylineTo(get_handle1(), lpPoints, nCount) != FALSE;
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, ::null()));
      }
      return bResult;
   }

   bool graphics::SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
   {
      ASSERT(get_handle1() != ::null());
      bool bResult = FALSE;
      if (get_handle1() != get_handle2())
         bResult = ::SetColorAdjustment(get_handle1(), lpColorAdjust) != FALSE;
      if (get_handle2() != ::null())
         bResult = ::SetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE;
      return bResult;
   }

   bool graphics::PolyBezierTo(const POINT* lpPoints, int32_t nCount)
   {
      ASSERT(get_handle1() != ::null());
      bool bResult = ::PolyBezierTo(get_handle1(), lpPoints, nCount) != FALSE;
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, ::null()));
      }
      return bResult;
   }

   bool graphics::SelectClipPath(int32_t nMode)
   {
      ASSERT(get_handle1() != ::null());

      // output DC always holds the current path
      if (!::SelectClipPath(get_handle1(), nMode))
         return FALSE;

      // transfer clipping region into the attribute DC
      bool bResult = TRUE;
      if (get_handle1() != get_handle2())
      {
         HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
         if (::GetClipRgn(get_handle1(), hRgn) < 0 || !::SelectClipRgn(get_handle2(), hRgn))
         {
            TRACE(::ca::trace::category_AppMsg, 0, "Error: unable to transfer clip region in graphics::SelectClipPath!\n");
            bResult = FALSE;
         }
         ::DeleteObject(hRgn);
      }
      return bResult;
   }

   int32_t graphics::SelectClipRgn(::ca::region* pRgn, int32_t nMode)
   {
      ASSERT(get_handle1() != ::null());
      int32_t nRetVal = ERROR;
      if (get_handle1() != get_handle2())
         nRetVal = ::ExtSelectClipRgn(get_handle1(), (HRGN)pRgn->get_os_data(), nMode);
      if (get_handle2() != ::null())
         nRetVal = ::ExtSelectClipRgn(get_handle2(), (HRGN)pRgn->get_os_data(), nMode);
      return nRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special handling for metafile playback

   int32_t CALLBACK __enum_meta_file_procedure(HDC hDC,
      HANDLETABLE* pHandleTable, METARECORD* pMetaRec, int32_t nHandles, LPARAM lParam)
   {
      ::ca::graphics * pgraphics = (::ca::graphics *)lParam;
      ASSERT_VALID(pgraphics);

      switch (pMetaRec->rdFunction)
      {
         // these records have effects different for each graphics derived class
      case META_SETMAPMODE:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetMapMode((int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetWindowExt(
            (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWORG:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetWindowOrg(
            (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetViewportExt(
            (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTORG:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetViewportOrg(
            (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SCALEWINDOWEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->ScaleWindowExt(
            (int32_t)(int16_t)pMetaRec->rdParm[3], (int32_t)(int16_t)pMetaRec->rdParm[2],
            (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SCALEVIEWPORTEXT:
         (dynamic_cast<::win::graphics * >(pgraphics))->ScaleViewportExt(
            (int32_t)(int16_t)pMetaRec->rdParm[3], (int32_t)(int16_t)pMetaRec->rdParm[2],
            (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_OFFSETVIEWPORTORG:
         (dynamic_cast<::win::graphics * >(pgraphics))->OffsetViewportOrg(
            (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SAVEDC:
         (dynamic_cast<::win::graphics * >(pgraphics))->SaveDC();
         break;
      case META_RESTOREDC:
         (dynamic_cast<::win::graphics * >(pgraphics))->RestoreDC((int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETBKCOLOR:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetBkColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
         break;
      case META_SETTEXTCOLOR:
         (dynamic_cast<::win::graphics * >(pgraphics))->SetTextColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
         break;

         // need to watch out for SelectObject(HFONT), for custom font mapping
      case META_SELECTOBJECT:
         {
            HGDIOBJ hObject = pHandleTable->objectHandle[pMetaRec->rdParm[0]];
            UINT nObjType = GetObjectType(hObject);
            if (nObjType == 0)
            {
               // object type is unknown, determine if it is a font
               HFONT hStockFont = (HFONT)::GetStockObject(SYSTEM_FONT);
               HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hStockFont);
               HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hObject);
               if (hObjOld == hStockFont)
               {
                  // got the stock object back, so must be selecting a font
                  throw not_implemented(::ca::get_thread_app());
//                  (dynamic_cast<::win::graphics * >(pgraphics))->SelectObject(::win::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
                  break;  // don't play the default record
               }
               else
               {
                  // didn't get the stock object back, so restore everything
                  ::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hFontOld);
                  ::SelectObject((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1(), hObjOld);
               }
               // and fall through to PlayMetaFileRecord...
            }
            else if (nObjType == OBJ_FONT)
            {
               // play back as graphics::SelectObject(::ca::font*)
//               (dynamic_cast<::win::graphics * >(pgraphics))->SelectObject(::win::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
               throw not_implemented(::ca::get_thread_app());
               break;  // don't play the default record
            }
         }
         // fall through...

      default:
         ::PlayMetaFileRecord(hDC, pHandleTable, pMetaRec, nHandles);
         break;
      }

      return 1;
   }

   bool graphics::PlayMetaFile(HMETAFILE hMF)
   {
      if (::GetDeviceCaps(get_handle1(), TECHNOLOGY) == DT_METAFILE)
      {
         // playing metafile in metafile, just use core windows API
         return ::PlayMetaFile(get_handle1(), hMF) != FALSE;
      }

      // for special playback, lParam == pgraphics
      return ::EnumMetaFile(get_handle1(), hMF, __enum_meta_file_procedure, (LPARAM)this) != FALSE;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Coordinate transforms

   void graphics::LPtoDP(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));
   }

   void graphics::DPtoLP(LPSIZE lpSize) const
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));
   }



   int32_t graphics::draw_text(const char * lpszString, int32_t nCount, LPRECT lpRect, UINT nFormat)
   { 
      /*if(get_handle1() == ::null())
         return -1;
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::ca::international::utf8_to_unicode(string(lpszString, nCount));
      return ::DrawTextW(get_handle1(), wstr, (int32_t) wcslen(wstr), lpRect, nFormat); */

      return draw_text(string(lpszString, nCount), lpRect, nFormat);

   }

   int32_t graphics::draw_text(const string & str, LPRECT lpRect, UINT nFormat)
   { 
      
      /*if(get_handle1() == ::null())
         return -1;
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::ca::international::utf8_to_unicode(str);
      return ::DrawTextW(get_handle1(), (const wchar_t *)wstr, (int32_t)wcslen(wstr), lpRect, nFormat); */

      try
      {

         if(m_pgraphics == ::null())
            return FALSE;

         switch(m_etextrendering)
         {
         case ::ca::text_rendering_anti_alias:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
            break;
         case ::ca::text_rendering_anti_alias_grid_fit:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
            break;
         case ::ca::text_rendering_single_bit_per_pixel:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixel);
            break;
         case ::ca::text_rendering_clear_type_grid_fit:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
            break;
         }

      }
      catch(...)
      {
      }
      


      Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());


      format.SetFormatFlags(format.GetFormatFlags() 
                             | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                             | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);

      if(nFormat & DT_LEFT)
      {
         format.SetAlignment(Gdiplus::StringAlignmentNear);
      }
      else if(nFormat & DT_RIGHT)
      {
         format.SetAlignment(Gdiplus::StringAlignmentFar);
      }
      else if(nFormat & DT_CENTER)
      {
         format.SetAlignment(Gdiplus::StringAlignmentCenter);
      }
      else
      {
         format.SetAlignment(Gdiplus::StringAlignmentNear);
      }

      if(nFormat & DT_BOTTOM)
      {
         format.SetLineAlignment(Gdiplus::StringAlignmentFar);
      }
      else if(nFormat & DT_TOP)
      {
         format.SetLineAlignment(Gdiplus::StringAlignmentNear);
      }
      else if(nFormat & DT_VCENTER)
      {
         format.SetLineAlignment(Gdiplus::StringAlignmentCenter);
      }
      else
      {
         format.SetLineAlignment(Gdiplus::StringAlignmentNear);
      }

      //m_dFontSize             = fontSrc.m_dFontSize;

      Gdiplus::Matrix m;
      m_pgraphics->GetTransform(&m);

      Gdiplus::Matrix * pmNew = m.Clone();

      pmNew->Translate((Gdiplus::REAL) lpRect->left, (Gdiplus::REAL) lpRect->top);
      pmNew->Scale((Gdiplus::REAL) m_fontxyz.m_dFontWidth, (Gdiplus::REAL) 1.0, Gdiplus::MatrixOrderAppend);

      Gdiplus::RectF rectf(0, 0, (Gdiplus::REAL) ((lpRect->right - lpRect->left) * m_fontxyz.m_dFontWidth), (Gdiplus::REAL) (lpRect->bottom - lpRect->top));

      m_pgraphics->SetTransform(pmNew);

      m_pgraphics->DrawString(::ca::international::utf8_to_unicode(str), -1, gdiplus_font(), rectf, &format, gdiplus_brush());

      m_pgraphics->SetTransform(&m);

      delete pmNew;

      return 1;

   }

   int32_t graphics::draw_text_ex(LPTSTR lpszString, int32_t nCount, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 
      if(get_handle1() == ::null())
         return -1;
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::ca::international::utf8_to_unicode(string(lpszString, nCount));
      return ::DrawTextExW(get_handle1(), const_cast<wchar_t *>((const wchar_t *)wstr), (int32_t)wcslen(wstr), lpRect, nFormat, lpDTParams); 
   }

   int32_t graphics::draw_text_ex(const string & str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 
      ASSERT(get_handle1() != ::null());
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::ca::international::utf8_to_unicode(str);
      return ::DrawTextExW(get_handle1(), const_cast<wchar_t *>((const wchar_t *)wstr), (int32_t)wcslen(wstr), lpRect, nFormat, lpDTParams); 
   }

   size graphics::GetTextExtent(const char * lpszString, strsize nCount, int32_t iIndex) const
   {

      if(lpszString == ::null() || *lpszString == '\0')
         return size(0, 0);

      if(nCount < 0)
         nCount = strlen(lpszString);

      if(iIndex > nCount)
         return size(0, 0);

      if(iIndex < 0)
         return size(0, 0);

      wstring wstr = ::ca::international::utf8_to_unicode(lpszString, nCount);

      strsize iRange = 0;
      strsize i = 0;
      strsize iLen;
      const char * psz = lpszString;
      while(i < iIndex)
      {
         iLen = ::ca::str::get_utf8_char(psz).length();
         iRange++;
         i += iLen;
         psz = ::ca::str::utf8_inc(psz);
         if(psz == ::null())
            break;
         if(*psz == '\0')
            break;
      }

      Gdiplus::CharacterRange charRanges[1] = { Gdiplus::CharacterRange(0, (INT) iRange) }; 

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());
      //Gdiplus::StringFormat strFormat;

      strFormat.SetMeasurableCharacterRanges(1, charRanges);

      strFormat.SetFormatFlags(strFormat.GetFormatFlags() 
                             | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                             | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);

      int32_t count = strFormat.GetMeasurableCharacterRangeCount();

      Gdiplus::Region * pCharRangeRegions = new Gdiplus::Region[count];

      Gdiplus::RectF box(0.0f, 0.0f, 128.0f * 1024.0f, 128.0f * 1024.0f);

      Gdiplus::PointF origin(0, 0);

      //m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, Gdiplus::StringFormat::GenericTypographic(), &box);

      ((graphics *)this)->m_pgraphics->MeasureCharacterRanges(wstr, (INT) wstr.get_length(), ((graphics *)this)->gdiplus_font(), box, &strFormat, (INT) count, pCharRangeRegions);

      Gdiplus::Region * pregion = ::null();


      if(count > 0)
      {

          pregion = pCharRangeRegions[0].Clone();

      }



      for(i = 1; i < count; i++)
      {
         pregion->Union(&pCharRangeRegions[i]);
      }


      if(pregion == ::null())
         return size(0, 0);

      delete [] pCharRangeRegions;


      Gdiplus::RectF rectBound;

      pregion->GetBounds(&rectBound, m_pgraphics);

      delete pregion;

      

      Gdiplus::SizeF size;

      rectBound.GetSize(&size);

      return class ::size((int64_t) (size.Width * m_fontxyz.m_dFontWidth), (int64_t) (size.Height));

   }

   size graphics::GetTextExtent(const char * lpszString, strsize nCount) const
   {

      //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

      wstring wstr = ::ca::international::utf8_to_unicode(lpszString, nCount);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

      strFormat.SetFormatFlags(strFormat.GetFormatFlags() 
                             | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                             | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);

      m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, &strFormat,  &box);

      return size((int64_t) (box.Width * m_fontxyz.m_dFontWidth), (int64_t) (box.Height));

      /*if(get_handle2() == ::null())
         return size(0, 0);
      SIZE size;
      string str(lpszString, nCount);
      wstring wstr = ::ca::international::utf8_to_unicode(str);
      if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int32_t)wstr.get_length(), &size))
      {
         return class size(0, 0);
      }
      return size;*/
   }

   size graphics::GetTextExtent(const string & str) const
   {
/*      if(get_handle2() == ::null())
         return size(0, 0);
      SIZE size;
      wstring wstr = ::ca::international::utf8_to_unicode(str);
      if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int32_t)wstr.get_length(), &size))
      {
         return class size(0, 0);
      }
      return size;*/

      class sized size;

      if(!GetTextExtent(size, str, (int32_t) str.get_length(), (int32_t) str.get_length()))
         return class size(0, 0);

      return class size((long) size.cx, (long) size.cy);

      /*if(m_pgraphics == ::null())
         return size(0, 0);

      wstring wstr = ::ca::international::utf8_to_unicode(str);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);


      if(m_pgraphics == ::null())
         return size(0, 0);

      try
      {
         m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, &box);
      }
      catch(...)
      {
         return size(0, 0);
      }

      return size((int64_t) (box.Width * m_fontxyz.m_dFontWidth), (int64_t) box.Height);*/

   }

   size graphics::GetOutputTextExtent(const char * lpszString, strsize nCount) const
   {
      ASSERT(get_handle1() != ::null());
      SIZE size;
      string str(lpszString, nCount);
      wstring wstr = ::ca::international::utf8_to_unicode(str);
      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int32_t)wstr.get_length(), &size));
      return size;
   }

   size graphics::GetOutputTextExtent(const string & str) const
   {
      ASSERT(get_handle1() != ::null());
      SIZE size;
      wstring wstr = ::ca::international::utf8_to_unicode(str);
      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int32_t)wstr.get_length(), &size));
      return size;
   }

   bool graphics::GetTextExtent(sized & size, const char * lpszString, strsize nCount, int32_t iIndex) const
   {

      //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

      if(lpszString == ::null() || *lpszString == '\0')
         return false;

      if(nCount < 0)
         nCount = strlen(lpszString);

      if(iIndex > nCount)
         return false;

      if(iIndex < 0)
         return false;

      wstring wstr = ::ca::international::utf8_to_unicode(lpszString, nCount);

      strsize iRange = 0;
      strsize i = 0;
      strsize iLen;
      const char * psz = lpszString;
      while(i < iIndex)
      {
         try
         {
            iLen = ::ca::str::get_utf8_char(psz).length();
         }
         catch(...)
         {
            break;
         }
         iRange++;
         i += iLen;
         if(i >= iIndex)
            break;
         try
         {
            psz = ::ca::str::utf8_inc(psz);
         }
         catch(...)
         {
            break;
         }
         if(psz == ::null())
            break;
         if(*psz == '\0')
            break;
      }

      Gdiplus::CharacterRange charRanges[1] = { Gdiplus::CharacterRange(0, (INT) iRange) }; 

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());
      //Gdiplus::StringFormat strFormat;

      strFormat.SetMeasurableCharacterRanges(1, charRanges);

      strFormat.SetFormatFlags(strFormat.GetFormatFlags() 
                             | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                             | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);

      int32_t count = strFormat.GetMeasurableCharacterRangeCount();

      Gdiplus::Region * pCharRangeRegions = new Gdiplus::Region[count];

      //Gdiplus::RectF box(0.0f, 0.0f, 128.0f * 1024.0f, 128.0f * 1024.0f);

      Gdiplus::PointF origin(0, 0);


      // Generate a layout rect for the text

      Gdiplus::RectF layoutRect;
      Gdiplus::Status status = ((graphics *)this)->m_pgraphics->MeasureString( wstr, (INT) nCount, ((graphics *)this)->gdiplus_font(), origin, &layoutRect );


      // Prevent clipping

      //StringFormat strFormat( StringFormat::GenericTypographic() );
      //status = ((graphics *)this)->m_pgraphics->SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsNoClip );



      //m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, Gdiplus::StringFormat::GenericTypographic(), &box);

      ((graphics *)this)->m_pgraphics->MeasureCharacterRanges(wstr, (INT) nCount, ((graphics *)this)->gdiplus_font(), layoutRect, &strFormat, (INT) count, pCharRangeRegions);

      Gdiplus::Region * pregion = ::null();


      if(count > 0)
      {

          pregion = pCharRangeRegions[0].Clone();

      }

      for(i = 1; i < count; i++)
      {
         pregion->Union(&pCharRangeRegions[i]);
      }

      delete [] pCharRangeRegions;

      if(pregion == ::null())
         return false;

      Gdiplus::RectF rectBound;

      pregion->GetBounds(&rectBound, m_pgraphics);

      delete pregion;

      Gdiplus::SizeF sizef;

      rectBound.GetSize(&sizef);

      size.cx = sizef.Width * m_fontxyz.m_dFontWidth;

      size.cy = sizef.Height;

      return true;
   }

   bool graphics::GetTextExtent(sized & size, const char * lpszString, strsize nCount) const
   {

      //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

      wstring wstr = ::ca::international::utf8_to_unicode(lpszString, nCount);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

      strFormat.SetFormatFlags(strFormat.GetFormatFlags() 
                             | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                             | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);
      bool bOk = true;

      try
      {
         if(m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, &strFormat,  &box) != Gdiplus::Status::Ok)
            bOk = false;
      }
      catch(...)
      {
         bOk = false;
      }

      if(!bOk)
         return false;

      size.cx = box.Width * m_fontxyz.m_dFontWidth;

      size.cy = box.Height;

      return true;

   }

   bool graphics::GetTextExtent(sized & size, const string & str) const
   {

      if(m_pgraphics == ::null())
         return false;

      wstring wstr = ::ca::international::utf8_to_unicode(str);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);


      if(m_pgraphics == ::null())
         return false;

      bool bOk = true;

      try
      {
         if(m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, &box) != Gdiplus::Status::Ok)
            bOk = false;
      }
      catch(...)
      {
         bOk = false;
      }

      if(!bOk)
         return false;

      size.cx = box.Width * m_fontxyz.m_dFontWidth;

      size.cy = box.Height;

      return true;

   }

   



} // namespace win



/*hdc_map* afxMapHDC(bool bCreate)
{
   UNREFERENCED_PARAMETER(bCreate);
   try
   {
      __MODULE_STATE* pState = __get_module_state();
      if(pState == ::null())
         return ::null();
      return pState->m_pmapHDC;
   }
   catch(...)
   {
      return ::null();
   }

}*/



namespace win
{

   void graphics::FillSolidRect(LPCRECT lpRect, COLORREF clr)
   {

      //g.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);
      
      set_color(clr);

      m_pgraphics->FillRectangle(gdiplus_brush(), lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);

      //::SetBkColor(get_handle1(), clr);
      //::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, lpRect, ::null(), 0, ::null());
   }

   void graphics::FillSolidRect(int32_t x, int32_t y, int32_t cx, int32_t cy, COLORREF clr)
   {
      //g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //g().SetCompositingQuality(Gdiplus::CompositingQualityGammaCorrected);


      try
      {

         if(m_pgraphics == ::null())
            return;

         set_color(clr);

         m_pgraphics->FillRectangle(gdiplus_brush(), x, y, cx, cy);

      }
      catch(...)
      {
         return;
      }

   }


   bool graphics::TextOut(int32_t x, int32_t y, const char * lpszString, int32_t nCount)
   {

      ::Gdiplus::PointF origin(0, 0);

      string str(lpszString, nCount);
      
      wstring wstr = ::ca::international::utf8_to_unicode(str);


      try
      {

         if(m_pgraphics == ::null())
            return FALSE;

         switch(m_etextrendering)
         {
         case ::ca::text_rendering_anti_alias:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
            break;
         case ::ca::text_rendering_anti_alias_grid_fit:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
            break;
         case ::ca::text_rendering_single_bit_per_pixel:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixel);
            break;
         case ::ca::text_rendering_clear_type_grid_fit:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
            break;
         }

      }
      catch(...)
      {
      }

      
      //
      //m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
      //m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
      //m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
   
      Gdiplus::Matrix m;
      m_pgraphics->GetTransform(&m);

      Gdiplus::Matrix * pmNew;

      if(m_ppath != ::null())
      {
         pmNew = new Gdiplus::Matrix();
      }
      else
      {
         pmNew = m.Clone();
      }

      pmNew->Translate((Gdiplus::REAL)  (x / m_fontxyz.m_dFontWidth), (Gdiplus::REAL) y);
      pmNew->Scale((Gdiplus::REAL) m_fontxyz.m_dFontWidth, (Gdiplus::REAL) 1.0, Gdiplus::MatrixOrderAppend);

      Gdiplus::Status status;

      Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());

      format.SetFormatFlags(format.GetFormatFlags() 
                        | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                        | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap
                        | Gdiplus::StringFormatFlagsNoFitBlackBox);


      format.SetLineAlignment(Gdiplus::StringAlignmentNear);

      if(m_ppath != ::null())
      {

         Gdiplus::GraphicsPath path;
         
         Gdiplus::FontFamily fontfamily;

         gdiplus_font()->GetFamily(&fontfamily);

         double d1 = gdiplus_font()->GetSize() * m_pgraphics->GetDpiX() / 72.0;
         double d2 = fontfamily.GetEmHeight(gdiplus_font()->GetStyle());
         double d3 = d1 * d2;

         status = path.AddString(::ca::international::utf8_to_unicode(str), -1, &fontfamily, gdiplus_font()->GetStyle(), (Gdiplus::REAL) d1, origin, &format);

         path.Transform(pmNew);


         m_ppath->AddPath(&path, FALSE);

      }
      else
      {

         m_pgraphics->SetTransform(pmNew);

         status = m_pgraphics->DrawString(::ca::international::utf8_to_unicode(str), -1, gdiplus_font(), origin, &format, gdiplus_brush());

         m_pgraphics->SetTransform(&m);

      }

      delete pmNew;

      return status  == Gdiplus::Status::Ok;

   }

   bool graphics::TextOut(double x, double y, const char * lpszString, int32_t nCount)
   {

      ::Gdiplus::PointF origin(0, 0);

      string str(lpszString, nCount);
      
      wstring wstr = ::ca::international::utf8_to_unicode(str);


      try
      {

         if(m_pgraphics == ::null())
            return FALSE;

         switch(m_etextrendering)
         {
         case ::ca::text_rendering_anti_alias:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
            break;
         case ::ca::text_rendering_anti_alias_grid_fit:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
            break;
         case ::ca::text_rendering_single_bit_per_pixel:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixel);
            break;
         case ::ca::text_rendering_clear_type_grid_fit:
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
            break;
         }

      }
      catch(...)
      {
      }

      
      //
      //m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
      //m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
      //m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
      //m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
   
      Gdiplus::Matrix m;
      m_pgraphics->GetTransform(&m);

      Gdiplus::Matrix * pmNew;

      if(m_ppath != ::null())
      {
         pmNew = new Gdiplus::Matrix();
      }
      else
      {
         pmNew = m.Clone();
      }

      pmNew->Translate((Gdiplus::REAL)  (x / m_fontxyz.m_dFontWidth), (Gdiplus::REAL) y);
      pmNew->Scale((Gdiplus::REAL) m_fontxyz.m_dFontWidth, (Gdiplus::REAL) 1.0, Gdiplus::MatrixOrderAppend);

      Gdiplus::Status status;

      Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());

      format.SetFormatFlags(format.GetFormatFlags() 
                        | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                        | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap
                        | Gdiplus::StringFormatFlagsNoFitBlackBox);


      format.SetLineAlignment(Gdiplus::StringAlignmentNear);

      if(m_ppath != ::null())
      {

         Gdiplus::GraphicsPath path;
         
         Gdiplus::FontFamily fontfamily;

         gdiplus_font()->GetFamily(&fontfamily);

         double d1 = gdiplus_font()->GetSize() * m_pgraphics->GetDpiX() / 72.0;
         double d2 = fontfamily.GetEmHeight(gdiplus_font()->GetStyle());
         double d3 = d1 * d2;

         status = path.AddString(::ca::international::utf8_to_unicode(str), -1, &fontfamily, gdiplus_font()->GetStyle(), (Gdiplus::REAL) d1, origin, &format);

         path.Transform(pmNew);


         m_ppath->AddPath(&path, FALSE);

      }
      else
      {

         m_pgraphics->SetTransform(pmNew);

         status = m_pgraphics->DrawString(::ca::international::utf8_to_unicode(str), -1, gdiplus_font(), origin, &format, gdiplus_brush());

         m_pgraphics->SetTransform(&m);

      }

      delete pmNew;

      return status  == Gdiplus::Status::Ok;

   }



   bool graphics::LineTo(double x, double y)
   {

//      ::Gdiplus::Pen pen(::Gdiplus::Color(GetAValue(m_crColor), GetRValue(m_crColor), GetGValue(m_crColor), GetBValue(m_crColor)), m_dPenWidth);

      gdiplus_pen()->SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);

      m_pgraphics->DrawLine(gdiplus_pen(), Gdiplus::PointF((Gdiplus::REAL) m_x, (Gdiplus::REAL)  m_y), Gdiplus::PointF((Gdiplus::REAL) x, (Gdiplus::REAL) y));

      m_x = x;
      m_y = y;

      return TRUE;

   }


   void graphics::set_alpha_mode(::ca::e_alpha_mode ealphamode)
   {

      try
      {

         if(m_pgraphics == ::null())
            return;

         ::ca::graphics::set_alpha_mode(ealphamode);
         if(m_ealphamode == ::ca::alpha_mode_blend)
         {
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
         }
         else if(m_ealphamode == ::ca::alpha_mode_set)
         {
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
         }

      }
      catch(...)
      {
         return;
      }

   }


   void graphics::set_text_rendering(::ca::e_text_rendering etextrendering)
   {
      m_etextrendering = etextrendering;

   }


   void * graphics::get_os_data() const
   {

      return (void *) m_pgraphics;

   }


   HDC graphics::get_handle() const
   {
      return m_hdc;
   }

   HDC graphics::get_handle1() const
   {
      return get_handle();
   }

   HDC graphics::get_handle2() const
   {
      return get_handle();
   }

   bool graphics::attach(simple_graphics & g)
   {
      
      if(m_pgraphics != ::null())
      {

         try
         {
      
            delete m_pgraphics;

         }
         catch(...)
         {
            
            TRACE("graphics::attach : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = ::null();

      }

      m_pgraphics = g.m_pgraphics;

      return false;

   }

   bool graphics::attach(void * pdata)
   {

      if(m_pgraphics != ::null())
      {

         try
         {
      
            delete m_pgraphics;

         }
         catch(...)
         {
            
            TRACE("graphics::attach : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = ::null();

      }

      m_pgraphics = (Gdiplus::Graphics *) pdata;

      return false;

   }


   void * graphics::detach()
   {
      
      Gdiplus::Graphics * pgraphics = m_pgraphics;

      m_pgraphics = ::null();

      m_hdc = ::null();

      return pgraphics;

   }

   Gdiplus::Font * graphics::gdiplus_font()
   {
      if(m_spfont.is_null())
      {
         m_spfont.create(allocer());
         m_spfont->operator=(m_fontxyz);
      }
      else if(!m_fontxyz.m_bUpdated)
      {
         m_fontxyz.m_bUpdated = true;
         m_spfont->operator=(m_fontxyz);
      }
      return (Gdiplus::Font *) m_spfont->get_os_data();      
   }

   Gdiplus::Brush * graphics::gdiplus_brush()
   {
      if(m_spbrush.is_null())
      {
         m_spbrush.create(allocer());
         m_spbrush->operator=(m_brushxyz);
      }
      else if(!m_brushxyz.m_bUpdated)
      {
         m_brushxyz.m_bUpdated = true;
         m_spbrush->operator=(m_brushxyz);
      }
      return (Gdiplus::Brush *) m_spbrush->get_os_data();      
   }

   Gdiplus::Pen * graphics::gdiplus_pen()
   {
      if(m_sppen.is_null())
      {
         m_sppen.create(allocer());
         m_sppen->operator=(m_penxyz);
      }
      else if(!m_penxyz.m_bUpdated)
      {
         m_penxyz.m_bUpdated = true;
         m_sppen->operator=(m_penxyz);
      }
      return (Gdiplus::Pen *) m_sppen->get_os_data();      
   }

   Gdiplus::FillMode graphics::gdiplus_get_fill_mode()
   {
      return Gdiplus::FillModeWinding;
   }

   bool graphics::blur(bool bExpand, double dRadius, LPCRECT lpcrect)
   {

      if(m_bitmap.is_null() || m_bitmap->get_os_data() == ::null())
         return false;

      Gdiplus::BlurParams myBlurParams;

      myBlurParams.expandEdge    = bExpand ? 1 : 0;
      myBlurParams.radius        = (float) dRadius;

      Gdiplus::Blur myBlur;
      myBlur.SetParameters(&myBlurParams);

      

      Gdiplus::Matrix m;
      m_pgraphics->GetTransform(&m);

      Gdiplus::PointF points[2];

      points[0].X    = (Gdiplus::REAL) lpcrect->left;
      points[0].Y    = (Gdiplus::REAL) lpcrect->top;
      points[1].X    = (Gdiplus::REAL) lpcrect->right;
      points[1].Y    = (Gdiplus::REAL) lpcrect->bottom;

      m.TransformPoints(points, 2);

      //Gdiplus::RectF rectf(points[0].X, points[0].Y, points[1].X - points[0].X, points[1].Y - points[0].Y);

      RECT rect;

      rect.left      = (LONG) points[0].X;
      rect.top       = (LONG) points[0].Y;
      rect.right     = (LONG) points[1].X;
      rect.bottom    = (LONG) points[1].Y;

      Gdiplus::Bitmap * pbitmap = ((Gdiplus::Bitmap *) m_bitmap->get_os_data());

      pbitmap->ApplyEffect(&myBlur, &rect);

      return true;

   }


   double graphics::get_dpix() const
   {

      return m_pgraphics->GetDpiX();

   }


   bool graphics::flush()
   {
      
      m_pgraphics->Flush();

      return true;

   }

   HDC graphics::get_hdc()
   {
      
      if(m_hdc != ::null())
         return m_hdc;

      if(m_pgraphics == ::null())
         return ::null();

      return m_pgraphics->GetHDC();

   }
   
   void graphics::release_hdc(HDC hdc)
   {
      
      if(m_hdc != ::null())
         return;

      m_pgraphics->ReleaseHDC(hdc);

   }

} // namespace win