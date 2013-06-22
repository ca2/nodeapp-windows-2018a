#pragma once


namespace win
{


   class CLASS_DECL_win graphics_path : 
      virtual public ::ca2::graphics_path
   {
   public:

      
      Gdiplus::GraphicsPath *    m_ppath;
      Gdiplus::PointF            m_ptInternal;
      bool                       m_bHasPointInternal;



      graphics_path(sp(::ca2::application) papp);
      virtual ~graphics_path();


      virtual void * get_os_data() const;


      virtual point internal_last_point();

      virtual point internal_current_point();


      virtual bool internal_begin_figure(bool bFill, ::ca2::e_fill_mode efillmode);
      virtual bool internal_end_figure(bool bClose);

      virtual bool internal_add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool internal_add_line(int32_t x, int32_t y);

      virtual bool internal_add_move(int32_t x, int32_t y);


      virtual bool create();
      virtual bool destroy();

      virtual bool set(const ::ca2::graphics_path::element & e);
   
      virtual bool set(const ::ca2::graphics_path::arc & a);

      virtual bool set(const ::ca2::graphics_path::line & l);

      virtual bool set(const ::ca2::graphics_path::move & p);


   };


} // namespace win




