#pragma once

#include <vfw.h>

namespace win
{

   //////////////////////////////////////////////////////////////////////
   //
   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //
   //////////////////////////////////////////////////////////////////////

   class CLASS_DECL_win draw_dib :
      virtual public ::ca::draw_dib
   {
   public:
      HDRAWDIB m_hdrawdib;

      draw_dib(::ca::application * papp);
      virtual ~draw_dib ();

      virtual int_ptr get_os_data();

      bool open ();
      bool close ();
      
      bool begin ( ::ca::graphics * pdc, int32_t dxDest, int32_t dyDest,
         LPBITMAPINFOHEADER lpbi, int32_t dxSrc, int32_t dySrc, UINT wFlags );
      bool End ();

      bool draw(::ca::graphics * pdc, int32_t xDst, int32_t yDst,
         int32_t dxDst, int32_t dyDst, LPBITMAPINFOHEADER lpbi, LPVOID lpBits,
         int32_t xSrc, int32_t ySrc, int32_t dxSrc, int32_t dySrc, UINT wFlags );
      bool draw(::ca::dib * pdib, ::ca::graphics * pdc, int32_t xDst, int32_t yDst,
         int32_t dxDst, int32_t dyDst, UINT wFlags );

      bool draw(::ca::graphics * pdc, int32_t xDst, int32_t yDst,
         int32_t dxDst, int32_t dyDst, 
          ::ca::dib * dib, int32_t xSrc, int32_t ySrc,
         int32_t dxSrc, int32_t dySrc, UINT wFlags );
      
      LPVOID GetBuffer ( LPBITMAPINFOHEADER lpbi,
         DWORD dwSize, DWORD dwFlags );
      
      bool ProfileDisplay ( LPBITMAPINFOHEADER lpbi );

      ::ca::palette * get_palette();
      bool set_palette ( ::ca::palette * ppal );
      bool ChangePalette ( ::ca::draw_dib * pdd, int32_t iStart, int32_t iLen,
         LPPALETTEENTRY lppe );
      UINT Realize ( ::ca::graphics * pdc, bool fBackground );

      bool Start ( LONG rate );
      bool Stop ();
      bool time ( LPDRAWDIBTIME lpddtime );

   };

} // namespace win