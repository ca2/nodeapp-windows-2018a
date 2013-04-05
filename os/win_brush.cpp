#include "framework.h"

#undef new

namespace win
{


   brush::brush(::ca::applicationsp papp) :
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

   //bool brush::create_solid(COLORREF crColor)
   //{

   //   if(m_pbrush != NULL)
   //   {
   //      delete m_pbrush;
   //   }

   //   m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(crColor), GetRValue(crColor), GetGValue(crColor), GetBValue(crColor)));

   //   return m_pbrush != NULL;
   //      
   //}

   // bool brush::CreateHatchBrush(int32_t nIndex, COLORREF crColor)
   // { 
   //    //return attach(::CreateHatchBrush(nIndex, crColor));
   //    return FALSE;
   // }
//    bool brush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
  //  {
       //return attach(::CreateBrushIndirect(lpLogBrush)); 
    //   return FALSE;
    //}
    //bool brush::CreatePatternBrush(::ca::bitmap* pBitmap)
    //{ 
    //   //return attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data()));
    //   return FALSE;
    //}
    //bool brush::CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage)
    //{ 
    //   //return attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
    //   return FALSE;
    //}
    //bool brush::CreateSysColorBrush(int32_t nIndex)
    //{ 
    //   //return attach(::GetSysColorBrush(nIndex));
    //   return FALSE;

    //}
/*    int32_t brush::GetLogBrush(LOGBRUSH* pLogBrush)
    { 
       //ASSERT(get_os_data() != NULL);
       //return ::GetObject(get_os_data(), sizeof(LOGBRUSH), pLogBrush); 
       return FALSE;
    }*/


//   void brush::construct(COLORREF crColor)
//   {
////      if (!attach(::create_solid(crColor)))
//  //       throw resource_exception();
//   }
//
//   void brush::construct(int32_t nIndex, COLORREF crColor)
//   {
//    //  if (!attach(::CreateHatchBrush(nIndex, crColor)))
//      //   throw resource_exception();
//   }
//
//   void brush::construct(::ca::bitmap* pBitmap)
//   {
//      //ASSERT_VALID(pBitmap);
//
//      //if (!attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data())))
//        // throw resource_exception();
//   }
//
//   bool brush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
//   {
//      //ASSERT(hPackedDIB != NULL);
//      //const void * lpPackedDIB = ::GlobalLock(hPackedDIB);
//      //ASSERT(lpPackedDIB != NULL);
//      //bool bResult = attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
//      //::GlobalUnlock(hPackedDIB);
//      //return bResult;
//      return FALSE;
//   }


   void brush::dump(dump_context & dumpcontext) const
   {
      ::ca::graphics_object::dump(dumpcontext);

      //if (get_os_data() == NULL)
        // return;

/*      LOGBRUSH lb;
      VERIFY(GetObject(sizeof(lb), &lb));
      dumpcontext << "lb.lbStyle = " << lb.lbStyle;
      dumpcontext << "\nlb.lbHatch = " << lb.lbHatch;
      dumpcontext << "\nlb.lbColor = " << (void *)(uint_ptr)lb.lbColor;

      dumpcontext << "\n";
      */
   }



   void * brush::get_os_data() const
   {
      
      if(m_pbrush == NULL || !m_bUpdated)
      {
         if(m_pbrush != NULL)
         {
            try
            {
               delete m_pbrush;
            }
            catch(...)
            {
            }
         }
         if(m_etype == type_solid)
         {
            try
            {
               ((brush *) this)->m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(m_cr), GetRValue(m_cr), GetGValue(m_cr), GetBValue(m_cr)));
            }
            catch(...)
            {
            }
         }
         else if(m_etype == type_linear_gradient_point_color)
         {
            try
            {
               ((brush *) this)->m_pbrush = new Gdiplus::LinearGradientBrush(
                  Gdiplus::Point(m_pt1.x, m_pt1.y),
                  Gdiplus::Point(m_pt2.x, m_pt2.y),
                  Gdiplus::Color(GetAValue(m_cr1), GetRValue(m_cr1), GetGValue(m_cr1), GetBValue(m_cr1)),
                  Gdiplus::Color(GetAValue(m_cr2), GetRValue(m_cr2), GetGValue(m_cr2), GetBValue(m_cr2)));
            }
            catch(...)
            {
            }
         }
         else
         {
            try
            {
               ((brush *) this)->m_pbrush = new Gdiplus::SolidBrush(Gdiplus::Color(GetAValue(m_cr), GetRValue(m_cr), GetGValue(m_cr), GetBValue(m_cr)));
            }
            catch(...)
            {
            }
         }
      }

      if(m_pbrush != NULL)
      {
         ((brush *) this)->m_bUpdated = true;
      }

      return (void *) (Gdiplus::Brush *) m_pbrush;

   }

} // namespace win


