#pragma once





namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS pen :
      virtual public ::draw2d_gdiplus::object,
      virtual public ::draw2d::pen
   {
      public:


         Gdiplus::PenAlignment m_egdiplusalign;

         ::Gdiplus::Pen *  m_ppen;

         pen(::aura::application * papp);
         virtual ~pen();


#ifdef DEBUG

         virtual void dump(dump_context & dumpcontext) const;

#endif

         /*virtual void construct(int32_t nPenStyle, double nWidth, COLORREF crColor);
         virtual void construct(int32_t nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int32_t nStyleCount = 0, const DWORD* lpStyle = NULL);
         bool CreatePen(int32_t nPenStyle, double nWidth, COLORREF crColor);
         bool CreatePen(int32_t nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int32_t nStyleCount = 0, const DWORD* lpStyle = NULL);*/


         virtual void * get_os_data() const;



   };


} // namespace draw2d_gdiplus


