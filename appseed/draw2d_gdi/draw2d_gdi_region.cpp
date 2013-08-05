#include "framework.h"


namespace draw2d_gdi
{


   region::region(::ca2::application * papp) :
      ca2(papp)
   {
   
   }


   region::~region()
   {
   
   }


   region::operator HRGN() const
   { 

      return (HRGN)(this == NULL ? NULL : get_os_data()); 

   }

   ::draw2d::region* region::from_handle(::ca2::application * papp, HRGN hRgn)
   { 
      return dynamic_cast < region * > (::draw2d_gdi::object::from_handle(papp, hRgn));
   }

   bool region::CreateRectRgn(int x1, int y1, int x2, int y2)
   { 
      
      return Attach(::CreateRectRgn(x1, y1, x2, y2)); }
   bool region::CreateRectRgnIndirect(LPCRECT lpRect)
   { 
      
      return Attach(::CreateRectRgnIndirect(lpRect)); }
   bool region::CreateEllipticRgn(int x1, int y1, int x2, int y2)
   { 
      
      return Attach(::CreateEllipticRgn(x1, y1, x2, y2)); }
   bool region::CreateEllipticRgnIndirect(LPCRECT lpRect)
   { 
      
      return Attach(::CreateEllipticRgnIndirect(lpRect)); }
   bool region::CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
   { 
      
      return Attach(::CreatePolygonRgn(lpPoints, nCount, nMode)); }
   bool region::CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
   { 
      
      return Attach(::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode)); }
   bool region::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
   { 
      
      return Attach(::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3)); }
   bool region::CreateFromPath(::draw2d::graphics * pgraphics)
   { 
      
      ASSERT(pgraphics != NULL); return Attach(::PathToRegion((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1())); }
   bool region::CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
   { 
      
      return Attach(::ExtCreateRegion(lpXForm, nCount, pRgnData)); }
   int region::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
   { 
      
      ASSERT(get_os_data() != NULL); return (int)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData); }
   void region::SetRectRgn(int x1, int y1, int x2, int y2)
   { 
      
      ASSERT(get_os_data() != NULL); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2); }
   
   void region::SetRectRgn(LPCRECT lpRect)
   { 
      
      ::SetRectRgn((HRGN)get_os_data(), lpRect->left, lpRect->top, lpRect->right, lpRect->bottom); 

   }


   int region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode)
   { 
      
      ASSERT(get_os_data() != NULL);
      
      return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(), (HRGN)pRgn2->get_os_data(), nCombineMode); 
   
   }
   
   
   int region::CopyRgn(const ::draw2d::region* pRgnSrc)
   { 
      
      ASSERT(get_os_data() != NULL);
      
      return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), NULL, RGN_COPY); 
   
   }


   bool region::EqualRgn(const ::draw2d::region* pRgn) const
   {
      
      ASSERT(get_os_data() != NULL); 
      
      return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()) != FALSE; 
   
   }


   int region::OffsetRgn(int x, int y)
   {
      
      ASSERT(get_os_data() != NULL); 
      
      return ::OffsetRgn((HRGN)get_os_data(), x, y) != FALSE; 
   
   
   }

   
   int region::OffsetRgn(POINT point)
   { 
      
      ASSERT(get_os_data() != NULL); 
      
      return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y); 
   
   }
   

   int region::GetRgnBox(LPRECT lpRect) const
   {
      
      ASSERT(get_os_data() != NULL); 
      
      return ::GetRgnBox((HRGN)get_os_data(), lpRect); 
   
   }
   
   
   bool region::PtInRegion(int x, int y) const
   {
      
      ASSERT(get_os_data() != NULL); 
      
      return ::PtInRegion((HRGN)get_os_data(), x, y) != FALSE; 
   
   }
   

   bool region::PtInRegion(POINT point) const
   {
      ASSERT(get_os_data() != NULL); 
      
      return ::PtInRegion((HRGN)get_os_data(), point.x, point.y) != FALSE; 
   
   }
   

   bool region::RectInRegion(LPCRECT lpRect) const
   {
      
      ASSERT(get_os_data() != NULL);
   
      return ::RectInRegion((HRGN)get_os_data(), lpRect) != FALSE; 
   
   }


   bool region::create()
   {

      ::draw2d_gdi::object::create();

      return true;
      
   }

   bool region::destroy()
   {

      ::draw2d::region::destroy();
      ::draw2d_gdi::object::destroy();

      return true;

   }

} // namespace draw2d_gdi





