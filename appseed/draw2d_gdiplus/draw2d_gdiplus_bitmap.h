#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS bitmap : 
      virtual public ::draw2d::bitmap
   {
   public:


      ::Gdiplus::Bitmap *  m_pbitmap;
      memory               m_mem;


      bitmap(::aura::application * papp);
      virtual ~bitmap();


      void * get_os_data() const;


      bool LoadBitmap(const char * lpszResourceName);
      bool LoadBitmap(UINT nIDResource);
      bool LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
   //#ifndef ___NO_AFXCMN_SUPPORT
   //   bool LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int32_t nMapSize = 0);
   //#endif
      bool CreateBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int32_t stride);
      bool CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap);
      bool CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight);
      bool CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight);
      bool HostDIBSection(::draw2d::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void *pvBits, int stride, HANDLE hSection, uint32_t offset);
      bool CreateDIBSection(::draw2d::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, int * stride, HANDLE hSection, uint32_t offset);
      bool CreateDIBitmap(::draw2d::graphics * pgraphics, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);


      int32_t GetBitmap(BITMAP* pBitMap);


      uint32_t SetBitmapBits(uint32_t dwCount, const void * lpBits);
      uint32_t GetBitmapBits(uint32_t dwCount, LPVOID lpBits) const;
      class size SetBitmapDimension(int32_t nWidth, int32_t nHeight);
      class size GetBitmapDimension() const;

      virtual void dump(dump_context & dumpcontext) const;


      virtual bool attach(void * posdata);
      virtual void * detach();

      virtual HBITMAP _GetHBITMAP();
      virtual void _ReleaseHBITMAP(HBITMAP hbitmap);

   };


} // namespace draw2d_gdiplus



