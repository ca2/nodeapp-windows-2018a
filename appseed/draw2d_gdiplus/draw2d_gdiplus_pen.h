#pragma once



#undef new

#include <GdiPlus.h>


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS pen : 
      virtual public ::draw2d_gdiplus::object,
      virtual public ::draw2d::pen
   {
   public:


      ::Gdiplus::Pen *  m_ppen;

      pen(sp(base_application) papp);
      /*virtual void construct(int32_t nPenStyle, double nWidth, COLORREF crColor);
      virtual void construct(int32_t nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int32_t nStyleCount = 0, const DWORD* lpStyle = NULL);
      bool CreatePen(int32_t nPenStyle, double nWidth, COLORREF crColor);
      bool CreatePen(int32_t nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int32_t nStyleCount = 0, const DWORD* lpStyle = NULL);*/


      virtual void * get_os_data() const;


      virtual ~pen();
      virtual void dump(dump_context & dumpcontext) const;

   };


} // namespace draw2d_gdiplus


