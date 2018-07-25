#include "framework.h"


#undef new


count g_cForkBlend = 0;


/**
*
* Trilinear scale two images, pixels and pixels2, to get a new
* interpolated image with size width * height.
* pixels is the larger image with size w * h.
* pixels2 is the smaller image with size w2 * h2.
* width must be w >= width >= w2, width != 0
* height must be h >= height >= h2, height != 0
* Note: in Mip Mapping pixels2 should be half of pixels in dimension.
*
* @param pixels Larger image pixels.
* @param w Larger image width.
* @param h Larger image height.
* @param pixels2 Smaller image pixels.
* @param w2 Smaller image width.
* @param h2 Smaller image height.
* @param width New width.
* @param height New height.
* @return New array with size width * height
*/
void trilinearImageScaling(
COLORREF * ret, int width, int height, int scan,
COLORREF * pixels, int w, int h, // larger image
COLORREF * pixels2, int w2, int h2, // smaller image
int scan2)
{

   int index, index2;
   COLORREF A, B, C, D, E, F, G, H;
   float x, y, x2, y2, w_diff, h_diff, w2_diff, h2_diff, red, green, blue, alpha;
   // find ratio for larger image
   float w_ratio = ((float)(w - 1)) / width;
   float h_ratio = ((float)(h - 1)) / height;
   // find ratio for smaller image
   float w2_ratio = ((float)(w2 - 1)) / width;
   float h2_ratio = ((float)(h2 - 1)) / height;
   // estimate h3 distance
   float h3_diff = (w - width) / (float)(w - w2);
   int offset = 0;
   COLORREF * line;
   COLORREF * line2;
   COLORREF * lineRet;
   int wscan = scan / sizeof(COLORREF);
   int wscan2 = scan2 / sizeof(COLORREF);
   for (int i = 0; i<height; i++)
   {
      lineRet = ret + wscan * i;
      line = pixels + wscan2 * i;
      line2 = pixels2 + wscan2 * i;
      for (int j = 0; j<width; j++)
      {
         // find w1 and h1 for larger image
         x = w_ratio * j;
         y = h_ratio * i;
         w_diff = x - (int)x;
         h_diff = y - (int)y;
         index = (y)*wscan2 + (x);
         A = line[index];
         B = line[index + 1];
         C = line[index + wscan2];
         D = line[index + wscan2 + 1];
         // find w2 and h2 for smaller image
         x2 = w2_ratio * j;
         y2 = h2_ratio * i;
         w2_diff = x2 - (int)x2;
         h2_diff = y2 - (int)y2;
         index2 = (y2)*wscan2 + (x2);
         E = line2[index2];
         F = line2[index2 + 1];
         G = line2[index2 + wscan2];
         H = line2[index2 + wscan2 + 1];


         // trilinear interpolate blue element
         blue = (A & 0xff)*(1 - w_diff)*(1 - h_diff)*(1 - h3_diff) +
                (B & 0xff)*(w_diff)*(1 - h_diff)*(1 - h3_diff) +
                (C & 0xff)*(h_diff)*(1 - w_diff)*(1 - h3_diff) +
                (D & 0xff)*(w_diff)*(h_diff)*(1 - h3_diff) +
                (E & 0xff)*(1 - w2_diff)*(1 - h2_diff)*(h3_diff)+
                (F & 0xff)*(w2_diff)*(1 - h2_diff)*(h3_diff)+
                (G & 0xff)*(h2_diff)*(1 - w2_diff)*(h3_diff)+
                (H & 0xff)*(w2_diff)*(h2_diff)*(h3_diff);

         // trilinear interpolate green element
         green = ((A >> 8) & 0xff)*(1 - w_diff)*(1 - h_diff)*(1 - h3_diff) +
                 ((B >> 8) & 0xff)*(w_diff)*(1 - h_diff)*(1 - h3_diff) +
                 ((C >> 8) & 0xff)*(h_diff)*(1 - w_diff)*(1 - h3_diff) +
                 ((D >> 8) & 0xff)*(w_diff)*(h_diff)*(1 - h3_diff) +
                 ((E >> 8) & 0xff)*(1 - w2_diff)*(1 - h2_diff)*(h3_diff)+
                 ((F >> 8) & 0xff)*(w2_diff)*(1 - h2_diff)*(h3_diff)+
                 ((G >> 8) & 0xff)*(h2_diff)*(1 - w2_diff)*(h3_diff)+
                 ((H >> 8) & 0xff)*(w2_diff)*(h2_diff)*(h3_diff);

         // trilinear interpolate red element
         red = ((A >> 16) & 0xff)*(1 - w_diff)*(1 - h_diff)*(1 - h3_diff) +
               ((B >> 16) & 0xff)*(w_diff)*(1 - h_diff)*(1 - h3_diff) +
               ((C >> 16) & 0xff)*(h_diff)*(1 - w_diff)*(1 - h3_diff) +
               ((D >> 16) & 0xff)*(w_diff)*(h_diff)*(1 - h3_diff) +
               ((E >> 16) & 0xff)*(1 - w2_diff)*(1 - h2_diff)*(h3_diff)+
               ((F >> 16) & 0xff)*(w2_diff)*(1 - h2_diff)*(h3_diff)+
               ((G >> 16) & 0xff)*(h2_diff)*(1 - w2_diff)*(h3_diff)+
               ((H >> 16) & 0xff)*(w2_diff)*(h2_diff)*(h3_diff);

         // trilinear interpolate alpha element
         alpha = ((A >> 24) & 0xff)*(1 - w_diff)*(1 - h_diff)*(1 - h3_diff) +
                 ((B >> 24) & 0xff)*(w_diff)*(1 - h_diff)*(1 - h3_diff) +
                 ((C >> 24) & 0xff)*(h_diff)*(1 - w_diff)*(1 - h3_diff) +
                 ((D >> 24) & 0xff)*(w_diff)*(h_diff)*(1 - h3_diff) +
                 ((E >> 24) & 0xff)*(1 - w2_diff)*(1 - h2_diff)*(h3_diff)+
                 ((F >> 24) & 0xff)*(w2_diff)*(1 - h2_diff)*(h3_diff)+
                 ((G >> 24) & 0xff)*(h2_diff)*(1 - w2_diff)*(h3_diff)+
                 ((H >> 24) & 0xff)*(w2_diff)*(h2_diff)*(h3_diff);


         *lineRet =
         (int)(blue) |
         ((int)(green)) << 8 |
         ((int)(red)) << 16 |
         ((int)(alpha)) << 24;
         lineRet++;
      }
   }

}

class g_keep
{

public:

   Gdiplus::Graphics * p;
   Gdiplus::GraphicsState s;

   g_keep(Gdiplus::Graphics * p1)
   {

      p = p1;

      s = p->Save();

   }

   ~g_keep()
   {

      p->Restore(s);

   }

};



namespace draw2d_gdiplus
{


   graphics::graphics(::aura::application * papp) :
      object(papp),
      ::draw2d::graphics(papp)
   {

      m_bPrinting       = FALSE;
      m_pgraphics       = NULL;
      m_hdc             = NULL;
      m_hdcGraphics     = NULL;
      m_ppath           = NULL;
      m_ppathPaint      = NULL;
      m_etextrendering  = ::draw2d::text_rendering_none;
      m_dFontFactor     = 1.0;

      m_pm = new Gdiplus::Matrix();

   }

#ifdef DEBUG
   void graphics::assert_valid() const
   {

      object::assert_valid();

   }


   void graphics::dump(dump_context & dumpcontext) const
   {
      object::dump(dumpcontext);

      dumpcontext << "get_handle1() = " << get_handle1();
      dumpcontext << "\nm_hAttribDC = " << get_handle2();
      dumpcontext << "\nm_bPrinting = " << m_bPrinting;

      dumpcontext << "\n";
   }

#endif

   graphics::~graphics()
   {
      ::aura::del(m_pm);

      DeleteDC();

   }


   //::window_sp graphics::GetWindow()
   //{
   //   //ASSERT(get_handle1() != NULL); return ::draw2d_gdiplus::user::interaction_impl::from_handle(::WindowFromDC(get_handle1()));
   //   return NULL;
   //}

   bool graphics::IsPrinting()
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


   bool graphics::CreateCompatibleDC(::draw2d::graphics * pgraphics)
   {

      HDC hdc = Detach();

      if(hdc != NULL)
      {

         bool bDeleted = ::DeleteDC(hdc) != FALSE;

         if(!bDeleted)
         {

            TRACE("Failed to delete GDI device context");

         }

      }

      if(m_hdcGraphics != NULL)
      {

         m_pgraphics->ReleaseHDC(m_hdcGraphics);

         m_hdcGraphics = NULL;

      }

      if(m_pgraphics != NULL)
      {

         try
         {

            delete m_pgraphics;

         }
         catch(...)
         {

            TRACE("graphics::CreateCompatibleDC(::draw2d::graphics * ) : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = NULL;

      }

      if(pgraphics == NULL)
      {

         hdc = ::CreateCompatibleDC(NULL);

      }
      else if(GDIPLUS_GRAPHICS(pgraphics)->m_hdc != NULL)
      {

         hdc = ::CreateCompatibleDC(GDIPLUS_GRAPHICS(pgraphics)->m_hdc);

      }
      else if(GDIPLUS_GRAPHICS(pgraphics)->m_hdcGraphics != NULL)
      {

         hdc = ::CreateCompatibleDC(GDIPLUS_GRAPHICS(pgraphics)->m_hdcGraphics);

      }
      else
      {

         HDC hdcTemplate = ((Gdiplus::Graphics *) pgraphics->get_os_data())->GetHDC();

         hdc = ::CreateCompatibleDC(hdcTemplate);

         ((Gdiplus::Graphics *) pgraphics->get_os_data())->ReleaseHDC(hdc);

      }

      if(hdc == NULL)
         return false;

      m_hdc = hdc;

      m_pgraphics = new Gdiplus::Graphics(m_hdc);

      m_pgraphics->SetPageUnit(Gdiplus::UnitPixel);

      set_text_rendering(::draw2d::text_rendering_anti_alias);

      set_smooth_mode(::draw2d::smooth_mode_high);

      return true;

   }


   //int32_t graphics::ExcludeUpdateRgn(::user::primitive * pwindow)
   //{
   //   ASSERT(get_handle1() != NULL);
   //   ::exception::throw_not_implemented(get_app());
   //   //return ::ExcludeUpdateRgn(get_handle1(), WIN_WINDOW(pwindow)->get_handle());
   //   return 0;
   //}

   int32_t graphics::GetDeviceCaps(int32_t nIndex)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetDeviceCaps(get_handle2(), nIndex);
   }

   point graphics::GetBrushOrg()
   {
      ASSERT(get_handle1() != NULL);
      POINT point;
      VERIFY(::GetBrushOrgEx(get_handle1(), &point));
      return point;
   }

   point graphics::SetBrushOrg(int32_t x, int32_t y)
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

   int32_t graphics::EnumObjects(int32_t nObjectType, int32_t (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData)
   {
      ASSERT(get_handle2() != NULL);
      return ::EnumObjects(get_handle2(), nObjectType, (GOBJENUMPROC)lpfn, lpData);
   }

   ::draw2d::bitmap* graphics::SelectObject(::draw2d::bitmap * pBitmap)
   {


      if(pBitmap == NULL)
         return NULL;

      /*      if(get_handle1() == NULL)
               return NULL;
            if(pBitmap == NULL)
               return NULL;
            return dynamic_cast < ::draw2d::bitmap* > (SelectGdiObject(get_app(), get_handle1(), pBitmap->get_os_data()));*/
      if(m_pgraphics != NULL)
      {
         try
         {

            delete m_pgraphics;

         }
         catch(...)
         {

            TRACE("graphics::SelectObject(::draw2d::bitmap *) : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = NULL;

      }

      m_pgraphics = new Gdiplus::Graphics((Gdiplus::Bitmap *) pBitmap->get_os_data());

      m_pgraphics->SetPageUnit(Gdiplus::UnitPixel);

      set_text_rendering(::draw2d::text_rendering_anti_alias);

      set_smooth_mode(::draw2d::smooth_mode_high);

      m_spbitmap = pBitmap;

      return m_spbitmap;
   }


   //::draw2d::object* graphics::SelectObject(::draw2d::object* pObject)
   //{
   //   /*      ASSERT(get_handle1() != NULL);
   //      if(pObject == NULL)
   //         return NULL;
   //      return SelectGdiObject(get_app(), get_handle1(), pObject->get_os_data()); */
   //   return NULL;
   //}

   //HGDIOBJ graphics::SelectObject(HGDIOBJ hObject) // Safe for NULL handles
   //{

   //   UINT uiType = GetObjectType(hObject);

   //   if(uiType == OBJ_BITMAP)
   //   {

   //      HBITMAP hbitmap = (HBITMAP) hObject;

   //      if(m_spbitmap.is_null())
   //         m_spbitmap.alloc(allocer());

   //      if(m_spbitmap.is_null())
   //         return NULL;

   //      (dynamic_cast < ::draw2d_gdiplus::bitmap * > (m_spbitmap.m_p))->m_pbitmap = new Gdiplus::Bitmap(hbitmap, NULL);

   //      if(m_pgraphics != NULL)
   //      {

   //         try
   //         {

   //            delete m_pgraphics;

   //         }
   //         catch(...)
   //         {

   //            TRACE("graphics::SelectObject(HGDIOBJ) OBJ_BITMAP : Failed to delete Gdiplus::Graphics");

   //         }

   //         m_pgraphics = NULL;

   //      }

   //      m_pgraphics = new Gdiplus::Graphics((Gdiplus::Bitmap *) m_spbitmap->get_os_data());

   //      m_pgraphics->SetPageUnit(Gdiplus::UnitPixel);

   //      set_text_rendering(::draw2d::text_rendering_anti_alias);

   //      set_smooth_mode(::draw2d::smooth_mode_high);

   //      return hbitmap;

   //   }

   //   //*ASSERT(get_handle1() == get_handle2()); // ASSERT a simple graphics object
   //   //return (hObject != NULL) ? ::SelectObject(get_handle1(), hObject) : NULL; */
   //   return NULL;
   //}

   COLORREF graphics::GetNearestColor(COLORREF crColor)
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

   int32_t graphics::GetPolyFillMode()
   {
      return ::GetPolyFillMode(get_handle2());
   }

   int32_t graphics::GetROP2()
   {
      return ::GetROP2(get_handle2());
   }

   int32_t graphics::GetStretchBltMode()
   {
      return ::GetStretchBltMode(get_handle2());
   }

   int32_t graphics::GetMapMode()
   {
      return ::GetMapMode(get_handle2());
   }


   int32_t graphics::GetGraphicsMode()
   {

      return ::GetGraphicsMode(get_handle2());

   }


   bool graphics::GetWorldTransform(XFORM* pXform)
   {



      m_pgraphics->GetTransform(((graphics *)this)->m_pm);

      Gdiplus::REAL r[6];

      m_pm->GetElements(r);

      pXform->eM11 = r[0];
      pXform->eM12 = r[1];
      pXform->eM21 = r[2];
      pXform->eM22 = r[3];

      pXform->eDx = r[4];
      pXform->eDy = r[5];


      return true;

   }

   size graphics::GetViewportExt()
   {
      SIZE size;
      ::GetViewportExtEx(get_handle2(), &size);
      return size;
   }

   point graphics::GetWindowOrg()
   {
      POINT point;
      ::GetWindowOrgEx(get_handle2(), &point);
      return point;
   }

   size graphics::GetWindowExt()
   {
      SIZE size;
      ::GetWindowExtEx(get_handle2(), &size);
      return size;
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

   void graphics::DPtoLP(LPPOINT lpPoints, count nCount)
   {
      ::DPtoLP(get_handle2(), lpPoints, (int) nCount);
   }

   void graphics::DPtoLP(LPRECT lpRect)
   {
      ::DPtoLP(get_handle2(), (LPPOINT)lpRect, 2);
   }

   void graphics::LPtoDP(LPPOINT lpPoints,count nCount)
   {
      ::LPtoDP(get_handle2(), lpPoints, (int)  nCount);
   }

   void graphics::LPtoDP(LPRECT lpRect)
   {
      ::LPtoDP(get_handle2(), (LPPOINT)lpRect, 2);
   }

   bool graphics::FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush)
   {

      return ::FillRgn(get_handle1(), (HRGN)pRgn->get_os_data(), (HBRUSH)pBrush->get_os_data()) != FALSE;

   }

   bool graphics::FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, int32_t nWidth, int32_t nHeight)
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

      return ::PaintRgn(get_handle1(), (HRGN)pRgn->get_os_data())  != FALSE;

   }

   bool graphics::PtVisible(int32_t x, int32_t y)
   {

      ASSERT(get_handle1() != NULL);

      return ::PtVisible(get_handle1(), x, y) != FALSE;

   }

   bool graphics::PtVisible(POINT point)
   {
      ASSERT(get_handle1() != NULL);   // call virtual
      return PtVisible(point.x, point.y);
   }

   bool graphics::RectVisible(const RECT &  lpRect)
   {

      ASSERT(get_handle1() != NULL);

      return ::RectVisible(get_handle1(), &lpRect) != FALSE;

   }

   pointd graphics::current_position()
   {
      ASSERT(get_handle2() != NULL);
      POINT point;
      VERIFY(::GetCurrentPositionEx(get_handle2(), &point));
      return point;
   }

   bool graphics::Polyline(const POINT* lpPoints,count nCount)
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

         bOk1 = m_pgraphics->DrawLines(gdiplus_pen(),ppoints,(INT) nCount) == Gdiplus::Status::Ok;

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


   bool graphics::Arc(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t x3,int32_t y3,int32_t x4,int32_t y4)
   {

      double centerx    = (x2 + x1) / 2.0;
      double centery    = (y2 + y1) / 2.0;

      double start      = atan2(y3 - centery,x3 - centerx) * 180.0 / System.math().GetPi();
      double end        = atan2(y4 - centery,x4 - centerx) * 180.0 / System.math().GetPi();
      double sweep      = fabs(end - start);

      /*if(GetArcDirection() == AD_COUNTERCLOCKWISE)
      {
         sweep = -sweep;
      }
      */

      return Arc(x1,y1,x2-x1,y2-y1,start,sweep);

   }


   bool graphics::Arc(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4)
   {

      double centerx    = (x2 + x1) / 2.0;
      double centery    = (y2 + y1) / 2.0;

      double start      = atan2(y3 - centery,x3 - centerx) * 180.0 / System.math().GetPi();
      double end        = atan2(y4 - centery,x4 - centerx) * 180.0 / System.math().GetPi();
      double sweep      = fabs(end - start);

      /*if(GetArcDirection() == AD_COUNTERCLOCKWISE)
      {
         sweep = -sweep;
      }
      */

      return Arc(x1,y1, x2-x1, y2-y1, start, sweep);

   }


   bool graphics::Arc(int32_t x1,int32_t y1,int32_t w,int32_t h,double start, double extends)
   {

      ::Gdiplus::Rect rect(x1,y1,w,h);

      return m_pgraphics->DrawArc(gdiplus_pen(),rect,(Gdiplus::REAL) start,(Gdiplus::REAL) extends) == Gdiplus::Status::Ok;

   }


   bool graphics::Arc(double x1,double y1,double w,double h,double start,double extends)
   {

      ::Gdiplus::RectF rectf((Gdiplus::REAL) x1,(Gdiplus::REAL) y1,(Gdiplus::REAL) w,(Gdiplus::REAL) h);

      return m_pgraphics->DrawArc(gdiplus_pen(),rectf,(Gdiplus::REAL) start,(Gdiplus::REAL) extends) == Gdiplus::Status::Ok;

   }


   void graphics::frame_rect(LPCRECT lpcrect,::draw2d::brush* pBrush)
   {

      draw3d_rect(lpcrect, pBrush->m_cr, pBrush->m_cr);

   }


   bool graphics::rectangle(LPCRECT lpcrect)
   {

      bool bOk1 = fill_rect(lpcrect);

      bool bOk2 = draw_rect(lpcrect);

      return bOk1 && bOk2;

   }


   bool graphics::rectangle(LPCRECTD lpcrect)
   {

      bool bOk1 = fill_rect(lpcrect);

      bool bOk2 = draw_rect(lpcrect);

      return bOk1 && bOk2;


   }


   bool graphics::draw_rect(LPCRECT lpcrect, ::draw2d::pen * ppen)
   {

      Gdiplus::Rect rect;

      copy(rect, lpcrect);

      return m_pgraphics->DrawRectangle((::Gdiplus::Pen *) ppen->get_os_data(), rect) == ::Gdiplus::Ok;

   }


   bool graphics::draw_rect(LPCRECTD lpcrect, ::draw2d::pen * ppen)
   {

      Gdiplus::RectF rect;

      copy(rect, lpcrect);

      return m_pgraphics->DrawRectangle((::Gdiplus::Pen *) ppen->get_os_data(), rect) == ::Gdiplus::Ok;

   }


   void graphics::invert_rect(LPCRECT lpcrect)
   {

      //::draw2d::savedc save(this);

      //Gdiplus::REAL colorMatrixElements[][] = {
      //   {-1,  0,  0,  0, 0},
      //   {0,  -1,  0,  0, 0},
      //   {0,   0,  -1, 0, 0},
      //   {0,   0,  0,  1, 0},
      //   {1,   1,  1,  0, 1}};

      //ColorMatrix * colorMatrix = new ColorMatrix(colorMatrixElements);

      //imageAttributes.SetColorMatrix(
      //   colorMatrix,
      //   ColorMatrixFlag.Default,
      //   ColorAdjustType.Bitmap);

      //e.Graphics.DrawImage(image, 10, 10, width, height);

      //e.Graphics.DrawImage(
      //   image,
      //   new Rectangle(150, 10, width, height),  // destination rectangle
      //   0, 0,        // upper-left corner of source rectangle
      //   width,       // width of source rectangle
      //   height,      // height of source rectangle
      //   GraphicsUnit.Pixel,
      //   imageAttributes);
      //auto e = m_pgraphics->GetCompositingMode();

      //m_pgraphics->SetCompositingMode(Gdiplus::CombineModeXor);

      //Gdiplus::SolidBrush b(Color(255, 255, 255, 255));

      //Gdiplus::Rect rect;

      //copy(rect, lpcrect);

      //m_pgraphics->FillRectangle(&b, rect);

      //m_pgraphics->SetCompositingMode(e);

   }


   bool graphics::DrawIcon(int32_t x, int32_t y, ::visual::icon * picon)
   {

      return BitBlt(x, y, 32, 32, picon->get_dib(32, 32)->get_graphics());

   }


   bool graphics::DrawIcon(POINT point, ::visual::icon * picon)
   {

      return DrawIcon(point.x, point.y, picon);

   }


   bool graphics::DrawIcon(int32_t x, int32_t y, ::visual::icon * picon, int32_t cx, int32_t cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags)
   {

      return BitBlt(x, y, cx, cy, picon->get_dib(cx, cy)->get_graphics());

   }


   bool graphics::DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags, HBRUSH hBrush)
   {

      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), hBrush, NULL, (LPARAM)hBitmap, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, ::draw2d::bitmap* pBitmap, UINT nFlags, ::draw2d::brush* pBrush)
   {

      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL, (LPARAM)pBitmap->get_os_data(), 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_BITMAP) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, HBRUSH hBrush)
   {

      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), hBrush, NULL, (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE;

   }


   bool graphics::DrawState(point pt, size size, HICON hIcon, UINT nFlags, ::draw2d::brush* pBrush)
   {

      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL, (LPARAM)hIcon, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_ICON) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, bool bPrefixText, int32_t nTextLen, HBRUSH hBrush)
   {

      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), hBrush,  NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, const char * lpszText, UINT nFlags, bool bPrefixText, int32_t nTextLen, ::draw2d::brush* pBrush)
   {

      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(), NULL, (LPARAM)lpszText, (WPARAM)nTextLen, pt.x, pt.y, size.cx, size.cy, nFlags|(bPrefixText ? DST_PREFIXTEXT : DST_TEXT)) != FALSE;

   }

   bool graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, HBRUSH hBrush)
   {
      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), hBrush,
                         lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE;
   }
   bool graphics::DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc, LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush)
   {
      ASSERT(get_handle1() != NULL);
      return ::DrawState(get_handle1(), (HBRUSH)pBrush->get_os_data(),
                         lpDrawProc, lData, 0, pt.x, pt.y, size.cx, size.cy, nFlags|DST_COMPLEX) != FALSE;
   }
   bool graphics::DrawEdge(const RECT & rectParam,UINT nEdge,UINT nFlags)
   {
      ASSERT(get_handle1() != NULL);
      return ::DrawEdge(get_handle1(),(LPRECT)&rectParam,nEdge,nFlags) != FALSE;
   }
   bool graphics::DrawFrameControl(const RECT & rectParam,UINT nType,UINT nState)
   {
      ASSERT(get_handle1() != NULL);
      return ::DrawFrameControl(get_handle1(),(LPRECT) &rectParam,nType,nState) != FALSE;
   }

   bool graphics::Chord(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
                        int32_t x4, int32_t y4)
   {
      ASSERT(get_handle1() != NULL);
      return ::Chord(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;
   }
   bool graphics::Chord(const RECT &  rectParam,POINT ptStart,POINT ptEnd)
   {
      ASSERT(get_handle1() != NULL);
      return ::Chord(get_handle1(),rectParam.left,rectParam.top,
                     rectParam.right,rectParam.bottom,ptStart.x,ptStart.y,
                     ptEnd.x, ptEnd.y) != FALSE;
   }
   void graphics::DrawFocusRect(const RECT &  rectParam)
   {
      ASSERT(get_handle1() != NULL);
      ::DrawFocusRect(get_handle1(),&rectParam);
   }


   bool graphics::DrawEllipse(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Pen * ppen = gdiplus_pen();

      if (ppen == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->DrawEllipse(ppen, x1, y1, x2 - x1, y2 - y1)) == Gdiplus::Status::Ok;

   }


   bool graphics::DrawEllipse(const RECT &  rectParam)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Pen * ppen = gdiplus_pen();

      if (ppen == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->DrawEllipse(ppen,rectParam.left,rectParam.top,rectParam.right - rectParam.left,rectParam.bottom - rectParam.top)) == Gdiplus::Status::Ok;

   }



   bool graphics::DrawEllipse(double x1,double y1,double x2,double y2)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Pen * ppen = gdiplus_pen();

      if (ppen == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->DrawEllipse(ppen,(Gdiplus::REAL)x1,(Gdiplus::REAL)y1,(Gdiplus::REAL)(x2 - x1),(Gdiplus::REAL)(y2 - y1))) == Gdiplus::Status::Ok;

   }


   bool graphics::DrawEllipse(const RECTD & rectParam)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Pen * ppen = gdiplus_pen();

      if (ppen == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->DrawEllipse(ppen,(Gdiplus::REAL)rectParam.left,(Gdiplus::REAL)rectParam.top,
                                       (Gdiplus::REAL)(rectParam.right - rectParam.left),
                                       (Gdiplus::REAL)(rectParam.bottom - rectParam.top))) == Gdiplus::Status::Ok;

   }


   bool graphics::FillEllipse(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Brush * pbrush = gdiplus_brush();

      if (pbrush == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->FillEllipse(pbrush, x1, y1, x2 - x1, y2 - y1)) == Gdiplus::Status::Ok;

   }


   bool graphics::FillEllipse(const RECT &  rectParam)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Brush * pbrush = gdiplus_brush();

      if (pbrush == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->FillEllipse(pbrush, rectParam.left, rectParam.top, rectParam.right - rectParam.left, rectParam.bottom - rectParam.top)) == Gdiplus::Status::Ok;

   }


   bool graphics::FillEllipse(double x1,double y1,double x2,double y2)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Brush * pbrush = gdiplus_brush();

      if (pbrush == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->FillEllipse(pbrush,(Gdiplus::REAL)x1,(Gdiplus::REAL)y1,(Gdiplus::REAL)(x2 - x1),(Gdiplus::REAL)(y2 - y1))) == Gdiplus::Status::Ok;

   }


   bool graphics::FillEllipse(const RECTD & rectParam)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Brush * pbrush = gdiplus_brush();

      if (pbrush == NULL)
      {

         return false;

      }

      set_smooth_mode(::draw2d::smooth_mode_high);

      return (m_pgraphics->FillEllipse(pbrush,(Gdiplus::REAL)rectParam.left,(Gdiplus::REAL)rectParam.top,
                                       (Gdiplus::REAL)(rectParam.right - rectParam.left),
                                       (Gdiplus::REAL)(rectParam.bottom - rectParam.top))) == Gdiplus::Status::Ok;

   }


   bool graphics::Pie(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4)
   {
      ASSERT(get_handle1() != NULL);
      return ::Pie(get_handle1(), x1, y1, x2, y2, x3, y3, x4, y4) != FALSE;
   }
   bool graphics::Pie(const RECT &  rectParam,POINT ptStart,POINT ptEnd)
   {
      ASSERT(get_handle1() != NULL);
      return ::Pie(get_handle1(), rectParam.left, rectParam.top,
                   rectParam.right, rectParam.bottom, ptStart.x, ptStart.y,
                   ptEnd.x, ptEnd.y) != FALSE;
   }

   bool graphics::fill_polygon(const POINTD * lpPoints,count nCount)
   {

      if(nCount <= 0)
         return TRUE;

      bool bOk1 = FALSE;

      Gdiplus::PointF * ppoints = new Gdiplus::PointF[nCount];

      try
      {

         for(int32_t i = 0; i < nCount; i++)
         {
            ppoints[i].X = (Gdiplus::REAL) lpPoints[i].x;
            ppoints[i].Y = (Gdiplus::REAL) lpPoints[i].y;
         }

         m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

         set_smooth_mode(::draw2d::smooth_mode_high);


         bOk1 = m_pgraphics->FillPolygon(gdiplus_brush(), ppoints, (INT) nCount, gdiplus_get_fill_mode()) == Gdiplus::Status::Ok;

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

   bool graphics::fill_polygon(const POINT* lpPoints,count nCount)
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

         m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

         bOk1 = m_pgraphics->FillPolygon(gdiplus_brush(), ppoints, (INT)  nCount, gdiplus_get_fill_mode()) == Gdiplus::Status::Ok;

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

   bool graphics::draw_polygon(const POINT* lpPoints,count nCount)
   {

      if (nCount <= 0)
         return TRUE;

      bool bOk1 = FALSE;

      Gdiplus::Point * ppoints = new Gdiplus::Point[nCount];

      try
      {

         for (int32_t i = 0; i < nCount; i++)
         {
            ppoints[i].X = lpPoints[i].x;
            ppoints[i].Y = lpPoints[i].y;
         }

         m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

         set_smooth_mode(::draw2d::smooth_mode_high);

         bOk1 = m_pgraphics->DrawPolygon(gdiplus_pen(), ppoints, (INT) nCount) == Gdiplus::Status::Ok;

      }
      catch (...)
      {
      }

      try
      {
         delete ppoints;
      }
      catch (...)
      {
      }


      return bOk1;
   }

   bool graphics::draw_polygon(const POINTD* lpPoints,count nCount)
   {

      if(nCount <= 0)
         return TRUE;

      bool bOk1 = FALSE;

      Gdiplus::PointF * ppoints = new Gdiplus::PointF[nCount];

      try
      {

         for(int32_t i = 0; i < nCount; i++)
         {
            ppoints[i].X = (Gdiplus::REAL) lpPoints[i].x;
            ppoints[i].Y = (Gdiplus::REAL) lpPoints[i].y;
         }

         m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

         bOk1 = m_pgraphics->DrawPolygon(gdiplus_pen(),ppoints,(INT) nCount) == Gdiplus::Status::Ok;

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


   bool graphics::fill_rect(LPCRECT lpcrect, ::draw2d::brush * pbrush)
   {

      Gdiplus::Rect rect;

      copy(rect, lpcrect);

      auto emodeOld = m_pgraphics->GetSmoothingMode();

      if (emodeOld != Gdiplus::SmoothingModeNone)
      {

         m_pgraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);

      }

      bool bOk = m_pgraphics->FillRectangle((Gdiplus::Brush *) pbrush->get_os_data(), rect) == Gdiplus::Status::Ok;

      if (emodeOld != Gdiplus::SmoothingModeNone)
      {

         m_pgraphics->SetSmoothingMode(emodeOld);

      }

      return bOk;

   }


   bool graphics::fill_rect(LPCRECTD lpcrect, ::draw2d::brush * pbrush)
   {

      Gdiplus::RectF rect;

      copy(rect, lpcrect);

      return m_pgraphics->FillRectangle((Gdiplus::Brush *) pbrush->get_os_data(), rect) == Gdiplus::Status::Ok;

   }


   bool graphics::round_rect(LPCRECT lpcrect, LPCPOINT lppt)
   {

      _throw(todo(get_app()));

   }


   bool graphics::round_rect(LPCRECTD lpcrect, LPCPOINTD lppt)
   {

      _throw(todo(get_app()));

   }


   bool graphics::PatBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, uint32_t dwRop)
   {
      ASSERT(get_handle1() != NULL);
      return ::PatBlt(get_handle1(), x, y, nWidth, nHeight, dwRop) != FALSE;
   }


   bool graphics::BitBltRaw(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, uint32_t dwRop)
   {

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Status ret = Gdiplus::Status::GenericError;

      try
      {

         if (pgraphicsSrc == NULL)
         {

            return false;

         }

         if (pgraphicsSrc->get_current_bitmap() == NULL)
         {

            goto gdi_fallback;

         }

         if (pgraphicsSrc->get_current_bitmap()->get_os_data() == NULL)
         {

            goto gdi_fallback;

         }


         bool bThreadToolsForIncreasedFps = ::get_thread()== NULL ? false : ::get_thread()->m_bThreadToolsForIncreasedFps;

         bool bAvoidProcFork = ::get_thread() == NULL ? true : ::get_thread()->m_bAvoidProcFork;

         if (!bAvoidProcFork && bThreadToolsForIncreasedFps && pgraphicsSrc->m_pdibDraw2dGraphics != NULL && m_pdibDraw2dGraphics != NULL)
         {

            if (m_ealphamode == ::draw2d::alpha_mode_blend)
            {

               single_lock sl(::get_thread_toolset(::thread::tool_draw2d)->m_pmutex);

               if (nHeight >= get_processor_count() * 4 && (nWidth * nHeight) >= (get_processor_count() * 64) && sl.lock(millis(0)))
               {

                  m_pdibDraw2dGraphics->fork_blend(point(x + GetViewportOrg().x, y + GetViewportOrg().y), pgraphicsSrc->m_pdibDraw2dGraphics,
                                                   point(xSrc + pgraphicsSrc->GetViewportOrg().x, ySrc + pgraphicsSrc->GetViewportOrg().y),
                                                   size(nWidth, nHeight));

                  g_cForkBlend++;

                  if (g_cForkBlend % 100 == 0)
                  {
                     output_debug_string("\nfork_blend(" + ::str::from(g_cForkBlend) + ") sample=" + ::str::from(nWidth) + "," + ::str::from(nHeight));
                  }

               }
               else
               {
                  m_pdibDraw2dGraphics->blend(point(x + GetViewportOrg().x, y + GetViewportOrg().y), pgraphicsSrc->m_pdibDraw2dGraphics,
                                              point(xSrc + pgraphicsSrc->GetViewportOrg().x, ySrc + pgraphicsSrc->GetViewportOrg().y),
                                              size(nWidth, nHeight));

               }
            }
            else
            {

               m_pdibDraw2dGraphics->from(point(x + GetViewportOrg().x, y + GetViewportOrg().y), pgraphicsSrc->m_pdibDraw2dGraphics,
                                          point(xSrc + pgraphicsSrc->GetViewportOrg().x, ySrc + pgraphicsSrc->GetViewportOrg().y),
                                          size(nWidth, nHeight));


            }


            return true;

         }

         ret = m_pgraphics->DrawImage(
               (Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(),
               x, y, xSrc + pgraphicsSrc->GetViewportOrg().x, ySrc + pgraphicsSrc->GetViewportOrg().y, nWidth, nHeight, Gdiplus::UnitPixel);

         if(ret != Gdiplus::Status::Ok)
         {

            return false;

         }

      }
      catch(...)
      {

         return false;

      }

      return true;

gdi_fallback:

      HDC hdcDst = get_hdc();

      if(hdcDst == NULL)
         return false;

      HDC hdcSrc = GDIPLUS_GRAPHICS(pgraphicsSrc)->get_hdc();

      if(hdcSrc == NULL)
      {

         release_hdc(hdcDst);

         return false;

      }

      bool bOk = ::BitBlt(hdcDst, x, y, nWidth, nHeight, hdcSrc, x, y, dwRop) != FALSE;

      GDIPLUS_GRAPHICS(pgraphicsSrc)->release_hdc(hdcSrc);

      release_hdc(hdcDst);

      return bOk;

   }


   bool graphics::StretchBlt(int32_t xDst, int32_t yDst, int32_t nDstWidth, int32_t nDstHeight, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, uint32_t dwRop)
   {

      if (nDstWidth == nSrcWidth && nDstHeight == nSrcHeight)
      {

         return BitBlt(xDst, yDst, nSrcWidth, nSrcHeight, pgraphicsSrc, xSrc, ySrc, dwRop);

      }

      if(pgraphicsSrc == NULL)
         return FALSE;

      Gdiplus::Rect dstRect( xDst,  yDst, nDstWidth, nDstHeight);

//      Gdiplus::Rect srcRect( xSrc, ySrc,  nSrcWidth, nSrcHeight);

      if(pgraphicsSrc == NULL || pgraphicsSrc->get_current_bitmap() == NULL)
         return false;

      Gdiplus::Status ret = Gdiplus::Status::GenericError;

      //m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

      //m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

      if (pgraphicsSrc->m_pdibDraw2dGraphics != NULL)
      {

         if (pgraphicsSrc->m_pdibDraw2dGraphics->m_emipmap == ::draw2d::mipmap_anisotropic
               && (pgraphicsSrc->m_pdibDraw2dGraphics->m_size.cx == nSrcWidth
                   && pgraphicsSrc->m_pdibDraw2dGraphics->m_size.cy == nSrcHeight
                   && xSrc == 0 && ySrc == 0 && nDstWidth > 0 && nDstHeight > 0))
         {

            try
            {

               /*int x1 = 0;
               int y1 = 0;
               int x2 = 0;
               int y2 = 0;
               int dx = nSrcWidth;
               int dy = nSrcHeight;
               int cx1 = nSrcWidth;
               int cy1 = nSrcHeight;
               int cx2 = nSrcWidth;
               int cy2 = nSrcHeight;

               while (dx >= nDstWidth)
               {
                  x1 = x2;
                  x2 += dx;
                  cx1 = dx;
                  dx /= 2;
                  cx2 = dx;
               }

               while (dy >= nSrcHeight)
               {
                  y1 = y2;
                  y2 += dy;
                  cy1 = dy;
                  dy /= 2;
                  cy2 = dy;
               }

               if (y1 == nSrcHeight)
               {
                  y1 = 0;
               }
               if (y2 == nSrcHeight)
               {
                  y2 = 0;
               }*/

               //::draw2d::dib * dib = m_pdibDraw2dGraphics;
               //int iScan = dib->m_iScan;
               //::draw2d::dib * dibMipmap = pgraphicsSrc->m_pdibDraw2dGraphics;
               //COLORREF * pcrMipmap = dibMipmap->m_pcolorref;
               //int iMimapScan = dibMipmap->m_iScan;
               //size sizeMipmap = dibMipmap->m_size;

               //trilinearImageScaling(
               //&dib->m_pcolorref[xDst + iScan * yDst / sizeof(COLORREF)],
               //nDstWidth, nDstHeight,
               //iScan,
               //&pcrMipmap[x1 + y1 * iMimapScan / sizeof(COLORREF)],
               //cx1, cy1,
               //&pcrMipmap[x2 + y2 * iMimapScan / sizeof(COLORREF)],
               //cx2, cy2,
               //iMimapScan);

               int iFind = -1;

               double dRateFound = 1024.0;

               int xFound;
               int yFound;
               int cxFound;
               int cyFound;

               for (index i = 0; i < pgraphicsSrc->m_pdibDraw2dGraphics->get_dib_count(); i++)
               {

                  int x1 = 0;
                  int y1 = 0;
                  int x2 = 0;
                  int y2 = 0;
                  int dx = nSrcWidth;
                  int dy = nSrcHeight;
                  int cx1 = nSrcWidth;
                  int cy1 = nSrcHeight;
                  int cx2 = nSrcWidth;
                  int cy2 = nSrcHeight;

                  while (dx >= nDstWidth)
                  {
                     x1 = x2;
                     x2 += dx;
                     cx1 = dx;
                     dx /= 2;
                     cx2 = dx;
                  }

                  while (dy >= nSrcHeight)
                  {
                     y1 = y2;
                     y2 += dy;
                     cy1 = dy;
                     dy /= 2;
                     cy2 = dy;
                  }

                  if (y1 == nSrcHeight)
                  {
                     y1 = 0;
                  }
                  if (y2 == nSrcHeight)
                  {
                     y2 = 0;
                  }

                  double dRateX = (double)nDstWidth / (double)cx1;

                  double dRateY = (double)nDstHeight / (double)cy1;

                  double dRateArea = dRateX * dRateY;

                  if (dRateArea < dRateFound)
                  {

                     iFind = i;

                     dRateFound = dRateArea;

                     xFound = x1;

                     yFound = y1;

                     cxFound = cx1;

                     cyFound = cy1;

                  }

               }

               if (iFind >= 0)
               {

                  ::draw2d::dib * pdib = pgraphicsSrc->m_pdibDraw2dGraphics->get_dib(iFind);

                  auto emode = m_pgraphics->GetInterpolationMode();

                  m_pgraphics->SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighSpeed);

                  m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);

                  //m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeBilinear);

                  ret = m_pgraphics->DrawImage(
                        (Gdiplus::Bitmap *) pdib->get_bitmap()->get_os_data(),
                        dstRect, xFound, yFound, cxFound, cyFound, Gdiplus::UnitPixel);

                  m_pgraphics->SetInterpolationMode(emode);

                  if (ret == Gdiplus::Status::Ok)
                  {

                     return true;

                  }

               }

            }
            catch (...)
            {


            }



         }

      }

      try
      {

         ret = m_pgraphics->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), dstRect, xSrc, ySrc, nSrcWidth, nSrcHeight, Gdiplus::UnitPixel);

         if(ret != Gdiplus::Status::Ok)
         {

            return false;

         }

      }
      catch(...)
      {

         return false;

      }

      return true;

   }


   COLORREF graphics::GetPixel(int32_t x, int32_t y)
   {
      ASSERT(get_handle1() != NULL);
      return ::GetPixel(get_handle1(), x, y);
   }
   COLORREF graphics::GetPixel(POINT point)
   {
      ASSERT(get_handle1() != NULL);
      return ::GetPixel(get_handle1(), point.x, point.y);
   }
   COLORREF graphics::SetPixel(int32_t x, int32_t y, COLORREF crColor)
   {
      return 0;
   }


   COLORREF graphics::SetPixel(POINT point, COLORREF crColor)
   {

      if (m_pdibDraw2dGraphics != NULL)
      {

         m_pdibDraw2dGraphics->map();

         m_pdibDraw2dGraphics->m_pcolorref[point.x + point.y * m_pdibDraw2dGraphics->m_iScan] = crColor;

      }
      else
      {

         fill_solid_rect_dim(point.x, point.y, 1, 1, crColor);

      }

      return crColor;

   }

   bool graphics::FloodFill(int32_t x, int32_t y, COLORREF crColor)
   {
      ASSERT(get_handle1() != NULL);
      return ::FloodFill(get_handle1(), x, y, crColor) != FALSE;
   }
   bool graphics::ExtFloodFill(int32_t x, int32_t y, COLORREF crColor, UINT nFillType)
   {
      ASSERT(get_handle1() != NULL);
      return ::ExtFloodFill(get_handle1(), x, y, crColor, nFillType) != FALSE;
   }


   // true blend
   // COLOR_DEST = SRC_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA) * COLOR_DST

   // double blend
   //// COLOR_DEST = SRC_ALPHA * BLEND_ALPHA * COLOR_SRC  + (1 - SRC_ALPHA * BLEND_ALPHA) * COLOR_DST


   bool graphics::ExtTextOut(int32_t x,int32_t y,UINT nOptions,const RECT &  rectParam,const char * lpszString,strsize nCount,LPINT lpDxWidths)
   {

      ASSERT(get_handle1() != NULL);

      return ::ExtTextOut(get_handle1(),x,y,nOptions,&rectParam,lpszString,(UINT) nCount,lpDxWidths) != FALSE;

   }


   bool graphics::ExtTextOut(int32_t x,int32_t y,UINT nOptions,const RECT &  rectParam, const string & str, LPINT lpDxWidths)
   {

      ASSERT(get_handle1() != NULL);

      return ::ExtTextOut(get_handle1(),x,y,nOptions,&rectParam, str, (UINT)str.get_length(), lpDxWidths) != FALSE;

   }


   size graphics::TabbedTextOut(int32_t x, int32_t y, const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin)
   {

      ASSERT(get_handle1() != NULL);

      return ::TabbedTextOut(get_handle1(), x, y, lpszString, (int) nCount, (int) nTabPositions, lpnTabStopPositions, nTabOrigin);

   }


   size graphics::TabbedTextOut(int32_t x, int32_t y, const string & str, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin)
   {

      ASSERT(get_handle1() != NULL);

      return ::TabbedTextOut(get_handle1(), x, y, str, (int32_t)str.get_length(), (int) nTabPositions, lpnTabStopPositions, nTabOrigin);

   }


   size graphics::GetTabbedTextExtent(const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions)
   {

      ASSERT(get_handle2() != NULL);

      return ::GetTabbedTextExtent(get_handle2(), lpszString, (int32_t) nCount, (int) nTabPositions, lpnTabStopPositions);

   }


   size graphics::GetTabbedTextExtent(const string & str, count nTabPositions, LPINT lpnTabStopPositions)
   {

      ASSERT(get_handle2() != NULL);

      return ::GetTabbedTextExtent(get_handle2(), str, (int32_t) str.get_length(), (int) nTabPositions, lpnTabStopPositions);

   }


   size graphics::GetOutputTabbedTextExtent(const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions)
   {

      ASSERT(get_handle1() != NULL);

      return ::GetTabbedTextExtent(get_handle1(), lpszString, (int32_t) nCount, (int) nTabPositions, lpnTabStopPositions);

   }


   size graphics::GetOutputTabbedTextExtent(const string & str, count nTabPositions, LPINT lpnTabStopPositions)
   {

      ASSERT(get_handle1() != NULL);

      return ::GetTabbedTextExtent(get_handle1(), str, (int32_t) str.get_length(), (int) nTabPositions, lpnTabStopPositions);

   }


   bool graphics::GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int32_t), LPARAM lpData, int32_t nCount,int32_t x, int32_t y, int32_t nWidth, int32_t nHeight)
   {

      ASSERT(get_handle1() != NULL);

      return ::GrayString(get_handle1(), (HBRUSH)pBrush->get_os_data(),(GRAYSTRINGPROC)lpfnOutput, lpData, nCount, x, y, nWidth, nHeight) != FALSE;

   }


   UINT graphics::GetTextAlign()
   {

      ASSERT(get_handle2() != NULL);

      return ::GetTextAlign(get_handle2());

   }


   int32_t graphics::GetTextFace(count nCount, LPTSTR lpszFacename)
   {

      ASSERT(get_handle2() != NULL);

      return ::GetTextFace(get_handle2(), (int) nCount, lpszFacename);

   }


   int32_t graphics::GetTextFace(string & rString)
   {

      ASSERT(get_handle2() != NULL);

      int32_t nResult = ::GetTextFace(get_handle2(), 256, rString.GetBuffer(256));
      rString.ReleaseBuffer();

      return nResult;

   }

   bool graphics::get_text_metrics(::draw2d::text_metric * lpMetrics)
   {

      if (m_pgraphics == NULL)
         return false;

      graphics * pgraphics = ((graphics *)this);

      if(pgraphics->gdiplus_font() == NULL)
         return false;

      Gdiplus::Font * pfont = pgraphics->gdiplus_font();

      Gdiplus::FontFamily family;

      pfont->GetFamily(&family);

      INT iStyle = pfont->GetStyle();

      double dHeight = family.GetEmHeight(iStyle);

      double dSize = pfont->GetSize();

      double dFontHeight = pfont->GetHeight((Gdiplus::REAL) pgraphics->get_dpiy());

      lpMetrics->tmAscent              = (LONG) (dSize * family.GetCellAscent(iStyle) / dHeight);
      lpMetrics->tmDescent             = (LONG) (dSize * family.GetCellDescent(iStyle) / dHeight);
      lpMetrics->tmHeight              = (LONG)dFontHeight;

      double dLineSpacing = MAX(dFontHeight, dSize * family.GetLineSpacing(iStyle) / dHeight);

      lpMetrics->tmInternalLeading     = (LONG) (lpMetrics->tmAscent + lpMetrics->tmDescent - lpMetrics->tmHeight);
      lpMetrics->tmExternalLeading     = (LONG) (dLineSpacing - (lpMetrics->tmAscent + lpMetrics->tmDescent));

      const Gdiplus::FontFamily * pfamilyMono = family.GenericMonospace();

      ::Gdiplus::Font font2(pfamilyMono, pfamilyMono->GetEmHeight(((graphics * )this)->gdiplus_font()->GetStyle()));

      wstring wstr(L"123AWZwmc123AWZwmcpQ軾");
      Gdiplus::RectF rect(0.f, 0.f, 1024.f * 1024.f, 1024.f * 1024.f);
      Gdiplus::RectF rect2;
      Gdiplus::PointF origin(0, 0);

      m_pgraphics->MeasureString(wstr, (INT) wstr.get_length(), ((graphics * )this)->gdiplus_font(), origin, &rect);


      /*wstr = L"";
      m_pgraphics->MeasureString(wstr.m_pwsz, -1, (Gdiplus::Font *) m_font->get_os_data(), origin, &rect2);*/

      lpMetrics->tmAveCharWidth = (LONG) (rect.Width * get_current_font()->m_dFontWidth / (double) wstr.get_length());


      return TRUE;
   }


   bool graphics::get_output_text_metrics(::draw2d::text_metric * lpMetrics)
   {

      ASSERT(get_handle1() != NULL);

      //return ::GetTextMetricsW(get_handle1(), lpMetrics) != FALSE;

      ::exception::throw_not_implemented(get_app());

      return false;

   }


   int32_t graphics::GetTextCharacterExtra()
   {
      ASSERT(get_handle2() != NULL);
      return ::GetTextCharacterExtra(get_handle2());
   }
   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetCharWidth(get_handle2(), nFirstChar, nLastChar, lpBuffer) != FALSE;
   }
   bool graphics::GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer)
   {
      ASSERT(get_handle1() != NULL);
      return ::GetCharWidth(get_handle1(), nFirstChar, nLastChar, lpBuffer) != FALSE;
   }
   uint32_t graphics::GetFontLanguageInfo()
   {
      ASSERT(get_handle1() != NULL);
      return ::GetFontLanguageInfo(get_handle1());
   }


   uint32_t graphics::GetCharacterPlacement(const char * lpString, strsize nCount, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags)
   {

      ASSERT(get_handle1() != NULL);

      return ::GetCharacterPlacement(get_handle1(), lpString, (int) nCount, (int) nMaxExtent, lpResults, dwFlags);

   }


   uint32_t graphics::GetCharacterPlacement(string & str, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags)
   {

      ASSERT(get_handle1() != NULL);

      return ::GetCharacterPlacement(get_handle1(), (const char *)str, (int32_t) str.get_length(), (int) nMaxExtent, lpResults, dwFlags);

   }


   size graphics::GetAspectRatioFilter()
   {
      ASSERT(get_handle2() != NULL);
      SIZE size;
      VERIFY(::GetAspectRatioFilterEx(get_handle2(), &size));
      return size;
   }
   bool graphics::ScrollDC(int32_t dx, int32_t dy,
                           const RECT &  lpRectScroll,const RECT &  rectClip,
                           ::draw2d::region* pRgnUpdate, LPRECT lpRectUpdate)
   {
      ASSERT(get_handle1() != NULL);
      //return ::ScrollDC(get_handle1(),dx,dy,&rectClip,
      //                  &rectClip, (HRGN)pRgnUpdate->get_os_data(), lpRectUpdate) != FALSE;
      return false;
   }

   // Printer Escape Functions
   int32_t graphics::Escape(int32_t nEscape, int32_t nCount, const char * lpszInData, LPVOID lpOutData)
   {
      ASSERT(get_handle1() != NULL);
      return ::Escape(get_handle1(), nEscape, nCount, lpszInData, lpOutData);
   }

   // graphics 3.1 Specific functions
   UINT graphics::SetBoundsRect(const RECT &  rectBounds, UINT flags)
   {
      ASSERT(get_handle1() != NULL);
      return ::SetBoundsRect(get_handle1(), &rectBounds, flags);
   }
   UINT graphics::GetBoundsRect(LPRECT lpRectBounds, UINT flags)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetBoundsRect(get_handle2(), lpRectBounds, flags);
   }
   bool graphics::ResetDC(const DEVMODE* lpDevMode)
   {
      ASSERT(get_handle2() != NULL);
      return ::ResetDC(get_handle2(), lpDevMode) != NULL;
   }
   UINT graphics::GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetOutlineTextMetricsW(get_handle2(), cbData, lpotm);
   }
   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetCharABCWidths(get_handle2(), nFirstChar, nLastChar, lpabc) != FALSE;
   }
   uint32_t graphics::GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData,
                                  uint32_t cbData)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetFontData(get_handle2(), dwTable, dwOffset, lpData, cbData);
   }
   int32_t graphics::GetKerningPairs(int32_t nPairs, LPKERNINGPAIR lpkrnpair)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetKerningPairs(get_handle2(), nPairs, lpkrnpair);
   }
   uint32_t graphics::GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
                                      uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetGlyphOutline(get_handle2(), nChar, nFormat,
                               lpgm, cbBuffer, lpBuffer, lpmat2);
   }

   // ::user::document handling functions
   int32_t graphics::StartDoc(LPDOCINFO lpDocInfo)
   {

      ASSERT(m_hdc != NULL);

      return ::StartDoc(m_hdc, lpDocInfo);

   }


   int32_t graphics::StartPage()
   {

      ASSERT(m_hdc != NULL);

      ::StartPage(m_hdc);

      m_pgraphics = new Gdiplus::Graphics(m_hdc);

      m_pgraphics->SetPageUnit(Gdiplus::UnitPixel);

      set_text_rendering(::draw2d::text_rendering_anti_alias);

      set_smooth_mode(::draw2d::smooth_mode_high);

      return 1;

   }


   int32_t graphics::EndPage()
   {

      ASSERT(m_hdc != NULL);

      delete m_pgraphics;


      return ::EndPage(m_hdc);

   }


   int32_t graphics::SetAbortProc(bool (CALLBACK* lpfn)(HDC, int32_t))
   {

      ASSERT(m_hdc != NULL);

      return ::SetAbortProc(m_hdc, (ABORTPROC)lpfn);

   }


   int32_t graphics::AbortDoc()
   {

      ASSERT(m_hdc != NULL);

      return ::AbortDoc(m_hdc);

   }


   int32_t graphics::EndDoc()
   {

      ASSERT(m_hdc != NULL);

      return ::EndDoc(m_hdc);

   }

   bool graphics::MaskBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::draw2d::graphics * pgraphicsSrc,
                          int32_t xSrc, int32_t ySrc, ::draw2d::bitmap& maskBitmap, int32_t xMask, int32_t yMask, uint32_t dwRop)
   {
      ASSERT(get_handle1() != NULL);
      return ::MaskBlt(get_handle1(), x, y, nWidth, nHeight, GDIPLUS_HDC(pgraphicsSrc),
                       xSrc, ySrc,  (HBITMAP)maskBitmap.get_os_data(), xMask, yMask, dwRop) != FALSE;
   }

   bool graphics::PlgBlt(LPPOINT lpPoint, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc,
                         int32_t nWidth, int32_t nHeight, ::draw2d::bitmap& maskBitmap, int32_t xMask, int32_t yMask)
   {

      Gdiplus::Status ret = Gdiplus::Status::GenericError;

      try
      {

         if(pgraphicsSrc == NULL)
            return FALSE;

         if(pgraphicsSrc->get_current_bitmap() == NULL)
            return false;

         if(pgraphicsSrc->get_current_bitmap()->get_os_data() == NULL)
            return false;

         Gdiplus::Point p[3];

         p[0].X = lpPoint[0].x;
         p[0].Y = lpPoint[0].y;
         p[1].X = lpPoint[1].x;
         p[1].Y = lpPoint[1].y;
         p[2].X = lpPoint[2].x;
         p[2].Y = lpPoint[2].y;

         ret = m_pgraphics->DrawImage((Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data(), p, 3);

         if (ret != Gdiplus::Status::Ok)
         {

            return false;

         }

      }
      catch(...)
      {

         return false;

      }

      return true;

   }



   bool graphics::SetPixelV(int32_t x, int32_t y, COLORREF crColor)
   {
      ASSERT(get_handle1() != NULL);
      return ::SetPixelV(get_handle1(), x, y, crColor) != FALSE;
   }
   bool graphics::SetPixelV(POINT point, COLORREF crColor)
   {
      ASSERT(get_handle1() != NULL);
      return ::SetPixelV(get_handle1(), point.x, point.y, crColor) != FALSE;
   }

   bool graphics::AngleArc(int32_t x, int32_t y, int32_t nRadius, float fStartAngle, float fSweepAngle)
   {
      ASSERT(get_handle1() != NULL);
      return ::AngleArc(get_handle1(), x, y, nRadius, fStartAngle, fSweepAngle) != FALSE;
   }


   bool graphics::ArcTo(const RECT &  rectParam,POINT ptStart,POINT ptEnd)
   {
      ASSERT(get_handle1() != NULL);
      return ArcTo(rectParam.left, rectParam.top, rectParam.right,
                   rectParam.bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
   }
   int32_t graphics::GetArcDirection()
   {
      ASSERT(get_handle2() != NULL);
      return ::GetArcDirection(get_handle2());
   }


   bool graphics::PolyPolyline(const POINT* lpPoints, const INT * lpPolyPoints, count nCount)
   {

      ASSERT(get_handle1() != NULL);

      return ::PolyPolyline(get_handle1(), lpPoints, (LPDWORD) lpPolyPoints, (DWORD) nCount) != FALSE;

   }


   bool graphics::GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE;
   }

   ::draw2d::pen_sp graphics::get_current_pen()
   {

      return m_sppen;

   }

   ::draw2d::brush_sp graphics::get_current_brush()
   {

      return m_spbrush;

   }

   ::draw2d::palette_sp graphics::get_current_palette()
   {

      return (::draw2d::palette *)NULL;

   }

   ::draw2d::font_sp graphics::get_current_font()
   {

      return m_spfont;

   }

   ::draw2d::bitmap_sp graphics::get_current_bitmap()
   {

      return m_spbitmap;

   }


   bool graphics::PolyBezier(const POINT* lpPoints, count nCount)
   {

      ASSERT(get_handle1() != NULL);

      return ::PolyBezier(get_handle1(), lpPoints, (DWORD) nCount) != FALSE;

   }


   int32_t graphics::DrawEscape(int32_t nEscape, int32_t nInputSize, const char * lpszInputData)
   {

      ASSERT(get_handle1() != NULL);

      return ::DrawEscape(get_handle1(), nEscape, nInputSize, lpszInputData);

   }


   int32_t graphics::Escape(int32_t nEscape, int32_t nInputSize, const char * lpszInputData,  int32_t nOutputSize, char * lpszOutputData)
   {

      ASSERT(get_handle1() != NULL);

      return ::ExtEscape(get_handle1(), nEscape, nInputSize, lpszInputData, nOutputSize, lpszOutputData);

   }


   bool graphics::GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
                                   LPABCFLOAT lpABCF)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetCharABCWidthsFloat(get_handle2(), nFirstChar, nLastChar, lpABCF) != FALSE;
   }
   bool graphics::GetCharWidth(UINT nFirstChar, UINT nLastChar, float* lpFloatBuffer)
   {
      ASSERT(get_handle2() != NULL);
      return ::GetCharWidthFloat(get_handle2(), nFirstChar, nLastChar, lpFloatBuffer) != FALSE;
   }


   bool graphics::abort_path()
   {

      if(m_ppath != NULL)
      {

         delete m_ppath;

         m_ppath = NULL;

      }

      return true;

   }


   bool graphics::begin_path()
   {

      if (m_ppath != NULL)
      {

         delete m_ppath;

      }

      m_ppath = new Gdiplus::GraphicsPath;

      return m_ppath != NULL;

   }


   bool graphics::close_figure()
   {

      ASSERT(m_ppath != NULL);

      return m_ppath->CloseFigure() == Gdiplus::Status::Ok;

   }


   bool graphics::end_path()
   {

      if (m_ppath == NULL)
      {

         return FALSE;

      }

      m_ppathPaint   = m_ppath;

      m_ppath        = NULL;

      return true;

   }


   bool graphics::fill_path()
   {

      return m_pgraphics->FillPath(gdiplus_brush(), m_ppath) == Gdiplus::Status::Ok;

   }


   bool graphics::flatten_path()
   {

      return m_ppath->Flatten() == Gdiplus::Status::Ok;

   }


   float graphics::GetMiterLimit()
   {
      ASSERT(get_handle1() != NULL);
      float fMiterLimit;
      VERIFY(::GetMiterLimit(get_handle1(), &fMiterLimit));
      return fMiterLimit;
   }


   int32_t graphics::GetPath(LPPOINT lpPoints, LPBYTE lpTypes, count nCount)
   {

      ASSERT(get_handle1() != NULL);

      return ::GetPath(get_handle1(), lpPoints, lpTypes, (int) nCount);

   }

   bool graphics::SetMiterLimit(float fMiterLimit)
   {
      ASSERT(get_handle1() != NULL);
      return ::SetMiterLimit(get_handle1(), fMiterLimit, NULL) != FALSE;
   }


   bool graphics::stroke_and_fill_path()
   {

      bool bOk1 = m_pgraphics->FillPath(gdiplus_brush(), m_ppathPaint) == Gdiplus::Status::Ok;

      bool bOk2 = m_pgraphics->DrawPath(gdiplus_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

      return bOk1 && bOk2;

   }


   bool graphics::stroke_path()
   {

      return m_pgraphics->DrawPath(gdiplus_pen(), m_ppathPaint) == Gdiplus::Status::Ok;

   }


   bool graphics::widen_path()
   {

      return m_ppath->Widen(gdiplus_pen()) == Gdiplus::Status::Ok;

   }


   bool graphics::draw_path(::draw2d::path * ppath)
   {

      m_pgraphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias8x8);
      m_pgraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);


      return m_pgraphics->DrawPath(gdiplus_pen(),(dynamic_cast < ::draw2d_gdiplus::path * > (ppath))->get_os_path(m_pgraphics)) == Gdiplus::Status::Ok;

   }

   bool graphics::draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen)
   {

      return m_pgraphics->DrawPath((::Gdiplus::Pen *) ppen->get_os_data(),(dynamic_cast < ::draw2d_gdiplus::path * > (ppath))->get_os_path(m_pgraphics)) == Gdiplus::Status::Ok;

   }

   bool graphics::fill_path(::draw2d::path * ppath)
   {

      return m_pgraphics->FillPath(gdiplus_brush(),(dynamic_cast < ::draw2d_gdiplus::path * > (ppath))->get_os_path(m_pgraphics)) == Gdiplus::Status::Ok;

   }

   bool graphics::fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrush)
   {

      if (ppath == NULL)
      {

         return false;

      }

      if (pbrush == NULL)
      {

         return false;

      }

      if (m_pgraphics == NULL)
      {

         return false;

      }

      ::Gdiplus::Brush * pbr = (::Gdiplus::Brush *) pbrush->get_os_data();

      if (pbr == NULL)
      {

         return false;

      }

      Gdiplus::GraphicsPath * ppathOs = (dynamic_cast <::draw2d_gdiplus::path *> (ppath))->get_os_path(m_pgraphics);

      if (ppathOs == NULL)
      {

         return false;

      }

      return m_pgraphics->FillPath(pbr, ppathOs) == Gdiplus::Status::Ok;

   }


   bool graphics::AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData)
   {

      ASSERT(get_handle1() != NULL);

      return ::GdiComment(get_handle1(), nDataSize, pCommentData) != FALSE;

   }


   bool graphics::PlayMetaFile(HENHMETAFILE hEnhMF, const RECT &  rectBounds)
   {

      Gdiplus::RectF rect((Gdiplus::REAL) rectBounds.left,(Gdiplus::REAL) rectBounds.top,(Gdiplus::REAL) width(rectBounds),(Gdiplus::REAL) height(rectBounds));

      ap(Gdiplus::Metafile) pMeta = new Gdiplus::Metafile(hEnhMF, false);

      Gdiplus::Status ret = m_pgraphics->DrawImage(pMeta, rect);

      if (ret != Gdiplus::Status::Ok)
      {

         return false;

      }

      return true;
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




   bool graphics::alpha_blendRaw(int32_t xDest, int32_t yDest, int32_t nDestWidth, int32_t nDestHeight, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, double dRate)
   {

      if (m_pgraphics == NULL || pgraphicsSrc == NULL)
      {

         return false;

      }

      bool bThreadToolsForIncreasedFps = ::get_thread()->m_bThreadToolsForIncreasedFps;

      bool bAvoidProcFork = ::get_thread()->m_bAvoidProcFork;

      if (!bAvoidProcFork && bThreadToolsForIncreasedFps && nDestWidth == nSrcWidth && nDestHeight == nSrcHeight)
      {

         if (m_ealphamode == ::draw2d::alpha_mode_blend)
         {

            if (nDestHeight >= get_processor_count() * 4 && (nDestWidth * nDestHeight) >= (get_processor_count() * 64))
            {

               m_pdibDraw2dGraphics->fork_blend(point(xDest + GetViewportOrg().x, yDest + GetViewportOrg().y), pgraphicsSrc->m_pdibDraw2dGraphics,
                                                point(xSrc + pgraphicsSrc->GetViewportOrg().x, ySrc + pgraphicsSrc->GetViewportOrg().y),
                                                size(nSrcWidth, nDestHeight), (byte)(dRate * 255.0f));

               g_cForkBlend++;

               if (g_cForkBlend % 100 == 0)
               {
                  output_debug_string("\nfork_blend(" + ::str::from(g_cForkBlend) + ") sample=" + ::str::from(nSrcWidth) + "," + ::str::from(nDestHeight));
               }

            }
            else
            {

               m_pdibDraw2dGraphics->blend(point(xDest + GetViewportOrg().x, yDest + GetViewportOrg().y), pgraphicsSrc->m_pdibDraw2dGraphics,
                                           point(xSrc+pgraphicsSrc->GetViewportOrg().x, ySrc + pgraphicsSrc->GetViewportOrg().y),
                                           size(nSrcWidth, nDestHeight), (byte)(dRate * 255.0f));

            }

         }
         else
         {

            m_pdibDraw2dGraphics->from(point(xDest + GetViewportOrg().x, yDest + GetViewportOrg().y), pgraphicsSrc->m_pdibDraw2dGraphics,
                                       point(xSrc + pgraphicsSrc->GetViewportOrg().x, ySrc + pgraphicsSrc->GetViewportOrg().y),
                                       size(nSrcWidth, nDestHeight), (byte) (dRate * 255.0f));


         }

         return true;

      }


      float fA = (float) (dRate);

      Gdiplus::ColorMatrix matrix =
      {
         1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f, fA, 0.0f,
         0.0f, 0.0f, 0.0f, 0.0f, 1.0f
      };

      Gdiplus::ImageAttributes attributes;

      attributes.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault, Gdiplus::ColorAdjustTypeBitmap);

      Gdiplus::RectF dstRect((Gdiplus::REAL) xDest, (Gdiplus::REAL) yDest, (Gdiplus::REAL) nDestWidth, (Gdiplus::REAL) nDestHeight);

      if (pgraphicsSrc == NULL)
      {

         return false;

      }

      if (pgraphicsSrc->get_current_bitmap() == NULL)
      {

         return false;

      }

      Gdiplus::Bitmap * pbitmap = NULL;

      try
      {

         pbitmap = (Gdiplus::Bitmap *) pgraphicsSrc->get_current_bitmap()->get_os_data();

      }
      catch(...)
      {

      }

      Gdiplus::Status ret = Gdiplus::Status::GenericError;

      if(pbitmap != NULL)
      {

         ret =  m_pgraphics->DrawImage(pbitmap,dstRect,(Gdiplus::REAL) xSrc,(Gdiplus::REAL) ySrc,(Gdiplus::REAL) nSrcWidth,(Gdiplus::REAL) nSrcHeight,Gdiplus::UnitPixel,&attributes);

         if (ret != Gdiplus::Status::Ok)
         {

            return false;

         }

      }

      return true;

   }



   bool graphics::TransparentBlt(int32_t xDest, int32_t yDest, int32_t nDestWidth, int32_t nDestHeight, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, UINT crTransparent)
   {

      ASSERT(get_handle1() != NULL);

      //return ::TransparentBlt(get_handle1(), xDest, yDest,  nDestWidth, nDestHeight, GDIPLUS_HDC(pgraphicsSrc), xSrc, ySrc, nSrcWidth,  nSrcHeight, crTransparent) != FALSE;

      return true;

   }

   bool graphics::GradientFill(TRIVERTEX* pVertices, ULONG nVertices, void * pMesh, ULONG nMeshElements, uint32_t dwMode)
   {

      ASSERT(get_handle1() != NULL);

      //return ::GradientFill(get_handle1(), pVertices, nVertices, pMesh, nMeshElements, dwMode) != FALSE;

      return true;

   }

   // This is core API library.
   //
   //
   //
   //
   //
   //
   //
   //

   // Always Inline. Functions only in Win98/Win2K or later

   inline COLORREF graphics::GetDCBrushColor()
   {
      ASSERT(get_handle1() != NULL);
      return ::GetDCBrushColor(get_handle1());
   }
   inline COLORREF graphics::SetDCBrushColor(COLORREF crColor)
   {
      ASSERT(get_handle1() != NULL);
      return ::SetDCBrushColor(get_handle1(), crColor);
   }

   inline COLORREF graphics::GetDCPenColor()
   {
      ASSERT(get_handle1() != NULL);
      return ::GetDCPenColor(get_handle1());
   }
   inline COLORREF graphics::SetDCPenColor(COLORREF crColor)
   {
      ASSERT(get_handle1() != NULL);
      return ::SetDCPenColor(get_handle1(), crColor);
   }

   inline bool graphics::GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc)
   {
      ASSERT(get_handle1() != NULL);
      return ::GetCharABCWidthsI(get_handle1(), giFirst, cgi, pgi, lpabc) != FALSE;
   }
   inline bool graphics::GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer)
   {
      ASSERT(get_handle1() != NULL);
      return ::GetCharWidthI(get_handle1(), giFirst, cgi, pgi, lpBuffer) != FALSE;
   }

   inline bool graphics::GetTextExtentExPointI(LPWORD pgiIn, int32_t cgi, int32_t nMaxExtent, LPINT lpnFit, LPINT alpDx, LPSIZE lpSize)
   {
      ENSURE(lpSize != NULL);
      ASSERT(get_handle1() != NULL);
      return ::GetTextExtentExPointI(get_handle1(), pgiIn, cgi, nMaxExtent, lpnFit, alpDx, lpSize) != FALSE;
   }
   inline bool graphics::GetTextExtentPointI(LPWORD pgiIn, int32_t cgi, LPSIZE lpSize)
   {
      ENSURE(lpSize != NULL);
      ASSERT(get_handle1() != NULL);
      return ::GetTextExtentPointI(get_handle1(), pgiIn, cgi, lpSize) != FALSE;
   }







#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

   void graphics::DPtoHIMETRIC(LPSIZE lpSize)
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      int32_t nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
            nMapMode != MM_TEXT)
      {
         // when using a constrained map mode, map against physical inch
         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
         DPtoLP(lpSize);
         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // map against logical inch for non-constrained mapping modes
         int32_t cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
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

   void graphics::HIMETRICtoDP(LPSIZE lpSize)
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      int32_t nMapMode;
      if (this != NULL && (nMapMode = GetMapMode()) < MM_ISOTROPIC &&
            nMapMode != MM_TEXT)
      {
         // when using a constrained map mode, map against physical inch
         ((::draw2d::graphics *)this)->SetMapMode(MM_HIMETRIC);
         LPtoDP(lpSize);
         ((::draw2d::graphics *)this)->SetMapMode(nMapMode);
      }
      else
      {
         // map against logical inch for non-constrained mapping modes
         int32_t cxPerInch, cyPerInch;
         if (this != NULL)
         {
            ASSERT_VALID(this);
            ASSERT(get_handle1() != NULL);  // no HDC attached or created?
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

   void graphics::LPtoHIMETRIC(LPSIZE lpSize)
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      LPtoDP(lpSize);
      DPtoHIMETRIC(lpSize);
   }

   void graphics::HIMETRICtoLP(LPSIZE lpSize)
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      HIMETRICtoDP(lpSize);
      DPtoLP(lpSize);
   }

   /////////////////////////////////////////////////////////////////////////////
   // special graphics drawing primitives/helpers

   ::draw2d::brush* graphics::GetHalftoneBrush(::aura::application * papp)
   {
      /*      ::core::LockGlobals(CRIT_HALFTONEBRUSH);
            if (gen_HalftoneBrush == NULL)
            {
               WORD grayPattern[8];
               for (int32_t i = 0; i < 8; i++)
                  grayPattern[i] = (WORD)(0x5555 << (i & 1));
               HBITMAP grayBitmap = CreateBitmap(8, 8, 1, 1, grayPattern);
               if (grayBitmap != NULL)
               {
                  gen_HalftoneBrush = ::CreatePatternBrush(grayBitmap);
                  ::DeleteObject(grayBitmap);
               }
            }
            if (!gen_WingdixTerm)
               gen_WingdixTerm = (char)!atexit(&__win_gdi_x_term);
            ::core::UnlockGlobals(CRIT_HALFTONEBRUSH);

      //      return ::draw2d_gdiplus::brush::from_handle(papp, gen_HalftoneBrush);*/
      return NULL;
   }

   void graphics::DrawDragRect(const RECT &  lpRect, SIZE size, const RECT &  lpRectLast, SIZE sizeLast, ::draw2d::brush* pBrush, ::draw2d::brush* pBrushLast)
   {

      ::exception::throw_not_implemented(get_app());

      /*

            ASSERT(__is_valid_address(lpRect, sizeof(RECT), FALSE));
            ASSERT(lpRectLast == NULL ||
               __is_valid_address(lpRectLast, sizeof(RECT), FALSE));

            // first, determine the update region and select it
            ::draw2d::region rgnNew;
            ::draw2d::region rgnOutside, rgnInside;
            rgnOutside.create_rect(lpRect);
            rect rect = *lpRect;
            rect.inflate(-size.cx, -size.cy);
            rect.intersect(rect, lpRect);
            rgnInside.create_rect(rect);
            rgnNew.create_rect(0, 0, 0, 0);
            rgnNew.combine(&rgnOutside, &rgnInside, RGN_XOR);

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

            ::draw2d::region rgnLast, rgnUpdate;
            if (lpRectLast != NULL)
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
            if (pBrush->get_os_data() != pBrushLast->get_os_data() && lpRectLast != NULL)
            {
               // brushes are different -- erase old region first
               SelectClipRgn(&rgnLast);
               GetClipBox(&rect);
               pBrushOld = SelectObject(pBrushLast);
               PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);
               SelectObject(pBrushOld);
               pBrushOld = NULL;
            }

            // draw into the update/new region
            SelectClipRgn(rgnUpdate.get_os_data() != NULL ? &rgnUpdate : &rgnNew);
            GetClipBox(&rect);
            pBrushOld = SelectObject(pBrush);
            PatBlt(rect.left, rect.top, rect.width(), rect.height(), PATINVERT);

            // cleanup DC
            if (pBrushOld != NULL)
               SelectObject(pBrushOld);
            SelectClipRgn(NULL);

      */
   }

   /*void graphics::fill_solid_rect(const RECT &  lpRect, COLORREF clr)
   {
      ::SetBkColor(get_handle1(), clr);
      ::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
   }*/

   void graphics::fill_solid_rect(LPCRECTD lpcrect, COLORREF cr)
   {

      try
      {

         if (m_pgraphics == NULL)
            return;

         Gdiplus::RectF r;

         copy(r, lpcrect);

         Gdiplus::SolidBrush b(Gdiplus::Color(argb_get_a_value(cr), argb_get_r_value(cr), argb_get_g_value(cr), argb_get_b_value(cr)));

         m_pgraphics->FillRectangle(&b, r);

      }
      catch (...)
      {

      }

   }


   /*

   void graphics::fill_solid_rect(int32_t x, int32_t y, int32_t cx, int32_t cy, COLORREF clr)
   {
      ::SetBkColor(get_handle1(), clr);
      rect rect(x, y, x + cx, y + cy);
      ::ExtTextOut(get_handle1(), 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
   }

   */

   //void graphics::draw3d_rect(const RECT &  rectParam,
   //                           COLORREF clrTopLeft, COLORREF clrBottomRight)
   //{
   //   draw3d_rect(rectParam.left, rectParam.top, rectParam.right - rectParam.left,
   //               rectParam.bottom - rectParam.top, clrTopLeft, clrBottomRight);
   //}


   void graphics::draw3d_rect(LPCRECT lpcrect, COLORREF crTopLeft, COLORREF crBottomRight)
   {

      {

         Gdiplus::Pen p(Gdiplus::Color(argb_get_a_value(crTopLeft), argb_get_r_value(crTopLeft), argb_get_g_value(crTopLeft), argb_get_b_value(crTopLeft)), 1.0);

         m_pgraphics->DrawLine(&p, lpcrect->left, lpcrect->bottom - 1, lpcrect->left, lpcrect->top);

         m_pgraphics->DrawLine(&p, lpcrect->left + 1, lpcrect->top, lpcrect->right - 1, lpcrect->top);

      }

      {

         Gdiplus::Pen p(Gdiplus::Color(argb_get_a_value(crBottomRight), argb_get_r_value(crBottomRight), argb_get_g_value(crBottomRight), argb_get_b_value(crBottomRight)), 1.0);

         m_pgraphics->DrawLine(&p, lpcrect->right - 1, lpcrect->top + 1, lpcrect->right - 1, lpcrect->bottom - 1);

         m_pgraphics->DrawLine(&p, lpcrect->right - 2, lpcrect->bottom - 1, lpcrect->left + 1, lpcrect->bottom - 1);

      }

   }


   void graphics::draw3d_rect(LPCRECTD lpcrect, COLORREF crTopLeft, COLORREF crBottomRight)
   {

      {

         Gdiplus::Pen p(Gdiplus::Color(argb_get_a_value(crTopLeft), argb_get_r_value(crTopLeft), argb_get_g_value(crTopLeft), argb_get_b_value(crTopLeft)), 1.0);

         m_pgraphics->DrawLine(&p, (Gdiplus::REAL) lpcrect->left, (Gdiplus::REAL) (lpcrect->bottom - 1.0), (Gdiplus::REAL)  lpcrect->left, (Gdiplus::REAL) lpcrect->top);

         m_pgraphics->DrawLine(&p, (Gdiplus::REAL) (lpcrect->left + 1.0), (Gdiplus::REAL) lpcrect->top, (Gdiplus::REAL) (lpcrect->right - 1.0), (Gdiplus::REAL) lpcrect->top);

      }

      {

         Gdiplus::Pen p(Gdiplus::Color(argb_get_a_value(crBottomRight), argb_get_r_value(crBottomRight), argb_get_g_value(crBottomRight), argb_get_b_value(crBottomRight)), 1.0);

         m_pgraphics->DrawLine(&p, (Gdiplus::REAL) (lpcrect->right - 1.0), (Gdiplus::REAL) (lpcrect->top + 1.0), (Gdiplus::REAL) (lpcrect->right - 1.0), (Gdiplus::REAL) (lpcrect->bottom - 1.0));

         m_pgraphics->DrawLine(&p, (Gdiplus::REAL) (lpcrect->right - 2.0), (Gdiplus::REAL) (lpcrect->bottom - 1.0), (Gdiplus::REAL) (lpcrect->left + 1.0), (Gdiplus::REAL) (lpcrect->bottom - 1.0));

      }

   }





   //::draw2d::graphics * ::draw2d_gdiplus::graphics::from_handle(HDC hDC)
   //{
   //hdc_map* pMap = afxMapHDC(TRUE); //create map if not exist
   //ASSERT(pMap != NULL);
//      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)pMap->from_handle(hDC);
   //    ASSERT(pgraphics == NULL || (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1() == hDC);
   //  return pgraphics;
   // return NULL;
   //}

   bool graphics::Attach(HDC hdc)
   {

      if(hdc == NULL)
         return FALSE;

      if(m_hdc == hdc)
      {

         if(m_pgraphics != NULL)
            return TRUE;

      }

      if(hdc != NULL)
      {

         m_pgraphics = new ::Gdiplus::Graphics((HDC) hdc);

         m_pgraphics->SetPageUnit(Gdiplus::UnitPixel);

         set_text_rendering(::draw2d::text_rendering_anti_alias);

         set_smooth_mode(::draw2d::smooth_mode_high);

         m_hdc = (HDC) hdc;

      }

      return m_pgraphics != NULL;

      /*ASSERT(get_handle1() == NULL);      // only attach once, detach on destroy
      ASSERT(get_handle2() == NULL);    // only attach to an is_empty DC

      if (hDC == NULL)
      {
         return FALSE;
      }
      // remember early to avoid leak
      set_handle1(hDC);
      hdc_map* pMap = afxMapHDC(TRUE); // create map if not exist
      ASSERT(pMap != NULL);
      pMap->set_permanent(get_handle1(), this);

      SetAttribDC(get_handle1());     // Default to same as output
      return TRUE;*/
   }

   HDC graphics::Detach()
   {

      if(m_hdc == NULL)
         return NULL;

      if(m_pgraphics == NULL)
         return NULL;

      try
      {

         delete m_pgraphics;

      }
      catch(...)
      {

         TRACE("graphics::Detach : Failed to delete Gdiplus::Graphics");

      }

      m_pgraphics = NULL;

      HDC hdc = m_hdc;
      m_hdc = NULL;

      return hdc;

   }


   bool graphics::AttachPrinter(HDC hdc)
   {

      if (hdc == NULL)
         return FALSE;

      if (m_hdc == hdc)
      {

         return TRUE;

      }

      if (hdc != NULL)
      {

         m_hdc = (HDC)hdc;

      }

      return m_hdc != NULL;

   }


   HDC graphics::DetachPrinter()
   {

      if (m_hdc == NULL)
         return NULL;

      HDC hdc = m_hdc;

      m_hdc = NULL;

      return hdc;

   }


   bool graphics::DeleteDC()
   {

      HDC hdc = Detach();

      if(hdc != NULL)
      {

         bool bDeleted = ::DeleteDC(hdc) != FALSE;

         if(!bDeleted)
         {

            TRACE("graphics::DeleteDC : Failed to delete GDI device context");

         }

      }

      if(m_pgraphics != NULL)
      {

         try
         {

            delete m_pgraphics;

         }
         catch(...)
         {

            TRACE("graphics::DeleteDC : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = NULL;

      }

      if(m_ppath != NULL)
      {

         try
         {

            delete m_ppath;

         }
         catch(...)
         {

         }

         m_ppath = NULL;

      }

      if(m_ppathPaint != NULL)
      {

         try
         {

            delete m_ppathPaint;

         }
         catch(...)
         {

         }

         m_ppathPaint = NULL;

      }

      return true;

   }


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

      Gdiplus::GraphicsState state = m_pgraphics->Save();

      return (int) state;

   }


   bool graphics::RestoreDC(int32_t nSavedDC)
   {

      Gdiplus::Status status = m_pgraphics->Restore((Gdiplus::GraphicsState) nSavedDC);

      return status == Gdiplus::Ok;

   }


//   ::draw2d::object* graphics::SelectGdiObject(::aura::application * papp, HDC hDC, HGDIOBJ h)
   // {
//      return ::draw2d_gdiplus::object::from_handle(papp, ::SelectObject(hDC, h));
   //}

   ::draw2d::object* graphics::SelectStockObject(int32_t nIndex)
   {
      /*      HGDIOBJ hObject = ::GetStockObject(nIndex);
            HGDIOBJ hOldObj = NULL;
            ASSERT(hObject != NULL);
            if(get_handle1() != NULL && get_handle1() != get_handle2())
               hOldObj = ::SelectObject(get_handle1(), hObject);
            if(get_handle2() != NULL)
               hOldObj = ::SelectObject(get_handle2(), hObject);
            return ::draw2d_gdiplus::object::from_handle(get_app(), hOldObj);*/

      return NULL;
   }

   ::draw2d::pen* graphics::SelectObject(::draw2d::pen* pPen)
   {
      /*HGDIOBJ hOldObj = NULL;
      if(pPen == NULL)
         return NULL;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         hOldObj = ::SelectObject(get_handle1(), pPen->get_os_data());
      if(get_handle2() != NULL)
         hOldObj = ::SelectObject(get_handle2(), pPen->get_os_data());
      return dynamic_cast < pen * > (::draw2d_gdiplus::object::from_handle(get_app(), hOldObj));*/
      m_sppen = pPen;
      return m_sppen;
   }

   ::draw2d::brush* graphics::SelectObject(::draw2d::brush* pBrush)
   {
      /*      HGDIOBJ hOldObj = NULL;
            if(pBrush == NULL)
               return NULL;
            if(get_handle1() != NULL && get_handle1() != get_handle2())
               hOldObj = ::SelectObject(get_handle1(), pBrush->get_os_data());
            if(get_handle2() != NULL)
               hOldObj = ::SelectObject(get_handle2(), pBrush->get_os_data());
            return dynamic_cast < ::draw2d::brush * > (::draw2d_gdiplus::object::from_handle(get_app(), hOldObj));*/
      m_spbrush = pBrush;
      return m_spbrush;

   }

   ::draw2d::font* graphics::SelectObject(::draw2d::font* pfont)
   {
      /*      HGDIOBJ hOldObj = NULL;
            if(pFont == NULL)
               return NULL;
            if(get_handle1() != NULL && get_handle1() != get_handle2())
               hOldObj = ::SelectObject(get_handle1(), pFont->get_os_data());
            if(get_handle2() != NULL)
               hOldObj = ::SelectObject(get_handle2(), pFont->get_os_data());
            return dynamic_cast < ::draw2d::font * > (::draw2d_gdiplus::object::from_handle(get_app(), hOldObj));*/

      /*ASSERT(pFont != NULL);

      if(pFont == NULL)
         return NULL;

      m_fontxyz = *pFont;
      return &m_fontxyz;*/

      if(!select_font(pfont))
         return NULL;

      return m_spfont;

   }

   int32_t graphics::SelectObject(::draw2d::region* pRgn)
   {
      int32_t nRetVal = GDI_ERROR;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = (int32_t)(int_ptr)::SelectObject(get_handle1(), (HGDIOBJ) pRgn->get_os_data());
      if(get_handle2() != NULL)
         nRetVal = (int32_t)(int_ptr)::SelectObject(get_handle2(), (HGDIOBJ) pRgn->get_os_data());
      return nRetVal;
   }

   ::draw2d::palette* graphics::SelectPalette(::draw2d::palette* pPalette, bool bForceBackground)
   {
      return NULL;
//      return dynamic_cast < ::draw2d::palette * > (::draw2d_gdiplus::object::from_handle(get_app(), ::SelectPalette(get_handle1(), (HPALETTE)pPalette->get_os_data(), bForceBackground)));
   }

   int32_t graphics::SetPolyFillMode(int32_t nPolyFillMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetPolyFillMode(get_handle1(), nPolyFillMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetPolyFillMode(get_handle2(), nPolyFillMode);
      return nRetVal;
   }

   int32_t graphics::SetROP2(int32_t nDrawMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetROP2(get_handle1(), nDrawMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetROP2(get_handle2(), nDrawMode);
      return nRetVal;
   }

   int32_t graphics::SetStretchBltMode(int32_t nStretchMode)
   {

      if(m_pgraphics == NULL)
      {

         return -1;

      }

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
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetStretchBltMode(get_handle1(), nStretchMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetStretchBltMode(get_handle2(), nStretchMode);
      return nRetVal;*/
   }


   int32_t graphics::SetGraphicsMode(int32_t iMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
      {
         nRetVal = ::SetGraphicsMode(get_handle1(), iMode);
      }
      if(get_handle2() != NULL)
      {
         nRetVal = ::SetGraphicsMode(get_handle2(), iMode);
      }
      return nRetVal;
   }

   bool graphics::SetWorldTransform(const XFORM* pXform)
   {

      Gdiplus::REAL r[6];

      r[0] = pXform->eDx;
      r[1] = pXform->eDy;

      r[2] = pXform->eM11;
      r[3] = pXform->eM12;
      r[4] = pXform->eM21;
      r[5] = pXform->eM22;

      Gdiplus::Matrix m;

      m.SetElements(r[2],r[3],r[4],r[5],r[0],r[1]);

      m_pgraphics->SetTransform(&m);

      return true;
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

   int32_t graphics::SetMapMode(int32_t nMapMode)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetMapMode(get_handle1(), nMapMode);
      if(get_handle2() != NULL)
         nRetVal = ::SetMapMode(get_handle2(), nMapMode);
      return nRetVal;

   }


   size graphics::SetViewportExt(int32_t x, int32_t y)
   {

      size size(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::SetViewportExtEx(get_handle1(), x, y, &size);
      //if(get_handle2() != NULL)
      //   ::SetViewportExtEx(get_handle2(), x, y, &size);
      return size;

   }


   point graphics::SetWindowOrg(int32_t x, int32_t y)
   {
      point point(0, 0);
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         ::SetWindowOrgEx(get_handle1(), x, y, &point);
      if(get_handle2() != NULL)
         ::SetWindowOrgEx(get_handle2(), x, y, &point);
      return point;
   }

   point graphics::OffsetWindowOrg(int32_t nWidth, int32_t nHeight)
   {
      point point(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::OffsetWindowOrgEx(get_handle1(), nWidth, nHeight, &point);
      //if(get_handle2() != NULL)
      //   ::OffsetWindowOrgEx(get_handle2(), nWidth, nHeight, &point);
      return point;
   }

   size graphics::SetWindowExt(int32_t x, int32_t y)
   {
      size size(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::SetWindowExtEx(get_handle1(), x, y, &size);
      //if(get_handle2() != NULL)
      //   ::SetWindowExtEx(get_handle2(), x, y, &size);
      return size;
   }

   size graphics::ScaleWindowExt(int32_t xNum, int32_t xDenom, int32_t yNum, int32_t yDenom)
   {
      size size(0, 0);
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   ::ScaleWindowExtEx(get_handle1(), xNum, xDenom, yNum, yDenom, &size);
      //if(get_handle2() != NULL)
      //   ::ScaleWindowExtEx(get_handle2(), xNum, xDenom, yNum, yDenom, &size);
      return size;
   }

   int32_t graphics::GetClipBox(LPRECT lpRect)
   {

      Gdiplus::Rect r;

      m_pgraphics->GetClipBounds(&r);

      lpRect->left = r.X;
      lpRect->top = r.Y;
      lpRect->right = r.X + r.Width;
      lpRect->bottom = r.Y + r.Height;

      return 1;

   }


   int32_t graphics::SelectClipRgn(::draw2d::region * pregion)
   {

      if(pregion == NULL)
      {

         m_pgraphics->ResetClip();

      }
      else
      {

         //return 0;

         m_pgraphics->SetClip((Gdiplus::Region *) pregion->get_os_data());

      }

      return 0;

   }


   int32_t graphics::ExcludeClipRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      int32_t nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::ExcludeClipRect(get_handle1(), x1, y1, x2, y2);
      //if(get_handle2() != NULL)
      //   nRetVal = ::ExcludeClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int32_t graphics::ExcludeClipRect(const RECT &  rectParam)
   {
      int32_t nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::ExcludeClipRect(get_handle1(), rectParam.left, rectParam.top,
      //                               rectParam.right, rectParam.bottom);
      //if(get_handle2() != NULL)
      //   nRetVal = ::ExcludeClipRect(get_handle2(), rectParam.left, rectParam.top,
      //                               rectParam.right, rectParam.bottom);
      return nRetVal;
   }

   int32_t graphics::IntersectClipRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
   {
      int32_t nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::IntersectClipRect(get_handle1(), x1, y1, x2, y2);
      //if(get_handle2() != NULL)
      //   nRetVal = ::IntersectClipRect(get_handle2(), x1, y1, x2, y2);
      return nRetVal;
   }

   int32_t graphics::IntersectClipRect(const RECT &  rectBounds)
   {
      int32_t nRetVal = ERROR;

      Gdiplus::Rect r(rectBounds.left, rectBounds.top, width(rectBounds),  height(rectBounds));

      m_pgraphics->IntersectClip(r);

      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::IntersectClipRect(get_handle1(),rectBounds.left,rectBounds.top,rectBounds.right,rectBounds.bottom);
      //if(get_handle2() != NULL)
      //   nRetVal = ::IntersectClipRect(get_handle2(),rectBounds.left,rectBounds.top,rectBounds.right,rectBounds.bottom);
      return SIMPLEREGION;
   }

   int32_t graphics::OffsetClipRgn(int32_t x, int32_t y)
   {
      int32_t nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::OffsetClipRgn(get_handle1(), x, y);
      //if(get_handle2() != NULL)
      //   nRetVal = ::OffsetClipRgn(get_handle2(), x, y);
      return nRetVal;
   }

   int32_t graphics::OffsetClipRgn(SIZE size)
   {
      int32_t nRetVal = ERROR;
      //if(get_handle1() != NULL && get_handle1() != get_handle2())
      //   nRetVal = ::OffsetClipRgn(get_handle1(), size.cx, size.cy);
      //if(get_handle2() != NULL)
      //   nRetVal = ::OffsetClipRgn(get_handle2(), size.cx, size.cy);
      return nRetVal;
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

   int32_t graphics::SetTextJustification(int32_t nBreakExtra, int32_t nBreakCount)
   {
      int32_t nRetVal = 0;
      if(get_handle1() != NULL && get_handle1() != get_handle2())
         nRetVal = ::SetTextJustification(get_handle1(), nBreakExtra, nBreakCount);
      if(get_handle2() != NULL)
         nRetVal = ::SetTextJustification(get_handle2(), nBreakExtra, nBreakCount);
      return nRetVal;
   }

   int32_t graphics::SetTextCharacterExtra(int32_t nCharExtra)
   {
      ASSERT(get_handle1() != NULL);
      int32_t nRetVal = 0x8000000;
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

   typedef uint32_t (CALLBACK* __GDIGETLAYOUTPROC)(HDC);
   typedef uint32_t (CALLBACK* __GDISETLAYOUTPROC)(HDC, uint32_t);

   uint32_t graphics::GetLayout()
   {
      HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != NULL);
      /*      uint32_t dwGetLayout = LAYOUT_LTR;
            __GDIGETLAYOUTPROC pfn;
            pfn = (__GDIGETLAYOUTPROC) GetProcAddress(hInst, "GetLayout");
            // if they API is available, just call it. If it is not
            // available, indicate an error.
            if (pfn != NULL)
               dwGetLayout = (*pfn)(get_handle1());
            else
            {
               dwGetLayout = GDI_ERROR;
               SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
            }*/
      //return dwGetLayout;
      return 0;
   }

   uint32_t graphics::SetLayout(uint32_t dwSetLayout)
   {
      /*HINSTANCE hInst = ::GetModuleHandleA("GDI32.DLL");
      ASSERT(hInst != NULL);
      uint32_t dwGetLayout = LAYOUT_LTR;
      __GDISETLAYOUTPROC pfn;
      pfn = (__GDISETLAYOUTPROC) GetProcAddress(hInst, "SetLayout");
      // If the API is availalbe, just call it. If it's not available,
      // setting anything other than LAYOUT_LTR is an error.
      if (pfn != NULL)
         dwGetLayout = (*pfn)(get_handle1(), dwSetLayout);
      else if (dwSetLayout != LAYOUT_LTR)
      {
         dwGetLayout = GDI_ERROR;
         SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
      }
      return dwGetLayout;*/

      return 0;

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

   int32_t graphics::SetArcDirection(int32_t nArcDirection)
   {
      ASSERT(get_handle1() != NULL);
      int32_t nResult = 0;
      if (get_handle1() != get_handle2())
         nResult = ::SetArcDirection(get_handle1(), nArcDirection);
      if (get_handle2() != NULL)
         nResult = ::SetArcDirection(get_handle2(), nArcDirection);
      return nResult;
   }

   bool graphics::PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, count nCount)
   {

      ASSERT(get_handle1() != NULL);

      bool bResult = ::PolyDraw(get_handle1(), lpPoints, lpTypes, (int) nCount) != FALSE;

      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      }
      return bResult;
   }

   bool graphics::PolylineTo(const POINT* lpPoints, count nCount)
   {
      ASSERT(get_handle1() != NULL);
      bool bResult = ::PolylineTo(get_handle1(), lpPoints, (DWORD) nCount) != FALSE;
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
      bool bResult = FALSE;
      if (get_handle1() != get_handle2())
         bResult = ::SetColorAdjustment(get_handle1(), lpColorAdjust) != FALSE;
      if (get_handle2() != NULL)
         bResult = ::SetColorAdjustment(get_handle2(), lpColorAdjust) != FALSE;
      return bResult;
   }

   bool graphics::PolyBezierTo(const POINT* lpPoints, count nCount)
   {
      ASSERT(get_handle1() != NULL);
      bool bResult = ::PolyBezierTo(get_handle1(), lpPoints, (DWORD) nCount) != FALSE;
      if (get_handle1() != get_handle2())
      {
         point pt;
         VERIFY(::GetCurrentPositionEx(get_handle1(), &pt));
         VERIFY(::MoveToEx(get_handle2(), pt.x, pt.y, NULL));
      }
      return bResult;
   }

   bool graphics::SelectClipPath(int32_t nMode)
   {
      //ASSERT(get_handle1() != NULL);

      //// output DC always holds the current path
      //if (!::SelectClipPath(get_handle1(), nMode))
      //   return FALSE;

      //// transfer clipping region into the attribute DC
      bool bResult = TRUE;
      //if (get_handle1() != get_handle2())
      //{
      //   HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);
      //   if (::GetClipRgn(get_handle1(), hRgn) < 0 || !::SelectClipRgn(get_handle2(), hRgn))
      //   {
      //      TRACE(::aura::trace::category_AppMsg, 0, "Error: unable to transfer clip region in graphics::SelectClipPath!\n");
      //      bResult = FALSE;
      //   }
      //   ::DeleteObject(hRgn);
      //}
      return bResult;
   }


   int32_t graphics::SelectClipRgn(::draw2d::region* pregion,int32_t nMode)
   {

      if(pregion == NULL)
      {

         m_pgraphics->ResetClip();

      }
      else
      {

         if(nMode == RGN_AND)
         {

            m_pgraphics->SetClip((Gdiplus::Region *) pregion->get_os_data(), Gdiplus::CombineModeIntersect);

         }
         else if(nMode == RGN_OR)
         {

            m_pgraphics->SetClip((Gdiplus::Region *) pregion->get_os_data(),Gdiplus::CombineModeUnion);

         }
         else if(nMode == RGN_XOR)
         {

            m_pgraphics->SetClip((Gdiplus::Region *) pregion->get_os_data(),Gdiplus::CombineModeXor);

         }
         else if(nMode == RGN_COPY)
         {

            m_pgraphics->SetClip((Gdiplus::Region *) pregion->get_os_data(),Gdiplus::CombineModeReplace);

         }
         else if(nMode == RGN_DIFF)
         {

            m_pgraphics->SetClip((Gdiplus::Region *) pregion->get_os_data(),Gdiplus::CombineModeExclude);

         }

      }

      return 0;

   }


   /////////////////////////////////////////////////////////////////////////////
   // Special handling for metafile playback

   int32_t CALLBACK __enum_meta_file_procedure(HDC hDC,
         HANDLETABLE* pHandleTable, METARECORD* pMetaRec, int32_t nHandles, LPARAM lParam)
   {
      ::draw2d::graphics * pgraphics = (::draw2d::graphics *)lParam;
      ASSERT_VALID(pgraphics);

      switch (pMetaRec->rdFunction)
      {
      // these records have effects different for each graphics derived class
      case META_SETMAPMODE:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SetMapMode((int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWEXT:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SetWindowExt(
         (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETWINDOWORG:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SetWindowOrg(
         (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTEXT:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SetViewportExt(
         (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETVIEWPORTORG:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SetViewportOrg(
         (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SCALEWINDOWEXT:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->ScaleWindowExt(
         (int32_t)(int16_t)pMetaRec->rdParm[3], (int32_t)(int16_t)pMetaRec->rdParm[2],
         (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SCALEVIEWPORTEXT:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->ScaleViewportExt(
         (int32_t)(int16_t)pMetaRec->rdParm[3], (int32_t)(int16_t)pMetaRec->rdParm[2],
         (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_OFFSETVIEWPORTORG:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->OffsetViewportOrg(
         (int32_t)(int16_t)pMetaRec->rdParm[1], (int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SAVEDC:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SaveDC();
         break;
      case META_RESTOREDC:
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->RestoreDC((int32_t)(int16_t)pMetaRec->rdParm[0]);
         break;
      case META_SETBKCOLOR:
      {
         ::draw2d::brush_sp brush((dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->allocer(), *(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SelectObject(brush);
      }
      break;
      case META_SETTEXTCOLOR:
      {
         ::draw2d::brush_sp brush((dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->allocer(), *(UNALIGNED COLORREF*)&pMetaRec->rdParm[0]);
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SelectObject(brush);
      }
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
            HFONT hFontOld = (HFONT)::SelectObject((dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1(), hStockFont);
            HGDIOBJ hObjOld = ::SelectObject((dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1(), hObject);
            if (hObjOld == hStockFont)
            {
               // got the stock object back, so must be selecting a font
               throw not_implemented(get_app());
//                  (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SelectObject(::draw2d_gdiplus::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
               break;  // don't play the default record
            }
            else
            {
               // didn't get the stock object back, so restore everything
               ::SelectObject((dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1(), hFontOld);
               ::SelectObject((dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1(), hObjOld);
            }
            // and fall through to PlayMetaFileRecord...
         }
         else if (nObjType == OBJ_FONT)
         {
            // play back as graphics::SelectObject(::draw2d::font*)
//               (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->SelectObject(::draw2d_gdiplus::font::from_handle(pgraphics->get_app(), (HFONT)hObject));
            throw not_implemented(get_app());
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

   void graphics::LPtoDP(LPSIZE lpSize)
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeVpExt.cx), abs(sizeWinExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeVpExt.cy), abs(sizeWinExt.cy));
   }

   void graphics::DPtoLP(LPSIZE lpSize)
   {
      ASSERT(__is_valid_address(lpSize, sizeof(SIZE)));

      size sizeWinExt = GetWindowExt();
      size sizeVpExt = GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx, abs(sizeWinExt.cx), abs(sizeVpExt.cx));
      lpSize->cy = MulDiv(lpSize->cy, abs(sizeWinExt.cy), abs(sizeVpExt.cy));
   }


   bool graphics::draw_text(const char * lpszString,strsize nCount,const RECT & lpRect,UINT nFormat)
   {

      return draw_text(string(lpszString,nCount),lpRect,nFormat);

   }


   bool graphics::draw_text(const string & str,const RECT & rectParam,UINT nFormat)
   {

      ::rectd rect;

      ::copy(rect,&rectParam);

      return draw_text(str,rect,nFormat);

   }


   bool graphics::draw_text(const char * lpszString, strsize nCount, const RECTD & rectParam, UINT nFormat)
   {

      return draw_text(string(lpszString,nCount),rectParam,nFormat);

   }


   bool graphics::draw_text(const string & str, const RECTD & rectParam, UINT nFormat)
   {

      synch_lock sl(m_pmutex);

      if (m_spfont.is_null())
      {

         return false;

      }

      if (m_pgraphics == NULL)
      {

         return false;

      }

      Gdiplus::Status status = Gdiplus::Status::GenericError;

      if (m_ealphamode != ::draw2d::alpha_mode_blend)
      {

         set_alpha_mode(::draw2d::alpha_mode_blend);

      }

      if (m_spfont.is_set() && m_etextrendering != m_spfont->m_etextrendering)
      {

         set_text_rendering(m_spfont->m_etextrendering);

      }

      Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());


      //format.SetFormatFlags(format.GetFormatFlags()
      //                      | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
      //                      | Gdiplus::StringFormatFlagsLineLimit);

      format.SetFormatFlags((format.GetFormatFlags()
                             //| Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                             | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                             | (nFormat & DT_SINGLELINE ? Gdiplus::StringFormatFlagsNoWrap: 0))
                            & ~(Gdiplus::StringFormatFlagsLineLimit));

      if (nFormat & DT_PATH_ELLIPSIS)
      {

         format.SetTrimming(Gdiplus::StringTrimmingEllipsisPath);

      }
      else if (nFormat & DT_END_ELLIPSIS)
      {

         format.SetTrimming(Gdiplus::StringTrimmingEllipsisCharacter);

      }

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

      //Gdiplus::REAL stops[] = {16,32,48,64,80,96,112,128};

      //format.SetTabStops(0,8,stops);

      //m_dFontSize             = fontSrc.m_dFontSize;

      try
      {

         if (m_spfont->m_dFontWidth == 1.0)
         {

            Gdiplus::RectF rectf((Gdiplus::REAL) rectParam.left, (Gdiplus::REAL) rectParam.top, (Gdiplus::REAL) (width(rectParam) * m_spfont->m_dFontWidth), (Gdiplus::REAL) (height(rectParam)));

            wstring wstr = ::str::international::utf8_to_unicode(str);

            Gdiplus::Font * pfont = gdiplus_font();

            Gdiplus::Brush * pbrush = gdiplus_brush();

            strsize iSize = wstr.get_length();

            auto e = m_pgraphics->GetTextRenderingHint();

            status = m_pgraphics->DrawString(wstr, (INT)iSize, pfont, rectf, &format, pbrush);

         }
         else
         {

            g_keep k(m_pgraphics);

            Gdiplus::Matrix m;

            status = m_pgraphics->GetTransform(&m);

            ap(Gdiplus::Matrix) pmNew = m.Clone();

            status = pmNew->Translate((Gdiplus::REAL) rectParam.left, (Gdiplus::REAL) rectParam.top);

            status = pmNew->Scale((Gdiplus::REAL) m_spfont->m_dFontWidth, (Gdiplus::REAL) 1.0, Gdiplus::MatrixOrderAppend);

            Gdiplus::RectF rectf(0, 0, (Gdiplus::REAL) (width(rectParam) * m_spfont->m_dFontWidth), (Gdiplus::REAL) (height(rectParam)));

            status = m_pgraphics->SetTransform(pmNew);

            wstring wstr = ::str::international::utf8_to_unicode(str);

            Gdiplus::Font * pfont = gdiplus_font();

            Gdiplus::Brush * pbrush = gdiplus_brush();

            strsize iSize = wstr.get_length();

            status = m_pgraphics->DrawString(wstr, (INT)iSize, pfont, rectf, &format, pbrush);

         }


      }
      catch (...)
      {

      }

      return status == Gdiplus::Status::Ok;

   }


   bool graphics::draw_text_ex(const char * lpszString,strsize nCount,const RECT & rectParam,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams)
   {

      if(get_handle1() == NULL)
         return -1;

      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::str::international::utf8_to_unicode(string(lpszString, nCount));
      return ::DrawTextExW(get_handle1(),const_cast<wchar_t *>((const wchar_t *)wstr),(int32_t)wcslen(wstr),(LPRECT) &rectParam,nFormat,lpDTParams);

   }


   bool graphics::draw_text_ex(const string & str,const RECT & rectParam,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams)
   {
      ASSERT(get_handle1() != NULL);
      // these flags would modify the string
      ASSERT((nFormat & (DT_END_ELLIPSIS | DT_MODIFYSTRING)) != (DT_END_ELLIPSIS | DT_MODIFYSTRING));
      ASSERT((nFormat & (DT_PATH_ELLIPSIS | DT_MODIFYSTRING)) != (DT_PATH_ELLIPSIS | DT_MODIFYSTRING));
      wstring wstr = ::str::international::utf8_to_unicode(str);
      return ::DrawTextExW(get_handle1(),const_cast<wchar_t *>((const wchar_t *)wstr),(int32_t)wcslen(wstr),(LPRECT) &rectParam,nFormat,lpDTParams);
   }


   bool graphics::draw_text_ex(const char * lpszString,strsize nCount,const RECTD & rectParam,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams)
   {

      return ::draw2d::graphics::draw_text_ex(lpszString,nCount,rectParam,nFormat,lpDTParams);

   }


   bool graphics::draw_text_ex(const string & str,const RECTD & rectParam,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams)
   {

      return ::draw2d::graphics::draw_text_ex(str,rectParam,nFormat,lpDTParams);

   }


   //::count graphics::GetEachCharTextExtent(array < size > & sizea, const string & str)
   //{
   //   sizea.remove_all();
   //   strsize_array iaLen;
   //   strsize iRange = 0;
   //   strsize i = 0;
   //   strsize iLen;
   //   const char * pszStart = str;
   //   const char * psz = pszStart;
   //   while (*psz)
   //   {
   //      const char * pszNext = ::str::utf8_inc(psz);
   //      if (pszNext == NULL)
   //         break;
   //      iLen = pszNext - psz;
   //      iRange++;
   //      i += iLen;
   //      iaLen.add(i);
   //      if (*pszNext == '\0')
   //         break;
   //      psz = pszNext;
   //   }
   //

   //   for (int iLen : iaLen)
   //   {

   //      sizea.add(GetTextExtent(str, str.get_length(), iLen));

   //   }



   //   return sizea.get_size();

   //}

   ::count graphics::GetEachCharTextExtent(array < sized > & sizea, const string & str, strsize iStartParam, strsize iCountParam)
   {

      if (iCountParam < 0)
      {

         iCountParam += str.get_length() - iStartParam + 1;

      }
      sizea.remove_all();
      //strsize_array iaLen;
      strsize iWideChar = 0;
      strsize i = 0;
      strsize iLen;
      const char * pszStart = str;
      const char * psz = pszStart;
      strsize iStart = -1;
      strsize iCount = -1;
      while (*psz)
      {
         const char * pszNext = ::str::utf8_inc(psz);
         if (i == iStartParam)
         {
            iStart = iWideChar;
         }
         if (pszNext == NULL)
            break;
         iLen = pszNext - psz;
         iWideChar++;
         i += iLen;
         if (iStartParam + iCountParam == i)
         {

            iCount = iWideChar - iStart;
            break;
         }
         //iaLen.add(i);
         if (*pszNext == '\0')
            break;
         psz = pszNext;
      }
      if (iStart < 0 || iCount < 0)
      {
         return 0;
      }

      /*for (int iLen : iaLen)
      {

         sizea.add(GetTextExtent(str, str.get_length(), iLen));

      }*/

      wstring wstr = ::str::international::utf8_to_unicode(str);

      //strsize iRange = 0;
      //strsize i = 0;
      //strsize iLen;
      //const char * psz = lpszString;
      //while (i < iIndex)
      //{
      //   iLen = ::str::get_utf8_char(psz).length();
      //   iRange++;
      //   i += iLen;
      //   psz = ::str::utf8_inc(psz);
      //   if (psz == NULL)
      //      break;
      //   if (*psz == '\0')
      //      break;
      //}

      auto iRange = MIN(iCount, wstr.get_length() + 1 - iStart);

      double dFontWidth = m_spfont->m_dFontWidth;

      // 32 is the maximum number of ranges SetMeasurableCharacterRanges accepts as workable argument
      int iStep = 32;

      for (index i = iStart; i < iStart + iCount; i+=iStep)
      {

         ::count cRanges = MIN(iStep, iStart + iCount);


         Gdiplus::CharacterRange * charRanges = new Gdiplus::CharacterRange[cRanges];

         for (index j = 0; j < cRanges; j++)
         {

            charRanges[j].First = 0;
            charRanges[j].Length = (INT) (i + j+ 1);

         }



         Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());
         //Gdiplus::StringFormat strFormat;

         strFormat.SetFormatFlags(strFormat.GetFormatFlags()
                                  | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                                 );

         strFormat.SetMeasurableCharacterRanges(INT (cRanges), charRanges);


         //strFormat.SetFormatFlags(strFormat.GetFormatFlags()
         //   | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
         //   | Gdiplus::StringFormatFlagsLineLimit | Gdiplus::StringFormatFlagsNoWrap);

         int32_t count = strFormat.GetMeasurableCharacterRangeCount();

         Gdiplus::Region * pCharRangeRegions = new Gdiplus::Region[count];


         Gdiplus::RectF box(0.0f, 0.0f, 128.0f * 1024.0f, 128.0f * 1024.0f);

         Gdiplus::PointF origin(0, 0);

         //m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, Gdiplus::StringFormat::GenericTypographic(), &box);

         ((graphics *)this)->m_pgraphics->MeasureCharacterRanges(wstr, (INT)wstr.get_length(), ((graphics *)this)->gdiplus_font(), box, &strFormat, (INT)count, pCharRangeRegions);

         //Gdiplus::Region * pregion = NULL;


         //if (count > 0)
         //{

         //   pregion = pCharRangeRegions[0].Clone();

         //}



         Gdiplus::RectF rectBound;

         for (int i = 0; i < count; i++)
         {
//            pCharRangeRegions[0].Union(&pCharRangeRegions[i]);
            pCharRangeRegions[i].GetBounds(&rectBound, m_pgraphics);

            Gdiplus::SizeF s;

            rectBound.GetSize(&s);

            ::sized sz((s.Width * dFontWidth), (s.Height));

            sizea.add(sz);

         }

         delete[] pCharRangeRegions;

         //delete pregion;

         delete[] charRanges;

      }

      return sizea.get_size();

   }



   sized graphics::GetTextExtent(const char * lpszString, strsize nCount, strsize iIndex)
   {

      if(lpszString == NULL || *lpszString == '\0')
         return sized(0, 0);

      if(nCount < 0)
         nCount = strlen(lpszString);

      if(iIndex > nCount)
         return sized(0, 0);

      if(iIndex < 0)
         return sized(0, 0);

      wstring wstr = ::str::international::utf8_to_unicode(lpszString, nCount);

      wstring wstrRange = ::str::international::utf8_to_unicode(lpszString, iIndex);

      Gdiplus::CharacterRange charRanges[1] = { Gdiplus::CharacterRange(0, (INT)wstrRange.get_length()) };

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

      strFormat.SetMeasurableCharacterRanges(1, charRanges);

      strFormat.SetFormatFlags(strFormat.GetFormatFlags()
                               | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                              );

      int32_t count = strFormat.GetMeasurableCharacterRangeCount();

      Gdiplus::PointF origin(0, 0);

      if (count <= 0)
      {

         Gdiplus::RectF box(0.0f, 0.0f, 0.0f, 0.0f);

         wstr = ::str::international::utf8_to_unicode(lpszString, iIndex);

         m_pgraphics->MeasureString(wstrRange, (int32_t)wstrRange.get_length(), ((graphics *)this)->gdiplus_font(), origin, Gdiplus::StringFormat::GenericTypographic(), &box);

         return class ::size((int64_t)(box.Width  * m_spfont->m_dFontWidth), (int64_t)(box.Height));

      }

      Gdiplus::RectF box(0.0f, 0.0f, 128.0f * 1024.0f, 128.0f * 1024.0f);

      Gdiplus::Region * pCharRangeRegions = new Gdiplus::Region[count];

      ((graphics *)this)->m_pgraphics->MeasureCharacterRanges(wstr, (INT) wstr.get_length(), ((graphics *)this)->gdiplus_font(), box, &strFormat, (INT) count, pCharRangeRegions);

      for(index i = 1; i < count; i++)
      {

         pCharRangeRegions[0].Union(&pCharRangeRegions[i]);

      }

      Gdiplus::RectF rectBound;

      pCharRangeRegions[0].GetBounds(&rectBound, m_pgraphics);

      delete[] pCharRangeRegions;

      Gdiplus::SizeF size;

      rectBound.GetSize(&size);

      return class ::sized((double)(size.Width  * m_spfont->m_dFontWidth), (double)(size.Height));

   }

   sized graphics::GetTextBegin(const char * lpszString, strsize nCount, strsize iIndex)
   {

      if (lpszString == NULL || *lpszString == '\0')
         return size(0, 0);

      if (nCount < 0)
         nCount = strlen(lpszString);

      if (iIndex > nCount)
         return size(0, 0);

      if (iIndex < 0)
         return size(0, 0);

      wstring wstr = ::str::international::utf8_to_unicode(lpszString, nCount);

      wstring wstrRange = ::str::international::utf8_to_unicode(lpszString, iIndex);

      Gdiplus::CharacterRange charRanges[1] = { Gdiplus::CharacterRange(INT(wstrRange.get_length()), INT(wstr.get_length() - wstrRange.get_length())) };

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

      strFormat.SetMeasurableCharacterRanges(1, charRanges);

      strFormat.SetFormatFlags(strFormat.GetFormatFlags()
                               | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                              );

      int32_t count = strFormat.GetMeasurableCharacterRangeCount();

      Gdiplus::PointF origin(0, 0);

      if (count <= 0)
      {

         Gdiplus::RectF box(0.0f, 0.0f, 0.0f, 0.0f);

         wstr = ::str::international::utf8_to_unicode(lpszString, iIndex);

         m_pgraphics->MeasureString(wstrRange, (int32_t)wstrRange.get_length(), ((graphics *)this)->gdiplus_font(), origin, Gdiplus::StringFormat::GenericTypographic(), &box);

         return class ::size((int64_t)(box.Width  * m_spfont->m_dFontWidth), (int64_t)(box.Height));

      }

      Gdiplus::RectF box(0.0f, 0.0f, 128.0f * 1024.0f, 128.0f * 1024.0f);

      Gdiplus::Region * pCharRangeRegions = new Gdiplus::Region[count];

      ((graphics *)this)->m_pgraphics->MeasureCharacterRanges(wstr, (INT)wstr.get_length(), ((graphics *)this)->gdiplus_font(), box, &strFormat, (INT)count, pCharRangeRegions);

      for (index i = 1; i < count; i++)
      {

         pCharRangeRegions[0].Union(&pCharRangeRegions[i]);

      }

      Gdiplus::RectF rectBound;

      pCharRangeRegions[0].GetBounds(&rectBound, m_pgraphics);

      delete[] pCharRangeRegions;

      return class ::sized((double)(rectBound.X  * m_spfont->m_dFontWidth), (double)(rectBound.Height));

   }

   sized graphics::GetTextExtent(const char * lpszString, strsize nCount)
   {

      //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

      wstring wstr = ::str::international::utf8_to_unicode(lpszString, nCount);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

      strFormat.SetFormatFlags(strFormat.GetFormatFlags()
                               | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                              );

      m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, &strFormat,  &box);

      return sized((double) (box.Width * m_spfont->m_dFontWidth), (double) (box.Height));

      /*if(get_handle2() == NULL)
         return size(0, 0);
      SIZE size;
      string str(lpszString, nCount);
      wstring wstr = ::str::international::utf8_to_unicode(str);
      if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int32_t)wstr.get_length(), &size))
      {
         return class size(0, 0);
      }
      return size;*/
   }

   sized graphics::GetTextExtent(const string & str)
   {
      /*      if(get_handle2() == NULL)
               return size(0, 0);
            SIZE size;
            wstring wstr = ::str::international::utf8_to_unicode(str);
            if(!::GetTextExtentPoint32W(get_handle2(), wstr, (int32_t)wstr.get_length(), &size))
            {
               return class size(0, 0);
            }
            return size;*/

      //class sized size;

      //if(!GetTextExtent(size, str, (int32_t) str.get_length(), (int32_t) str.get_length()))
      //   return class size(0, 0);

      return GetTextExtent(str, str.get_length());

      //return class size(0, 0);

      //return class size((long) size.cx, (long) size.cy);

      /*if(m_pgraphics == NULL)
         return size(0, 0);

      wstring wstr = ::str::international::utf8_to_unicode(str);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);


      if(m_pgraphics == NULL)
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

   size graphics::GetOutputTextExtent(const char * lpszString, strsize nCount)
   {
      ASSERT(get_handle1() != NULL);
      SIZE size;
      string str(lpszString, nCount);
      wstring wstr = ::str::international::utf8_to_unicode(str);
      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int32_t)wstr.get_length(), &size));
      return size;

   }


   size graphics::GetOutputTextExtent(const string & str)
   {

      ASSERT(get_handle1() != NULL);

      SIZE size;

      wstring wstr = ::str::international::utf8_to_unicode(str);

      VERIFY(::GetTextExtentPoint32W(get_handle1(), wstr, (int32_t)wstr.get_length(), &size));

      return size;

   }


   bool graphics::GetTextExtent(sized & size, const char * lpszString, strsize nCount, strsize iIndex)
   {

      //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

      if (m_pgraphics == NULL)
         return false;

      if (m_spfont.is_null())
         return false;

      if(lpszString == NULL || *lpszString == '\0')
         return false;

      if(nCount < 0)
         nCount = strlen(lpszString);

      if(iIndex > nCount)
         return false;

      if(iIndex < 0)
         return false;

      wstring wstr = ::str::international::utf8_to_unicode(lpszString, nCount);

      strsize iRange = 0;
      strsize i = 0;
      strsize iLen;
      const char * psz = lpszString;
      while(i < iIndex)
      {
         try
         {
            iLen = ::str::get_utf8_char_length(psz);
         }
         catch(...)
         {
            break;
         }
         if (iLen < 0)
            break;
         iRange++;
         i += iLen;
         if(i >= iIndex)
            break;
         try
         {
            psz += iLen;
         }
         catch(...)
         {
            break;
         }
         if(psz == NULL)
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
                              );

      int32_t count = strFormat.GetMeasurableCharacterRangeCount();

      Gdiplus::Region * pCharRangeRegions = new Gdiplus::Region[count];

      //Gdiplus::RectF box(0.0f, 0.0f, 128.0f * 1024.0f, 128.0f * 1024.0f);

      Gdiplus::PointF origin(0, 0);


      // Generate a on_layout rect for the text

      Gdiplus::RectF layoutRect(0, 0, 10000, 10000);
      Gdiplus::Status status = ((graphics *)this)->m_pgraphics->MeasureString( wstr, (INT) nCount, ((graphics *)this)->gdiplus_font(), origin, &layoutRect );


      // Prevent clipping

      //StringFormat strFormat( StringFormat::GenericTypographic() );
      //status = ((graphics *)this)->m_pgraphics->SetFormatFlags( StringFormatFlagsNoWrap | StringFormatFlagsNoClip );



      //m_pgraphics->MeasureString(wstr, (int32_t) wstr.get_length(), ((graphics *)this)->gdiplus_font(), origin, Gdiplus::StringFormat::GenericTypographic(), &box);

      ((graphics *)this)->m_pgraphics->MeasureCharacterRanges(wstr, (INT) nCount, ((graphics *)this)->gdiplus_font(), layoutRect, &strFormat, (INT) count, pCharRangeRegions);

      Gdiplus::Region * pregion = NULL;


      if(count > 0)
      {

         pregion = pCharRangeRegions[0].Clone();

      }

      for(i = 1; i < count; i++)
      {
         pregion->Union(&pCharRangeRegions[i]);
      }

      delete [] pCharRangeRegions;

      if(pregion == NULL)
         return false;

      Gdiplus::RectF rectBound;

      pregion->GetBounds(&rectBound, m_pgraphics);

      delete pregion;

      Gdiplus::SizeF sizef;

      rectBound.GetSize(&sizef);

      size.cx = sizef.Width * m_spfont->m_dFontWidth;

      size.cy = sizef.Height;

      return true;
   }

   bool graphics::GetTextExtent(sized & size, const char * lpszString, strsize nCount)
   {

      //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

      wstring wstr = ::str::international::utf8_to_unicode(lpszString, nCount);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);

      Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericTypographic());

      strFormat.SetFormatFlags(strFormat.GetFormatFlags()
                               | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsMeasureTrailingSpaces
                              );
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

      size.cx = box.Width * m_spfont->m_dFontWidth;

      size.cy = box.Height;

      return true;

   }

   bool graphics::GetTextExtent(sized & size, const string & str)
   {

      if(m_pgraphics == NULL)
         return false;

      wstring wstr = ::str::international::utf8_to_unicode(str);

      Gdiplus::RectF box;

      Gdiplus::PointF origin(0, 0);


      if(m_pgraphics == NULL)
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

      size.cx = box.Width * m_spfont->m_dFontWidth;

      size.cy = box.Height;

      return true;

   }


   void graphics::fill_solid_rect(LPCRECT lpcrect, COLORREF cr)
   {

      try
      {

         if(m_pgraphics == NULL)
            return;

         auto emodeOld = m_pgraphics->GetSmoothingMode();

         if(emodeOld != Gdiplus::SmoothingModeNone)
         {

            m_pgraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);

         }

         Gdiplus::SolidBrush b(Gdiplus::Color(argb_get_a_value(cr), argb_get_r_value(cr), argb_get_g_value(cr), argb_get_b_value(cr)));

         m_pgraphics->FillRectangle(&b, lpcrect->left, lpcrect->top, ::width(lpcrect), ::height(lpcrect));

         if (emodeOld != Gdiplus::SmoothingModeNone)
         {

            m_pgraphics->SetSmoothingMode(emodeOld);

         }

      }
      catch(...)
      {

      }

   }


   bool graphics::TextOutRaw(double x, double y, const char * lpszString, strsize nCount)
   {

      if (m_spfont.is_null())
      {

         return false;

      }

      string str(lpszString, nCount);

      wstring wstr = ::str::international::utf8_to_unicode(str);

      if (m_pgraphics == NULL)
      {

         return false;

      }

      ::draw2d::savedc k(this);

      set_alpha_mode(::draw2d::alpha_mode_blend);

      //if (m_etextrendering != m_spfont->m_etextrendering)
      {

         set_text_rendering(m_spfont->m_etextrendering);

      }

      Gdiplus::Status status;

      Gdiplus::StringFormat format(Gdiplus::StringFormat::GenericTypographic());

      format.SetFormatFlags(format.GetFormatFlags()
                            | Gdiplus::StringFormatFlagsNoClip
                           );

      format.SetLineAlignment(Gdiplus::StringAlignmentNear);

      if (m_spfont->m_dFontWidth == 1.0)
      {

         ::Gdiplus::PointF origin((Gdiplus::REAL)x, (Gdiplus::REAL)y);

         if (m_ppath != NULL)
         {

            FLOAT fDpiX = m_pgraphics->GetDpiX();

            Gdiplus::GraphicsPath path;

            Gdiplus::FontFamily fontfamily;

            gdiplus_font()->GetFamily(&fontfamily);

            double d1 = gdiplus_font()->GetSize() * m_pgraphics->GetDpiX() / 72.0;

            double d2 = fontfamily.GetEmHeight(gdiplus_font()->GetStyle());

            double d3 = d1 * d2;

            status = path.AddString(wstr, (INT)wstr.get_length(), &fontfamily, gdiplus_font()->GetStyle(), (Gdiplus::REAL) d1, origin, &format);

            m_ppath->AddPath(&path, FALSE);

         }
         else
         {

            auto e = m_pgraphics->GetTextRenderingHint();

            status = m_pgraphics->DrawString(wstr, (INT)wstr.get_length(), gdiplus_font(), origin, &format, gdiplus_brush());

         }

      }
      else
      {

         g_keep k(m_pgraphics);

         ::Gdiplus::PointF origin(0, 0);

         FLOAT fDpiX = m_pgraphics->GetDpiX();

         Gdiplus::Matrix m;

         m_pgraphics->GetTransform(&m);

         ap(Gdiplus::Matrix) pmNew;

         if (m_ppath != NULL)
         {

            pmNew = new Gdiplus::Matrix();

         }
         else
         {

            pmNew = m.Clone();

         }

         pmNew->Translate((Gdiplus::REAL)  (x / m_spfont->m_dFontWidth), (Gdiplus::REAL) y);

         pmNew->Scale((Gdiplus::REAL) m_spfont->m_dFontWidth, (Gdiplus::REAL) 1.0, Gdiplus::MatrixOrderAppend);

         if (m_ppath != NULL)
         {

            Gdiplus::GraphicsPath path;

            Gdiplus::FontFamily fontfamily;

            gdiplus_font()->GetFamily(&fontfamily);

            double d1 = gdiplus_font()->GetSize() * m_pgraphics->GetDpiX() / 72.0;

            double d2 = fontfamily.GetEmHeight(gdiplus_font()->GetStyle());

            double d3 = d1 * d2;

            status = path.AddString(wstr, (INT)wstr.get_length(), &fontfamily, gdiplus_font()->GetStyle(), (Gdiplus::REAL) d1, origin, &format);

            path.Transform(pmNew);

            m_ppath->AddPath(&path, FALSE);

         }
         else
         {

            m_pgraphics->SetTransform(pmNew);

            status = m_pgraphics->DrawString(wstr, (INT)wstr.get_length(), gdiplus_font(), origin, &format, gdiplus_brush());

            m_pgraphics->SetTransform(&m);

         }
      }

      return status  == Gdiplus::Status::Ok;

   }


   bool graphics::line_to(LPCPOINTD lppt)
   {

      synch_lock sl(m_pmutex);

      if(m_sppen.cast < ::draw2d_gdiplus::pen >()->m_egdiplusalign != Gdiplus::PenAlignment::PenAlignmentCenter)
      {

         gdiplus_pen()->SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);

         m_sppen.cast < ::draw2d_gdiplus::pen >()->m_egdiplusalign = Gdiplus::PenAlignment::PenAlignmentCenter;

      }

      m_pgraphics->DrawLine(gdiplus_pen(), Gdiplus::PointF((Gdiplus::REAL) m_pt.x, (Gdiplus::REAL)  m_pt.y), Gdiplus::PointF((Gdiplus::REAL) lppt->x, (Gdiplus::REAL) lppt->y));

      m_pt = *lppt;

      return true;

   }

   bool graphics::line_to(LPCPOINT lppt)
   {

      synch_lock sl(m_pmutex);

      if(m_sppen.cast < ::draw2d_gdiplus::pen >()->m_egdiplusalign != Gdiplus::PenAlignment::PenAlignmentCenter)
      {

         gdiplus_pen()->SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);

         m_sppen.cast < ::draw2d_gdiplus::pen >()->m_egdiplusalign = Gdiplus::PenAlignment::PenAlignmentCenter;

      }

      if (fmod(m_pt.x, 1.0) != 0.0 || fmod(m_pt.y, 1.0) != 0)
      {

         m_pgraphics->DrawLine(gdiplus_pen(), Gdiplus::PointF((Gdiplus::REAL)m_pt.x, (Gdiplus::REAL)m_pt.y), Gdiplus::PointF((Gdiplus::REAL)lppt->x, (Gdiplus::REAL) lppt->y));

      }
      else
      {

         m_pgraphics->DrawLine(gdiplus_pen(), Gdiplus::Point((INT)m_pt.x, (INT)m_pt.y), Gdiplus::Point((INT)lppt->x, (INT)lppt->y));

      }

      m_pt.x = lppt->x;

      m_pt.y = lppt->y;

      return true;

   }


   bool graphics::draw_line(LPCPOINTD lppt1, LPCPOINTD lppt2, ::draw2d::pen * ppen)
   {

      synch_lock sl(m_pmutex);

      ((Gdiplus::Pen *) ppen->get_os_data())->SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);

      m_pgraphics->DrawLine((Gdiplus::Pen *) ppen->get_os_data(), Gdiplus::PointF((Gdiplus::REAL) lppt1->x, (Gdiplus::REAL) lppt1->y), Gdiplus::PointF((Gdiplus::REAL) lppt2->x, (Gdiplus::REAL) lppt2->y));

      m_pt = *lppt2;

      return TRUE;

   }


   bool graphics::draw_line(LPCPOINT lppt1, LPCPOINT lppt2, ::draw2d::pen * ppen)
   {

      synch_lock sl(m_pmutex);

      ((Gdiplus::Pen *) ppen->get_os_data())->SetAlignment(Gdiplus::PenAlignment::PenAlignmentCenter);

      m_pgraphics->DrawLine((Gdiplus::Pen *) ppen->get_os_data(), Gdiplus::Point(lppt1->x, lppt1->y), Gdiplus::Point(lppt2->x, lppt2->y));

      m_pt.x = lppt2->x;

      m_pt.y = lppt2->y;

      return TRUE;

   }

   void graphics::set_smooth_mode(::draw2d::e_smooth_mode esmoothmode)
   {

      ::draw2d::graphics::set_smooth_mode(esmoothmode);

      try
      {

         if(m_pgraphics == NULL)
            return;

         if(m_esmoothmode == ::draw2d::smooth_mode_none)
         {

            m_pgraphics->SetSmoothingMode(Gdiplus::SmoothingModeNone);

         }
         else if(m_esmoothmode == ::draw2d::smooth_mode_antialias)
         {

            m_pgraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighSpeed);

         }
         else if(m_esmoothmode == ::draw2d::smooth_mode_high)
         {

            m_pgraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

         }

      }
      catch(...)
      {
         return;
      }

   }


   void graphics::set_alpha_mode(::draw2d::e_alpha_mode ealphamode)
   {

      try
      {

         if(m_pgraphics == NULL)
            return;

         ::draw2d::graphics::set_alpha_mode(ealphamode);
         if(m_ealphamode == ::draw2d::alpha_mode_blend)
         {
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceOver);
         }
         else if(m_ealphamode == ::draw2d::alpha_mode_set)
         {
            m_pgraphics->SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
         }

      }
      catch(...)
      {
         return;
      }

   }


   void graphics::set_text_rendering(::draw2d::e_text_rendering etextrendering)
   {

      //if (etextrendering != m_etextrendering)
      {

         m_etextrendering = etextrendering;

         switch (m_etextrendering)
         {
         case ::draw2d::text_rendering_anti_alias:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
            break;
         case ::draw2d::text_rendering_anti_alias_grid_fit:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);
            break;
         case ::draw2d::text_rendering_single_bit_per_pixel:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixel);
            break;
         case ::draw2d::text_rendering_single_bit_per_pixel_grid_fit:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintSingleBitPerPixelGridFit);
            break;
         case ::draw2d::text_rendering_clear_type_grid_fit:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
            break;
         case ::draw2d::text_rendering_none:
            m_pgraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintSystemDefault);
            break;
         }

      }


   }


   void * graphics::get_os_data() const
   {

      return (void *) m_pgraphics;

   }


   void * graphics::get_os_data_ex(int i) const
   {

      if(i == 1)
      {
         return m_pgraphics->GetHDC();
      }
      else
      {
         return ::draw2d::graphics::get_os_data_ex(i);
      }

   }


   void graphics::release_os_data_ex(int i,void * p)
   {

      if(i == 1)
      {

         m_pgraphics->ReleaseHDC((HDC)p);

      }
      else
      {

         ::draw2d::graphics::release_os_data_ex(i, p);

      }

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


   bool graphics::attach(void * pdata)
   {

      if(m_pgraphics != NULL)
      {

         try
         {

            delete m_pgraphics;

         }
         catch(...)
         {

            TRACE("graphics::attach : Failed to delete Gdiplus::Graphics");

         }

         m_pgraphics = NULL;

      }

      m_pgraphics = (Gdiplus::Graphics *) pdata;

      return false;

   }


   void * graphics::detach()
   {

      Gdiplus::Graphics * pgraphics = m_pgraphics;

      m_pgraphics = NULL;

      m_hdc = NULL;

      return pgraphics;

   }

   Gdiplus::Font * graphics::gdiplus_font()
   {
      if(m_spfont.is_null())
      {
         m_spfont.alloc(allocer());
         if(m_spfont.is_set())
         {
            m_spfont->m_powner = this;
         }
      }
      if(m_spfont.is_null())
      {
         return NULL;
      }
      return (Gdiplus::Font *) m_spfont->get_os_data();
   }

   Gdiplus::Brush * graphics::gdiplus_brush()
   {
      if(m_spbrush.is_null())
      {
         m_spbrush.alloc(allocer());
         if(m_spbrush.is_set())
         {
            m_spbrush->m_powner = this;
         }
      }
      if(m_spbrush.is_null())
      {
         return NULL;
      }
      return (Gdiplus::Brush *) m_spbrush->get_os_data();
   }

   Gdiplus::Pen * graphics::gdiplus_pen()
   {

      synch_lock sl(m_pmutex);

      if(m_sppen.is_null())
      {
         m_sppen.alloc(allocer());
         if(m_sppen.is_set())
         {
            m_sppen->m_powner = this;
         }
      }
      if(m_sppen.is_null())
      {
         return NULL;
      }
      return (Gdiplus::Pen *) m_sppen->get_os_data();
   }

   Gdiplus::FillMode graphics::gdiplus_get_fill_mode()
   {
      return Gdiplus::FillModeWinding;
   }

   bool graphics::blur(bool bExpand,double dRadius,const RECT & rectParam)
   {

      // Commented Out for Running in cosan machine running Windows 2008

      //if(m_spbitmap.is_null() || m_spbitmap->get_os_data() == NULL)
      //   return false;

      //Gdiplus::BlurParams myBlurParams;

      //myBlurParams.expandEdge    = bExpand ? 1 : 0;
      //myBlurParams.radius        = (float) dRadius;

      //Gdiplus::Blur myBlur;
      //myBlur.SetParameters(&myBlurParams);

      //

      //Gdiplus::Matrix m;
      //m_pgraphics->GetTransform(&m);

      //Gdiplus::PointF points[2];

      //points[0].X    = (Gdiplus::REAL) rectParam.left;
      //points[0].Y    = (Gdiplus::REAL) rectParam.top;
      //points[1].X    = (Gdiplus::REAL) rectParam.right;
      //points[1].Y    = (Gdiplus::REAL) rectParam.bottom;

      //m.TransformPoints(points, 2);

      ////Gdiplus::RectF rectf(points[0].X, points[0].Y, points[1].X - points[0].X, points[1].Y - points[0].Y);

      //RECT rect;

      //rect.left      = (LONG) points[0].X;
      //rect.top       = (LONG) points[0].Y;
      //rect.right     = (LONG) points[1].X;
      //rect.bottom    = (LONG) points[1].Y;

      //Gdiplus::Bitmap * pbitmap = ((Gdiplus::Bitmap *) m_spbitmap->get_os_data());

      //pbitmap->ApplyEffect(&myBlur, &rect);

      return true;

   }


   double graphics::get_dpix()
   {

      if (m_pgraphics == NULL)
         return 96.0;

      return m_pgraphics->GetDpiX();

   }


   double graphics::get_dpiy()
   {

      if (m_pgraphics == NULL)
         return 96.0;

      return m_pgraphics->GetDpiY();

   }


   bool graphics::flush()
   {

      synch_lock sl(m_pmutex);

      m_pgraphics->Flush();

      return true;

   }


   bool graphics::sync_flush()
   {

      synch_lock sl(m_pmutex);

      m_pgraphics->Flush(Gdiplus::FlushIntentionSync);

      return true;

   }

   HDC graphics::get_hdc()
   {

      if(m_hdc != NULL)
         return m_hdc;

      if(m_pgraphics == NULL)
         return NULL;

      return m_pgraphics->GetHDC();

   }

   void graphics::release_hdc(HDC hdc)
   {

      if(m_hdc != NULL)
         return;

      m_pgraphics->ReleaseHDC(hdc);

   }


   void graphics::enum_fonts(::draw2d::font::enum_item_array & itema)
   {

      ::draw2d::wingdi_enum_fonts(itema, false, true, true);

   }


   bool graphics::prefer_mapped_dib_on_mix()
   {

      return true;

   }


   bool graphics::TextOutAlphaBlend(double x, double y, const char * lpszString, strsize nCount)
   {

      if (m_pdibAlphaBlend != NULL)
      {

         single_lock sl(m_pmutex);

         if (nCount < 0)
         {

            return false;

         }

         // "Reference" implementation for TextOutAlphaBlend

         rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());

         ::size size = ::size(GetTextExtent(lpszString, nCount));

         //size.cx = size.cx * 110 / 100;

         //size.cy = size.cy * 110 / 100;

         rect rectText(point((int64_t)x, (int64_t)y), size);

         if (rectIntersect.intersect(rectIntersect, rectText))
         {

            ::draw2d::dib_sp dib1(allocer());
            //#ifdef METROWIN
            //            g_pdiba->add(dib1);
            //#endif

            dib1->create(rectText.get_size());

            dib1->get_graphics()->SelectObject(get_current_font());

            dib1->get_graphics()->SelectObject(get_current_brush());

            dib1->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

            dib1->get_graphics()->text_out(0, 0, lpszString, nCount);

            dib1->blend2(null_point(), m_pdibAlphaBlend, point((int)MAX(0, x - m_ptAlphaBlend.x), (int)MAX(0, y - m_ptAlphaBlend.y)), rectText.get_size(), 255);

            BitBltRaw((int)x, (int)y, rectText.width(), rectText.height(), dib1->get_graphics(), 0, 0, SRCCOPY);

            return true;

         }



      }

      return false;

   }


   bool graphics::BitBltAlphaBlend(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, uint32_t dwRop)
   {

      if (m_pdibAlphaBlend != NULL)
      {

         rect rectIntersect(m_ptAlphaBlend, m_pdibAlphaBlend->size());

         rect rectBlt(point((int64_t)x, (int64_t)y), size(nWidth, nHeight));

         if (rectIntersect.intersect(rectIntersect, rectBlt))
         {

            //if (m_ptAlphaBlend.x < 0)
            //{

            //   xSrc += -m_ptAlphaBlend.x;

            //}
            //if (m_ptAlphaBlend.y < 0)
            //{

            //   ySrc += -m_ptAlphaBlend.y;

            //}

            // The following commented out code does not work well when there is clipping
            // and some calculations are not precise
            //if (m_pdibDraw2dGraphics != NULL && pgraphicsSrc->m_pdibDraw2dGraphics != NULL)
            //{

            //   point ptOff = GetViewportOrg();

            //   x += ptOff.x;

            //   y += ptOff.y;

            //   return m_pdibDraw2dGraphics->blend(point(x, y), pgraphicsSrc->m_pdibDraw2dGraphics, point(xSrc, ySrc), m_pdibAlphaBlend, point(m_ptAlphaBlend.x - x, m_ptAlphaBlend.y - y), rectBlt.size());

            //}
            //else
            {

               ::draw2d::dib_sp dib1(allocer());

               dib1->create(rectBlt.get_size());

               dib1->get_graphics()->set_alpha_mode(::draw2d::alpha_mode_set);

               if (!dib1->from(null_point(), pgraphicsSrc, point(xSrc, ySrc), rectBlt.get_size()))
                  return false;

               dib1->blend2(point(0, 0), m_pdibAlphaBlend, point(x - m_ptAlphaBlend.x, y - m_ptAlphaBlend.y), rectBlt.get_size(), 255);

               BitBltRaw(x, y, nWidth, nHeight, dib1->get_graphics(), 0, 0, dwRop);

            }

            return true;

         }

      }

      return false;

   }


   bool graphics::_get(::draw2d::matrix & matrix)
   {

      Gdiplus::Matrix m;

      if (m_pgraphics->GetTransform(&m) != Gdiplus::Ok)
      {

         return false;

      }

      float fa[6];

      m.GetElements(fa);

      matrix = ::draw2d::matrix();

      matrix.SetElements(fa);

      return true;

   }


   bool graphics::_set(const ::draw2d::matrix & matrix)
   {

      Gdiplus::Matrix m;

      m.SetElements(matrix.a1, matrix.a2, matrix.b1, matrix.b2, matrix.c1, matrix.c2);

      return m_pgraphics->SetTransform(&m) == Gdiplus::Ok;

   }


   //bool graphics::append(const ::draw2d::matrix & matrix)
   //{

   //   ::draw2d::matrix m;

   //   get(m);

   //   m.append(matrix);

   //   return set(m);

   //}


   //bool graphics::prepend(const ::draw2d::matrix & matrix)
   //{

   //   ::draw2d::matrix m;

   //   get(m);

   //   m.prepend(matrix);

   //   return set(m);

   //}


} // namespace draw2d_gdiplus





