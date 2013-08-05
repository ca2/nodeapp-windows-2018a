#pragma once

namespace draw2d_gdi
{

   class region :
      virtual public ::draw2d_gdi::object,
      virtual public ::draw2d::region
   {
   public:

      
      region(::ca2::application * papp);
      virtual ~region();

      
      operator HRGN() const;


      static ::draw2d::region * from_handle(::ca2::application * papp, HRGN hRgn);


      bool CreateRectRgn(int x1, int y1, int x2, int y2);
      bool CreateRectRgnIndirect(LPCRECT lpRect);
      bool CreateEllipticRgn(int x1, int y1, int x2, int y2);
      bool CreateEllipticRgnIndirect(LPCRECT lpRect);
      bool CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
      bool CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
            int nCount, int nPolyFillMode);
      bool CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
      bool CreateFromPath(::draw2d::graphics * pgraphics);
      bool CreateFromData(const XFORM* lpXForm, int nCount,
         const RGNDATA* pRgnData);

   // Operations
      void SetRectRgn(int x1, int y1, int x2, int y2);
      void SetRectRgn(LPCRECT lpRect);
      int CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode);
      int CopyRgn(const ::draw2d::region* pRgnSrc);
      bool EqualRgn(const ::draw2d::region* pRgn) const;
      int OffsetRgn(int x, int y);
      int OffsetRgn(POINT point);
      int GetRgnBox(LPRECT lpRect) const;
      bool PtInRegion(int x, int y) const;
      bool PtInRegion(POINT point) const;
      bool RectInRegion(LPCRECT lpRect) const;
      int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;

      
      virtual bool create();

      virtual bool destroy();


   };


} // namespace draw2d_gdi




