#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS font :
      virtual public ::draw2d::font
   {
      public:


         Gdiplus::Font *   m_pfont;


         font(::aura::application * papp);
         font(const font & font);
         virtual ~font();

         virtual void construct(const ::draw2d::font & fontParam);


#ifdef DEBUG

         virtual void dump(dump_context & dumpcontext) const;

#endif

         virtual void * get_os_data() const;



         virtual e_cs calc_cs(::draw2d::graphics * pgraphics);


   };


} // namespace draw2d_gdiplus



