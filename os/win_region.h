#pragma once


namespace win
{


   class region :
      virtual public ::ca::region
   {
   public:


      Gdiplus::Region *       m_pregion;

      region(::ca::application * papp);
      virtual ~region();


      virtual void * get_os_data() const;


      void defer_update();

      //void SetRectRgn(int x1, int y1, int x2, int y2);
      //void SetRectRgn(LPCRECT lpRect);
      //int CombineRgn(const ::ca::region* pRgn1, const ::ca::region* pRgn2, int nCombineMode);
      //int CopyRgn(const ::ca::region* pRgnSrc);
      //bool EqualRgn(const ::ca::region* pRgn) const;
      int OffsetRgn(int x, int y);
      //int OffsetRgn(POINT point);
      bool get_bounding_box(LPRECT lpRect) const;
      //bool PtInRegion(int x, int y) const;
      bool contains(POINT point) const;
      //bool RectInRegion(LPCRECT lpRect) const;
      //int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;

      virtual bool destroy();

      // platform-specific or platform-internals
      // "get" set of funcionts translate ::ca::region data into platform specific data
      Gdiplus::Region * get();
      Gdiplus::Region * get_rect();
      Gdiplus::Region * get_oval();
      Gdiplus::Region * get_polygon();
      Gdiplus::Region * get_poly_polygon();
      Gdiplus::Region * get_combine();

   };


} // namespace win



