#pragma once



#undef new

#include <GdiPlus.h>


namespace win
{


   class CLASS_DECL_VMSWIN pen : 
      virtual public ::win::graphics_object,
      virtual public ::ca::pen
   {
   public:


      ::Gdiplus::Pen *  m_ppen;

      pen(::ca::application * papp);
      /*virtual void construct(int nPenStyle, double nWidth, COLORREF crColor);
      virtual void construct(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
      BOOL CreatePen(int nPenStyle, double nWidth, COLORREF crColor);
      BOOL CreatePen(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);*/


      virtual void * get_os_data() const;


      virtual ~pen();
   #ifdef _DEBUG
      virtual void dump(dump_context & dumpcontext) const;
   #endif

   };

} // namespace win