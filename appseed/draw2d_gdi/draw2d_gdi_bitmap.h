#pragma once


namespace draw2d_gdi
{


   class CLASS_DECL_DRAW2D_GDI bitmap : 
      virtual public ::draw2d_gdi::object,
      virtual public ::draw2d::bitmap
   {
   public:


      bitmap(::aura::application * papp);
      virtual ~bitmap();


      operator HBITMAP() const;
      int GetBitmap(BITMAP* pBitMap);


      bool LoadBitmap(const char * lpszResourceName);
      bool LoadBitmap(UINT nIDResource);
      bool LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
      bool CreateBitmap(::draw2d::graphics * pdc, int32_t nWidth, int32_t nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int32_t stride);
      bool CreateBitmapIndirect(LPBITMAP lpBitmap);
      bool CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight);
      bool CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int nWidth, int nHeight);
      bool CreateDIBSection(::draw2d::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, int32_t * stride, HANDLE hSection, uint32_t offset);
      bool CreateDIBitmap(::draw2d::graphics * pdc, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);


      uint32_t SetBitmapBits(uint32_t dwCount, const void * lpBits);
      uint32_t GetBitmapBits(uint32_t dwCount, LPVOID lpBits) const;
      class size SetBitmapDimension(int nWidth, int nHeight);
      class size GetBitmapDimension() const;

      virtual void dump(dump_context & dumpcontext) const;


      virtual HBITMAP _GetHBITMAP();
      virtual void _ReleaseHBITMAP(HBITMAP hbitmap);

   };


} // namespace draw2d_gdi




