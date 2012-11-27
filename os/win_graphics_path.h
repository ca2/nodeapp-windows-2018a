#pragma once


namespace win
{


   class CLASS_DECL_win graphics_path : 
      virtual public ::ca::graphics_path
   {
   public:

      
      Gdiplus::GraphicsPath *   m_ppath;


      graphics_path(::ca::application * papp);
      virtual ~graphics_path();


      virtual void * get_os_data() const;


      virtual point last_point();

      virtual point current_point();


      virtual bool start_figure();
      virtual bool close_figure();

      virtual bool add_line(int x1, int y1, int x2, int y2);
      virtual bool add_line(int x, int y);

   };


} // namespace win




