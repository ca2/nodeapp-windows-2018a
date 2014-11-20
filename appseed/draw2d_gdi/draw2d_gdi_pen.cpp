#include "framework.h"


namespace draw2d_gdi
{


   pen::pen(::aura::application * papp) :
      element(papp)
   { 

   }


   pen::~pen()
   { 

   }


   pen::operator HPEN() const
   { 

      return (HPEN)(this == NULL ? NULL : get_handle()); 

   }


   bool pen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
   {

      return Attach(::CreatePen(nPenStyle, nWidth, crColor)); 

   }


   bool pen::CreatePenIndirect(LPLOGPEN lpLogPen)
   {

      return Attach(::CreatePenIndirect(lpLogPen)); 

   }


   bool pen::CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount, const uint32_t* lpStyle)
   { 

      return Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, (DWORD *) lpStyle)); 

   }


   int pen::GetExtLogPen(EXTLOGPEN* pLogPen)
   { 

      ASSERT(get_handle() != NULL);

      return get_object(sizeof(EXTLOGPEN), pLogPen); 

   }


   int pen::GetLogPen(LOGPEN* pLogPen)
   { 

      if(get_handle() == NULL)
         return 0;

      return get_object(sizeof(LOGPEN), pLogPen); 

   }


   void pen::construct(int nPenStyle, int nWidth, COLORREF crColor)
   {

      if (!Attach(::CreatePen(nPenStyle, nWidth, crColor)))
         throw resource_exception(get_app());

   }

   void pen::construct(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount, const uint32_t* lpStyle)
   {

      if (!Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, (DWORD *) lpStyle)))
         throw resource_exception(get_app());

   }



   void pen::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

      if (get_handle() == NULL)
         return;

     /* if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_PEN)
      {
         // not a valid object
         dumpcontext << "has ILLEGAL HPEN!";
         return;
      }*/

      LOGPEN lp;
      VERIFY(get_object(sizeof(lp), &lp));
      dumpcontext << "lgpn.lopnStyle = " << lp.lopnStyle;
      dumpcontext << "\nlgpn.lopnWidth.x (width) = " << lp.lopnWidth.x;
      dumpcontext << "\nlgpn.lopnColor = " << (void *)(DWORD_PTR)lp.lopnColor;

      dumpcontext << "\n";
   }


   bool pen::create()
   {

      ::draw2d_gdi::object::create();

      if(m_bProcess)
      {

         CreatePen(PS_SOLID, (int32_t) m_dWidth, RGB(255, 255, 255));

      }
      else if(m_etype == type_solid)
      {
         
         CreatePen(PS_SOLID, (int32_t) m_dWidth, m_cr);

      }

      return true;
      
   }


} // namespace draw2d_gdi



