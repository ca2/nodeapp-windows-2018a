#pragma once


namespace win
{


   class CLASS_DECL_VMSWIN bitmap : 
      virtual public ::ca::bitmap
   {
   public:


      ::Gdiplus::Bitmap *  m_pbitmap;
      void *               m_pdata;


      bitmap(::ca::application * papp);
      virtual ~bitmap();

      void * get_os_data() const;


      BOOL LoadBitmap(const char * lpszResourceName);
      BOOL LoadBitmap(UINT nIDResource);
      BOOL LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
   #ifndef _AFX_NO_AFXCMN_SUPPORT
      BOOL LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0,
         LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
   #endif
      BOOL CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount,
            const void * lpBits);
      BOOL CreateBitmapIndirect(LPBITMAP lpBitmap);
      BOOL CreateCompatibleBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight);
      BOOL CreateDiscardableBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight);
      BOOL CreateDIBSection(::ca::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, HANDLE hSection, DWORD offset);
      BOOL CreateDIBitmap(::ca::graphics * pdc, const BITMAPINFOHEADER *pbmih, DWORD flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);

      int GetBitmap(BITMAP* pBitMap);

      DWORD SetBitmapBits(DWORD dwCount, const void * lpBits);
      DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
      class size SetBitmapDimension(int nWidth, int nHeight);
      class size GetBitmapDimension() const;

   #ifdef _DEBUG
      virtual void dump(dump_context & dumpcontext) const;
   #endif
   };


} // namespace win

