#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS path :
      virtual public ::draw2d::path
   {
   public:


      Gdiplus::GraphicsPath *    m_ppath;
      Gdiplus::PointF            m_ptInternal;
      bool                       m_bHasPointInternal;



      path(::aura::application * papp);
      virtual ~path();


      virtual Gdiplus::GraphicsPath * get_os_path(Gdiplus::Graphics * pgraphics);

      virtual bool internal_add_arc(const RECT & rect, double iStart, double iAngle);

      virtual pointd internal_last_point();

      virtual pointd internal_current_point();


      virtual bool internal_begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);

      virtual bool internal_add_line(double x1, double y1, double x2, double y2);
      virtual bool internal_add_rect(double x, double y, double cx, double cy);
      virtual bool internal_add_line(double x, double y);
      virtual bool internal_add_move(double x, double y);

      //virtual bool internal_add_rect(double x,double y,double cx,double cy);
      //virtual bool internal_add_move(double x,double y);

      virtual bool internal_add_string(Gdiplus::Graphics * pgraphics, int32_t x,int32_t y,const string & strText,::draw2d::font_sp spfont);
      virtual bool internal_add_draw_text(Gdiplus::Graphics * pgraphics, LPCRECT lpcrect, const string & strText, ::draw2d::font_sp spfont, int nFormat);

      virtual bool create(Gdiplus::Graphics * pgraphics);
      virtual bool destroy();

      virtual bool set(Gdiplus::Graphics * pgraphics, const ::draw2d::path::element & e);

      virtual bool set(const ::draw2d::path::arc & a);

      virtual bool set(const ::draw2d::path::rect & r);

      virtual bool set(const ::draw2d::path::line & l);

      virtual bool set(const ::draw2d::path::move & p);

      virtual bool set(Gdiplus::Graphics * pgraphics, ::draw2d::path::element::e_type etype, const ::draw2d::path::string_path & p);


   };


} // namespace draw2d_gdiplus




