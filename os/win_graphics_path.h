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

      virtual bool add_line(int x1, int y1, int x2, int y2);
      virtual bool add_line(int x, int y);

      virtual bool add_move(int x, int y);

   };


} // namespace win




