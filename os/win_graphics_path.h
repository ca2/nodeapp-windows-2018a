#pragma once


namespace win
{


   class CLASS_DECL_win graphics_path : 
      virtual public ::ca::graphics_path
   {
   public:

      
      Gdiplus::GraphicsPath *    m_ppath;
      bool                       m_bMove;
      Gdiplus::PointF            m_point;


      graphics_path(::ca::application * papp);
      virtual ~graphics_path();


      virtual void * get_os_data() const;


      virtual point last_point();

      virtual point current_point();


      virtual bool begin_figure(bool bFill, ::ca::e_fill_mode efillmode);
      virtual bool end_figure(bool bClose);

      virtual bool add_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool add_line(int32_t x, int32_t y);

      virtual bool add_move(int32_t x, int32_t y);

   };


} // namespace win




