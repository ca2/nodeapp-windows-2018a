#pragma once





namespace win
{

   class CLASS_DECL_VMSWIN brush : 
      virtual public ::ca::brush
   {
   public:


      Gdiplus::Brush * m_pbrush;

      brush(::ca::application * papp);
      //virtual void construct(COLORREF crColor);                // CreateSolidBrush
      //virtual void construct(int nIndex, COLORREF crColor);    // CreateHatchBrush
      //virtual void construct(::ca::bitmap * pbitmap);                // CreatePatternBrush

      virtual ~brush();

      virtual void * get_os_data() const;

      //BOOL CreateSolidBrush(COLORREF crColor);
      //BOOL CreateHatchBrush(int nIndex, COLORREF crColor);
      //BOOL CreatePatternBrush(::ca::bitmap* pBitmap);
      //BOOL CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
      //BOOL CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage);
      //BOOL CreateSysColorBrush(int nIndex);


#ifdef _DEBUG
      virtual void dump(dump_context & dumpcontext) const;
#endif
   };


} // namespace win