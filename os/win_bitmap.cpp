#include "framework.h"


#undef new


namespace win
{


   bitmap::bitmap(::ca::application * papp) :
      ca(papp)
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
         ca2_free(m_pdata);
         m_pdata = NULL;
      }

   }

   bool bitmap::CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits)
   { 

      if(m_pdata != NULL)
      {
         ca2_free(m_pdata);
         m_pdata = NULL;
      }
      
      m_pbitmap = new ::Gdiplus::Bitmap(nWidth, nHeight, Gdiplus::PixelOffsetModeHighQuality);

      return TRUE;
   
   }

   bool bitmap::CreateBitmapIndirect(LPBITMAP lpBitmap)
   { 
      return FALSE;
   }


   bool bitmap::CreateDIBSection(::ca::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, HANDLE hSection, DWORD offset)
   { 

      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
      }

      if(m_pdata != NULL)
      {
         ca2_free(m_pdata);
         m_pdata = NULL;
      }

      m_pdata = ca2_alloc(abs(4 * lpbmi->bmiHeader.biWidth * lpbmi->bmiHeader.biHeight));

      if(m_pdata == NULL)
         return FALSE;

      m_pbitmap = new Gdiplus::Bitmap(abs(lpbmi->bmiHeader.biWidth), abs(lpbmi->bmiHeader.biHeight), abs(lpbmi->bmiHeader.biWidth) * 4, PixelFormat32bppARGB, (BYTE *) m_pdata);

      if(m_pbitmap == NULL)
      {
         ca2_free(m_pdata);
         m_pdata = NULL;
         return FALSE;
      }

      if(ppvBits != NULL)
      {
         *ppvBits = m_pdata; 
      }

      return TRUE;

   }


   bool bitmap::CreateDIBitmap(::ca::graphics * pdc, const BITMAPINFOHEADER *pbmih, DWORD flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage)
   { 
      return FALSE;
   }


   DWORD bitmap::SetBitmapBits(DWORD dwCount, const void * lpBits)
   { 
   
      //return ::SetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   
   }
   DWORD bitmap::GetBitmapBits(DWORD dwCount, LPVOID lpBits) const
   { 
      //return ::GetBitmapBits((HBITMAP)get_handle(), dwCount, lpBits);
      return 0;
   }
   bool bitmap::LoadBitmap(const char * lpszResourceName)
   { 
   //   return Attach(::LoadBitmap(gen::FindResourceHandle(
   //lpszResourceName, RT_BITMAP), lpszResourceName));
      return FALSE;
   
   }
   
   size bitmap::SetBitmapDimension(int nWidth, int nHeight)
   {


      throw not_implemented_exception();

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
      //return Attach(::LoadBitmap(gen::FindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource))); 
      return FALSE;
   }
   bool bitmap::LoadOEMBitmap(UINT nIDBitmap)
   { 
      //return Attach(::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap))); 
      return FALSE;
   }
   bool bitmap::CreateCompatibleBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight)
   {

      if(m_pdata != NULL)
      {
         ca2_free(m_pdata);
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
   bool bitmap::CreateDiscardableBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight)
   { 

      if(m_pdata != NULL)
      {
         ca2_free(m_pdata);
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


   int bitmap::GetBitmap(BITMAP* pBitMap)
   { 
   //   ASSERT(get_handle() != NULL);
     // return ::GetObject(get_handle(), sizeof(BITMAP), pBitMap); 
      return 0;
   }


      /////////////////////////////////////////////////////////////////////////////


      void bitmap::dump(dump_context & dumpcontext) const
      {
         ::ca::graphics_object::dump(dumpcontext);

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




   int_ptr bitmap::get_os_data() const
   {
      
      return (int_ptr) (Gdiplus::Bitmap *) m_pbitmap;

   }

   bool bitmap::Attach(HBITMAP hbitmap)
   {
      if(m_pbitmap != NULL)
      {
         delete m_pbitmap;
         m_pbitmap = NULL;
      }
      
      m_pbitmap = new Gdiplus::Bitmap(hbitmap, NULL);


      return true;

   }


} // namespace win