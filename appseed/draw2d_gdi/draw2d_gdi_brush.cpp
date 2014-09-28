#include "framework.h"


namespace draw2d_gdi
{


   brush::brush(sp(::aura::application) papp) :
      element(papp)
   { 
      m_bProcess = false;
   }


   brush::~brush()
   { 

   }


   brush::operator HBRUSH() const
   {

      return (HBRUSH)(this == NULL ? NULL : get_os_data()); 


   }


   bool brush::CreateSolid(COLORREF crColor)
   {

      return Attach(::CreateSolidBrush(argb_invert(crColor)));

   }


   bool brush::CreateHatchBrush(int nIndex, COLORREF crColor)
   {

      return Attach(::CreateHatchBrush(nIndex, argb_invert(crColor)));

   }


   bool brush::CreateBrushIndirect(const LOGBRUSH* lpLogBrush)
   { return Attach(::CreateBrushIndirect(lpLogBrush)); }
   bool brush::CreatePatternBrush(::draw2d::bitmap* pBitmap)
   { return Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data())); }
   bool brush::CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage)
   { return Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage)); }

   bool brush::CreateSysColorBrush(int nIndex)
   {

      return Attach(::GetSysColorBrush(nIndex)); 

   }


   int brush::GetLogBrush(LOGBRUSH* pLogBrush)
   {

      return get_object(sizeof(LOGBRUSH), pLogBrush); 

   }


   void brush::construct(COLORREF crColor)
   {

      if (!Attach(::CreateSolidBrush(crColor)))
         throw resource_exception(get_app());

   }

   void brush::construct(int nIndex, COLORREF crColor)
   {
      if (!Attach(::CreateHatchBrush(nIndex, crColor)))
         throw resource_exception(get_app());
   }

   void brush::construct(::draw2d::bitmap* pBitmap)
   {
      ASSERT_VALID(pBitmap);

      if (!Attach(::CreatePatternBrush((HBITMAP)pBitmap->get_os_data())))
         throw resource_exception(get_app());
   }

   bool brush::CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage)
   {
      ASSERT(hPackedDIB != NULL);
      const void * lpPackedDIB = ::GlobalLock(hPackedDIB);
      ASSERT(lpPackedDIB != NULL);
      bool bResult = Attach(::CreateDIBPatternBrushPt(lpPackedDIB, nUsage));
      ::GlobalUnlock(hPackedDIB);
      return bResult;
   }

   void brush::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

      if (get_os_data() == NULL)
         return;

/*      if (!afxData.bWin95 && ::GetObjectType(get_os_data()) != OBJ_BRUSH)
      {
         // not a valid window
         dumpcontext << "has ILLEGAL HBRUSH!";
         return;
      }*/

      LOGBRUSH lb;
      VERIFY(get_object(sizeof(lb), &lb));
      dumpcontext << "lb.lbStyle = " << lb.lbStyle;
      dumpcontext << "\nlb.lbHatch = " << (uint32_t) lb.lbHatch;
      dumpcontext << "\nlb.lbColor = " << (void *)(DWORD_PTR)lb.lbColor;

      dumpcontext << "\n";
   }



   bool brush::create()
   {

      ::draw2d_gdi::object::create();

      if(m_bProcess)
      {
            
         CreateSolid(RGB(255, 255, 255));

      }
      else if(m_etype == type_solid)
      {
         
         CreateSolid(m_cr);

      }

      return true;
      
   }


} // namespace draw2d_gdi




