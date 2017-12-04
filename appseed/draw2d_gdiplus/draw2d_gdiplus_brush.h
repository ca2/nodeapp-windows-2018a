#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS brush :
      virtual public ::draw2d::brush
   {
      public:


         Gdiplus::Brush * m_pbrush;


         brush(::aura::application * papp);
         //virtual void construct(COLORREF crColor);                // create_solid
         //virtual void construct(int32_t nIndex, COLORREF crColor);    // CreateHatchBrush
         //virtual void construct(::draw2d::bitmap * pbitmap);                // CreatePatternBrush
         virtual ~brush();

#ifdef DEBUG

         virtual void dump(dump_context & dumpcontext) const;

#endif

         virtual void * get_os_data() const;



         //bool create_solid(COLORREF crColor);
         //bool CreateHatchBrush(int32_t nIndex, COLORREF crColor);
         //bool CreatePatternBrush(::draw2d::bitmap* pBitmap);
         //bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
         //bool CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage);
         //bool CreateSysColorBrush(int32_t nIndex);



   };


} // namespace draw2d_gdiplus


