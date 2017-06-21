#pragma once


namespace app_app
{


   struct spa_canvas_toolbox;


   class application;


   class SPALIB_API canvas :
      virtual public ::object
   {
   public:


      application *               m_pspa;

      ::draw2d::bitmap_sp        m_pbitmap;
      ::draw2d::graphics_sp      m_pgraphics;
      ::draw2d::font_sp          m_pfont;
      ::draw2d::font_sp          m_pfontBold;
      ::draw2d::font_sp          m_pfontHeader;
      ::draw2d::pen_sp           m_ppenBorder;
      ::draw2d::brush_sp         m_ptextColor1;
      ::draw2d::brush_sp         m_pBar;
      ::draw2d::brush_sp         m_pBarBk;
      ::draw2d::pen_sp           m_pBarBorder;



      string                     m_strLastStatus;

      int                        m_cx;

      int                        m_cy;

      COLORREF *                 m_pcolorref;

      HBITMAP                    m_hbitmap;

      HDC                        m_hdc;

      HBRUSH                     m_hbrushBk;

      HBITMAP                    m_hbmAlpha;

      HDC                        m_hdcAlpha;

      bool                       m_bDraw;


      canvas(::aura::application * papp);
      ~canvas();

      bool update_layered_window();

      bool initialize(int cx, int cy);

      void paint_install(LPCRECT lpcrect, int iMode);


   };


   struct spa_canvas_toolbox
   {



   };




} // namespace app_app



