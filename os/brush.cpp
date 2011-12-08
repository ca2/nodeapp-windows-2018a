#include "StdAfx.h"

#undef new

namespace win
{


   brush::brush(::ca::application * papp) :
      ca(papp)
   { 

      m_pbrush = NULL;

   }

   brush::~brush()
   { 
      if(m_pbrush != NULL)
      {
         delete m_pbrush;
         m_pbrush = NULL;
      }
   }

   //BOOL brush::CreateSolidBrush(COLORREF crColor)
   //{

   //   if(m_pbrush != NULL)
   //   {
   //      delete m_pbrush;
   //   }

   //   m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(crColor), GetRValue(crColor), GetGValue(crColor), GetBValue(crColor)));

   //   return m_pbrush != NULL;
   //      
   //}

   // BOOL brush::CreateHatchBrush(int nIndex, COLORREF crColor)
   // { 
   //    //return Attach(::CreateHatchBrush(nIndex, crColor));
   //    return FALSE;
   // }
//    BOOL brush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
  //  {
       //return Attach(::CreateBrushIndirect(lpLogBrush)); 
    //   return FALSE;
    //}
    //BOOL brush::CreatePatternBrush(::ca::bitmap* pBitmap)
    //{ 
    //   //return Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data()));
    //   return FALSE;
    //}
    //BOOL brush::CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage)
    //{ 
    //   //return Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
    //   return FALSE;
    //}
    //BOOL brush::CreateSysColorBrush(int nIndex)
    //{ 
    //   //return Attach(::GetSysColorBrush(nIndex));
    //   return FALSE;

    //}
/*    int brush::GetLogBrush(LOGBRUSH* pLogBrush)
    { 
       //ASSERT(get_os_data() != NULL);
       //return ::GetObject(get_os_data(), sizeof(LOGBRUSH), pLogBrush); 
       return FALSE;
    }*/


//   void brush::construct(COLORREF crColor)
//   {
////      if (!Attach(::CreateSolidBrush(crColor)))
//  //       AfxThrowResourceException();
//   }
//
//   void brush::construct(int nIndex, COLORREF crColor)
//   {
//    //  if (!Attach(::CreateHatchBrush(nIndex, crColor)))
//      //   AfxThrowResourceException();
//   }
//
//   void brush::construct(::ca::bitmap* pBitmap)
//   {
//      //ASSERT_VALID(pBitmap);
//
//      //if (!Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data())))
//        // AfxThrowResourceException();
//   }
//
//   BOOL brush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
//   {
//      //ASSERT(hPackedDIB != NULL);
//      //const void * lpPackedDIB = ::GlobalLock(hPackedDIB);
//      //ASSERT(lpPackedDIB != NULL);
//      //BOOL bResult = Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
//      //::GlobalUnlock(hPackedDIB);
//      //return bResult;
//      return FALSE;
//   }

#ifdef _DEBUG
   void brush::dump(dump_context & dumpcontext) const
   {
      ::ca::graphics_object::dump(dumpcontext);

      //if (get_os_data() == NULL)
        // return;

/*      LOGBRUSH lb;
      VERIFY(GetObject(sizeof(lb), &lb));
      dumpcontext << "lb.lbStyle = " << lb.lbStyle;
      dumpcontext << "\nlb.lbHatch = " << lb.lbHatch;
      dumpcontext << "\nlb.lbColor = " << (void *)(DWORD_PTR)lb.lbColor;

      dumpcontext << "\n";
      */
   }
#endif


   void * brush::get_os_data() const
   {
      
      if(m_pbrush == NULL || !m_bUpdated)
      {
         if(m_pbrush != NULL)
         {
            delete m_pbrush;
         }
         ((brush *) this)->m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(m_crColor), GetRValue(m_crColor), GetGValue(m_crColor), GetBValue(m_crColor)));
      }

      if(m_pbrush != NULL)
      {
         ((brush *) this)->m_bUpdated = true;
      }

      return (Gdiplus::Brush *) m_pbrush;

   }

} // namespace win


