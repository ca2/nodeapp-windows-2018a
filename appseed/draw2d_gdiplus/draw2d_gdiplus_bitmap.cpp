#include "framework.h"


#undef new


namespace draw2d_gdiplus
{


   bitmap::bitmap(::aura::application * papp) :
      ::object(papp)
   { 

      m_pbitmap   = NULL;
      m_pdata     = NULL;

   }

   bitmap::~bitmap()
   { 

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }

      if(m_pdata != NULL)
      {
         memory_free(m_pdata);
         m_pdata = NULL;
      }

   }

   bool bitmap::CreateBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits, int32_t stride)
   { 

      UNREFERENCED_PARAMETER(pgraphics);

      if(m_pdata != NULL)
      {
         memory_free(m_pdata);
         m_pdata = NULL;
      }
      
      m_pbitmap = new ::Gdiplus::Bitmap(nWidth, nHeight, Gdiplus::PixelOffsetModeHighQuality);

      return TRUE;
   
   }

   bool bitmap::CreateBitmapIndirect(::draw2d::graphics * pgraphics, LPBITMAP lpBitmap)
   { 

      UNREFERENCED_PARAMETER(pgraphics);

      return FALSE;
   }


   bool bitmap::CreateDIBSection(::draw2d::graphics * pgraphics, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, int * stride, HANDLE hSection, uint32_t offset)
   { 

      UNREFERENCED_PARAMETER(pgraphics);

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }

      if(m_pdata != NULL)
      {
         memory_free(m_pdata);
         m_pdata = NULL;
      }

      try
      {

         m_pdata = memory_alloc(abs(4 * lpbmi->bmiHeader.biWidth * lpbmi->bmiHeader.biHeight));

      }
      catch(...)
      {
         
         return false;

      }

      if(m_pdata == NULL)
         return false;

      m_pbitmap = new Gdiplus::Bitmap(abs(lpbmi->bmiHeader.biWidth), abs(lpbmi->bmiHeader.biHeight), abs(lpbmi->bmiHeader.biWidth) * 4, PixelFormat32bppARGB, (BYTE *) m_pdata);

      if(m_pbitmap == NULL)
      {
         memory_free(m_pdata);
         m_pdata = NULL;
         return FALSE;
      }

      if(ppvBits != NULL)
      {
         *ppvBits = m_pdata; 
      }

      if(stride != NULL)
      {
         *stride = lpbmi->bmiHeader.biWidth * sizeof(COLORREF);
      }

      return TRUE;

   }


   bool bitmap::CreateDIBitmap(::draw2d::graphics * pdc, const BITMAPINFOHEADER *pbmih, uint32_t flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage)
   { 
      return FALSE;
   }


   uint32_t bitmap::SetBitmapBits(uint32_t dwCount, const void * lpBits)
   { 
   
      //return ::SetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   
   }
   uint32_t bitmap::GetBitmapBits(uint32_t dwCount, LPVOID lpBits) const
   { 
      //return ::GetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   }
   bool bitmap::LoadBitmap(const char * lpszResourceName)
   { 
   //   return attach(::LoadBitmap(::core::FindResourceHandle(
   //lpszResourceName, RT_BITMAP), lpszResourceName));
      return FALSE;
   
   }
   
   size bitmap::SetBitmapDimension(int32_t nWidth, int32_t nHeight)
   {


      ::exception::throw_not_implemented(get_app());

      //SIZE size;
      //VERIFY(::SetBitmapDimensionEx((HBITMAP)get_handle(), nWidth, nHeight, &size));
      //return size;
      return class ::size(0, 0);

   }

   size bitmap::GetBitmapDimension() const
   {
      
      if(m_pbitmap == NULL)
         return class size(0, 0);

      return class size(m_pbitmap->GetWidth(), m_pbitmap->GetHeight());

   }

   bool bitmap::LoadBitmap(UINT nIDResource)
   { 
      //return attach(::LoadBitmap(::core::FindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource))); 
      return FALSE;
   }
   bool bitmap::LoadOEMBitmap(UINT nIDBitmap)
   { 
      //return attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap))); 
      return FALSE;
   }
   bool bitmap::CreateCompatibleBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight)
   {

      if(m_pdata != NULL)
      {
         memory_free(m_pdata);
         m_pdata = NULL;
      }

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }

      m_pbitmap = new ::Gdiplus::Bitmap(nWidth, nHeight, Gdiplus::PixelOffsetModeHighQuality);

      return TRUE;
   
   }
   bool bitmap::CreateDiscardableBitmap(::draw2d::graphics * pgraphics, int32_t nWidth, int32_t nHeight)
   { 

      if(m_pdata != NULL)
      {
         memory_free(m_pdata);
         m_pdata = NULL;
      }

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }

      m_pbitmap = new ::Gdiplus::Bitmap(nWidth, nHeight, Gdiplus::PixelOffsetModeHighQuality);

      return TRUE;
   }


   int32_t bitmap::GetBitmap(BITMAP* pBitMap)
   { 
   //   ASSERT(get_handle() != NULL);
     // return ::GetObject(get_handle(), sizeof(BITMAP), pBitMap); 
      return 0;
   }


      /////////////////////////////////////////////////////////////////////////////


      void bitmap::dump(dump_context & dumpcontext) const
      {
         ::draw2d::object::dump(dumpcontext);

/*         if (get_handle() == NULL)
            return;

         if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_BITMAP)
         {
            // not a valid object
            dumpcontext << "has ILLEGAL HBITMAP!";
            return;
         }*/

         /*BITMAP bm;
         VERIFY(GetObject(sizeof(bm), &bm));
         dumpcontext << "bm.bmType = " << bm.bmType;
         dumpcontext << "\nbm.bmHeight = " << bm.bmHeight;
         dumpcontext << "\nbm.bmWidth = " << bm.bmWidth;
         dumpcontext << "\nbm.bmWidthBytes = " << bm.bmWidthBytes;
         dumpcontext << "\nbm.bmPlanes = " << bm.bmPlanes;
         dumpcontext << "\nbm.bmBitsPixel = " << bm.bmBitsPixel;
         */
         dumpcontext << "\n";
      }




   void * bitmap::get_os_data() const
   {
      
      return (void *) (Gdiplus::Bitmap *) m_pbitmap;

   }

   bool bitmap::attach(void * posdata)
   {
      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }
      
      m_pbitmap = (Gdiplus::Bitmap *) posdata;


      return true;

   }

   void * bitmap::detach()
   {

      Gdiplus::Bitmap * pbitmap = m_pbitmap;

      m_pbitmap = NULL;

      return m_pbitmap;

   }


   HBITMAP bitmap::_GetHBITMAP()
   {

      if(m_pbitmap == NULL)
         return NULL;
      
      HBITMAP hbitmap = NULL;

      Gdiplus::Color colorBk(0,0,0,0);

      m_pbitmap->GetHBITMAP(colorBk,&hbitmap);

      return hbitmap;

   }


   void bitmap::_ReleaseHBITMAP(HBITMAP hbitmap)
   {

      ::DeleteObject(hbitmap);

   }


} // namespace draw2d_gdiplus
