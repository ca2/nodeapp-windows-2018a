#pragma once


namespace win
{


   class region :
      virtual public ::ca::region
   {
   public:


      Gdiplus::Region *       m_pregion;

      //static region * PASCAL from_handle(::ca::application * papp, HRGN hRgn);
      //operator HRGN() const;

      region(::ca::application * papp);
      virtual ~region();

      /*BOOL CreateRectRgn(int x1, int y1, int x2, int y2);
      BOOL CreateRectRgnIndirect(LPCRECT lpRect);
      BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2);
      BOOL CreateEllipticRgnIndirect(LPCRECT lpRect);
      BOOL CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
      BOOL CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
            int nCount, int nPolyFillMode);
      BOOL CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
      BOOL CreateFromPath(::ca::graphics * pgraphics);
      BOOL CreateFromData(const XFORM* lpXForm, int nCount,
         const RGNDATA* pRgnData);*/

   // Operations
      void SetRectRgn(int x1, int y1, int x2, int y2);
      void SetRectRgn(LPCRECT lpRect);
      int CombineRgn(const ::ca::region* pRgn1, const ::ca::region* pRgn2, int nCombineMode);
      int CopyRgn(const ::ca::region* pRgnSrc);
      BOOL EqualRgn(const ::ca::region* pRgn) const;
      int OffsetRgn(int x, int y);
      int OffsetRgn(POINT point);
      int GetRgnBox(LPRECT lpRect) const;
      BOOL PtInRegion(int x, int y) const;
      BOOL PtInRegion(POINT point) const;
      BOOL RectInRegion(LPCRECT lpRect) const;
      int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;

      virtual void * get_os_data() const;

   };


} // namespace win


