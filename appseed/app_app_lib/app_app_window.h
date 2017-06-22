#pragma once


namespace app_app
{


   class SPALIB_API window :
      virtual public ::object
   {
   public:


      HWND                          m_hwnd;


      MESSAGE                       m_msg;
      wstring                       m_wstrWindowTitle;
      wstring                       m_wstrWindowClass;


      int                           m_iHealingSurface;
      double                        m_dAnime;
      double                        m_dPi;
      bool                          m_bDrag;
      POINT                         m_ptDragStart;
      RECT                          m_rectWindowDragStart;

      ::draw2d::dib_sp              m_dib;
      ::draw2d::font_sp             m_font;
      ::draw2d::font_sp             m_fontBold;
      ::draw2d::font_sp             m_fontHeader;
      ::draw2d::pen_sp              m_penBorder;
      ::draw2d::brush_sp            m_brushText;
      ::draw2d::brush_sp            m_brushBar;
      ::draw2d::brush_sp            m_brushBarBk;
      ::draw2d::pen_sp              m_penBarBorder;
      ::draw2d::brush_sp            m_brushBk;
      string                        m_strLastStatus;
      int                           m_cx;
      int                           m_cy;
      COLORREF *                    m_pcolorref;
      HBITMAP                       m_hbitmap;
      HDC                           m_hdc;
      HBRUSH                        m_hbrushBk;
      HBITMAP                       m_hbmAlpha;
      HDC                           m_hdcAlpha;
      bool                          m_bDraw;

      bool                          m_bRunMainLoop;

      static window *               s_p;


      window(::aura::application * papp);
      virtual ~window();


      virtual bool create();

      virtual void draw();

      virtual bool update_layered_window();

      virtual void drag();

      virtual bool show();

      virtual bool hide();

      virtual bool initialize(int cx, int cy);

      virtual void paint_install(LPCRECT lpcrect, int iMode);

      LRESULT message_handler(UINT message, WPARAM wParam, LPARAM lParam);

      static LRESULT CALLBACK window_procedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

      virtual ATOM register_class();

      virtual void main();

   };


} // namespace app_app



