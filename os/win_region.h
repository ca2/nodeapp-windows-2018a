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

      //void SetRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      //void SetRectRgn(LPCRECT lpRect);
      //int32_t CombineRgn(const ::ca::region* pRgn1, const ::ca::region* pRgn2, int32_t nCombineMode);
      //int32_t CopyRgn(const ::ca::region* pRgnSrc);
      //bool EqualRgn(const ::ca::region* pRgn) const;
      int32_t OffsetRgn(int32_t x, int32_t y);
      //int32_t OffsetRgn(POINT point);
      bool get_bounding_box(LPRECT lpRect) const;
      //bool PtInRegion(int32_t x, int32_t y) const;
      bool contains(POINT point) const;
      //bool RectInRegion(LPCRECT lpRect) const;
      //int32_t GetRegionData(LPRGNDATA lpRgnData, int32_t nCount) const;

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



