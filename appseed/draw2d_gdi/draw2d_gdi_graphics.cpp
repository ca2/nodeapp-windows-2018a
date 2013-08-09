#include "framework.h"


CLASS_DECL_THREAD ::draw2d::dibmap_ex1 * t_pdibmap = NULL;


inline ::size rect_size(LPCRECT lpcrect)
{

   return ::size(width(lpcrect), height(lpcrect));

}


namespace draw2d_gdi
{


   graphics::graphics(::ca2::application * papp) :
      ca2(papp)
   {


      m_hdc                = NULL;
      m_bPrinting          = FALSE;
      m_pdibAlphaBlend     = NULL;

   }


   /*graphics::graphics()
   {

   m_hdc                = NULL;
   m_bPrinting          = FALSE;
   m_pdibAlphaBlend     = NULL;

   }*/


   graphics::~graphics()
   {

      mutex_lock ml(user_mutex());

      for(int i = 0; i < m_ptraObject.get_count(); i++)
      {

         try
         {

            m_ptraObject[i]->m_ptraGraphics.remove(this);

         }
         catch(...)
         {
         }

      }

      if (get_handle1() != NULL)
      {

         ::DeleteDC(Detach());

      }

   }


   graphics::operator HDC() const
   { 

      return this == NULL ? NULL : (HDC) get_os_data();

   }




   HDC graphics::get_handle1() const
   { 

      return this == NULL ? NULL : (HDC) m_hdc; 

   }

   HDC graphics::get_handle2() const
   { 

      return this == NULL ? NULL : (HDC) m_hdc; 

   }

   void * graphics::get_os_data() const
   {

      return (void *) get_handle1();

   }

   void * graphics::get_os_data_ex(int i) const
   {
      switch((e_data) i)
      {
      case data_graphics:
         return m_hdc;
      case data_bitmap:
         return ::GetCurrentObject(m_hdc, OBJ_BITMAP);
      case data_pen:
         return ::GetCurrentObject(m_hdc, OBJ_PEN);
      case data_brush:
         return ::GetCurrentObject(m_hdc, OBJ_BRUSH);
      case data_font:
         return ::GetCurrentObject(m_hdc, OBJ_FONT);
      case data_palette:
         return ::GetCurrentObject(m_hdc, OBJ_PAL);
      case data_region:
         return ::GetCurrentObject(m_hdc, OBJ_REGION);
      default:
         break;
      };

      return ::draw2d::graphics::get_os_data_ex(i);

   }


   HDC graphics::get_handle() const
   {

      return get_handle1();

   }


   bool graphics::IsPrinting() const
   {

      return m_bPrinting; 

   }


   bool graphics::CreateDC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   {

      return Attach(::CreateDC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*)lpInitData)) != FALSE;

   }


   bool graphics::CreateIC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData)
   { 

      return Attach(::CreateIC(lpszDriverName, lpszDeviceName, lpszOutput, (const DEVMODE*) lpInitData)) != FALSE;

   }


   bool graphics::CreateCompatibleDC(::draw2d::graphics * pgraphics)
   { 

      if(pgraphics == NULL)
      {

         return Attach(::CreateCompatibleDC(NULL)) != FALSE;

      }
      else
      {

         return Attach(::CreateCompatibleDC((HDC)(dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1())) != FALSE;

      }

   }


   int graphics::ExcludeUpdateRgn(::ca2::window * pWnd)
   { 

      ASSERT(get_handle1() != NULL); 

      return ::ExcludeUpdateRgn(get_handle1(), (oswindow) pWnd->get_handle()) ;

   }


   int graphics::GetDeviceCaps(int nIndex) const
   { 

      ASSERT(get_handle2() != NULL); 

      return ::GetDeviceCaps(get_handle2(), nIndex) ;

   }


   point graphics::GetBrushOrg() const
   {

      ASSERT(get_handle1() != NULL);

      POINT point;

      VERIFY(::GetBrushOrgEx(get_handle1(), &point));

      return point;

   }


   point graphics::SetBrushOrg(int x, int y)
   {

      ASSERT(get_handle1() != NULL);

      POINT point;

      VERIFY(::SetBrushOrgEx(get_handle1(), x, y, &point));

      return point;

   }


   point graphics::SetBrushOrg(POINT point)
   {

      ASSERT(get_handle1() != NULL);

      VERIFY(::SetBrushOrgEx(get_handle1(), point.x, point.y, &point));

      return point;

   }


   int graphics::EnumObjects(int nObjectType, int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   {

      ASSERT(get_handle2() != NULL);

      return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData) ;

   }


   ::draw2d::bitmap * graphics::SelectObject(::draw2d::bitmap * pbitmap)
   { 

      if(get_handle1() == NULL)
         return NULL;

      if(pbitmap == NULL)
         return NULL;

      mutex_lock ml(user_mutex());

      pbitmap->m_bUpdated = true;

      ::SelectObject(get_handle1(), pbitmap->get_os_data());

      GDI_BITMAP(pbitmap)->m_ptraGraphics.add_unique(this);

      m_ptraObject.add_unique(GDI_BITMAP(pbitmap));

      m_spbitmap = pbitmap;

      return m_spbitmap;

   }



   HGDIOBJ graphics::SelectObject(HGDIOBJ hObject) // Safe for NULL handles
   {

      ASSERT(get_handle1() == get_handle2()); // ASSERT a simple graphics object

      return (hObject != NULL) ? ::SelectObject(get_handle1(), hObject) : NULL; 

   }

   COLORREF graphics::GetNearestColor(COLORREF crColor) const
   { 

      return ::GetNearestColor(get_handle2(), crColor) ;

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



   int graphics::GetBkMode() const
   { 

      return ::GetBkMode(get_handle2());

   }



   int graphics::GetPolyFillMode() const
   { 

      return ::GetPolyFillMode(get_handle2());

   }



   int graphics::GetROP2() const
   { 

      return ::GetROP2(get_handle2());

   }



   int graphics::GetStretchBltMode() const
   { 

      return ::GetStretchBltMode(get_handle2());

   }




   COLORREF graphics::GetTextColor() const
   { 

      return ::GetTextColor(get_handle2()); 

   }


   int graphics::GetMapMode() const
   { 

      return ::GetMapMode(get_handle2()); 

   }


   int graphics::GetGraphicsMode() const
   { 

      return ::GetGraphicsMode(get_handle2()); 

   }


   bool graphics::GetWorldTransform(XFORM* pXform) const
   { 

      return ::GetWorldTransform(get_handle2(),pXform) != FALSE; 

   }


   point graphics::GetViewportOrg() const
   {

      POINT point;

      ::GetViewportOrgEx(get_handle2(), &point);

      return point;

   }


   class size graphics::GetViewportExt() const
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


   class size graphics::GetWindowExt() const
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




   class size graphics::SetViewportExt(SIZE size)
   {

      return SetViewportExt(size.cx, size.cy);

   }




   point graphics::SetWindowOrg(POINT point)
   {

      return SetWindowOrg(point.x, point.y);

   }




   class size graphics::SetWindowExt(SIZE size)
   {

      return SetWindowExt(size.cx, size.cy);

   }




   void graphics::DPtoLP(LPPOINT lpPoints, int nCount) const
   { 

      ::DPtoLP(get_handle2(), lpPoints, nCount);

   }




   void graphics::DPtoLP(LPRECT lpRect) const
   { 

      ::DPtoLP(get_handle2(), (LPPOINT)lpRect, 2);

   }




   void graphics::LPtoDP(LPPOINT lpPoints, int nCount) const
   {

      ::LPtoDP(get_handle2(), lpPoints, nCount);

   }




   void graphics::LPtoDP(LPRECT lpRect) const
   {

      ::LPtoDP(get_handle2(), (LPPOINT)lpRect, 2);

   }




   bool graphics::FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush)
   { 

      return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != FALSE; 

   }


   bool graphics::FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, int nWidth, int nHeight)
   { 

      return ::FrameRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data(), nWidth, nHeight) != FALSE; 

   }


   bool graphics::InvertRgn(::draw2d::region* pRgn)
   {

      ASSERT(get_handle1() != NULL); 

      return ::InvertRgn(get_handle1(), (HRGN)pRgn->get_os_data()) != FALSE; 

   }


   bool graphics::PaintRgn(::draw2d::region* pRgn)
   {

      ASSERT(get_handle1() != NULL); 

      return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data()) != FALSE; 

   }


   bool graphics::PtVisible(int x, int y) const
   { 

      ASSERT(get_handle1() != NULL); 

      return ::PtVisible(get_handle1(), x, y) != FALSE; 

   }


   bool graphics::PtVisible(POINT point) const
   { 

      ASSERT(get_handle1() != NULL); 

      return PtVisible(point.x, point.y) != FALSE; 

   }


   bool graphics::RectVisible(LPCRECT lpRect) const
   { 

      ASSERT(get_handle1() != NULL);

      return ::RectVisible(get_handle1(), lpRect) != FALSE; 

   }


   point graphics::GetCurrentPosition() const
   {

      ASSERT(get_handle2() != NULL);

      POINT point;

      VERIFY(::GetCurrentPositionEx(get_handle2(), &point));

      return point;

   }


   point graphics::MoveTo(POINT point)
   { 

      ASSERT(get_handle1() != NULL); 

      return MoveTo(point.x, point.y); 

   }


   bool graphics::LineTo(POINT point)
   {

      ASSERT(get_handle1() != NULL); 

      return LineTo(point.x, point.y); 

   }


   bool graphics::Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {

      ASSERT(get_handle1() != NULL); 

      return ::Arc(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE; 

   }


   bool graphics::Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   { 

      ASSERT(get_handle1() != NULL); 

      return ::Arc(get_handle1(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y) != FALSE; 

   }


   bool graphics::Polyline(const POINT* lpPoints, int nCount)
   {

      ASSERT(get_handle1() != NULL); 

      return ::Polyline(get_handle1(), lpPoints, nCount) != FALSE; 

   }


   void graphics::FillRect(LPCRECT lpRect, ::draw2d::brush* pBrush)
   { 

      ASSERT(get_handle1() != NULL); 

      ::FillRect(get_handle1(), lpRect, (HBRUSH)pBrush->get_os_data());

   }




   void graphics::FrameRect(LPCRECT lpRect, ::draw2d::brush* pBrush)
   { 

      ASSERT(get_handle1() != NULL); 

      ::FrameRect(get_handle1(), lpRect, (HBRUSH)pBrush->get_os_data());

   }




   void graphics::InvertRect(LPCRECT lpRect)
   {

      ASSERT(get_handle1() != NULL); 

      ::InvertRect(get_handle1(), lpRect);

   }





   bool graphics::DrawIcon(int x, int y, ::visual::icon * picon)
   {

      ASSERT(get_handle1() != NULL); 

      if(picon == NULL)
         return FALSE;

      return false;
      //      return ::DrawIcon(get_handle1(), x, y, picon->m_hicon)) != FALSE; 

   }


   bool graphics::DrawIcon(POINT point, ::visual::icon * picon)
   { 

      ASSERT(get_handle1() != NULL); 

      if(picon == NULL)
         return FALSE;

      return false;
      //return ::DrawIcon(get_handle1(), point.x, point.y, picon->m_hicon) != FALSE; 

   }

   bool graphics::DrawIcon(int x, int y, ::visual::icon * picon, int cx, int cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
   { 

      ASSERT(get_handle1() != NULL);

      if(picon == NULL)
         return FALSE;

      if(m_pdib == NULL)
      {

         return ::DrawIconEx(get_handle1(), x, y, (HICON) picon->m_picon, cx, cy, istepIfAniCur, hbrFlickerFreeDraw, diFlags) != FALSE;

      }
      else
      {

         ::draw2d::dib_sp dib(allocer());

         dib->create(cx,cy);

         dib->Fill(0, 0, 0, 0);

         ::DrawIconEx((HDC)dib->get_graphics()->get_os_data(), 0, 0, (HICON) picon->m_picon, cx, cy, istepIfAniCur, NULL, DI_IMAGE | DI_MASK);

         dib->div_alpha();

         m_pdib->from(point(x, y), dib, point(0, 0), size(cx, cy));

      }

   }


   bool graphics::DrawState(point pt, class size size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), hBrush,
         NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;

   }


   bool graphics::DrawState(point pt, class size size, ::draw2d::bitmap* pBitmap, UINT nFlags, ::draw2d::brush* pBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
         NULL, (LPARAM)pBitmap->get_os_data(), 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;

   }


   bool graphics::DrawState(point pt, class size size, HICON hIcon, UINT nFlags, HBRUSH hBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), hBrush, NULL,
         (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE;

   }


   bool graphics::DrawState(point pt, class size size, HICON hIcon, UINT nFlags, ::draw2d::brush* pBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL,
         (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE;

   }


   bool graphics::DrawState(point pt, class size size, const char * lpszText, UINT nFlags, bool bPrefixText, int nTextLen, HBRUSH hBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), hBrush,
         NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }


   bool graphics::DrawState(point pt, class size size, const char * lpszText, UINT nFlags, bool bPrefixText, int nTextLen, ::draw2d::brush* pBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
         NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }


   bool graphics::DrawState(point pt, class size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), hBrush,
         lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE;

   }


   bool graphics::DrawState(point pt, class size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
         lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE;

   }


   bool graphics::DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawEdge(get_handle1(), lpRect, nEdge, nFlags) != FALSE;

   }


   bool graphics::DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawFrameControl(get_handle1(), lpRect, nType, nState) != FALSE;

   }



   bool graphics::Chord(int x1, int y1, int x2, int y2, int x3, int y3,
      int x4, int y4)
   {

      ASSERT(get_handle1() != NULL);

      return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;

   }


   bool graphics::Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   {

      ASSERT(get_handle1() != NULL);

      return ::Chord(get_handle1(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y) != FALSE;

   }


   void graphics::DrawFocusRect(LPCRECT lpRect)
   { 

      ASSERT(get_handle1() != NULL); 

      ::DrawFocusRect(get_handle1(), lpRect);

   }


   bool graphics::Ellipse(LPCRECT lpRect)
   {

      if(width(lpRect) <= 0 || height(lpRect) <= 0)
         return false;

      bool bOk = internal_fill_and_stroke_path(&::draw2d_gdi::graphics::internal_set_path_ellipse, (void *) lpRect, lpRect);

      return bOk;

   }


   bool graphics::DrawEllipse(LPCRECT lpRect)
   {

      if(width(lpRect) <= 0 || height(lpRect) <= 0)
         return false;

      bool bOk = internal_stroke_path(&::draw2d_gdi::graphics::internal_set_path_ellipse, (void *) lpRect, lpRect);

      return bOk;

   }


   bool graphics::FillEllipse(LPCRECT lpRect)
   {

      if(width(lpRect) <= 0 || height(lpRect) <= 0)
         return false;

      bool bOk = internal_fill_path(&::draw2d_gdi::graphics::internal_set_path_ellipse, (void *) lpRect, lpRect);

      return bOk;

   }


   bool graphics::Rectangle(LPCRECT lpRect)
   {

      if(width(lpRect) <= 0 || height(lpRect) <= 0)
         return false;

      bool bOk = internal_fill_and_stroke_path(&::draw2d_gdi::graphics::internal_set_path_rectangle, (void *) lpRect, lpRect);

      return bOk;

   }


   bool graphics::DrawRectangle(LPCRECT lpRect)
   {

      if(width(lpRect) <= 0 || height(lpRect) <= 0)
         return false;

      bool bOk = internal_stroke_path(&::draw2d_gdi::graphics::internal_set_path_rectangle, (void *) lpRect, lpRect);

      return bOk;

   }


   bool graphics::FillRectangle(LPCRECT lpRect)
   {

      if(width(lpRect) <= 0 || height(lpRect) <= 0)
         return false;

      bool bOk = internal_fill_path(&::draw2d_gdi::graphics::internal_set_path_rectangle, (void *) lpRect, lpRect);

      return bOk;

   }

   bool graphics::Polygon(const POINT * lpPoints, int nCount)
   {

      rect rect;

      rect.get_bounding_rect(lpPoints, nCount);

      if(rect.width() <= 0 || rect.height() <= 0)
         return false;

      draw_item item;

      ZERO(item);

      item.lpPoints = lpPoints;

      item.nCount = nCount;

      bool bOk = internal_fill_and_stroke_path(&::draw2d_gdi::graphics::internal_set_path_polygon, (void *) &item, &rect);

      return bOk;

   }


   bool graphics::DrawPolygon(const POINT * lpPoints, int nCount)
   {

      rect rect;

      rect.get_bounding_rect(lpPoints, nCount);

      if(rect.width() <= 0 || rect.height() <= 0)
         return false;

      draw_item item;

      ZERO(item);

      item.lpPoints = lpPoints;

      item.nCount = nCount;

      bool bOk = internal_stroke_path(&::draw2d_gdi::graphics::internal_set_path_polygon, (void *) &item, &rect);

      return bOk;

   }


   bool graphics::FillPolygon(const POINT * lpPoints, int nCount)
   {

      rect rect;

      rect.get_bounding_rect(lpPoints, nCount);

      if(rect.width() <= 0 || rect.height() <= 0)
         return false;

      draw_item item;

      ZERO(item);

      item.lpPoints = lpPoints;

      item.nCount = nCount;

      bool bOk = internal_fill_path(&::draw2d_gdi::graphics::internal_set_path_polygon, (void *) &item, &rect);

      return bOk;

   }


   bool graphics::PolyPolygon(const POINT * lpPoints, const INT * lpPolyCounts, int nCount)
   {

      rect rect;

      rect.get_bounding_rect(lpPoints, nCount);

      if(rect.width() <= 0 || rect.height() <= 0)
         return false;

      draw_item item;

      ZERO(item);

      item.lpPoints = lpPoints;

      item.lpPolyCounts = lpPolyCounts;

      item.nCount = nCount;

      bool bOk = internal_fill_and_stroke_path(&::draw2d_gdi::graphics::internal_set_path_poly_polygon, (void *) &item, &rect);

      return bOk;

   }


   bool graphics::DrawPolyPolygon(const POINT * lpPoints, const INT * lpPolyCounts, int nCount)
   {

      rect rect;

      rect.get_bounding_rect(lpPoints, nCount);

      if(rect.width() <= 0 || rect.height() <= 0)
         return false;

      draw_item item;

      ZERO(item);

      item.lpPoints = lpPoints;

      item.lpPolyCounts = lpPolyCounts;

      item.nCount = nCount;

      bool bOk = internal_stroke_path(&::draw2d_gdi::graphics::internal_set_path_poly_polygon, (void *) &item, &rect);

      return bOk;

   }


   bool graphics::FillPolyPolygon(const POINT * lpPoints, const INT * lpPolyCounts, int nCount)
   {

      rect rect;

      rect.get_bounding_rect(lpPoints, nCount);

      if(rect.width() <= 0 || rect.height() <= 0)
         return false;

      draw_item item;

      ZERO(item);

      item.lpPoints = lpPoints;

      item.lpPolyCounts = lpPolyCounts;

      item.nCount = nCount;

      bool bOk = internal_fill_path(&::draw2d_gdi::graphics::internal_set_path_poly_polygon, (void *) &item, &rect);

      return bOk;

   }

   bool graphics::Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {

      ASSERT(get_handle1() != NULL);

      return ::Pie(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;

   }


   bool graphics::Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   {

      ASSERT(get_handle1() != NULL);

      return ::Pie(get_handle1(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom, ptStart.x, ptStart.y,
         ptEnd.x, ptEnd.y) != FALSE;

   }


   bool graphics::RoundRect(int x1, int y1, int x2, int y2, int x3, int y3)
   {

      ASSERT(get_handle1() != NULL);

      return ::RoundRect(get_handle1(), x1, y1, x2, y2, x3, y3) != FALSE;

   }


   bool graphics::RoundRect(LPCRECT lpRect, POINT point)
   {

      ASSERT(get_handle1() != NULL);

      return ::RoundRect(get_handle1(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom, point.x, point.y) != FALSE;

   }


   bool graphics::PatBlt(int x, int y, int nWidth, int nHeight, uint32_t dwRop)
   {

      ASSERT(get_handle1() != NULL);

      return ::PatBlt(get_handle1(), x, y, nWidth, nHeight, dwRop) != FALSE;

   }




   bool graphics::BitBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, uint32_t dwRop)
   { 

      mutex_lock ml(user_mutex());

      if(get_handle1() == NULL)
         return false;

      if(m_pdib == NULL || GDI_GRAPHICS(pgraphicsSrc)->m_pdib == NULL)
      {

         return ::BitBlt(get_handle1(), x, y, nWidth, nHeight, GDI_HDC(pgraphicsSrc), xSrc, ySrc, dwRop) != FALSE;

      }
      else
      {

         GdiFlush();


         if(m_ealphamode == ::draw2d::alpha_mode_blend)
         {
            class point ptDst(x, y);
            class size size(nWidth, nHeight);
            class point ptSrc(xSrc, ySrc);

            ::draw2d::dib * pdib = pgraphicsSrc->m_pdib;


            if(xSrc < 0)
            {
               ptDst.x -= xSrc;
               xSrc = 0;
            }
            if(ySrc < 0)
            {
               ptDst.x -= ySrc;
               ySrc = 0;
            }

            if(ptDst.x < 0)
            {
               size.cx += ptDst.x;
               ptDst.x = 0;
            }

            if(size.cx < 0)
               return true;

            if(ptDst.y < 0)
            {
               size.cy += ptDst.y;
               ptDst.y = 0;
            }

            if(size.cy < 0)
               return true;

            int xEnd = min(size.cx, min(pdib->cx - ptSrc.x, m_pdib->cx - ptDst.x));

            int yEnd = min(size.cy, min(pdib->cy - ptSrc.y, m_pdib->cy - ptDst.y));

            if(xEnd < 0)
               return false;

            if(yEnd < 0)
               return false;


            size.cx = xEnd;
            size.cy = yEnd;



            pdib  = dib_work(size, false);

            if(pdib == NULL)
               return false;

            pdib->from(point(0, 0), pgraphicsSrc->m_pdib, point(xSrc, ySrc), size);


            BLENDFUNCTION bf;
            bf.BlendOp     = AC_SRC_OVER;
            bf.BlendFlags  = 0;
            bf.SourceConstantAlpha = 0xFF;
            bf.AlphaFormat = AC_SRC_ALPHA;
            /*
            COLORREF * pcolorref = GDI_GRAPHICS(pgraphicsSrc)->m_pdib->m_pcolorref;
            int32_t cx = GDI_GRAPHICS(pgraphicsSrc)->m_pdib->cx;
            int32_t cy = GDI_GRAPHICS(pgraphicsSrc)->m_pdib->cy;
            int32_t scan = GDI_GRAPHICS(pgraphicsSrc)->m_pdib->scan;

            COLORREF * pcolorref1 = m_pdib->m_pcolorref;
            int32_t cx1 = m_pdib->cx;
            int32_t cy1 = m_pdib->cy;
            int32_t scan1 = m_pdib->scan;

            point pt = GetViewportOrg();
            x += pt.x;
            y += pt.y;

            nWidth  = min(nWidth   , min(cx - xSrc, cx1 - x));
            nHeight = min(nHeight  , min(cy - ySrc, cy1 - y));

            for(int i = 0; i < nHeight; i++)
            {
            byte * p = &((byte *) pcolorref)[scan * (i + ySrc) + xSrc * sizeof(COLORREF)];
            byte * p1 = &((byte *) pcolorref1)[scan1 * (i + y) + x * sizeof(COLORREF)];
            for(int j = 0; j < nWidth; j++)
            {
            p1[0] = ((p[0] * p[3]) + ((255 - p[3]) * p1[0]))/ 255;
            p1[1] = ((p[1] * p[3]) + ((255 - p[3]) * p1[1]))/ 255;
            p1[2] = ((p[2] * p[3]) + ((255 - p[3]) * p1[2]))/ 255;
            p1[3] = ((p[3] * p[3]) + ((255 - p[3]) * p1[3]))/ 255;

            /*if(p1[3] == 0)
            {
            p1[0] = 0;
            p1[1] = 0;
            p1[2] = 0;
            }
            else
            {
            p1[0] = (p1[0] * 255 / p1[3]);
            p1[1] = (p1[1] * 255 / p1[3]);
            p1[2] = (p1[2] * 255 / p1[3]);
            }*/
            /*
            p += 4;
            p1 += 4;
            }
            }
            */

            pdib->mult_alpha();

            /*      int64_t iArea = pdib->area();

            byte * pcolorref = (byte *) pdib->m_pcolorref;

            GdiFlush();

            for(int y = 0; y < pdib->cy; y++)
            {
            byte * p = &pcolorref[pdib->scan * y];
            for(int x = 0; x < pdib->cx; x++)
            {
            p[0] = (p[0] * p[3] / 255);
            p[1] = (p[1] * p[3] / 255);
            p[2] = (p[2] * p[3] / 255);
            p += 4;
            }
            }*/

            GdiFlush();

            ::AlphaBlend(m_hdc, ptDst.x, ptDst.y, size.cx, size.cy, (HDC) pdib->get_graphics()->get_os_data(), 0, 0, size.cx, size.cy, bf);

            /*for(int y = 0; y < nHeight; y++)
            {
            byte * p = &((byte *) pcolorref)[scan * (y + ySrc) + xSrc * sizeof(COLORREF)];
            for(int x = 0; x < nWidth; x++)
            {
            if(p[3] == 0)
            {
            p[0] = 0;
            p[1] = 0;
            p[2] = 0;
            }
            else
            {
            p[0] = (p[0] * 255 / p[3]);
            p[1] = (p[1] * 255 / p[3]);
            p[2] = (p[2] * 255 / p[3]);
            }
            p += 4;
            }
            }*/

            /*            for(int i = 0; i < nHeight; i++)
            {
            byte * p1 = &((byte *) pcolorref1)[scan1 * (i + y) + x * sizeof(COLORREF)];
            for(int j = 0; j < nWidth;jx++)
            {
            if(p1[3] == 0)
            {
            p1[0] = 0;
            p1[1] = 0;
            p1[2] = 0;
            }
            else
            {
            p1[0] = (p1[0] * 255 / p1[3]);
            p1[1] = (p1[1] * 255 / p1[3]);
            p1[2] = (p1[2] * 255 / p1[3]);
            }
            p1 += 4;
            }
            }*/

            return true;

         }
         else
         {

            m_pdib->from(point(x, y), GDI_GRAPHICS(pgraphicsSrc)->m_pdib, point(0, 0), size(nWidth, nHeight));

            return true;

         }

      }


   }




   bool graphics::StretchBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, uint32_t dwRop)
   {

      mutex_lock ml(user_mutex());

      ASSERT(get_handle1() != NULL);

      if(m_pdib == NULL)
      {

         return ::StretchBlt(get_handle1(), x, y, nWidth, nHeight, GDI_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, nSrcHeight,  dwRop) != FALSE;

      }
      else
      {

         ::draw2d::dib * pdib1 = dib_work(size(nWidth, nHeight), false);

         if(pdib1 == NULL)
            return false;

         ::draw2d::dib * pdib2 = dib_work(size(nSrcWidth, nSrcHeight), false);

         if(pdib2 == NULL)
            return false;

         pdib2->from(point(0, 0), pgraphicsSrc->m_pdib, point(xSrc, ySrc), pdib2->size());

         pdib1->stretch_dib(pdib2);

         pdib1->mult_alpha();

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 0xFF;
         bf.AlphaFormat = AC_SRC_ALPHA;

         return ::AlphaBlend(m_hdc, x, y, nWidth, nHeight, (HDC) pdib1->get_graphics()->get_os_data(), 0, 0, nSrcWidth, nSrcHeight, bf) != FALSE;



      }

   }


   COLORREF graphics::GetPixel(int x, int y) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetPixel(get_handle1(), x, y);

   }


   COLORREF graphics::GetPixel(POINT point) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetPixel(get_handle1(), point.x, point.y);

   }


   COLORREF graphics::SetPixel(int x, int y, COLORREF crColor)
   {

      ASSERT(get_handle1() != NULL);

      return ::SetPixel(get_handle1(), x, y, crColor);

   }


   COLORREF graphics::SetPixel(POINT point, COLORREF crColor)
   {

      ASSERT(get_handle1() != NULL);

      return ::SetPixel(get_handle1(), point.x, point.y, crColor);

   }


   bool graphics::FloodFill(int x, int y, COLORREF crColor)
   {

      ASSERT(get_handle1() != NULL);

      return ::FloodFill(get_handle1(), x, y, crColor) != FALSE;

   }


   bool graphics::ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType)
   {

      ASSERT(get_handle1() != NULL);

      return ::ExtFloodFill(get_handle1(), x, y, crColor, nFillType) != FALSE;

   }



   bool graphics::TextOut(int x, int y, const char * lpszString, int nCount)
   {

      mutex_lock ml(user_mutex());

      if(get_handle1() == NULL)
         return FALSE;

      if(m_pdibAlphaBlend != NULL)
      {
         if(GetBkMode() == TRANSPARENT)
         {
            //   return TRUE;
            rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());
            rect rectText(point(x, y), GetTextExtent(lpszString, nCount));
            if(rectIntersect.intersect(rectIntersect, rectText))
            {
               ::draw2d::dib_sp dib0(get_app());
               dib0->create(rectText.size());
               ::draw2d::brush_sp brush(allocer());
               brush->create_solid(ARGB(255, 255, 255, 255));
               dib0->get_graphics()->SelectObject(brush);
               dib0->get_graphics()->SelectObject(get_current_font());
               //dib0->get_graphics()->SetBkMode(TRANSPARENT);
               dib0->get_graphics()->TextOut(0, 0, lpszString, nCount);
               dib0->ToAlpha(0);
               ::draw2d::dib_sp dib1(get_app());
               dib1->create(rectText.size());
               brush->create_solid(get_current_brush()->m_cr);
               dib0->get_graphics()->SelectObject(brush);
               //dib1->get_graphics()->SetTextColor(GetTextColor());
               dib1->get_graphics()->SelectObject(get_current_font());
               //dib1->get_graphics()->SetBkMode(TRANSPARENT);
               dib1->get_graphics()->TextOut(0, 0, lpszString, nCount);
               dib1->channel_from(visual::rgba::channel_alpha, dib0);
               ::draw2d::dib_sp dib2(get_app());
               dib2->create(rectText.size());
               dib2->Fill(255, 0, 0, 0);
               dib2->from(point(max(0, m_ptAlphaBlend.x - x), max(0, m_ptAlphaBlend.y - y)),
                  m_pdibAlphaBlend->get_graphics(), point(max(0, x - m_ptAlphaBlend.x), max(0, y - m_ptAlphaBlend.y)), rectText.size());
               dib1->channel_multiply(visual::rgba::channel_alpha, dib2);
               /*::draw2d::dib_sp dib3(get_app());
               dib1->mult_alpha(dib3);*/

               keeper < ::draw2d::dib * > keep(&m_pdibAlphaBlend, NULL, m_pdibAlphaBlend, true);

               return System.visual().imaging().true_blend(this, point(x, y), rectText.size(), dib1->get_graphics(), null_point());

               /*BLENDFUNCTION bf;
               bf.BlendOp     = AC_SRC_OVER;
               bf.BlendFlags  = 0;
               bf.SourceConstantAlpha = 0xFF;
               bf.AlphaFormat = AC_SRC_ALPHA;
               return ::AlphaBlend(get_handle1(), x, y, 
               rectText.width(), rectText.height(), GDI_HDC(dib1->get_graphics()), 0, 0, rectText.width(), 
               rectText.height(), bf) != FALSE; */
            }
         }
      }

      ASSERT(get_handle1() != NULL); 

      string str(lpszString, nCount);

      wstring wstr = ::ca2::international::utf8_to_unicode(str);

      ::draw2d::brush & brush = get_current_brush();
      ::draw2d::font & font = get_current_font();

      //if(m_spbrush.is_null() || GetAValue(m_spbrush->m_cr) == 255 || m_spdib.is_null())

      select_font();

      if(m_pdib == NULL)
      {

         ::SetBkMode(m_hdc, TRANSPARENT);

         ::SetTextColor(m_hdc, RGB(GetRValue(brush.m_cr), GetGValue(brush.m_cr), GetBValue(brush.m_cr)));

         return ::TextOutW(get_handle1(), x, y, wstr, wstr.length()) != FALSE;

      }
      else
      {

         SIZE size;

         if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int)wstr.get_length(), &size))
         {

            return false;

         }

         ::draw2d::dib * pdib = dib_work(size, true);

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 0xFF;
         bf.AlphaFormat = AC_SRC_ALPHA;

         if(!GDI_DIB(pdib)->process_initialize(&brush))
            return false;

         if(GDI_BRUSH(&brush)->m_bProcess)
         {

            GDI_GRAPHICS(pdib->get_graphics())->SetTextColor(RGB(255, 255, 255));

         }
         else
         {

            GDI_GRAPHICS(pdib->get_graphics())->SetTextColor(RGB(GetRValue(brush.m_cr), GetGValue(brush.m_cr), GetBValue(brush.m_cr)));

         }


         pdib->get_graphics()->SelectObject(&font);

         ::SetBkMode((HDC) pdib->get_graphics()->get_os_data(), TRANSPARENT);

         ::TextOutW((HDC) pdib->get_graphics()->get_os_data(), 0, 0, wstr, wstr.length());

         GDI_DIB(pdib)->process_blend(&brush, x, y, m_ealphamode, m_pdib);

         if(m_ealphamode == ::draw2d::alpha_mode_blend)
         {

            ::AlphaBlend(m_hdc, x, y, size.cx, size.cy, (HDC) pdib->get_graphics()->get_os_data(), 0, 0, size.cx, size.cy, bf);

         }

         return true;

      }


   }



   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   //// COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   bool graphics::TextOut(int x, int y, const string & str)
   { 

      return TextOut(x, y, str, str.get_length());

   }

   // call virtual

   bool graphics::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect, const char * lpszString, UINT nCount, LPINT lpDxWidths)
   { 
      ASSERT(get_handle1() != NULL); 
      return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths) != FALSE;

   }



   bool graphics::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
      const string & str, LPINT lpDxWidths)
   {

      ASSERT(get_handle1() != NULL);

      return ::ExtTextOut(get_handle1(), x, y, nOptions, lpRect,
         str, (UINT)str.get_length(), lpDxWidths) != FALSE;

   }


   size graphics::TabbedTextOut(int x, int y, const char * lpszString, int nCount,
      int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
   {

      ASSERT(get_handle1() != NULL);

      return ::TabbedTextOut(get_handle1(), x, y, lpszString, nCount,
         nTabPositions, lpnTabStopPositions, nTabOrigin);

   }


   size graphics::TabbedTextOut(int x, int y, const string & str,
      int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin)
   {

      ASSERT(get_handle1() != NULL);

      return ::TabbedTextOut(get_handle1(), x, y, str, (int)str.get_length(),
         nTabPositions, lpnTabStopPositions, nTabOrigin);

   }


   int graphics::draw_text(const char * lpszString, int nCount, LPRECT lpRect, UINT nFormat)
   { 

      return ::visual::graphics_extension(get_app())._DrawText(this, string(lpszString, nCount), lpRect, nFormat);

      //wstring wstr(string(lpszString, nCount));

      //return ::DrawTextW(get_handle(), wstr, wstr.get_length(), lpRect, nFormat);

      //System.visual().

   }


   int graphics::draw_text(const string & str, LPRECT lpRect, UINT nFormat)
   { 

      return draw_text(str, str.get_length(), lpRect, nFormat);

   }


   int graphics::draw_text_ex(LPTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 

      wstring wstr(string(lpszString, nCount));

      return DrawTextExW(get_handle(), (wchar_t *) (const wchar_t *)  wstr, wstr.get_length(), lpRect, nFormat, lpDTParams);

   }


   int graphics::draw_text_ex(const string & str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams)
   { 

      return draw_text_ex((char *) (const char *) str, str.get_length(), lpRect, nFormat, lpDTParams);

   }


   class size graphics::GetTabbedTextExtent(const char * lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions) const
   {

      ASSERT(get_handle2() != NULL);

      return ::GetTabbedTextExtent(get_handle2(), lpszString, nCount, nTabPositions, lpnTabStopPositions);

   }


   class size graphics::GetTabbedTextExtent(const string & str, int nTabPositions, LPINT lpnTabStopPositions) const
   { 

      ASSERT(get_handle2() != NULL);

      return ::GetTabbedTextExtent(get_handle2(), str, (int)str.get_length(), nTabPositions, lpnTabStopPositions);

   }


   class size graphics::GetOutputTabbedTextExtent(const char * lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetTabbedTextExtent(get_handle1(), lpszString, nCount, nTabPositions, lpnTabStopPositions);

   }


   class size graphics::GetOutputTabbedTextExtent(const string & str, int nTabPositions, LPINT lpnTabStopPositions) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetTabbedTextExtent(get_handle1(), str, (int)str.get_length(), nTabPositions, lpnTabStopPositions);

   }


   bool graphics::GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight)
   {

      ASSERT(get_handle1() != NULL);

      return ::GrayString(get_handle1(), (HBRUSH)pBrush->get_os_data(), (GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight) != FALSE; 

   }


   UINT graphics::GetTextAlign() const
   { 

      ASSERT(get_handle2() != NULL); 

      return ::GetTextAlign(get_handle2());

   }


   int graphics::GetTextFace(int nCount, __out_ecount_part_z(nCount, return + 1) LPTSTR lpszFacename) const
   { 

      ASSERT(get_handle2() != NULL);

      return ::GetTextFace(get_handle2(), nCount, lpszFacename);

   }


   int graphics::GetTextFace(string & rString) const
   {

      ASSERT(get_handle2() != NULL);

      int nResult = ::GetTextFace(get_handle2(), 256, rString.GetBuffer(256)); rString.ReleaseBuffer();

      return nResult; 

   }


   bool graphics::get_text_metrics(LPTEXTMETRICW lpMetrics) const
   { 

      ASSERT(get_handle2() != NULL); 

      return ::GetTextMetricsW(get_handle2(), lpMetrics) != FALSE;

   }


   bool graphics::get_output_text_metrics(LPTEXTMETRICW lpMetrics) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetTextMetricsW(get_handle1(), lpMetrics) != FALSE;

   }


   int graphics::GetTextCharacterExtra() const
   { ASSERT(get_handle2() != NULL); return ::GetTextCharacterExtra(get_handle2());

   }


   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   { ASSERT(get_handle2() != NULL); return ::GetCharWidth(get_handle2(), nFirstChar, nLastChar, lpBuffer) != FALSE;

   }


   bool graphics::GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetCharWidth(get_handle1(), nFirstChar, nLastChar, lpBuffer) != FALSE;

   }


   uint32_t graphics::GetFontLanguageInfo() const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetFontLanguageInfo(get_handle1());

   }



   uint32_t graphics::GetCharacterPlacement(const char * lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetCharacterPlacement(get_handle1(), lpString, nCount, nMaxExtent, lpResults, dwFlags);

   }


   uint32_t graphics::GetCharacterPlacement(string & str, int nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetCharacterPlacement(get_handle1(), (const char *)str, str.get_length(), nMaxExtent, lpResults, dwFlags);

   }




   size graphics::GetAspectRatioFilter() const
   {
      ASSERT(get_handle2() != NULL);
      SIZE size;
      VERIFY(::GetAspectRatioFilterEx(get_handle2(), &size));
      return size;
   }
   bool graphics::ScrollDC(int dx, int dy,
      LPCRECT lpRectScroll, LPCRECT lpRectClip,
      ::draw2d::region* pRgnUpdate, LPRECT lpRectUpdate)
   {

      ASSERT(get_handle1() != NULL);

      return ::ScrollDC(get_handle1(), dx, dy, lpRectScroll,
         lpRectClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate) != FALSE;

   }



   // Printer Escape Functions
   int graphics::Escape(int nEscape, int nCount, const char * lpszInData, LPVOID lpOutData)
   {

      ASSERT(get_handle1() != NULL);

      return ::Escape(get_handle1(), nEscape, nCount, lpszInData, lpOutData);}

   // graphics 3.1 Specific functions
   UINT graphics::SetBoundsRect(LPCRECT lpRectBounds, UINT flags)
   {

      ASSERT(get_handle1() != NULL);

      return ::SetBoundsRect(get_handle1(), lpRectBounds, flags);

   }


   UINT graphics::GetBoundsRect(LPRECT lpRectBounds, UINT flags)
   { ASSERT(get_handle2() != NULL); return ::GetBoundsRect(get_handle2(), lpRectBounds, flags);

   }


   bool graphics::ResetDC(const DEVMODE* lpDevMode)
   { ASSERT(get_handle2() != NULL); return ::ResetDC(get_handle2(), lpDevMode) != NULL; }
   UINT graphics::GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm) const
   { ASSERT(get_handle2() != NULL); return ::GetOutlineTextMetricsW(get_handle2(), cbData, lpotm) != FALSE;

   }


   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const
   { ASSERT(get_handle2() != NULL); return ::GetCharABCWidths(get_handle2(), nFirstChar, nLastChar, lpabc) != FALSE;

   }


   uint32_t graphics::GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData,
      uint32_t cbData) const
   { ASSERT(get_handle2() != NULL); return ::GetFontData(get_handle2(), dwTable, dwOffset, lpData, cbData);

   }


   int graphics::GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const
   { ASSERT(get_handle2() != NULL); return ::GetKerningPairs(get_handle2(), nPairs, lpkrnpair);

   }


   uint32_t graphics::GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
      uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const
   { ASSERT(get_handle2() != NULL); return ::GetGlyphOutline(get_handle2(), nChar, nFormat,
   lpgm, cbBuffer, lpBuffer, lpmat2);

   }



   // ::userbase::document handling functions
   int graphics::StartDoc(LPDOCINFO lpDocInfo)
   {

      ASSERT(get_handle1() != NULL);

      return ::StartDoc(get_handle1(), lpDocInfo);

   }


   int graphics::StartPage()
   {

      ASSERT(get_handle1() != NULL);

      return ::StartPage(get_handle1());

   }


   int graphics::EndPage()
   {

      ASSERT(get_handle1() != NULL);

      return ::EndPage(get_handle1());

   }


   int graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, int))
   {

      ASSERT(get_handle1() != NULL);

      return ::SetAbortProc(get_handle1(), (ABORTPROC)lpfn);

   }


   int graphics::AbortDoc()
   {

      ASSERT(get_handle1() != NULL);

      return ::AbortDoc(get_handle1());

   }


   int graphics::EndDoc()
   {

      ASSERT(get_handle1() != NULL);

      return ::EndDoc(get_handle1());

   }



   bool graphics::MaskBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc,
      int xSrc, int ySrc, ::draw2d::bitmap& maskBitmap, int xMask, int yMask, uint32_t dwRop)
   {

      ASSERT(get_handle1() != NULL);

      return ::MaskBlt(get_handle1(), x, y, nWidth, nHeight, GDI_HDC(pgraphicsSrc),
         xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop) != FALSE;

   }


   bool graphics::PlgBlt(LPPOINT lpPoint, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc,
      int nWidth, int nHeight, ::draw2d::bitmap& maskBitmap, int xMask, int yMask)
   {

      ASSERT(get_handle1() != NULL);

      return ::PlgBlt(get_handle1(), lpPoint, GDI_HDC(pgraphicsSrc), xSrc, ySrc, nWidth,
         nHeight, (HBITMAP)maskBitmap.get_os_data(), xMask, yMask) != FALSE;

   }


   bool graphics::SetPixelV(int x, int y, COLORREF crColor)
   {

      ASSERT(get_handle1() != NULL);

      return ::SetPixelV(get_handle1(), x, y, crColor) != FALSE;

   }


   bool graphics::SetPixelV(POINT point, COLORREF crColor)
   {

      ASSERT(get_handle1() != NULL);

      return ::SetPixelV(get_handle1(), point.x, point.y, crColor) != FALSE;

   }


   bool graphics::AngleArc(int x, int y, int nRadius,
      float fStartAngle, float fSweepAngle)
   {

      ASSERT(get_handle1() != NULL);

      return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle) != FALSE;

   }




   bool graphics::ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd)
   {

      ASSERT(get_handle1() != NULL); return ArcTo(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y) != FALSE;

   }




   int graphics::GetArcDirection() const
   { 

      ASSERT(get_handle2() != NULL); return ::GetArcDirection(get_handle2());

   }




   bool graphics::PolyPolyline(const POINT* lpPoints, const uint32_t* lpPolyPoints, int nCount)
   { 

      ASSERT(get_handle1() != NULL);

      return ::PolyPolyline(get_handle1(), lpPoints, (const DWORD *) lpPolyPoints, nCount) != FALSE;

   }




   bool graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const
   { 

      ASSERT(get_handle2() != NULL); 

      return ::GetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE;

   }


   void graphics::set_original_object(int iType)
   {

      switch(iType)
      {
      case OBJ_BITMAP:
         ::SelectObject(m_hdc, m_hbitmapOriginal);
         return;
      case OBJ_PEN:
         ::SelectObject(m_hdc, m_hpenOriginal);
         return;
      case OBJ_BRUSH:
         ::SelectObject(m_hdc, m_hbrushOriginal);
         return;
      case OBJ_FONT:
         ::SelectObject(m_hdc, m_hfontOriginal);
         return;
      case OBJ_REGION:
         ::SelectObject(m_hdc, m_hrgnOriginal);
         return;
      default:
         break;
      }

   }


   ::draw2d::pen_sp graphics::get_current_pen() const
   { 

      if(m_sppen.is_set())
         return m_sppen;

      ASSERT(get_handle2() != NULL); 

      ((graphics *) this)->m_sppen = ::draw2d_gdi::pen::from_handle(get_app(), (HPEN)::GetCurrentObject(get_handle2(), OBJ_PEN)); 

      return m_sppen;

   }


   ::draw2d::brush_sp graphics::get_current_brush() const
   { 

      if(m_spbrush.is_set())
         return m_spbrush;

      ASSERT(get_handle2() != NULL); 

      ((graphics *) this)->m_spbrush = ::draw2d_gdi::brush::from_handle(get_app(), (HBRUSH)::GetCurrentObject(get_handle2(), OBJ_BRUSH)); 

      LOGBRUSH lb;

      (dynamic_cast < ::draw2d_gdi::brush * > (((graphics *) this)->m_spbrush.m_p))->GetLogBrush(&lb);

      m_spbrush->m_cr = lb.lbColor | 255 << 24;
      m_spbrush->m_etype = ::draw2d::brush::type_solid;
      m_spbrush->m_bUpdated = true;

      return m_spbrush;

   }

   ::draw2d::palette_sp graphics::get_current_palette() const
   { 


      return ((::draw2d::palette *) NULL);

      /*      if(m_sppalette.is_set() && m_sppalette->get_os_data() == (void *) ::GetCurrentObject(get_handle2(), OBJ_PAL))
      return m_sppalette;

      ASSERT(get_handle2() != NULL); 

      ((graphics *) this)->m_sppalette = ::draw2d_gdi::palette::from_handle(get_app(), (HPALETTE)::GetCurrentObject(get_handle2(), OBJ_PAL)); 

      return m_sppalette;*/

   }


   ::draw2d::font_sp graphics::get_current_font() const
   { 

      if(m_spfont.is_set())
         return m_spfont;

      ASSERT(get_handle2() != NULL);

      ((graphics *) this)->m_spfont = ::draw2d_gdi::font::from_handle(get_app(), (HFONT)::GetCurrentObject(get_handle2(), OBJ_FONT)); 

      return m_spfont;

   }

   ::draw2d::bitmap_sp graphics::get_current_bitmap() const
   {

      if(m_spbitmap.is_set() && m_spbitmap->get_os_data() == (void *) ::GetCurrentObject(get_handle2(), OBJ_BITMAP))
         return m_spbitmap;

      ASSERT(get_handle2() != NULL);

      ((graphics *) this)->m_spbitmap = ::draw2d_gdi::bitmap::from_handle(get_app(), (HBITMAP)::GetCurrentObject(get_handle2(), OBJ_BITMAP)); 

      return m_spbitmap;

   }


   bool graphics::PolyBezier(const POINT* lpPoints, int nCount)
   { 

      ASSERT(get_handle1() != NULL); 

      return ::PolyBezier(get_handle1(), lpPoints, nCount) != FALSE; 

   }


   int graphics::DrawEscape(int nEscape, int nInputSize, const char * lpszInputData)
   { 

      ASSERT(get_handle1() != NULL);

      return ::DrawEscape(get_handle1(), nEscape, nInputSize, lpszInputData);

   }




   int graphics::Escape(int nEscape, int nInputSize, const char * lpszInputData, int nOutputSize, char * lpszOutputData)
   { 

      ASSERT(get_handle1() != NULL); 

      return ::ExtEscape(get_handle1(), nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData);

   }




   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABCFLOAT lpABCF) const
   { 

      ASSERT(get_handle2() != NULL); 

      return ::GetCharABCWidthsFloat(get_handle2(), nFirstChar, nLastChar, lpABCF) != FALSE; 

   }


   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, float* lpFloatBuffer) const
   { 

      ASSERT(get_handle2() != NULL); 

      return ::GetCharWidthFloat(get_handle2(), nFirstChar, nLastChar, lpFloatBuffer) != FALSE; 

   }

   bool graphics::AbortPath()
   { 

      ASSERT(get_handle1() != NULL); 

      return ::AbortPath(get_handle1()) != FALSE;

   }


   bool graphics::BeginPath()
   { 

      if(get_handle1() == NULL)
         return false;

      return ::BeginPath(get_handle1()) != FALSE;

   }


   bool graphics::CloseFigure()
   { 

      ASSERT(get_handle1() != NULL); 

      return ::CloseFigure(get_handle1()) != FALSE;

   }


   bool graphics::EndPath()
   { 

      if(get_handle1() == NULL)
         return false;

      return ::EndPath(get_handle1()) != FALSE;

   }


   bool graphics::FillPath()
   { 

      rect rect;

      m_sppath->get_bounding_rect(rect);

      return internal_fill_path(&::draw2d_gdi::graphics::internal_set_path, m_sppath.m_p, rect);

      /*if(m_sppath.is_null())
      return false;

      ASSERT(get_handle1() != NULL); 

      ::draw2d::brush & brush = get_current_brush();

      if(m_pdib == NULL)
      {

      set(m_sppath);

      return ::FillPath(get_handle1()) != FALSE;

      }
      else
      {

      ::rect rect;

      m_sppath->get_bounding_rect(rect);

      ::draw2d::dib * pdib = dib_work(rect.size());

      BLENDFUNCTION bf;
      bf.BlendOp     = AC_SRC_OVER;
      bf.BlendFlags  = 0;
      bf.SourceConstantAlpha = 0xFF;
      bf.AlphaFormat = AC_SRC_ALPHA;

      if(!GDI_DIB(pdib)->process_initialize(&brush))
      return false;

      pdib->get_graphics()->SelectObject(&brush);

      GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(-rect.top_left());

      GDI_GRAPHICS(pdib->get_graphics())->set(m_sppath);

      ::FillPath((HDC) pdib->get_graphics()->get_os_data());

      GDI_DIB(pdib)->process_blend(&brush, rect.left, rect.top, m_ealphamode);

      GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(0, 0);

      ::AlphaBlend(m_hdc, rect.left, rect.top, rect.width(), rect.height(), (HDC) pdib->get_graphics()->get_os_data(), 0, 0, rect.width(), rect.height(), bf);

      return true;

      }*/

   }

   void graphics::internal_set_path(void * pparam)
   {

      ::draw2d::path * ppath = (::draw2d::path *) pparam;

      set(ppath);

   }

   void graphics::internal_set_path_ellipse(void * pparam)
   {

      LPRECT lprect = (LPRECT) pparam;

      BeginPath();

      ::Ellipse(m_hdc, lprect->left, lprect->top, lprect->right, lprect->bottom);

      EndPath();

   }

   void graphics::internal_set_path_rectangle(void * pparam)
   {

      LPRECT lprect = (LPRECT) pparam;

      BeginPath();

      ::Rectangle(m_hdc, lprect->left, lprect->top, lprect->right, lprect->bottom);

      EndPath();

   }

   void graphics::internal_set_path_line(void * pparam)
   {

      LPRECT lprect = (LPRECT) pparam;

      BeginPath();

      ::MoveToEx(m_hdc, lprect->left, lprect->top, NULL);

      ::LineTo(m_hdc, lprect->right, lprect->bottom);

      EndPath();

   }

   void graphics::internal_set_path_polygon(void * pparam)
   {

      draw_item * pitem = (draw_item *) pparam;

      BeginPath();

      ::Polygon(m_hdc, pitem->lpPoints, pitem->nCount);

      EndPath();

   }

   void graphics::internal_set_path_poly_polygon(void * pparam)
   {

      draw_item * pitem = (draw_item *) pparam;

      BeginPath();

      ::PolyPolygon(m_hdc, pitem->lpPoints, pitem->lpPolyCounts, pitem->nCount);

      EndPath();

   }

   bool graphics::internal_fill_path(void (::draw2d_gdi::graphics::* pfnInternalSetPath)(void *), void * pparam, LPCRECT lpcrect)
   { 

      mutex_lock ml(user_mutex());

      ASSERT(get_handle1() != NULL); 

      ::draw2d::brush & brush = get_current_brush();

      if(brush.m_etype == ::draw2d::brush::type_null)
         return true;

      if(m_pdib == NULL)
      {

         (this->*pfnInternalSetPath)(pparam);

         return ::FillPath(get_handle1()) != FALSE;

      }
      else
      {

         ::rect rect(lpcrect);

         //m_sppath->get_bounding_rect(rect);

         ::draw2d::dib * pdib = dib_work(rect.size(), false);

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 0xFF;
         bf.AlphaFormat = AC_SRC_ALPHA;

         if(!GDI_DIB(pdib)->process_initialize(&brush))
            return false;

         pdib->get_graphics()->SelectObject(&brush);

         GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(-rect.top_left());

         (GDI_GRAPHICS(pdib->get_graphics())->*pfnInternalSetPath)(pparam);

         ::FillPath((HDC) pdib->get_graphics()->get_os_data());

         GDI_DIB(pdib)->process_blend(&brush, rect.left, rect.top, m_ealphamode, m_pdib);

         if(m_ealphamode == ::draw2d::alpha_mode_blend)
         {

            GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(0, 0);

            ::AlphaBlend(m_hdc, rect.left, rect.top, rect.width(), rect.height(), (HDC) pdib->get_graphics()->get_os_data(), 0, 0, rect.width(), rect.height(), bf);

         }

         return true;

      }

   }


   bool graphics::FlattenPath()
   { 

      ASSERT(get_handle1() != NULL);

      return ::FlattenPath(get_handle1()) != FALSE;

   }


   float graphics::GetMiterLimit() const
   {

      ASSERT(get_handle1() != NULL); 

      float fMiterLimit;

      VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit)); 

      return fMiterLimit; 

   }


   int graphics::GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetPath(get_handle1(), lpPoints, lpTypes, nCount);

   }




   bool graphics::SetMiterLimit(float fMiterLimit)
   { 

      ASSERT(get_handle1() != NULL); 

      return ::SetMiterLimit(get_handle1(), fMiterLimit, NULL) != FALSE;

   }


   bool graphics::StrokeAndFillPath()
   { 

      rect rect;

      m_sppath->get_bounding_rect(rect);

      return internal_fill_and_stroke_path(&::draw2d_gdi::graphics::internal_set_path, m_sppath.m_p, rect);

   }

   bool graphics::StrokePath()
   { 

      rect rect;

      m_sppath->get_bounding_rect(rect);

      return internal_stroke_path(&::draw2d_gdi::graphics::internal_set_path, m_sppath.m_p, rect);

   }

   bool graphics::internal_stroke_path(void (::draw2d_gdi::graphics::* pfnInternalSetPath)(void *), void * pparam, LPCRECT lpcrect)
   {

      mutex_lock ml(user_mutex());

      ASSERT(get_handle1() != NULL); 

      ::draw2d::pen & pen = get_current_pen();

      if(pen.m_etype == ::draw2d::pen::type_null)
         return true;

      if(m_pdib == NULL)
      {

         (this->*pfnInternalSetPath)(pparam);

         return ::FillPath(get_handle1()) != FALSE;

      }
      else
      {

         ::rect rectBound(lpcrect);

         //m_sppath->get_bounding_rect(rectBound);

         ::rect rect(rectBound);

         rect.left   -= (int32_t) floor(pen.m_dWidth / 2.0);
         rect.right  += (int32_t) ceil(pen.m_dWidth / 2.0);
         rect.top    -= (int32_t) floor(pen.m_dWidth / 2.0);
         rect.bottom += (int32_t) ceil(pen.m_dWidth / 2.0);

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 0xFF;
         bf.AlphaFormat = AC_SRC_ALPHA;

         ::draw2d::dib * pdib = dib_work(rect.size(), false);

         if(!GDI_DIB(pdib)->process_initialize(&pen))
            return false;

         pdib->get_graphics()->SelectObject(&pen);

         pdib->get_graphics()->SetViewportOrg(-rectBound.top_left());

         (GDI_GRAPHICS(pdib->get_graphics())->*pfnInternalSetPath)(pparam);

         ::StrokePath(GDI_HDC(pdib->get_graphics()));

         GDI_DIB(pdib)->process_blend(&pen, rect.left, rect.top, m_ealphamode, m_pdib);

         if(m_ealphamode == ::draw2d::alpha_mode_blend)
         {

            GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(0, 0);

            ::AlphaBlend(m_hdc, rect.left, rect.top, rect.width(), rect.height(), (HDC) pdib->get_graphics()->get_os_data(), 0, 0, rect.width(), rect.height(), bf);

         }

         return true;

      }

   }


   bool graphics::internal_fill_and_stroke_path(void (::draw2d_gdi::graphics::* pfnInternalSetPath)(void *), void * pparam, LPCRECT lpcrect)
   {

      mutex_lock ml(user_mutex());

      ASSERT(get_handle1() != NULL); 

      ::draw2d::pen & pen = get_current_pen();

      if(pen.m_etype == ::draw2d::pen::type_null)
         return internal_fill_path(pfnInternalSetPath, pparam, lpcrect);

      ::draw2d::brush & brush = get_current_brush();

      if(brush.m_etype == ::draw2d::brush::type_null)
         return internal_stroke_path(pfnInternalSetPath, pparam, lpcrect);

      if(m_pdib == NULL)
      {

         (this->*pfnInternalSetPath)(pparam);

         return ::StrokeAndFillPath(get_handle1()) != FALSE;

      }
      else
      {

         ::rect rectBound(lpcrect);

         ::rect rect(rectBound);

         rect.left   -= (int32_t) floor(pen.m_dWidth / 2.0);
         rect.right  += (int32_t) ceil(pen.m_dWidth / 2.0);
         rect.top    -= (int32_t) floor(pen.m_dWidth / 2.0);
         rect.bottom += (int32_t) ceil(pen.m_dWidth / 2.0);

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 0xFF;
         bf.AlphaFormat = AC_SRC_ALPHA;

         ::draw2d::dib * pdib = dib_work(rect.size(), false);

         if(GDI_DIB(pdib)->process_initialize(&brush))
         {

            pdib->get_graphics()->SelectObject(&brush);

            GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(-rect.top_left() + point((int32_t) floor(pen.m_dWidth / 2.0), (int32_t) floor(pen.m_dWidth / 2.0)));

            (GDI_GRAPHICS(pdib->get_graphics())->*pfnInternalSetPath)(pparam);

            ::FillPath((HDC) pdib->get_graphics()->get_os_data());

            if(GDI_DIB(pdib)->process_blend(&brush, rect.left, rect.top, m_ealphamode, m_pdib))
            {

               if(m_ealphamode == ::draw2d::alpha_mode_blend)
               {

                  GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(0, 0);

                  ::AlphaBlend(m_hdc, rect.left, rect.top, rect.width(), rect.height(), (HDC) pdib->get_graphics()->get_os_data(), 0, 0, rect.width(), rect.height(), bf);

               }
               /*
               else
               {

               m_pdib->from_ignore_alpha(rect.top_left() + GetViewportOrg(), pdib, point(0, 0), rect.size());

               }
               */

            }

         }


         //::draw2d::dib * pdib2 = dib_work(rect.size());

         if(GDI_DIB(pdib)->process_initialize(&pen))
         {

            // pdib2->from(point((int32_t) floor(pen.m_dWidth / 2.0), (int32_t) floor(pen.m_dWidth / 2.0)), pdib, point(0, 0), rect.size());

            pdib->get_graphics()->SelectObject(&pen);

            pdib->get_graphics()->SetViewportOrg(-rectBound.top_left());

            (GDI_GRAPHICS(pdib->get_graphics())->*pfnInternalSetPath)(pparam);

            ::StrokePath(GDI_HDC(pdib->get_graphics()));

            if(GDI_DIB(pdib)->process_blend(&pen, rect.left, rect.top, m_ealphamode, m_pdib))
            {

               if(m_ealphamode == ::draw2d::alpha_mode_blend)
               {

                  GDI_GRAPHICS(pdib->get_graphics())->SetViewportOrg(0, 0);

                  ::AlphaBlend(m_hdc, rect.left, rect.top, rect.width(), rect.height(), (HDC) pdib->get_graphics()->get_os_data(), 0, 0, rect.width(), rect.height(), bf);

               }
               /*             
               else
               {

               m_pdib->from_ignore_alpha(rect.top_left() + GetViewportOrg(), pdib, point(0, 0), rect.size());

               }
               */

            }

         }

         return true;

      }

   }


   bool graphics::WidenPath()
   {

      ASSERT(get_handle1() != NULL);

      return ::WidenPath(get_handle1()) != FALSE;

   }




   bool graphics::AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData)
   {

      ASSERT(get_handle1() != NULL);

      return ::GdiComment(get_handle1(), nDataSize, pCommentData) != FALSE;

   }




   bool graphics::PlayMetaFile(HENHMETAFILE hEnhMF, LPCRECT lpBounds)
   { 

      return ::PlayEnhMetaFile(get_handle1(), hEnhMF, lpBounds) != FALSE;

   }


   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   // COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST

   /*bool graphics::alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, BLENDFUNCTION blend)
   { 


   if(get_handle1() == NULL)
   return false;


   if(m_pdibAlphaBlend != NULL)
   {


   rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());


   ::draw2d::dib * pdibWork = NULL;
   ::draw2d::dib * pdibWork2 = NULL;
   //         ::draw2d::dib * pdibWork3 = NULL;
   ::draw2d::dib * pdibWork4 = NULL;


   class point ptSrc(xSrc, ySrc);
   class point ptDest(xDest, yDest);
   class size size(nDestWidth, nDestHeight);



   ::draw2d::dib_sp spdib;

   if(pdibWork == NULL)
   {

   spdib.create(allocer());

   pdibWork = spdib;

   }

   if(pdibWork == NULL)
   return false;

   if(!pdibWork->create(size))
   return false;

   if(!pdibWork->from(null_point(), pgraphicsSrc, ptSrc, size))
   return false;




   ::draw2d::dib_sp spdib2;
   if(pdibWork2 == NULL)
   {
   spdib2.create(allocer());
   pdibWork2 = spdib2;
   }


   ::draw2d::dib_sp spdib4;
   if(pdibWork4 == NULL)
   {
   spdib4.create(allocer());
   pdibWork4 = spdib4;
   }
   if(pdibWork4 == NULL)
   return false;
   if(!pdibWork4->create(size))
   return false;


   pdibWork4->Fill(255, 0, 0, 0);

   pdibWork4->from(point(max(0, m_ptAlphaBlend.x - xDest), max(0, m_ptAlphaBlend.y - yDest)),
   m_pdibAlphaBlend->get_graphics(), point(max(0, xDest - m_ptAlphaBlend.x), max(0, yDest - m_ptAlphaBlend.y)), size);

   pdibWork->channel_multiply(visual::rgba::channel_alpha, pdibWork4);


   keeper < ::draw2d::dib * > keep(&m_pdibAlphaBlend, NULL, m_pdibAlphaBlend, true);


   return System.visual().imaging().true_blend(this, ptDest, size, pdibWork->get_graphics(), ptSrc) != FALSE;

   }



   return ::AlphaBlend(get_handle1(), xDest, yDest, 
   nDestWidth, nDestHeight, GDI_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
   nSrcHeight, blend) != FALSE; 
   }*/

   bool graphics::alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, double dOpacity)
   { 


      if(get_handle1() == NULL)
         return false;


      if(m_pdibAlphaBlend != NULL)
      {


         rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());


         ::draw2d::dib * pdibWork = NULL;
         ::draw2d::dib * pdibWork2 = NULL;
         //         ::draw2d::dib * pdibWork3 = NULL;
         ::draw2d::dib * pdibWork4 = NULL;


         class point ptSrc(xSrc, ySrc);
         class point ptDest(xDest, yDest);
         class size size(nDestWidth, nDestHeight);



         ::draw2d::dib_sp spdib;

         if(pdibWork == NULL)
         {

            spdib.create(allocer());

            pdibWork = spdib;

         }

         if(pdibWork == NULL)
            return false;

         if(!pdibWork->create(size))
            return false;

         if(!pdibWork->from(null_point(), pgraphicsSrc, ptSrc, size))
            return false;




         ::draw2d::dib_sp spdib2;
         if(pdibWork2 == NULL)
         {
            spdib2.create(allocer());
            pdibWork2 = spdib2;
         }


         ::draw2d::dib_sp spdib4;
         if(pdibWork4 == NULL)
         {
            spdib4.create(allocer());
            pdibWork4 = spdib4;
         }
         if(pdibWork4 == NULL)
            return false;
         if(!pdibWork4->create(size))
            return false;


         pdibWork4->Fill(255, 0, 0, 0);

         pdibWork4->from(point(max(0, m_ptAlphaBlend.x - xDest), max(0, m_ptAlphaBlend.y - yDest)),
            m_pdibAlphaBlend->get_graphics(), point(max(0, xDest - m_ptAlphaBlend.x), max(0, yDest - m_ptAlphaBlend.y)), size);

         pdibWork->channel_multiply(visual::rgba::channel_alpha, pdibWork4);


         keeper < ::draw2d::dib * > keep(&m_pdibAlphaBlend, NULL, m_pdibAlphaBlend, true);


         return System.visual().imaging().true_blend(this, ptDest, size, pdibWork->get_graphics(), ptSrc) != FALSE;

      }
      if(dOpacity < 0.0)
         dOpacity = 0.0;
      else if(dOpacity > 1.0)
         dOpacity = 1.0;

      BLENDFUNCTION bf;
      bf.BlendOp     = AC_SRC_OVER;
      bf.BlendFlags  = 0;
      bf.SourceConstantAlpha = (BYTE) (dOpacity * 0xFF);
      bf.AlphaFormat = AC_SRC_ALPHA;


      return ::AlphaBlend(get_handle1(), xDest, yDest, 
         nDestWidth, nDestHeight, GDI_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
         nSrcHeight, bf) != FALSE; 
   }


   bool graphics::TransparentBlt(int xDest, int yDest, int nDestWidth, 
      int nDestHeight, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, 
      int nSrcHeight, UINT crTransparent)
   {

      ASSERT(get_handle1() != NULL);

      return ::TransparentBlt(get_handle1(), xDest, yDest, 
         nDestWidth, nDestHeight, GDI_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth, 
         nSrcHeight, crTransparent) != FALSE;

   }


   bool graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices,
      void * pMesh, ULONG nMeshElements, uint32_t dwMode)
   {

      ASSERT(get_handle1() != NULL);

      return ::GradientFill(get_handle1(), pVertices, nVertices,
         pMesh, nMeshElements, dwMode) != FALSE;

   }



   // This is ca2 API library.
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
   {

      ASSERT(get_handle1() != NULL);

      return ::GetDCBrushColor(get_handle1());

   }


   inline COLORREF graphics::SetDCBrushColor(COLORREF crColor)
   {

      ASSERT(get_handle1() != NULL);

      return ::SetDCBrushColor(get_handle1(), crColor);

   }



   inline COLORREF graphics::GetDCPenColor() const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetDCPenColor(get_handle1());

   }


   inline COLORREF graphics::SetDCPenColor(COLORREF crColor)
   {

      ASSERT(get_handle1() != NULL);

      return ::SetDCPenColor(get_handle1(), crColor);

   }



   inline bool graphics::GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetCharABCWidthsI(get_handle1(), giFirst, cgi, pgi, lpabc) != FALSE;

   }


   bool graphics::GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const
   {

      ASSERT(get_handle1() != NULL);

      return ::GetCharWidthI(get_handle1(), giFirst, cgi, pgi, lpBuffer) != FALSE;

   }


   bool graphics::GetTextExtent(sized & size, const char * lpszString, strsize nCount, int32_t iIndex) const
   {

      ::size sz = GetTextExtent(string(lpszString), iIndex);

      size.cx = sz.cx;
      size.cy = sz.cy;

      return true;

   }



   inline bool graphics::GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize) const
   { 
      ENSURE(lpSize != NULL); 
      ASSERT(get_handle1() != NULL); 
      return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, lpSize) != FALSE;

   }


   inline bool graphics::GetTextExtentPointI(LPWORD pgiIn, int cgi, LPSIZE lpSize) const
   {
      ENSURE(lpSize != NULL); 
      ASSERT(get_handle1() != NULL); 
      return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, lpSize) != FALSE;

   }









   /////////////////////////////////////////////////////////////////////////////
   // More coordinate transforms (in separate file to avoid transitive refs)

#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(LPSIZE lpSize) const
   {

      int nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained ::collection::map mode, ::collection::map against physical inch
         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
         DPtoLP(lpSize);
         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);

      }


      else
      {
         // ::collection::map against logical inch for non-constrained mapping modes
         int cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);

         }


         else
         {
            //cxPerInch = afxData.cxPixelsPerInch;
            //cyPerInch = afxData.cyPixelsPerInch;
            cxPerInch = 96;
            cyPerInch = 96;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, HIMETRIC_INCH, cxPerInch);
         lpSize->cy = MulDiv(lpSize->cy, HIMETRIC_INCH, cyPerInch);

      }


   }

   void graphics::HIMETRICtoDP(LPSIZE lpSize) const
   {

      int nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
         nMapMode != MM_TEXT)
      {
         // when using a constrained ::collection::map mode, ::collection::map against physical inch
         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
         LPtoDP(lpSize);
         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);

      }


      else
      {
         // ::collection::map against logical inch for non-constrained mapping modes
         int cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
            cxPerInch = GetDeviceCaps(LOGPIXELSX);
            cyPerInch = GetDeviceCaps(LOGPIXELSY);

         }


         else
         {
            //cxPerInch = afxData.cxPixelsPerInch;
            //cyPerInch = afxData.cyPixelsPerInch;
            cxPerInch = 96;
            cyPerInch = 96;
         }
         ASSERT(cxPerInch != 0 && cyPerInch != 0);
         lpSize->cx = MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
         lpSize->cy = MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);

      }


   }

   void graphics::LPtoHIMETRIC(LPSIZE lpSize) const
   {

      LPtoDP(lpSize);
      DPtoHIMETRIC(lpSize);

   }



   void graphics::HIMETRICtoLP(LPSIZE lpSize) const
   {

      HIMETRICtoDP(lpSize);
      DPtoLP(lpSize);

   }



   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing primitives/helpers

   ::draw2d::brush* graphics::GetHalftoneBrush(::ca2::application * papp)
   {
      /*      AfxLockGlobals(CRIT_HALFTONEBRUSH);
      if (_afxHalftoneBrush == NULL)
      {
      WORD grayPattern[8];
      for (int i = 0; i < 8; i++)
      grayPattern[i] = (WORD)(0x5555 << (i & 1));
      HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
      if (grayBitmap != NULL)
      {
      _afxHalftoneBrush = ::CreatePatternBrush(grayBitmap);
      ::DeleteObject(grayBitmap);

      }


      }
      if (!_afxWingdixTerm)
      _afxWingdixTerm = (char)!atexit(&AfxWingdixTerm);
      AfxUnlockGlobals(CRIT_HALFTONEBRUSH);

      return ::draw2d_gdi::brush::from_handle(papp, _afxHalftoneBrush);
      */
      return NULL;
   }



   void graphics::DrawDragRect(LPCRECT lpRect, SIZE size,
      LPCRECT lpRectLast, SIZE sizeLast, ::draw2d::brush* pBrush, ::draw2d::brush* pBrushLast)
   {

      // first, determine the update region and select it
      ::draw2d::region_sp rgnNew(allocer());
      ::draw2d::region_sp rgnOutside(allocer()), rgnInside(allocer());
      rgnOutside->create_rect(lpRect);
      rect rect = *lpRect;
      rect.inflate(-size.cx, -size.cy);
      rect.intersect(rect, lpRect);
      rgnInside->create_rect(rect);
      rgnNew->create_rect(0, 0, 0, 0);
      rgnNew->combine(rgnOutside, rgnInside, ::draw2d::region::combine_xor);

      ::draw2d::brush* pBrushOld = NULL;
      if (pBrush == NULL)
      {
         pBrush = graphics::GetHalftoneBrush(get_app());

      }



      ENSURE(pBrush);

      if (pBrushLast == NULL)
      {
         pBrushLast = pBrush;
      }

      ::draw2d::region_sp rgnLast(allocer()), rgnUpdate(allocer());
      if (lpRectLast != NULL)
      {
         // find difference between new region and old region
         rgnLast->create_rect(0, 0, 0, 0);
         rgnOutside->create_rect(lpRectLast);
         rect = *lpRectLast;
         rect.inflate(-sizeLast.cx, -sizeLast.cy);
         rect.intersect(rect, lpRectLast);
         rgnInside->create_rect(rect);
         rgnLast->combine(rgnOutside, rgnInside, ::draw2d::region::combine_xor);

         // only diff them if brushes are the same
         if (pBrush->get_os_data() == pBrushLast->get_os_data())
         {
            rgnUpdate->create_rect(0, 0, 0, 0);
            rgnUpdate->combine(rgnLast, rgnNew, ::draw2d::region::combine_xor);

         }


      }
      if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != NULL)
      {
         // brushes are different -- erase old region first
         SelectClipRgn(rgnLast);
         GetClipBox(&rect);
         pBrushOld = SelectObject(pBrushLast);
         PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);
         SelectObject(pBrushOld);
         pBrushOld = NULL;
      }

      // draw into the update/new region
      SelectClipRgn(rgnUpdate->get_os_data() != NULL ? rgnUpdate : rgnNew);
      GetClipBox(&rect);
      pBrushOld = SelectObject(pBrush);
      PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);

      // cleanup DC
      if (pBrushOld != NULL)
         SelectObject(pBrushOld);
      SelectClipRgn(NULL);

   }



   void graphics::FillSolidRect(LPCRECT lpRect, COLORREF clr)
   {

      FillSolidRect(lpRect->left, lpRect->top, width(lpRect), height(lpRect), clr);

   }

   ::draw2d::dib * graphics::dib_work(class size size, bool bReset)
   {

      ::draw2d::dib::descriptor d;

      d.m_etype = ::draw2d::dib::type_complex;
      d.m_cr = 0;
      d.m_size = size;

      if(t_pdibmap == NULL)
         t_pdibmap = new ::draw2d::dibmap_ex1(get_app());


      ::draw2d::dib * pdib = t_pdibmap->operator[](d);

      pdib->m_descriptor = d;

      if(bReset)
      {

         pdib->Fill(0, 0, 0, 0);

      }

      return pdib;

   }

   ::draw2d::dib * graphics::fill_dib_work(COLORREF clr, class size size, bool bReset)
   {

      ::draw2d::dib::descriptor d;

      d.m_etype = ::draw2d::dib::type_plain_color;
      d.m_cr = clr;
      d.m_size = size;


      if(t_pdibmap == NULL)
         t_pdibmap = new ::draw2d::dibmap_ex1(get_app());

      ::draw2d::dib * pdib = t_pdibmap->operator[](d);

      if(pdib->m_descriptor != d || bReset)
      {

         pdib->m_descriptor = d;


         pdib->Fill(GetAValue(clr), GetRValue(clr) * GetAValue(clr) / 255, GetGValue(clr) * GetAValue(clr) / 255, GetBValue(clr) * GetAValue(clr) / 255);
         //pdib->Fill(GetAValue(clr), GetRValue(clr), GetGValue(clr), GetBValue(clr));

      }

      return pdib;


   }


   void graphics::FillSolidRect(const __rect64 * lpRect, COLORREF clr)
   {

      rect rect32;
      ::copy(rect32, lpRect);
      FillSolidRect(rect32, clr);

   }




   void graphics::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
   {

      mutex_lock ml(user_mutex());

      if(m_pdib == NULL)
      {

         ::SetBkColor(get_handle1(), clr);
         rect rect(x, y, x + cx, y + cy);
         ::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);

      }
      else if(GetAValue(clr) == 255)
      {
         
         m_pdib->FillRect(x, y, cx, cy, 255, GetRValue(clr), GetGValue(clr), GetBValue(clr));

      }
      else
      {

         ::draw2d::dib * pdib = fill_dib_work(clr, ::size(cx, cy), false);

         BLENDFUNCTION bf;
         bf.BlendOp     = AC_SRC_OVER;
         bf.BlendFlags  = 0;
         bf.SourceConstantAlpha = 0xFF;
         bf.AlphaFormat = AC_SRC_ALPHA;

         ::AlphaBlend(m_hdc, x, y, cx, cy, (HDC) pdib->get_graphics()->get_os_data(), 0, 0, cx, cy, bf);

      }

   }



   void graphics::Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
   {

      Draw3dRect(lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);

   }



   void graphics::Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
   {

      FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
      FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
      FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
      FillSolidRect(x, y + cy, cx, -1, clrBottomRight);

   }





#ifdef _DEBUG
   void graphics::assert_valid() const
   {
      ::ca2::object::assert_valid();

   }



   void graphics::dump(dump_context & dumpcontext) const
   {
      ::ca2::object::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << get_handle1();
      dumpcontext << "\nm_hAttribDC = " << get_handle2();
      dumpcontext << "\nm_bPrinting = " << m_bPrinting;

      dumpcontext << "\n";
   }
#endif //_DEBUG


   ::draw2d::graphics * graphics::from_handle(::ca2::application * papp, HDC hdc)
   {

      ::draw2d_gdi::graphics * pgraphics = new ::draw2d_gdi::graphics (papp);

      pgraphics->attach(hdc);

      return dynamic_cast < ::draw2d::graphics * > (pgraphics);

   }



   bool graphics::Attach(HDC hdc)
   {

      if (hdc == NULL)
      {

         return false;

      }

      if(m_hdc != NULL)
      {

         DeleteDC();

      }

      m_hdc = hdc;


      m_hbitmapOriginal    = (HBITMAP) ::GetCurrentObject(m_hdc , OBJ_BITMAP);
      m_hpenOriginal       = (HPEN)    ::GetCurrentObject(m_hdc , OBJ_PEN);
      m_hbrushOriginal     = (HBRUSH)  ::GetCurrentObject(m_hdc , OBJ_BRUSH );
      m_hfontOriginal      = (HFONT)   ::GetCurrentObject(m_hdc , OBJ_FONT);
      m_hrgnOriginal       = (HRGN)    ::GetCurrentObject(m_hdc , OBJ_REGION);


      return TRUE;

   }

   HDC graphics::Detach()
   {

      HDC hdc = get_handle1();

      //      ReleaseAttribDC();

      m_hdc = NULL;

      return hdc;

   }


   bool graphics::DeleteDC()
   {


      if (get_handle1() == NULL)
         return FALSE;

      return ::DeleteDC(Detach()) != FALSE;


   }




   int graphics::StartDoc(const char * lpszDocName)
   {

      DOCINFO di;

      memset(&di, 0, sizeof(DOCINFO));

      di.cbSize = sizeof(DOCINFO);

      di.lpszDocName = lpszDocName;

      return StartDoc(&di);

   }



   int graphics::SaveDC()
   {

      int nRetVal = 0;

      if(get_handle2() != NULL)
         nRetVal = ::SaveDC(get_handle2());

      if(get_handle1() != NULL && get_handle1() != get_handle2() && ::SaveDC(get_handle1()) != 0)
         nRetVal = -1;   // -1 is the only valid restore value for complex DCs

      return nRetVal;

   }


   bool graphics::RestoreDC(int nSavedDC)
   {

      bool bRetVal = true;

      if(get_handle1() != NULL && get_handle1() != get_handle2())
         bRetVal = ::RestoreDC(get_handle1(), nSavedDC) != FALSE;

      if(get_handle2() != NULL)
         bRetVal = (bRetVal && ::RestoreDC(get_handle2(), nSavedDC)) != FALSE;

      return bRetVal;

   }

   ::draw2d::object* graphics::SelectStockObject(int nIndex)
   {

      HGDIOBJ hObject = ::GetStockObject(nIndex);

      HGDIOBJ hOldObj = NULL;

      ASSERT(hObject != NULL);

      if(get_handle1() != NULL && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), hObject);

      if(get_handle2() != NULL)
         hOldObj = ::SelectObject(get_handle2(), hObject);

      return ::draw2d_gdi::object::from_handle(get_app(), hOldObj);

   }



   ::draw2d::pen * graphics::SelectObject(::draw2d::pen * ppen)
   {

      if(get_handle1() == NULL)
         return NULL;

      if(ppen == NULL)
         return NULL;

      mutex_lock ml(user_mutex());

      SelectObject(ppen->get_os_data());

      GDI_PEN(ppen)->m_ptraGraphics.add_unique(this);

      m_ptraObject.add_unique(GDI_PEN(ppen));

      m_sppen = ppen;

      return m_sppen;


   }



   ::draw2d::brush * graphics::SelectObject(::draw2d::brush * pbrush)
   {

      if(get_handle1() == NULL)
         return NULL;

      if(pbrush == NULL)
         return NULL;

      mutex_lock ml(user_mutex());

      SelectObject(pbrush->get_os_data());

      GDI_BRUSH(pbrush)->m_ptraGraphics.add_unique(this);

      m_ptraObject.add_unique(GDI_BRUSH(pbrush));

      m_spbrush = pbrush;

      return m_spbrush;

   }



   ::draw2d::font * graphics::SelectObject(::draw2d::font * pfont)
   {

      if(get_handle1() == NULL)
         return NULL;

      if(pfont == NULL)
         return NULL;

      mutex_lock ml(user_mutex());

      SelectObject(pfont->get_os_data());

      GDI_FONT(pfont)->m_ptraGraphics.add_unique(this);

      m_ptraObject.add_unique(GDI_FONT(pfont));

      m_spfont = pfont;

      return m_spfont;

   }



   int graphics::SelectObject(::draw2d::region* pregion)
   {

      int nRetVal = GDI_ERROR;

      if(get_handle1() == NULL)
         return nRetVal;

      if(pregion == NULL)
         return nRetVal;

      mutex_lock ml(user_mutex());

      SelectObject(pregion->get_os_data());

      GDI_REGION(pregion)->m_ptraGraphics.add_unique(this);

      m_ptraObject.add_unique(GDI_REGION(pregion));

      return nRetVal;

   }


   ::draw2d::palette* graphics::SelectPalette(::draw2d::palette* pPalette, bool bForceBackground)
   {

      // return dynamic_cast < ::draw2d::palette * > (::draw2d_gdi::object::from_handle(get_app(), ::SelectPalette(get_handle1(), (HPALETTE)pPalette->get_os_data(), bForceBackground))) != FALSE;

      throw not_implemented(get_app());

      return NULL;

   }



   COLORREF graphics::SetBkColor(COLORREF crColor)
   {
      COLORREF crRetVal = CLR_INVALID;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         crRetVal = ::SetBkColor(get_handle1(), crColor);
      if(get_handle2() != NULL)
         crRetVal = ::SetBkColor(get_handle2(), crColor);
      return crRetVal;
   }

   int graphics::SetBkMode(int nBkMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetBkMode(get_handle1(), nBkMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetBkMode(get_handle2(), nBkMode);
      return nRetVal;
   }

   int graphics::SetPolyFillMode(int nPolyFillMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetPolyFillMode(get_handle1(), nPolyFillMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetPolyFillMode(get_handle2(), nPolyFillMode);
      return nRetVal;
   }

   int graphics::SetROP2(int nDrawMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetROP2(get_handle1(), nDrawMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetROP2(get_handle2(), nDrawMode);
      return nRetVal;
   }

   int graphics::SetStretchBltMode(int nStretchMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetStretchBltMode(get_handle1(), nStretchMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetStretchBltMode(get_handle2(), nStretchMode);
      return nRetVal;
   }

   COLORREF graphics::SetTextColor(COLORREF crColor)
   {
      COLORREF crRetVal = CLR_INVALID;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         crRetVal = ::SetTextColor(get_handle1(), crColor);
      if(get_handle2() != NULL)
         crRetVal = ::SetTextColor(get_handle2(), crColor);
      return crRetVal;
   }

   int graphics::SetGraphicsMode(int iMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      {
         nRetVal = ::SetGraphicsMode(get_handle1(), iMode) != FALSE;

      }


      if(get_handle2() != NULL)
      {
         nRetVal = ::SetGraphicsMode(get_handle2(), iMode) != FALSE;

      }


      return nRetVal;
   }

   bool graphics::SetWorldTransform(const XFORM* pXform)
   {
      bool nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      {
         nRetVal = ::SetWorldTransform(get_handle1(), pXform) != FALSE;

      }


      if(get_handle2() != NULL)
      {
         nRetVal = ::SetWorldTransform(get_handle2(), pXform) != FALSE;

      }


      return nRetVal;
   }

   bool graphics::ModifyWorldTransform(const XFORM* pXform,uint32_t iMode)
   {
      bool nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      {
         nRetVal = ::ModifyWorldTransform(get_handle1(), pXform, iMode) != FALSE;

      }


      if(get_handle2() != NULL)
      {
         nRetVal = ::ModifyWorldTransform(get_handle2(), pXform, iMode) != FALSE;

      }


      return nRetVal;
   }

   int graphics::SetMapMode(int nMapMode)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetMapMode(get_handle1(), nMapMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetMapMode(get_handle2(), nMapMode);
      return nRetVal;
   }

   point graphics::SetViewportOrg(int x, int y)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetViewportOrgEx(get_handle1(), x, y, &point);
      if(get_handle2() != NULL)
         ::SetViewportOrgEx(get_handle2(), x, y, &point);
      return point;
   }

   point graphics::OffsetViewportOrg(int nWidth, int nHeight)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::OffsetViewportOrgEx(get_handle1(), nWidth, nHeight, &point);
      if(get_handle2() != NULL)
         ::OffsetViewportOrgEx(get_handle2(), nWidth, nHeight, &point);
      return point;
   }

   size graphics::SetViewportExt(int x, int y)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetViewportExtEx(get_handle1(), x, y, &size);
      if(get_handle2() != NULL)
         ::SetViewportExtEx(get_handle2(), x, y, &size);
      return size;
   }

   size graphics::ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::ScaleViewportExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      if(get_handle2() != NULL)
         ::ScaleViewportExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      return size;
   }

   point graphics::SetWindowOrg(int x, int y)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetWindowOrgEx(get_handle1(), x, y, &point);
      if(get_handle2() != NULL)
         ::SetWindowOrgEx(get_handle2(), x, y, &point);
      return point;
   }

   point graphics::OffsetWindowOrg(int nWidth, int nHeight)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::OffsetWindowOrgEx(get_handle1(), nWidth, nHeight, &point);
      if(get_handle2() != NULL)
         ::OffsetWindowOrgEx(get_handle2(), nWidth, nHeight, &point);
      return point;
   }

   size graphics::SetWindowExt(int x, int y)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetWindowExtEx(get_handle1(), x, y, &size);
      if(get_handle2() != NULL)
         ::SetWindowExtEx(get_handle2(), x, y, &size);
      return size;
   }

   size graphics::ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom)
   {
      size size(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::ScaleWindowExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      if(get_handle2() != NULL)
         ::ScaleWindowExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      return size;
   }

   int graphics::GetClipBox(LPRECT lpRect) const
   {
      return ::GetClipBox(get_handle1(), lpRect);

   }



   int graphics::SelectClipRgn(::draw2d::region* pRgn)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SelectClipRgn(get_handle1(), pRgn == NULL ? NULL : (HRGN) pRgn->get_os_data());
      if(get_handle2() != NULL)
         nRetVal = ::SelectClipRgn(get_handle2(), pRgn == NULL ? NULL : (HRGN) pRgn->get_os_data());
      return nRetVal;
   }

   int graphics::ExcludeClipRect(int x1, int y1, int x2, int y2)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::ExcludeClipRect(get_handle1(), x1, y1, x2, y2);
      if(get_handle2() != NULL)
         nRetVal = ::ExcludeClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int graphics::ExcludeClipRect(LPCRECT lpRect)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::ExcludeClipRect(get_handle1(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom);
      if(get_handle2() != NULL)
         nRetVal = ::ExcludeClipRect(get_handle2(), lpRect->left, lpRect->top,
         lpRect->right, lpRect->bottom);
      return nRetVal;
   }

   int graphics::IntersectClipRect(int x1, int y1, int x2, int y2)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::IntersectClipRect(get_handle1(), x1, y1, x2, y2);
      if(get_handle2() != NULL)
         nRetVal = ::IntersectClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int graphics::IntersectClipRect(LPCRECT lpRect)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::IntersectClipRect(get_handle1(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
      if(get_handle2() != NULL)
         nRetVal = ::IntersectClipRect(get_handle2(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
      return nRetVal;
   }

   int graphics::OffsetClipRgn(int x, int y)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::OffsetClipRgn(get_handle1(), x, y);
      if(get_handle2() != NULL)
         nRetVal = ::OffsetClipRgn(get_handle2(), x, y);
      return nRetVal;
   }

   int graphics::OffsetClipRgn(SIZE size)
   {
      int nRetVal = ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::OffsetClipRgn(get_handle1(), size.cx, size.cy);
      if(get_handle2() != NULL)
         nRetVal = ::OffsetClipRgn(get_handle2(), size.cx, size.cy);
      return nRetVal;
   }

   point graphics::MoveTo(int x, int y)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::MoveToEx(get_handle1(), x, y, &point);
      if(get_handle2() != NULL)
         ::MoveToEx(get_handle2(), x, y, &point);
      return point;
   }

   bool graphics::LineTo(int x, int y)
   {

      rect rect;

      rect.top_left() = GetCurrentPosition();

      rect.right = x;

      rect.bottom = y;

      class rect rectBound(rect);

      ::sort::sort(rectBound.left, rectBound.right);

      ::sort::sort(rectBound.top, rectBound.bottom);

      internal_stroke_path(&::draw2d_gdi::graphics::internal_set_path_line, &rect, &rectBound);

      ::MoveToEx(m_hdc, rect.right, rect.bottom, NULL);

      return true;

   }



   UINT graphics::SetTextAlign(UINT nFlags)
   {
      UINT nRetVal = GDI_ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetTextAlign(get_handle1(), nFlags);
      if(get_handle2() != NULL)
         nRetVal = ::SetTextAlign(get_handle2(), nFlags);
      return nRetVal;
   }

   int graphics::SetTextJustification(int nBreakExtra, int nBreakCount)
   {
      int nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetTextJustification(get_handle1(), nBreakExtra, nBreakCount);
      if(get_handle2() != NULL)
         nRetVal = ::SetTextJustification(get_handle2(), nBreakExtra, nBreakCount);
      return nRetVal;
   }

   int graphics::SetTextCharacterExtra(int nCharExtra)
   {
      ASSERT(get_handle1() != NULL);
      int nRetVal = 0x8000000;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetTextCharacterExtra(get_handle1(), nCharExtra);
      if(get_handle2() != NULL)
         nRetVal = ::SetTextCharacterExtra(get_handle2(), nCharExtra);
      return nRetVal;
   }

   uint32_t graphics::SetMapperFlags(uint32_t dwFlag)
   {
      ASSERT(get_handle1() != NULL);
      uint32_t dwRetVal = GDI_ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         dwRetVal = ::SetMapperFlags(get_handle1(), dwFlag);
      if(get_handle2() != NULL)
         dwRetVal = ::SetMapperFlags(get_handle2(), dwFlag);
      return dwRetVal;
   }

   typedef uint32_t (CALLBACK* AFX_GDIGETLAYOUTPROC)(HDC);
   typedef uint32_t (CALLBACK* AFX_GDISETLAYOUTPROC)(HDC, uint32_t);

   uint32_t graphics::GetLayout() const
   {
      HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != NULL);
      uint32_t dwGetLayout = LAYOUT_LTR;
      AFX_GDIGETLAYOUTPROC pfn;
      pfn = (AFX_GDIGETLAYOUTPROC) GetProcAddress(hInst, "GetLayout");
      // if they API is available, just call it. If it is not
      // available, indicate an error.
      if (pfn != NULL)
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
      ASSERT(hInst != NULL);
      uint32_t dwGetLayout = LAYOUT_LTR;
      AFX_GDISETLAYOUTPROC pfn;
      pfn = (AFX_GDISETLAYOUTPROC) GetProcAddress(hInst, "SetLayout");
      // If the API is availalbe, just call it. If it's not available,
      // setting anything other than LAYOUT_LTR is an error.
      if (pfn != NULL)
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
   rect::swap_left_right(lpRect) != FALSE;

   }



   void window::ClientToScreen(LPRECT lpRect)
   {
   ASSERT(::IsWindow(get_handle1()));
   ::ClientToScreen(get_handle1(), (LPPOINT)lpRect);
   ::ClientToScreen(get_handle1(), ((LPPOINT)lpRect)+1);
   if (GetExStyle() & WS_EX_LAYOUTRTL)
   rect::swap_left_right(lpRect) != FALSE;

   }

   */


   /////////////////////////////////////////////////////////////////////////////
   // Advanced Win32 GDI functions

   bool graphics::ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
   {

      ASSERT(get_handle1() != NULL);

      bool bResult = ::ArcTo(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;

      if (get_handle1() != get_handle2())
      {

         point pt;

         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));

         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));

      }

      return bResult;

   }


   int graphics::SetArcDirection(int nArcDirection)
   {
      ASSERT(get_handle1() != NULL);
      int nResult = 0;
      if (get_handle1() != get_handle2())
         nResult = ::SetArcDirection(get_handle1(), nArcDirection);
      if (get_handle2() != NULL)
         nResult = ::SetArcDirection(get_handle2(), nArcDirection);
      return nResult;
   }

   bool graphics::PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount)
   {

      ASSERT(get_handle1() != NULL);

      bool bResult = ::PolyDraw(get_handle1(), lpPoints, lpTypes, nCount) != FALSE;

      if (get_handle1() != get_handle2())
      {

         point pt;

         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));

         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));


      }


      return bResult;

   }


   bool graphics::PolylineTo(const POINT* lpPoints, int nCount)
   {

      ASSERT(get_handle1() != NULL);

      bool bResult = ::PolylineTo(get_handle1(), lpPoints, nCount) != FALSE;

      if (get_handle1() != get_handle2())
      {

         point pt;

         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));

         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));

      }

      return bResult;

   }

   bool graphics::SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust)
   {
      ASSERT(get_handle1() != NULL);
      bool bResult = false;
      if (get_handle1() != get_handle2())
         bResult = ::SetColorAdjustment(get_handle1(), lpColorAdjust) != FALSE;
      if (get_handle2() != NULL)
         bResult = ::SetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE;
      return bResult;
   }

   bool graphics::PolyBezierTo(const POINT* lpPoints, int nCount)
   {
      ASSERT(get_handle1() != NULL);
      bool bResult = ::PolyBezierTo(get_handle1(), lpPoints, nCount) != FALSE;
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));

      }


      return bResult;
   }

   bool graphics::SelectClipPath(int nMode)
   {
      ASSERT(get_handle1() != NULL);

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
            TRACE("Error: unable to transfer clip region in graphics::SelectClipPath!\n");
            bResult = FALSE;
         }
         ::DeleteObject(hRgn);

      }


      return bResult;
   }

   int graphics::SelectClipRgn(::draw2d::region* pRgn, int nMode)
   {
      ASSERT(get_handle1() != NULL);
      int nRetVal = ERROR;
      if (get_handle1() != get_handle2())
         nRetVal = ::ExtSelectClipRgn(get_handle1(), (HRGN)pRgn->get_os_data(), nMode);
      if (get_handle2() != NULL)
         nRetVal = ::ExtSelectClipRgn(get_handle2(), (HRGN)pRgn->get_os_data(), nMode);
      return nRetVal;
   }

   /////////////////////////////////////////////////////////////////////////////
   // Special handling for metafile playback

   int CALLBACK AfxEnumMetaFileProc(HDC hDC,
      HANDLETABLE* pHandleTable, METARECORD* pMetaRec, int nHandles, LPARAM lParam)
   {
      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)lParam;
      ASSERT_VALID(pgraphics);

      switch (pMetaRec->rdFunction)
      {
         // these records have effects different for each graphics derived class
      case META_SETMAPMODE:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SetMapMode((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWEXT:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SetWindowExt(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWORG:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SetWindowOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTEXT:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SetViewportExt(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTORG:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SetViewportOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEWINDOWEXT:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->ScaleWindowExt(
            (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SCALEVIEWPORTEXT:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->ScaleViewportExt(
            (int)(short)pMetaRec->rdParm[3], (int)(short)pMetaRec->rdParm[2],
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_OFFSETVIEWPORTORG:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->OffsetViewportOrg(
            (int)(short)pMetaRec->rdParm[1], (int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SAVEDC:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SaveDC();
         break;
      case META_RESTOREDC:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->RestoreDC((int)(short)pMetaRec->rdParm[0]);
         break;
      case META_SETBKCOLOR:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SetBkColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
         break;
      case META_SETTEXTCOLOR:
         (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SetTextColor(*(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
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
               HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1(), hStockFont);
               HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1(), hObject);
               if (hObjOld == hStockFont)
               {
                  // got the stock object back, so must be selecting a font
                  (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SelectObject(::draw2d_gdi::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
                  break;  // don't play the default record
               }
               else
               {
                  // didn't get the stock object back, so restore everything
                  ::SelectObject((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1(), hFontOld);
                  ::SelectObject((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1(), hObjOld);

               }


               // and fall through to PlayMetaFileRecord...
            }
            else if (nObjType == OBJ_FONT)
            {
               // play back as graphics::SelectObject(::draw2d::font*)
               (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->SelectObject(::draw2d_gdi::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
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
      return ::EnumMetaFile(get_handle1(), hMF, AfxEnumMetaFileProc, (LPARAM)this) != FALSE;

   }



   /////////////////////////////////////////////////////////////////////////////
   // Coordinate transforms

   void graphics::LPtoDP(LPSIZE lpSize) const
   {

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));

   }



   void graphics::DPtoLP(LPSIZE lpSize) const
   {

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));

   }








   size graphics::GetTextExtent(const char * lpszString, int nCount) const
   {

      if(get_handle2() == NULL)
         return size(0, 0);

      SIZE size;

      string str(lpszString, nCount);

      wstring wstr = ::ca2::international::utf8_to_unicode(str);

      if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int)wstr.get_length(), &size))
      {

         return class size(0, 0);

      }


      return size;
   }


   size graphics::GetTextExtent(const string & str) const
   {

      if(get_handle2() == NULL)
         return size(0, 0);

      ((graphics *) this)->select_font();

      SIZE size;

      wstring wstr = ::ca2::international::utf8_to_unicode(str);

      if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int)wstr.get_length(), &size))
      {
         return class size(0, 0);

      }

      return size;

   }


   size graphics::GetOutputTextExtent(const char * lpszString, int nCount) const
   {
      ASSERT(get_handle1() != NULL);
      SIZE size;
      string str(lpszString, nCount);
      wstring wstr = ::ca2::international::utf8_to_unicode(str);
      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int)wstr.get_length(), &size));
      return size;
   }
   size graphics::GetOutputTextExtent(const string & str) const
   {
      ASSERT(get_handle1() != NULL);
      SIZE size;
      wstring wstr = ::ca2::international::utf8_to_unicode(str);
      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int)wstr.get_length(), &size));
      return size;
   }

   bool graphics::select_path(::draw2d::path * ppath)
   {

      m_sppath = ppath;

      return m_sppath.is_set();

   }

   bool graphics::set(::draw2d::path * ppath)
   {

      if(ppath == NULL)
         return false;

      BeginPath();


      for(int i = 0; i < ppath->m_elementa.get_count(); i++)
      {

         set(ppath->m_elementa[i]);

      }

      EndPath();

      return true;

   }


   bool graphics::set(::draw2d::path::element & e)
   {

      switch(e.m_etype)
      {
      case ::draw2d::path::element::type_arc:
         set(e.m_arc);
         break;
      case ::draw2d::path::element::type_line:
         set(e.m_line);
         break;
      case ::draw2d::path::element::type_move:
         set(e.m_move);
         break;
      case ::draw2d::path::element::type_end:
         {

            if(e.m_end.m_bClose)
            {
               CloseFigure();
            }

         }
         break;
      default:
         throw "unexpected simple os graphics element type";
      }

      return false;


   }


   bool graphics::set(::draw2d::path::line & line)
   {

      return ::LineTo(m_hdc, (int32_t) line.m_x, (int32_t) line.m_y) != FALSE;

   }


   bool graphics::set(::draw2d::path::arc & arc)
   {

      rect r;

      r.left         = (LONG) (arc.m_xCenter - arc.m_dRadiusX);
      r.right        = (LONG) (arc.m_xCenter + arc.m_dRadiusX);
      r.top          = (LONG) (arc.m_yCenter - arc.m_dRadiusY);
      r.bottom       = (LONG) (arc.m_yCenter + arc.m_dRadiusY);

      POINT pt1;

      POINT pt2;

      double e = arc.m_dRadiusY / arc.m_dRadiusX;

      double f1 = arc.m_dAngle1;

      double f2 = arc.m_dAngle2;

      pt1.x          = (LONG) (arc.m_xCenter + arc.m_dRadiusX * cos(f1));
      pt1.y          = (LONG) (arc.m_yCenter - arc.m_dRadiusY * sin(f1));
      pt2.x          = (LONG) (arc.m_xCenter + arc.m_dRadiusX * cos(f2));
      pt2.y          = (LONG) (arc.m_yCenter - arc.m_dRadiusY * sin(f2));

      return ::ArcTo(m_hdc, r.left, r.top, r.right - r.left, r.bottom - r.top, pt1.x, pt1.y, pt2.x, pt2.y) != FALSE;

   }

   bool graphics::set(::draw2d::path::move & move)
   {

      ::MoveToEx(m_hdc, (int32_t) move.m_x, (int32_t) move.m_y, NULL);

      return true;

   }


   bool graphics::draw_path(::draw2d::path * ppath)
   {

      if(!select_path(ppath))
         return false;

      return StrokePath();

   }

   bool graphics::fill_path(::draw2d::path * ppath)
   {

      if(!select_path(ppath))
         return false;

      return FillPath();

   }


   void graphics::select_pen()
   {

      if(m_sppen.is_set() && m_sppen->get_os_data() != NULL)
      {

         if(m_sppen->get_os_data() != GetCurrentObject(m_hdc, OBJ_PEN))
         {

            SelectObject(m_sppen);

         }

      }
      else
      {

         select_null_pen();

      }

   }

   void graphics::select_brush()
   {

      if(m_spbrush.is_set() && m_spbrush->get_os_data() != NULL)
      {

         if(m_spbrush->get_os_data() != GetCurrentObject(m_hdc, OBJ_BRUSH))
         {

            SelectObject(m_spbrush);

         }

      }
      else
      {

         ::SelectObject(m_hdc, ::GetStockObject(NULL_BRUSH));

      }

   }


   void graphics::select_font()
   {

      if(m_spfont.is_set() && m_spfont->get_os_data() != NULL)
      {

         if(m_spfont->get_os_data() != GetCurrentObject(m_hdc, OBJ_FONT))
         {

            SelectObject(m_spfont);

         }

      }

   }


   void graphics::select_null_pen()
   {

      ::SelectObject(m_hdc, ::GetStockObject(NULL_PEN));

   }

   void graphics::select_null_brush()
   {

      ::SelectObject(m_hdc, ::GetStockObject(NULL_BRUSH));

   }

   bool graphics::SelectFont(::draw2d::font * pfont)
   {
      // SIOOT - Should implemennt one of them
      // OASOWO - otherwise a stack overflow will occur
      // BTAIOM - because these are interface only methods

      return SelectObject(pfont) != NULL;

   }

} // namespace draw2d_gdi





