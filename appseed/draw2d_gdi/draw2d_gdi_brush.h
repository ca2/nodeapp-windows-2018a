#pragma once


namespace draw2d_gdi
{


   class CLASS_DECL_DRAW2D_GDI brush : 
      virtual public ::draw2d_gdi::object,
      virtual public ::draw2d::brush
   {
   public:

      bool m_bProcess;

      // Constructors
      brush(base_application * papp);
      virtual ~brush();


      operator HBRUSH() const;
      int GetLogBrush(LOGBRUSH* pLogBrush);


      virtual void construct(COLORREF crColor);                // CreateSolidBrush
      virtual void construct(int nIndex, COLORREF crColor);    // CreateHatchBrush
      virtual void construct(::draw2d::bitmap * pbitmap);                // CreatePatternBrush


      bool CreateSolid(COLORREF crColor);
      bool CreateHatchBrush(int nIndex, COLORREF crColor);
      bool CreateBrushIndirect(const LOGBRUSH* lpLogBrush);
      bool CreatePatternBrush(::draw2d::bitmap* pBitmap);
      bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
      bool CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage);
      bool CreateSysColorBrush(int nIndex);


#ifdef _DEBUG
      virtual void dump(dump_context & dumpcontext) const;
#endif


      virtual bool create();

   };


} // namespace draw2d_gdi




