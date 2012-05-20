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
      
      bool Begin ( ::ca::graphics * pdc, int dxDest, int dyDest,
         LPBITMAPINFOHEADER lpbi, int dxSrc, int dySrc, UINT wFlags );
      bool End ();

      bool draw(::ca::graphics * pdc, int xDst, int yDst,
         int dxDst, int dyDst, LPBITMAPINFOHEADER lpbi, LPVOID lpBits,
         int xSrc, int ySrc, int dxSrc, int dySrc, UINT wFlags );
      bool draw(::ca::dib * pdib, ::ca::graphics * pdc, int xDst, int yDst,
         int dxDst, int dyDst, UINT wFlags );

      bool draw(::ca::graphics * pdc, int xDst, int yDst,
         int dxDst, int dyDst, 
          ::ca::dib * dib, int xSrc, int ySrc,
         int dxSrc, int dySrc, UINT wFlags );
      
      LPVOID GetBuffer ( LPBITMAPINFOHEADER lpbi,
         DWORD dwSize, DWORD dwFlags );
      
      bool ProfileDisplay ( LPBITMAPINFOHEADER lpbi );

      ::ca::palette * get_palette();
      bool set_palette ( ::ca::palette * ppal );
      bool ChangePalette ( ::ca::draw_dib * pdd, int iStart, int iLen,
         LPPALETTEENTRY lppe );
      UINT Realize ( ::ca::graphics * pdc, bool fBackground );

      bool Start ( LONG rate );
      bool Stop ();
      bool time ( LPDRAWDIBTIME lpddtime );

   };

} // namespace win