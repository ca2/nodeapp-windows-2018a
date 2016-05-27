#include "framework.h"


namespace draw2d_gdi
{


   bitmap::bitmap(::aura::application * papp):
      ::object(papp)
   {

   }


   bitmap::~bitmap()
   {

   }


   bitmap::operator HBITMAP() const
   {

      return (HBITMAP)(this == NULL ? NULL : get_handle());

   }


   bool bitmap::CreateBitmap(::draw2d::graphics * pgraphics,int nWidth,int nHeight,UINT nPlanes,UINT nBitcount,const void * lpBits,int32_t stride)
   {

      return Attach(::CreateBitmap(nWidth,nHeight,nPlanes,nBitcount,lpBits));

   }


   bool bitmap::CreateBitmapIndirect(LPBITMAP lpBitmap)
   {

      return Attach(::CreateBitmapIndirect(lpBitmap));

   }

   bool bitmap::CreateDIBSection(::draw2d::graphics * pgraphics,const BITMAPINFO * lpbmi,UINT usage,void **ppvBits,int32_t * stride,HANDLE hSection,uint32_t offset)
   {


      if(!Attach(::CreateDIBSection(pgraphics == NULL ? NULL : GDI_HDC(pgraphics),lpbmi,usage,ppvBits,hSection,offset)))
         return false;

      if(stride != NULL)
      {

         int32_t scan = abs((long)lpbmi->bmiHeader.biSizeImage) / abs((long)lpbmi->bmiHeader.biHeight);

         *stride = scan;

      }

      return true;

   }


   bool bitmap::CreateDIBitmap(::draw2d::graphics * pgraphics,const BITMAPINFOHEADER *pbmih,uint32_t flInit,const void *pjBits,const BITMAPINFO *pbmi,UINT iUsage)
   {

      return Attach(::CreateDIBitmap(GDI_HDC(pgraphics),pbmih,flInit,pjBits,pbmi,iUsage));

   }


   uint32_t bitmap::SetBitmapBits(uint32_t dwCount,const void * lpBits)
   {

      return ::SetBitmapBits((HBITMAP)get_handle(),dwCount,lpBits);

   }


   uint32_t bitmap::GetBitmapBits(uint32_t dwCount,LPVOID lpBits) const
   {

      return ::GetBitmapBits((HBITMAP)get_handle(),dwCount,lpBits);

   }


   bool bitmap::LoadBitmap(const char * lpszResourceName)
   {

      //      return Attach(::LoadBitmap(AfxFindResourceHandle(lpszResourceName, RT_BITMAP), lpszResourceName));
      return false;

   }


   size bitmap::SetBitmapDimension(int nWidth,int nHeight)
   {

      SIZE size;

      VERIFY(::SetBitmapDimensionEx((HBITMAP)get_handle(),nWidth,nHeight,&size));

      return size;

   }


   size bitmap::GetBitmapDimension() const
   {

      SIZE size;

      VERIFY(::GetBitmapDimensionEx((HBITMAP)get_handle(),&size));

      return size;

   }


   bool bitmap::LoadBitmap(UINT nIDResource)
   {

      //      return Attach(::LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource), RT_BITMAP), MAKEINTRESOURCE(nIDResource))); 
      return false;

   }


   bool bitmap::LoadOEMBitmap(UINT nIDBitmap)
   {

      return Attach(::LoadBitmap(NULL,MAKEINTRESOURCE(nIDBitmap)));

   }


   bool bitmap::CreateCompatibleBitmap(::draw2d::graphics * pgraphics,int nWidth,int nHeight)
   {

      return Attach(::CreateCompatibleBitmap((dynamic_cast<::draw2d_gdi::graphics *>(pgraphics))->get_handle1(),nWidth,nHeight));

   }


   bool bitmap::CreateDiscardableBitmap(::draw2d::graphics * pgraphics,int nWidth,int nHeight)
   {

      return Attach(::CreateDiscardableBitmap((dynamic_cast<::draw2d_gdi::graphics *>(pgraphics))->get_handle1(),nWidth,nHeight));

   }


   int bitmap::GetBitmap(BITMAP* pBitMap)
   {

      return get_object(sizeof(BITMAP),pBitMap);

   }


   void bitmap::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

      if(get_handle() == NULL)
         return;

      /*if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_BITMAP)
      {
      // not a valid object
      dumpcontext << "has ILLEGAL HBITMAP!";
      return;
      }*/

      BITMAP bm;
      VERIFY(get_object(sizeof(bm),&bm));
      dumpcontext << "bm.bmType = " << bm.bmType;
      dumpcontext << "\nbm.bmHeight = " << bm.bmHeight;
      dumpcontext << "\nbm.bmWidth = " << bm.bmWidth;
      dumpcontext << "\nbm.bmWidthBytes = " << bm.bmWidthBytes;
      dumpcontext << "\nbm.bmPlanes = " << bm.bmPlanes;
      dumpcontext << "\nbm.bmBitsPixel = " << bm.bmBitsPixel;

      dumpcontext << "\n";
   }




   HBITMAP bitmap::_GetHBITMAP()
   {
      
      return operator HBITMAP ();

   }


   void bitmap::_ReleaseHBITMAP(HBITMAP hbitmap)
   {

      // Do nothing, _GetHBITMAP does not create additional bitmap for the GetHBITMAP/ReleaseHBITMAP protocol,
      // so does not delete underlying original HBITMAP handle
      UNREFERENCED_PARAMETER(hbitmap);
   
   }




} // namespace draw2d_gdi





