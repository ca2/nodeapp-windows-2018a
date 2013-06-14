#pragma once



#undef new

#include <GdiPlus.h>


namespace win
{


   class CLASS_DECL_win pen : 
      virtual public ::win::graphics_object,
      virtual public ::ca2::pen
   {
   public:


      ::Gdiplus::Pen *  m_ppen;

      pen(sp(::ca2::application) papp);
      /*virtual void construct(int32_t nPenStyle, double nWidth, COLORREF crColor);
      virtual void construct(int32_t nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int32_t nStyleCount = 0, const DWORD* lpStyle = NULL);
      bool CreatePen(int32_t nPenStyle, double nWidth, COLORREF crColor);
      bool CreatePen(int32_t nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int32_t nStyleCount = 0, const DWORD* lpStyle = NULL);*/


      virtual void * get_os_data() const;


      virtual ~pen();
      virtual void dump(dump_context & dumpcontext) const;

   };


} // namespace win


