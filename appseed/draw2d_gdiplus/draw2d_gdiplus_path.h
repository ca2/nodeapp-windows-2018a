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



      path(sp(::base::application) papp);
      virtual ~path();


      virtual Gdiplus::GraphicsPath * get_os_path(Gdiplus::Graphics * pgraphics);

      virtual bool internal_add_arc(const RECT & rect, int iStart, int iAngle);

      virtual point internal_last_point();

      virtual point internal_current_point();


      virtual bool internal_begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);

      virtual bool internal_add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool internal_add_line(int32_t x, int32_t y);

      virtual bool internal_add_move(int32_t x, int32_t y);
      
      virtual bool internal_add_string(Gdiplus::Graphics * pgraphics, int32_t x,int32_t y,const string & strText,::draw2d::font_sp spfont);

      virtual bool create(Gdiplus::Graphics * pgraphics);
      virtual bool destroy();

      virtual bool set(Gdiplus::Graphics * pgraphics, const ::draw2d::path::element & e);
   
      virtual bool set(const ::draw2d::path::arc & a);

      virtual bool set(const ::draw2d::path::line & l);

      virtual bool set(const ::draw2d::path::move & p);

      virtual bool set(Gdiplus::Graphics * pgraphics, const ::draw2d::path::string_path & p);


   };


} // namespace draw2d_gdiplus




