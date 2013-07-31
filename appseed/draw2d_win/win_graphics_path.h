#pragma once


namespace win
{


   class CLASS_DECL_win path : 
      virtual public ::draw2d::path
   {
   public:

      
      Gdiplus::GraphicsPath *    m_ppath;
      Gdiplus::PointF            m_ptInternal;
      bool                       m_bHasPointInternal;



      path(sp(::ca2::application) papp);
      virtual ~path();


      virtual void * get_os_data() const;


      virtual point internal_last_point();

      virtual point internal_current_point();


      virtual bool internal_begin_figure(bool bFill, ::draw2d::e_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);

      virtual bool internal_add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool internal_add_line(int32_t x, int32_t y);

      virtual bool internal_add_move(int32_t x, int32_t y);


      virtual bool create();
      virtual bool destroy();

      virtual bool set(const ::draw2d::path::element & e);
   
      virtual bool set(const ::draw2d::path::arc & a);

      virtual bool set(const ::draw2d::path::line & l);

      virtual bool set(const ::draw2d::path::move & p);


   };


} // namespace win




