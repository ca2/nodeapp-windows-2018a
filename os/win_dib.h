#pragma once

namespace win
{

   //////////////////////////////////////////////////////////////////////
   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////

   class CLASS_DECL_win dib :
      virtual public ::ca::dib
   {
   public:

      static float Cosines[360];
      static float Sines[360];
      static int64_t CosN[360]; // * 1 << 31
      static int64_t SinN[360]; 
      static int64_t Cos10N[10]; // until 10 degress
      static int64_t Sin10N[10]; // more precision * 1 << 34
      static double dPi;

      COLORREF *           m_pcolorref;
      BITMAPINFO           m_info;
      ::ca::bitmap_sp      m_spbitmap;
      ::ca::graphics_sp    m_spgraphics;

      virtual ::ca::graphics * get_graphics();
      virtual ::ca::bitmap_sp get_bitmap();
      virtual ::ca::bitmap_sp detach_bitmap();


      virtual COLORREF * get_data();

      dib(::ca::applicationsp papp);
      virtual void construct(int32_t cx, int32_t cy);
      virtual ~dib();

      static void s_initialize();
      void stretch_dib(::ca::dib * pdib);

      bool dc_select(bool bSelect = true);

      COLORREF GetAverageColor();
      bool Blend(::ca::dib *pdib, ::ca::dib * pdibA, int32_t A);
      bool color_blend(COLORREF cr, BYTE bAlpha);
      void BitBlt(::ca::dib * pdib, int32_t op);
      int32_t cos(int32_t i, int32_t iAngle);
      int32_t sin(int32_t i, int32_t iAngle);
      int32_t cos10(int32_t i, int32_t iAngle);
      int32_t sin10(int32_t i, int32_t iAngle);

      bool is_rgb_black();
      void xor(::ca::dib * pdib);

      void ToAlpha(int32_t i);
      void ToAlphaAndFill(int32_t i, COLORREF cr);
      void GrayToARGB(COLORREF cr);

      void from_alpha();
      void mult_alpha(::ca::dib * pdibWork, bool bPreserveAlpha = true);
      void set_rgb(int32_t R, int32_t G, int32_t B);

      void rotate(::ca::dib * pdib, LPCRECT lpcrect, double dAngle, double dScale);
      void rotate(::ca::dib * pdib, double dAngle, double dScale);
      void Rotate034(::ca::dib * pdib, double dAngle, double dScale);


      void SetIconMask(::visual::icon * picon, int32_t cx, int32_t cy);
      void RadialFill(BYTE a, BYTE r, BYTE g, BYTE b, int32_t x, int32_t y, int32_t iRadius);
      void RadialFill(
         BYTE a1, BYTE r1, BYTE g1, BYTE b1, // center colors
         BYTE a2, BYTE r2, BYTE g2, BYTE b2, // border colors
         int32_t x, int32_t y, int32_t iRadius);

      uint32_t GetPixel(int32_t x, int32_t y);
      void Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask);
      void channel_mask(BYTE uchFind, BYTE uchSet, BYTE uchUnset, visual::rgba::echannel echannel);
      void transparent_color(color color);
      

      bool create(class size size);
      bool create(int32_t iWidth, int32_t iHeight);
      bool create(::ca::graphics * pdc);
      bool Destroy();


      void DivideRGB(int32_t iDivide);
      void DivideARGB(int32_t iDivide);
      void DivideA(int32_t iDivide);

      bool from(::ca::graphics * pdc);
      bool from(point ptDest, ::ca::graphics * pdc, point pt, class size sz);

      bool to(::ca::graphics * pgraphics, point pt, class size size, point ptSrc);
      
      virtual void fill_channel(int32_t C, visual::rgba::echannel echannel);
      void Fill (int32_t A, int32_t R, int32_t G, int32_t B );
      void Fill ( int32_t R, int32_t G, int32_t B );
      void FillGlass ( int32_t R, int32_t G, int32_t B, int32_t A );
      void FillStippledGlass ( int32_t R, int32_t G, int32_t B );
      void Invert();
      void channel_invert(visual::rgba::echannel echannel);
      void channel_multiply(visual::rgba::echannel echannel, double dRate);

      void Map (int32_t ToRgb, int32_t FromRgb );

      void copy( ::ca::dib *dib );
      void Paste ( ::ca::dib *dib );

      void Blend ( ::ca::dib *dib, int32_t A );
      void Darken ( ::ca::dib *dib );
      void Difference ( ::ca::dib *dib );
      void Lighten ( ::ca::dib *dib );
      void Multiply ( ::ca::dib *dib );
      void Screen ( ::ca::dib *dib );

      void copy ( ::ca::dib *dib, int32_t x, int32_t y );
      void PasteRect ( ::ca::dib *dib, int32_t x, int32_t y );

      void FillRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B );
      void FillGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B, int32_t A );
      void FillStippledGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B );
      
      void BlendRect ( ::ca::dib *dib, int32_t x, int32_t y, int32_t A );
      void DarkenRect ( ::ca::dib *dib, int32_t x, int32_t y );
      void DifferenceRect ( ::ca::dib *dib, int32_t x, int32_t y );
      void LightenRect ( ::ca::dib *dib, int32_t x, int32_t y );
      void MultiplyRect ( ::ca::dib *dib, int32_t x, int32_t y );
      void ScreenRect ( ::ca::dib *dib, int32_t x, int32_t y );

      void Line ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B );
      void LineGlass ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B, int32_t A );

      void create_frame(class size size, int32_t iFrameCount);
      void set_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount);
      void set_frame1(void * lpdata, int32_t iFrame, int32_t iFrameCount);
      void get_frame(void * lpdata, int32_t iFrame, int32_t iFrameCount);
      void xor_dib_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount);


      double pi();

      virtual bool from(::ca::graphics * pgraphics, FIBITMAP *pfibitmap, bool bUnloadFI);

   };

} // namespace win