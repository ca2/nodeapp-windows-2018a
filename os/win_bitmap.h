#pragma once


namespace win
{


   class CLASS_DECL_win bitmap : 
      virtual public ::ca::bitmap
   {
   public:


      ::Gdiplus::Bitmap *  m_pbitmap;
      void *               m_pdata;


      bitmap(::ca::applicationsp papp);
      virtual ~bitmap();


      void * get_os_data() const;


      bool LoadBitmap(const char * lpszResourceName);
      bool LoadBitmap(UINT nIDResource);
      bool LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
   #ifndef ___NO_AFXCMN_SUPPORT
      bool LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int32_t nMapSize = 0);
   #endif
      bool CreateBitmap(::ca::graphics * pdc, int32_t nWidth, int32_t nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int32_t stride);
      bool CreateBitmapIndirect(::ca::graphics * pdc, LPBITMAP lpBitmap);
      bool CreateCompatibleBitmap(::ca::graphics * pgraphics, int32_t nWidth, int32_t nHeight);
      bool CreateDiscardableBitmap(::ca::graphics * pgraphics, int32_t nWidth, int32_t nHeight);
      bool CreateDIBSection(::ca::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, int * stride, HANDLE hSection, uint32_t offset);
      bool CreateDIBitmap(::ca::graphics * pdc, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);


      int32_t GetBitmap(BITMAP* pBitMap);


      uint32_t SetBitmapBits(uint32_t dwCount, const void * lpBits);
      uint32_t GetBitmapBits(uint32_t dwCount, LPVOID lpBits) const;
      class size SetBitmapDimension(int32_t nWidth, int32_t nHeight);
      class size GetBitmapDimension() const;

      virtual void dump(dump_context & dumpcontext) const;


      virtual bool attach(void * posdata);
      virtual void * detach();


   };


} // namespace win



