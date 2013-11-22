#pragma once


namespace draw2d_gdi
{


   //////////////////////////////////////////////////////////////////////
   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////

   class CLASS_DECL_DRAW2D_GDI dib :
      virtual public ::draw2d::dib
   {
   public:


      BITMAPINFO              m_info;
      ::draw2d::bitmap_sp     m_spbitmap;
      ::draw2d::graphics_sp   m_spgraphics;
      HBITMAP                 m_hbitmapOriginal;
      bool                    m_bMapped;



      dib(base_application * papp);
      virtual ~dib();


      virtual void construct(int cx, int cy);




      virtual ::draw2d::graphics * get_graphics();
      virtual ::draw2d::bitmap_sp get_bitmap();
      virtual ::draw2d::bitmap_sp detach_bitmap();


      void stretch_dib(::draw2d::dib * pdib);

      //bool dc_select(bool bSelect = true);

      virtual void map(bool bApplyAlphaTransform = true); // some implementations may requrire to map to m_pcolorref before manipulate it
      virtual void unmap(); // some implementations may require to unmap from m_pcolorref to update *os* bitmap


      COLORREF GetAverageColor();
      bool Blend(::draw2d::dib *pdib, ::draw2d::dib * pdibA, int A);
      bool color_blend(COLORREF cr, BYTE bAlpha);
      void BitBlt(::draw2d::dib * pdib, int op);
      int cos(int i, int iAngle);
      int sin(int i, int iAngle);
      int cos10(int i, int iAngle);
      int sin10(int i, int iAngle);

      bool is_rgb_black();
      void xor(::draw2d::dib * pdib);

      void ToAlpha(int i);
      void ToAlphaAndFill(int i, COLORREF cr);
      void GrayToARGB(COLORREF cr);

      void from_alpha();
      void mult_alpha(::draw2d::dib * pdibWork, bool bPreserveAlpha = true);
      void set_rgb(int R, int G, int B);

      void rotate(::draw2d::dib * pdib, LPCRECT lpcrect, double dAngle, double dScale);
      void rotate(::draw2d::dib * pdib, double dAngle, double dScale);
      void Rotate034(::draw2d::dib * pdib, double dAngle, double dScale);


      void SetIconMask(::visual::icon * picon, int cx, int cy);
      void RadialFill(BYTE a, BYTE r, BYTE g, BYTE b, int x, int y, int iRadius);
      void RadialFill(
         BYTE a1, BYTE r1, BYTE g1, BYTE b1, // center colors
         BYTE a2, BYTE r2, BYTE g2, BYTE b2, // border colors
         int x, int y, int iRadius);

      uint32_t GetPixel(int x, int y);
      void Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask);
      void channel_mask(BYTE uchFind, BYTE uchSet, BYTE uchUnset, visual::rgba::echannel echannel);
      void transparent_color(color color);
      

      bool create(class size size);
      bool create(int iWidth, int iHeight);
      bool create(::draw2d::graphics * pdc);
      bool Destroy();


      void DivideRGB(int iDivide);
      void DivideARGB(int iDivide);
      void DivideA(int iDivide);

      bool from(::draw2d::graphics * pdc);
      bool from(point ptDest, ::draw2d::graphics * pdc, point pt, class size sz);

      bool to(::draw2d::graphics * pgraphics, point pt, class size size, point ptSrc);
      
      virtual void fill_channel(int C, visual::rgba::echannel echannel);
      void Fill (int A, int R, int G, int B );
      void Fill ( int R, int G, int B );
      void FillGlass ( int R, int G, int B, int A );
      void FillStippledGlass ( int R, int G, int B );
      void Invert();
      void channel_invert(visual::rgba::echannel echannel);
      void channel_multiply(visual::rgba::echannel echannel, double dRate);

      void Map (int ToRgb, int FromRgb );

      void copy( ::draw2d::dib *dib );
      void Paste ( ::draw2d::dib *dib );

      void Blend ( ::draw2d::dib *dib, int A );
      void Darken ( ::draw2d::dib *dib );
      void Difference ( ::draw2d::dib *dib );
      void Lighten ( ::draw2d::dib *dib );
      void Multiply ( ::draw2d::dib *dib );
      void Screen ( ::draw2d::dib *dib );

      void copy ( ::draw2d::dib *dib, int x, int y );
      void PasteRect ( ::draw2d::dib *dib, int x, int y );

      void FillRect ( int x, int y, int w, int h, int R, int G, int B );
      void FillGlassRect ( int x, int y, int w, int h, int R, int G, int B, int A );
      void FillStippledGlassRect ( int x, int y, int w, int h, int R, int G, int B );
      
      void BlendRect ( ::draw2d::dib *dib, int x, int y, int A );
      void DarkenRect ( ::draw2d::dib *dib, int x, int y );
      void DifferenceRect ( ::draw2d::dib *dib, int x, int y );
      void LightenRect ( ::draw2d::dib *dib, int x, int y );
      void MultiplyRect ( ::draw2d::dib *dib, int x, int y );
      void ScreenRect ( ::draw2d::dib *dib, int x, int y );

      void Line ( int x1, int y1, int x2, int y2, int R, int G, int B );
      void LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A );

      void create_frame(class size size, int iFrameCount);
      void set_frame2(void * lpdata, int iFrame, int iFrameCount);
      void set_frame1(void * lpdata, int iFrame, int iFrameCount);
      void get_frame(void * lpdata, int iFrame, int iFrameCount);
      void xor_dib_frame2(void * lpdata, int iFrame, int iFrameCount);


      int width();
      int height();
      double pi();

      virtual bool update_window(::user::window * pwnd, signal_details * pobj);
      virtual bool print_window(::user::window * pwnd, signal_details * pobj);


      virtual bool process_initialize(::draw2d::brush * pbrush, bool bReset = true);
      virtual bool process_initialize(::draw2d::pen * ppen, bool bReset = true);
      virtual bool process_blend(::draw2d::brush * pbrush, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdibSrc);
      virtual bool process_blend(COLORREF clr, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdibSrc);
      virtual bool process_blend(::draw2d::dib * pdib, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdibSrc);
      virtual bool process_blend(::draw2d::pen * ppen, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdibSrc);


   };


} // namespace draw2d_gdi






