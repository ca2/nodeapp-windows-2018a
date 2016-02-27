#include "framework.h"

#include "omp.h"

#include "aqua/aqua.h"


namespace draw2d_gdiplus
{


   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////



   dib::dib(::aura::application * papp) :
      object(papp),
      m_spbitmap(papp),
      m_spgraphics(allocer())
   {

      m_sizeWnd.cx         = 0;
      m_sizeWnd.cy         = 0;
      m_hbitmap            = NULL;

      ZERO(m_bitmapinfo);

   }

   //COLORREF * dib::get_data()
   //{
   //   return m_pcolorref;
   //}
   ::draw2d::bitmap_sp dib::get_bitmap() const
   {
      return m_spbitmap;
   }

   ::draw2d::bitmap_sp dib::detach_bitmap()
   {
      return m_spbitmap.detach();
   }



   void    dib::construct (int32_t cx,  int32_t cy)
   {
      m_pcolorref    = NULL;
      cx             = 0;
      cy             = 0;
      create(cx, cy);
   }

   dib::~dib ()
   {
      
      destroy ();

   }

   bool dib::create(class size size)
   {
      return create(size.cx, size.cy);
   }

   bool dib::create(int32_t width, int32_t height)
   {

      if(width <= 0 || height <= 0)
         return false;

      if(m_spbitmap.is_set()
      && m_spbitmap->get_os_data() != NULL 
      && width == m_size.cx
      && height == m_size.cy)
         return true;

      destroy();

      m_info ={};

      m_info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
      m_info.bmiHeader.biWidth         = width;
      m_info.bmiHeader.biHeight        =- height;
      m_info.bmiHeader.biPlanes        = 1;
      m_info.bmiHeader.biBitCount      = 32; 
      m_info.bmiHeader.biCompression   = BI_RGB;
      m_info.bmiHeader.biSizeImage     = width * height * 4;

      m_spbitmap.alloc(allocer());
      m_spgraphics.alloc(allocer());

      if(m_spbitmap.m_p == NULL)
      {

         m_size.cx       = 0;

         m_size.cy       = 0;

         m_iScan     = 0;

         return false;

      }
      
      if(!m_spbitmap->CreateDIBSection(NULL, &m_info, DIB_RGB_COLORS, (void **) &m_pcolorref, &m_iScan, NULL, 0))
      {

         m_size.cx      = 0;

         m_size.cy      = 0;

         m_iScan        = 0;

         return false;

      }

      if(m_spbitmap->get_os_data() == NULL)
      {
         
         destroy();

         return false;

      }

      m_spgraphics->SelectObject(m_spbitmap);

      m_spgraphics->m_pdib = this;

      m_size.cx       = width;

      m_size.cy       = height;

      m_iScan         = m_spbitmap->m_iStride;

      return true;

   }


   bool dib::dc_select(bool bSelect)
   {
/*      if(bSelect)
      {
         return m_spgraphics->SelectObject(m_spbitmap) != NULL;
      }
      else
      {
         return m_spgraphics->SelectObject(m_hbitmapOriginal) != NULL;
      }*/

      return true;

   }


   bool dib::create(::draw2d::graphics * pgraphics)
   {

      ::draw2d::bitmap * pbitmap = (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_current_bitmap();

      if(pbitmap == NULL)
         return FALSE;

      if (!create(pbitmap->get_size()))
      {
         return FALSE;
      }

      from(pgraphics);

      return TRUE;

   }

   
   bool dib::destroy ()
   {
      
      m_spbitmap.release();

      m_spgraphics.release();
 
      m_size         = {};
      m_iScan        = 0;
      m_pcolorref    = NULL;
      
      return true;

   }


   bool dib::to(::draw2d::graphics * pgraphics, point pt, class size size, point ptSrc)
   {

      return pgraphics->BitBlt(pt, size, get_graphics(), ptSrc);

    /*  return SetDIBitsToDevice(
         (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1(), 
         pt.x, pt.y, 
         size.cx, size.cy, 
         ptSrc.x, ptSrc.y, ptSrc.y, cy - ptSrc.y, 
         m_pcolorref, &m_info, 0)
            != FALSE; */

   }


   bool dib::from(::draw2d::graphics * pgraphics)
   {

      ::draw2d::bitmap_sp bitmap(get_app());

      bitmap->CreateCompatibleBitmap(pgraphics, 1, 1);

      ::draw2d::bitmap * pbitmap = GDIPLUS_GRAPHICS(pgraphics)->SelectObject(bitmap);

      if (pbitmap == NULL)
      {

         return false;

      }

      class size size = pbitmap->get_size();

      if(!create(size))
      {

         GDIPLUS_GRAPHICS(pgraphics)->SelectObject(pbitmap);

         return false;

      }

      bool bOk = GetDIBits(GDIPLUS_HDC(pgraphics), (HBITMAP) pbitmap->get_os_data(), 0, m_size.cy, m_pcolorref, &(m_info), DIB_RGB_COLORS) != FALSE;

      GDIPLUS_GRAPHICS(pgraphics)->SelectObject(pbitmap);

      return bOk;

   }


   bool dib::from(point ptDest, ::draw2d::graphics * pdc, point pt, class size sz)
   {

      if (m_spgraphics.is_null())
         return false;

      return m_spgraphics->BitBlt(ptDest, sz, pdc, pt, SRCCOPY) != FALSE;

   }


   /*void dib::Fill ( int32_t R, int32_t G, int32_t B )
   {
      COLORREF color=RGB ( B, G, R );
      int64_t size = area();

      COLORREF * pcr;

      int64_t iSize32 = size / 32;
      int32_t i;
      for (i=0; i < iSize32; i+=32 )
      {
         pcr = &m_pcolorref[i];
         pcr[0] = color;
         pcr[1] = color;
         pcr[2] = color;
         pcr[3] = color;
         pcr[4] = color;
         pcr[5] = color;
         pcr[6] = color;
         pcr[7] = color;
         pcr[8] = color;
         pcr[9] = color;
         pcr[10] = color;
         pcr[11] = color;
         pcr[12] = color;
         pcr[13] = color;
         pcr[14] = color;
         pcr[15] = color;
         pcr[16] = color;
         pcr[17] = color;
         pcr[18] = color;
         pcr[19] = color;
         pcr[20] = color;
         pcr[21] = color;
         pcr[22] = color;
         pcr[23] = color;
         pcr[24] = color;
         pcr[25] = color;
         pcr[26] = color;
         pcr[27] = color;
         pcr[28] = color;
         pcr[29] = color;
         pcr[30] = color;
         pcr[31] = color;
      }

      for (i=0; i<size; i++ )
      {
         m_pcolorref[i]=color;
      }
   }

   void dib::set_rgb(int32_t R, int32_t G, int32_t B)
   {
      int64_t size = area();

      BYTE * pbyte = (BYTE *) m_pcolorref;

      int32_t i;
      for (i=0; i<size; i++ )
      {
         *pbyte++ = (BYTE) R;
         *pbyte++ = (BYTE) G;
         *pbyte++ = (BYTE) B;
         pbyte++;
      }
   }

   void dib::ToAlpha(int32_t i)
   {
      BYTE *dst=(BYTE*)m_pcolorref;
      int64_t size = area();

      while ( size-- )
      {
         dst[3] = dst[i];
         dst+=4;
      }
   }

   void dib::from_alpha()
   {
      BYTE *dst=(BYTE*)m_pcolorref;
      int64_t size = area();

      while ( size-- )
      {
         dst[0] = dst[3];
         dst[1] = dst[3];
         dst[2] = dst[3];
         dst+=4;
      }
   }

   //DIB = DIB * SRC_ALPHA

   void dib::mult_alpha(::draw2d::dib * pdibWork, bool bPreserveAlpha)
   {
      ::draw2d::dib::mult_alpha(pdibWork, bPreserveAlpha);
      return ;
      /*
      if(area() <= 0)
         return;

      //return ::draw2d::dib::mult_alpha(NULL, true);
      ::draw2d::dib_sp dibWork;

      if(pdibWork == NULL)
      {
         dibWork.alloc(allocer());
         pdibWork = dibWork;
      }

      if(pdibWork->create(width(), height()))
         return;

      pdibWork->FillByte(0);

      pdibWork->channel_from(visual::rgba::channel_alpha, this);

      pdibWork->channel_invert(visual::rgba::channel_alpha);


      BLENDFUNCTION bf;

      bf.BlendOp = AC_SRC_OVER;
      bf.BlendFlags = 0;
      bf.SourceConstantAlpha = 255;
      bf.AlphaFormat = AC_SRC_ALPHA;

      get_graphics()->alpha_blend(size(), pdibWork->get_graphics(), bf);

      if(bPreserveAlpha)
      {

         pdibWork->channel_invert(visual::rgba::channel_alpha);

         channel_from(visual::rgba::channel_alpha, pdibWork);

      }

      */

   //}

   //void dib::Map(int32_t ToRgb, int32_t FromRgb)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();

   //   while ( size-- )
   //   {
   //      *dst = (byte) (*dst == FromRgb ? ToRgb : *dst);
   //      dst+=4;
   //   }
   //}


   //void dib::ToAlphaAndFill(int32_t i, COLORREF cr)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();

   //   BYTE uchB = rgba_get_b(cr);
   //   BYTE uchG = rgba_get_g(cr);
   //   BYTE uchR = rgba_get_r(cr);

   //   while ( size-- )
   //   {
   //      dst[3] = dst[i];
   //      dst[0] = uchB;
   //      dst[1] = uchG;
   //      dst[2] = uchR;
   //      dst+=4;
   //   }
   //}

   //void dib::GrayToARGB(COLORREF cr)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();

   //   DWORD dwB = rgba_get_b(cr);
   //   DWORD dwG = rgba_get_g(cr);
   //   DWORD dwR = rgba_get_r(cr);

   //   while (size-- > 0)
   //   {
   //      dst[3] = dst[0];
   //      dst[0] = (BYTE)(((dwB * dst[3]) / 256) & 0xff);
   //      dst[1] = (BYTE)(((dwG * dst[3]) / 256) & 0xff);
   //      dst[2] = (BYTE)(((dwR * dst[3]) / 256) & 0xff);
   //      dst+=4;
   //   }
   //}


   //void dib::BitBlt(::draw2d::dib *pdib, int32_t op)
   //{
   //   if(op == 123) // zero dest RGB, invert alpha, and OR src RGB
   //   {
   //      int64_t isize = area();
   //      LPDWORD lpbitsSrc= (LPDWORD) pdib->m_pcolorref;
   //      LPDWORD lpbitsDest= (LPDWORD) m_pcolorref;

   //      COLORREF _colorref = RGB ( 0, 0, 0 ) | (255 << 24);
   //      COLORREF colorrefa[2];
   //      colorrefa[0] = _colorref;
   //      colorrefa[1] = _colorref;

   //      COLORREF _colorrefN = RGB ( 255, 255, 255) | (0 << 24);
   //      COLORREF colorrefaN[2];
   //      colorrefaN[0] = _colorrefN;
   //      colorrefaN[1] = _colorrefN;
   //      throw todo(get_app());
   //   }

   //}


   //void dib::Invert()
   //{
   //   int64_t size = area();
   //   LPBYTE lpb = (LPBYTE) m_pcolorref;
   //   for ( int32_t i=0; i<size; i++ )
   //   {
   //      lpb[0] = 255 - lpb[0];
   //      lpb[1] = 255 - lpb[1];
   //      lpb[2] = 255 - lpb[2];
   //      lpb += 4;
   //   }
   //}

   //void dib::channel_invert(visual::rgba::echannel echannel)
   //{
   //   int64_t size   = area();
   //   register int64_t size64 = size / 64;
   //   LPBYTE lpb = (LPBYTE) m_pcolorref;
   //   lpb += ((int32_t)echannel) % 4;
   //   register int64_t i = 0;
   //   for(; i < size64; i++)
   //   {
   //      lpb[4 *  0] = 255 - lpb[4 *  0];
   //      lpb[4 *  1] = 255 - lpb[4 *  1];
   //      lpb[4 *  2] = 255 - lpb[4 *  2];
   //      lpb[4 *  3] = 255 - lpb[4 *  3];
   //      lpb[4 *  4] = 255 - lpb[4 *  4];
   //      lpb[4 *  5] = 255 - lpb[4 *  5];
   //      lpb[4 *  6] = 255 - lpb[4 *  6];
   //      lpb[4 *  7] = 255 - lpb[4 *  7];
   //      lpb[4 *  8] = 255 - lpb[4 *  8];
   //      lpb[4 *  9] = 255 - lpb[4 *  9];
   //      lpb[4 * 10] = 255 - lpb[4 * 10];
   //      lpb[4 * 11] = 255 - lpb[4 * 11];
   //      lpb[4 * 12] = 255 - lpb[4 * 12];
   //      lpb[4 * 13] = 255 - lpb[4 * 13];
   //      lpb[4 * 14] = 255 - lpb[4 * 14];
   //      lpb[4 * 15] = 255 - lpb[4 * 15];
   //      lpb[4 * 16] = 255 - lpb[4 * 16];
   //      lpb[4 * 17] = 255 - lpb[4 * 17];
   //      lpb[4 * 18] = 255 - lpb[4 * 18];
   //      lpb[4 * 19] = 255 - lpb[4 * 19];
   //      lpb[4 * 20] = 255 - lpb[4 * 20];
   //      lpb[4 * 21] = 255 - lpb[4 * 21];
   //      lpb[4 * 22] = 255 - lpb[4 * 22];
   //      lpb[4 * 23] = 255 - lpb[4 * 23];
   //      lpb[4 * 24] = 255 - lpb[4 * 24];
   //      lpb[4 * 25] = 255 - lpb[4 * 25];
   //      lpb[4 * 26] = 255 - lpb[4 * 26];
   //      lpb[4 * 27] = 255 - lpb[4 * 27];
   //      lpb[4 * 28] = 255 - lpb[4 * 28];
   //      lpb[4 * 29] = 255 - lpb[4 * 29];
   //      lpb[4 * 30] = 255 - lpb[4 * 30];
   //      lpb[4 * 31] = 255 - lpb[4 * 31];

   //      lpb[4 * 32] = 255 - lpb[4 * 32];
   //      lpb[4 * 33] = 255 - lpb[4 * 33];
   //      lpb[4 * 34] = 255 - lpb[4 * 34];
   //      lpb[4 * 35] = 255 - lpb[4 * 35];
   //      lpb[4 * 36] = 255 - lpb[4 * 36];
   //      lpb[4 * 37] = 255 - lpb[4 * 37];
   //      lpb[4 * 38] = 255 - lpb[4 * 38];
   //      lpb[4 * 39] = 255 - lpb[4 * 39];
   //      lpb[4 * 40] = 255 - lpb[4 * 40];
   //      lpb[4 * 41] = 255 - lpb[4 * 41];
   //      lpb[4 * 42] = 255 - lpb[4 * 42];
   //      lpb[4 * 43] = 255 - lpb[4 * 43];
   //      lpb[4 * 44] = 255 - lpb[4 * 44];
   //      lpb[4 * 45] = 255 - lpb[4 * 45];
   //      lpb[4 * 46] = 255 - lpb[4 * 46];
   //      lpb[4 * 47] = 255 - lpb[4 * 47];
   //      lpb[4 * 48] = 255 - lpb[4 * 48];
   //      lpb[4 * 49] = 255 - lpb[4 * 49];
   //      lpb[4 * 50] = 255 - lpb[4 * 50];
   //      lpb[4 * 51] = 255 - lpb[4 * 51];
   //      lpb[4 * 52] = 255 - lpb[4 * 52];
   //      lpb[4 * 53] = 255 - lpb[4 * 53];
   //      lpb[4 * 54] = 255 - lpb[4 * 54];
   //      lpb[4 * 55] = 255 - lpb[4 * 55];
   //      lpb[4 * 56] = 255 - lpb[4 * 56];
   //      lpb[4 * 57] = 255 - lpb[4 * 57];
   //      lpb[4 * 58] = 255 - lpb[4 * 58];
   //      lpb[4 * 59] = 255 - lpb[4 * 59];
   //      lpb[4 * 60] = 255 - lpb[4 * 60];
   //      lpb[4 * 61] = 255 - lpb[4 * 61];
   //      lpb[4 * 62] = 255 - lpb[4 * 62];
   //      lpb[4 * 63] = 255 - lpb[4 * 63];

   //      lpb += 4 * 64;
   //   }
   //   i *= 64;
   //   for(; i < size; i++ )
   //   {
   //      *lpb = 255 - *lpb;
   //      lpb += 4;
   //   }
   //}
   //void dib::channel_multiply(visual::rgba::echannel echannel, double dRate)
   //{
   //   if(dRate < 0)
   //      return;
   //   register int64_t size = area();
   //   LPBYTE lpb = (LPBYTE) get_data();
   //   lpb += ((int32_t)echannel) % 4;
   //   register int32_t iDiv = 256 * 256;
   //   register int32_t iMul = (int32_t) (dRate * ((double) iDiv));
   //   register int32_t iRes;
   //   for(register int64_t i = 0; i < size; i++)
   //   {
   //      iRes = *lpb * iMul / iDiv; 
   //      *lpb = (byte) (iRes > 255 ? 255 : iRes);
   //      lpb += 4;
   //   }
   //}

   //void dib::FillGlass ( int32_t R, int32_t G, int32_t B, int32_t A )
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);   
   //      dst+=4;
   //   }
   //}

   //void dib::FillStippledGlass ( int32_t R, int32_t G, int32_t B )
   //{   
   //   COLORREF color=RGB ( B, G, R );
   //   int32_t w=m_size.cx;
   //   int32_t h=m_size.cy;

   //   for ( int32_t j=0; j<w; j++ )
   //   {
   //      for ( int32_t i=0; i<h; i++ )
   //      {
   //         m_pcolorref[j*w+i]=((i+j)&0x1) ? m_pcolorref[j*w+i] : color;
   //      }
   //   }
   //}

   //void dib::copy(::draw2d::dib * pdib)
   //{
   //   // If DibSize Wrong Re-create dib
   //   if (pdib->m_size != m_size)
   //      pdib->create (m_size);
   //   // do copy
   //   memcpy ( pdib->m_pcolorref, m_pcolorref, (size_t) area() * sizeof(COLORREF) );
   //}


   //void dib::Paste ( ::draw2d::dib * pdib )
   //{
   //   // If DibSize Wrong Re-create dib
   //   if (m_size !=pdib->m_size )
   //      create ( pdib->m_size );
   //   // do Paste
   //   memcpy ( m_pcolorref, pdib->m_pcolorref, (size_t) area() * sizeof(COLORREF) );
   //}

   //bool dib::color_blend(COLORREF cr, BYTE bAlpha)
   //{

   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();

   //   DWORD dwB = rgba_get_b(cr);
   //   DWORD dwG = rgba_get_g(cr);
   //   DWORD dwR = rgba_get_r(cr);
   // 
   //   DWORD dwB_ = dwB << 8;
   //   DWORD dwG_ = dwG << 8;
   //   DWORD dwR_ = dwR << 8;
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((dst[0]-dwB)*bAlpha+dwB_)>>8);
   //      dst[1]=(BYTE)(((dst[1]-dwG)*bAlpha+dwG_)>>8);
   //      dst[2]=(BYTE)(((dst[2]-dwG)*bAlpha+dwR_)>>8);   
   //      dst+=4;
   //   }
   //   return true;
   //}


   //void dib::Blend (::draw2d::dib * pdib, int32_t A )
   //{
   //   if(size() != pdib->size())
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);   
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //bool dib::Blend(::draw2d::dib *pdib, ::draw2d::dib *pdibA, int32_t A)
   //{
   //   if(size() != pdib->size() ||
   //      size() != pdibA->size())
   //      return false;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   BYTE *alf=(BYTE*)pdibA->m_pcolorref;
   //   int64_t size = area();

   //   A = 2 - A;
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0]-dst[0])*alf[A]+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((src[1]-dst[1])*alf[A]+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((src[2]-dst[2])*alf[A]+(dst[2]<<8))>>8);   
   //      dst+=4;
   //      src+=4;
   //      alf+=4;
   //   }

   //   return true;
   //}

   //void dib::Darken (::draw2d::dib * pdib )
   //{
   //   if(size() != pdib->size())
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
   //      dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
   //      dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);   
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void dib::Difference (::draw2d::dib * pdib )
   //{
   //   if(size() != pdib->size())
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();
   //      
   //   while ( size-- )
   //   {
   //      int32_t Difference;
   //      Difference=src[0]-dst[0];
   //      dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      Difference=src[1]-dst[1];
   //      dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      Difference=src[2]-dst[2];
   //      dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);   
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void dib::Lighten (::draw2d::dib * pdib )
   //{
   //   if(size() != pdib->size())
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
   //      dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
   //      dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);   
   //      dst+=4;
   //      src+=4;
   //   }
   //}


   //void dib::Multiply (::draw2d::dib * pdib )
   //{
   //   if(size() != pdib->size())
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
   //      dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
   //      dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);   
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void dib::Screen (::draw2d::dib * pdib )
   //{
   //   if(size() != pdib->size())
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int64_t size = area();
   //      
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
   //      dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
   //      dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));   
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   ////////////////////////////////////////////////////////////////////////
   //// Rectangle Functions
   ////////////////////////////////////////////////////////////////////////

   //void dib::copy (::draw2d::dib * pdib, int32_t x, int32_t y )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->m_size.cx)<m_size.cx) ? pdib->m_size.cx : m_size.cx-x;
   //   int32_t dy=((y+pdib->m_size.cy)<m_size.cy) ? pdib->m_size.cy : m_size.cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to copy return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;
   //   // If DibSize Wrong Re-create dib
   //   if ( (dx!=pdib->m_size.cx) || (dy!=pdib->m_size.cy) )
   //      pdib->create ( dx, dy );

   //   // Prepare buffer Addresses
   //   COLORREF *src=m_pcolorref+(py*m_size.cx)+px;
   //   COLORREF *dst=pdib->m_pcolorref;

   //   // Do copy
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=m_size.cx;
   //      dst+=pdib->m_size.cx;
   //   }
   //}

   //void dib::PasteRect (::draw2d::dib * pdib, int32_t x, int32_t y )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->m_size.cx)<m_size.cx) ? pdib->m_size.cx : m_size.cx-x;
   //   int32_t dy=((y+pdib->m_size.cy)<m_size.cy) ? pdib->m_size.cy : m_size.cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Paste return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   COLORREF *src=pdib->m_pcolorref+((py-y)*pdib->m_size.cx)+px-x;
   //   COLORREF *dst=m_pcolorref+(py*m_size.cx)+px;

   //   // Do Paste
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=pdib->m_size.cx;
   //      dst+=m_size.cx;
   //   }
   //}

   //void dib::FillRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+w)<m_size.cx) ? w : m_size.cx-x;
   //   int32_t dy=((y+h)<m_size.cy) ? h : m_size.cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Fill return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*m_size.cx)+px;
   //   COLORREF color=RGB ( B, G, R );

   //   // Do Fill
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[i]=color;   
   //      }
   //      dst+=m_size.cx;
   //   }
   //}

   //void dib::FillGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B, int32_t A )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+w)<m_size.cx) ? w : m_size.cx-x;
   //   int32_t dy=((y+h)<m_size.cy) ? h : m_size.cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*m_size.cx)+px)*4;

   //   // Do FillGlass
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);   
   //         dst+=4;
   //      }
   //      dst+=(m_size.cx-dx)<<2;
   //   }
   //}

   //void dib::FillStippledGlassRect ( int32_t x, int32_t y, int32_t w, int32_t h, int32_t R, int32_t G, int32_t B )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+w)<m_size.cx) ? w : m_size.cx-x;
   //   int32_t dy=((y+h)<m_size.cy) ? h : m_size.cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillStippledGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*m_size.cx)+px;
   //   COLORREF color=RGB ( B, G, R );

   //   // Do FillStippledGlass
   //   for ( int32_t j=0; j<dy; j++ )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[i]=((i+j)&0x1) ? dst[i] : color;   
   //      }
   //      dst+=m_size.cx;
   //   }
   //}

   //void dib::BlendRect (::draw2d::dib * pdib, int32_t x, int32_t y, int32_t A )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->m_size.cx)<m_size.cx) ? pdib->m_size.cx : m_size.cx-x;
   //   int32_t dy=((y+pdib->m_size.cy)<m_size.cy) ? pdib->m_size.cy : m_size.cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Blend return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->m_size.cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*m_size.cx)+px)*4;

   //   // Do Blend
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);   
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(m_size.cx-dx)<<2;
   //      src+=(pdib->m_size.cx-dx)<<2;
   //   }
   //}

   //void dib::DarkenRect (::draw2d::dib * pdib, int32_t x, int32_t y )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int32_t dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Darken return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Darken
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);   
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::DifferenceRect (::draw2d::dib * pdib, int32_t x, int32_t y )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int32_t dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Difference return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Difference
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         int32_t Difference;
   //         Difference=src[0]-dst[0];
   //         dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[1]-dst[1];
   //         dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[2]-dst[2];
   //         dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);   
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::LightenRect (::draw2d::dib * pdib, int32_t x, int32_t y )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int32_t dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Lighten return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Lighten
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::MultiplyRect (::draw2d::dib * pdib, int32_t x, int32_t y )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int32_t dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Multiply return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Multiply
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
   //         dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
   //         dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::ScreenRect (::draw2d::dib * pdib, int32_t x, int32_t y )
   //{
   //   // Clip Rect
   //   int32_t px=(x>=0) ? x : 0;
   //   int32_t py=(y>=0) ? y : 0;
   //   int32_t dx=((x+pdib->cx)<cx) ? pdib->cx : cx-x;
   //   int32_t dy=((y+pdib->cy)<cy) ? pdib->cy : cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Screen return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*cx)+px)*4;

   //   // Do Screen
   //   while ( dy-- )
   //   {
   //      for ( int32_t i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
   //         dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
   //         dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //////////////////////////////////////////////////////////////////////
   // Line Functions
   //////////////////////////////////////////////////////////////////////

   /*void dib::Line ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B )
   {
      int32_t dx, dy, k1, k2, d, x, y;
      COLORREF color=RGB ( B, G, R );

      dx=x2-x1;
      dy=y2-y1;
      d=(dy<<1)-dx;
      k1=dy<<1;
      k2=(dy-dx)<<1;
      x=x1;
      y=y1;

      m_pcolorref[y*cx+x]=color;
      while (x<dx) 
      {
         if (d<=0) 
         {
            d+=k1;
            x++;
         } 
         else 
         {
            d+=k2;
            x++;
            y++;
         }
         m_pcolorref[y*cx+x]=color;
      }
   }*/

//   void dib::Line ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B )
//   {
//      int32_t d, x, y, ax, ay, sx, sy, dx, dy;
//      COLORREF color=RGB ( B, G, R );
//      
//      dx=x2-x1;
//      ax=abs ( dx )<<1;
//      sx=(dx<0) ? -1 : 1;
//      dy=y2-y1;
//      ay=abs ( dy )<<1;
//      sy=(dy<0) ? -1 : 1;
//      x=x1;
//      y=y1;
//      
//      if ( ax>ay )
//      {
//         d=ay-(ax>>1);
//         while ( x!=x2 )
//         {
//            m_pcolorref[y*cx+x]=color;
//            if ( d>=0 )
//            {
//               y+=sy;
//               d-=ax;
//            }
//            x+=sx;
//            d+=ay;
//         }
//      }
//      else
//      {
//         d=ax-(ay>>1);
//         while ( y!=y2 )
//         {
//            m_pcolorref[y*cx+x]=color;
//            if ( d>=0 )
//            {
//               x+=sx;
//               d-=ay;
//            }
//            y+=sy;
//            d+=ax;
//         }
//      }
//   }
//
//   void dib::LineGlass ( int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t R, int32_t G, int32_t B, int32_t A )
//   {
//      int32_t d, x, y, ax, ay, sx, sy, dx, dy;
////      COLORREF color=RGB ( B, G, R );
//      BYTE *dst=(BYTE *)m_pcolorref;
//      
//      dx=x2-x1;
//      ax=abs ( dx )<<1;
//      sx=(dx<0) ? -1 : 1;
//      dy=y2-y1;
//      ay=abs ( dy )<<1;
//      sy=(dy<0) ? -1 : 1;
//      x=x1;
//      y=y1;
//      
//      if ( ax>ay )
//      {
//         d=ay-(ax>>1);
//         while ( x!=x2 )
//         {
//            dst[(y*cx+x)<<2]=(BYTE)(((B-dst[(y*cx+x)<<2])*A+(dst[(y*cx+x)<<2]<<8))>>8);
//            dst[((y*cx+x)<<2)+1]=(BYTE)(((G-dst[((y*cx+x)<<2)+1])*A+(dst[((y*cx+x)<<2)+1]<<8))>>8);
//            dst[((y*cx+x)<<2)+2]=(BYTE)(((R-dst[((y*cx+x)<<2)+2])*A+(dst[((y*cx+x)<<2)+2]<<8))>>8);
//            if ( d>=0 )
//            {
//               y+=sy;
//               d-=ax;
//            }
//            x+=sx;
//            d+=ay;
//         }
//      }
//      else
//      {
//         d=ax-(ay>>1);
//         while ( y!=y2 )
//         {
//            dst[(y*cx+x)<<2]=(BYTE)(((B-dst[(y*cx+x)<<2])*A+(dst[(y*cx+x)<<2]<<8))>>8);
//            dst[((y*cx+x)<<2)+1]=(BYTE)(((G-dst[((y*cx+x)<<2)+1])*A+(dst[((y*cx+x)<<2)+1]<<8))>>8);
//            dst[((y*cx+x)<<2)+2]=(BYTE)(((R-dst[((y*cx+x)<<2)+2])*A+(dst[((y*cx+x)<<2)+2]<<8))>>8);
//            if ( d>=0 )
//            {
//               x+=sx;
//               d-=ay;
//            }
//            y+=sy;
//            d+=ax;
//         }
//      }
//   }

   //void dib::Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask)
   //{
   //   COLORREF crFind = RGB(rgba_get_b(crMask), rgba_get_g(crMask), rgba_get_r(crMask));
   //   COLORREF crSet = RGB(rgba_get_b(crInMask), rgba_get_g(crInMask), rgba_get_r(crInMask));
   //   COLORREF crUnset  = RGB(rgba_get_b(crOutMask), rgba_get_g(crOutMask), rgba_get_r(crOutMask));

   //   int32_t size=cx*cy;

   //   for ( int32_t i=0; i<size; i++ )
   //      if(m_pcolorref[i]== crFind)
   //         m_pcolorref[i] = crSet;
   //      else
   //         m_pcolorref[i] = crUnset;

   //}

   //void dib::transparent_color(color color)
   //{
   //   COLORREF crFind = color.get_rgb();
   //   int64_t size = area();

   //   for ( int32_t i=0; i<size; i++ )
   //      if((m_pcolorref[i] & 0x00ffffff) == crFind)
   //         ((LPBYTE)&m_pcolorref[i])[3] = 255;
   //      else
   //         ((LPBYTE)&m_pcolorref[i])[3] = 0;
   //}

   //void dib::channel_mask(uchar uchFind, uchar uchSet, uchar uchUnset, visual::rgba::echannel echannel)
   //{
   //   int32_t size = cx * cy;
   //   uchar * puch = (uchar * ) m_pcolorref;
   //   puch += ((int32_t) echannel) % 4;

   //   for(int32_t i = 0; i < size; i++)
   //   {
   //      if(*puch == uchFind)
   //         *puch = uchSet;
   //      else
   //         *puch = uchUnset;
   //      puch += 4;
   //   }
   //}

   //uint32_t dib::GetPixel(int32_t x, int32_t y)
   //{
   //   DWORD dw = *(m_pcolorref + x + (cy - y - 1) * cx);
   //   return RGB(rgba_get_b(dw), rgba_get_g(dw), rgba_get_r(dw));
   //}

   // too slow for animation on AMD XP gen_hon.
   // TOP SUGGESTION:
   // The gradient can´t have more then 256 levels of the most bright color
   // (white). So creating a radial fill of radius 256 and then using fasting
   // stretching algorithms is much faster than calculating radial fill.
//   void dib::RadialFill(BYTE alpha, BYTE red, BYTE green, BYTE blue, int32_t xCenter, int32_t yCenter, int32_t iRadius)
//   {
//      if (iRadius == 0)
//         return;
//      /*if(version == 0)
//      {
//         
//         int32_t iR = iRadius - 1;
//
//         int32_t xL = xCenter - iR;
//         int32_t xU = xCenter + iR;
//         int32_t yL = yCenter - iR;
//         int32_t yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= cx) xU = cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= cy) yU = cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
//         DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
//         int32_t size=cx*cy;
//         double iLevel;
//
//         int32_t dx, dy;
//         int32_t dx0, dy0;
//         int32_t dx1, dy1;
//         int32_t dx2, dy2;
//         int32_t dx3, dy3;
//         int32_t dx4, dy4;
//         int32_t dx5, dy5;
//         int32_t dx6, dy6;
//         int32_t dx7, dy7;
//         int32_t dx8, dy8;
//         int32_t dx9, dy9;
//         int32_t dxA, dyA;
//         int32_t dxB, dyB;
//         int32_t dxC, dyC;
//         int32_t dxD, dyD;
//         int32_t dxE, dyE;
//         int32_t dxF, dyF;
//
//         unsigned long dr;
//         unsigned long dq;
//         unsigned long dr0, dq0;
//         unsigned long dr1, dq1;
//         unsigned long dr2, dq2;
//         unsigned long dr3, dq3;
//         unsigned long dr4, dq4;
//         unsigned long dr5, dq5;
//         unsigned long dr6, dq6;
//         unsigned long dr7, dq7;
//         unsigned long dr8, dq8;
//         unsigned long dr9, dq9;
//         unsigned long drA, dqA;
//         unsigned long drB, dqB;
//         unsigned long drC, dqC;
//         unsigned long drD, dqD;
//         unsigned long drE, dqE;
//         unsigned long drF, dqF;
//         int32_t x, y;
//
//         {
//            for(y = yL; y <= yU; y++)
//            {
//               for(x = xL; x <= xU; x++)
//               {
//                  dx = abs(x - xCenter);
//                  dy = abs(y - yCenter);
//                  isqrt((dx * dx) + (dy * dy), &dr, &dq);
//                  if(dr < iRadius)
//                  {
//                     iLevel = 1.0 - dr * 1.0 / iRadius;
//                     dst[0] = blue  * iLevel;
//                     dst[1] = green * iLevel;
//                     dst[2] = red   * iLevel;   
//                  }
//                  dst += 4;
//               }
//               dst += dwAdd;
//            }
//         }
//      }
//      else if(version == 1)*/
//      {
//
//         LPBYTE lpbAlloc = (LPBYTE) malloc((iRadius * iRadius) + 4);
//         LPBYTE lpb = (LPBYTE) (((int32_t) lpbAlloc + 3) & ~3);
//
//
//         int32_t x, y;
//         int32_t b;
//
////         int32_t r2 = iRadius * iRadius;
//
//         for(y = 0; y < iRadius; y++)
//         {
//            for(x = y; x < iRadius; x++)
//            {
//               b = (int32_t) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
//               if(b > 255)
//                  b = 0;
//               else
//                  b = 255 - b;
//
//               
//               lpb[x + y * iRadius] = (byte) b;
//               lpb[y + x * iRadius] = (byte) b;
//            }
//         }
//
//         
//         int32_t iR = iRadius - 1;
//
//         int32_t xL = xCenter - iR;
//         int32_t xU = xCenter + iR;
//         int32_t yL = yCenter - iR;
//         int32_t yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= cx) xU = cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= cy) yU = cy - 1;
//      
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
//         DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
////         int32_t size=cx*cy;
//      
//         int32_t dx, dy;
//
//         // Top Left
//
//         for(y = yL; y <= yU; y++)
//         {
//            for(x = xL; x <= xU; x++)
//            {
//               dx = abs(x - xCenter);
//               dy = abs(y - yCenter);
//               b = lpb[dx + dy * iRadius];
//               dst[0] = (byte) (blue     * b / 255);
//               dst[1] = (byte) (green    * b / 255);
//               dst[2] = (byte) (red      * b / 255);   
//               dst[3] = (byte) (alpha    * b / 255);   
//               dst += 4;
//            }
//            dst += dwAdd;
//         }
//
//         free(lpbAlloc);
//      }
//   }
//
//
//   void dib::RadialFill(
//      BYTE alpha1, BYTE red1, BYTE green1, BYTE blue1,
//      BYTE alpha2, BYTE red2, BYTE green2, BYTE blue2,
//      int32_t xCenter, int32_t yCenter, int32_t iRadius)
//   {
//      if (iRadius == 0)
//         return;
//      /*if(version == 0)
//      {
//         
//         int32_t iR = iRadius - 1;
//
//         int32_t xL = xCenter - iR;
//         int32_t xU = xCenter + iR;
//         int32_t yL = yCenter - iR;
//         int32_t yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= cx) xU = cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= cy) yU = cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
//         DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
//         int32_t size=cx*cy;
//         double iLevel;
//
//         int32_t dx, dy;
//         int32_t dx0, dy0;
//         int32_t dx1, dy1;
//         int32_t dx2, dy2;
//         int32_t dx3, dy3;
//         int32_t dx4, dy4;
//         int32_t dx5, dy5;
//         int32_t dx6, dy6;
//         int32_t dx7, dy7;
//         int32_t dx8, dy8;
//         int32_t dx9, dy9;
//         int32_t dxA, dyA;
//         int32_t dxB, dyB;
//         int32_t dxC, dyC;
//         int32_t dxD, dyD;
//         int32_t dxE, dyE;
//         int32_t dxF, dyF;
//
//         unsigned long dr;
//         unsigned long dq;
//         unsigned long dr0, dq0;
//         unsigned long dr1, dq1;
//         unsigned long dr2, dq2;
//         unsigned long dr3, dq3;
//         unsigned long dr4, dq4;
//         unsigned long dr5, dq5;
//         unsigned long dr6, dq6;
//         unsigned long dr7, dq7;
//         unsigned long dr8, dq8;
//         unsigned long dr9, dq9;
//         unsigned long drA, dqA;
//         unsigned long drB, dqB;
//         unsigned long drC, dqC;
//         unsigned long drD, dqD;
//         unsigned long drE, dqE;
//         unsigned long drF, dqF;
//         int32_t x, y;
//
//         {
//            for(y = yL; y <= yU; y++)
//            {
//               for(x = xL; x <= xU; x++)
//               {
//                  dx = abs(x - xCenter);
//                  dy = abs(y - yCenter);
//                  isqrt((dx * dx) + (dy * dy), &dr, &dq);
//                  if(dr < iRadius)
//                  {
//                     iLevel = 1.0 - dr * 1.0 / iRadius;
//                     dst[0] = blue  * iLevel;
//                     dst[1] = green * iLevel;
//                     dst[2] = red   * iLevel;   
//                  }
//                  dst += 4;
//               }
//               dst += dwAdd;
//            }
//         }
//      }
//      else if(version == 1)*/
//      {
//
//         LPBYTE lpbAlloc = (LPBYTE) malloc((iRadius * iRadius) + 4);
//         LPBYTE lpb = (LPBYTE) (((int_ptr) lpbAlloc + 3) & ~3);
//
//
//         int32_t x, y;
//         int32_t b;
//
////         int32_t r2 = iRadius * iRadius;
//
//         for(y = 0; y < iRadius; y++)
//         {
//            for(x = y; x < iRadius; x++)
//            {
//               b = (int32_t) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
//               if(b > 255)
//                  b = 0;
//               else
//                  b = ~b;
//
//               
//               lpb[x + y * iRadius] = (byte) b;
//               lpb[y + x * iRadius] = (byte) b;
//            }
//         }
//
//         
//         int32_t iR = iRadius - 1;
//
//         int32_t xL = xCenter - iR;
//         int32_t xU = xCenter + iR;
//         int32_t yL = yCenter - iR;
//         int32_t yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= cx) xU = cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= cy) yU = cy - 1;
//      
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * cx));
//         DWORD dwAdd = ((cx - 1 - xU) + xL) * 4;
////         int32_t size=cx*cy;
//      
//         int32_t dx, dy;
//
//         BYTE bComp;
//
//         // Top Left
//
//         for(y = yL; y <= yU; y++)
//         {
//            for(x = xL; x <= xU; x++)
//            {
//               dx = abs(x - xCenter);
//               dy = abs(y - yCenter);
//               b = lpb[dx + dy * iRadius];
//               bComp = (byte) ~b;
//               dst[0] = (byte) (((blue1  * b) + (blue2  * bComp)) / 255);
//               dst[1] = (byte) (((green1 * b) + (green2 * bComp)) / 255);
//               dst[2] = (byte) (((red1   * b) + (red2   * bComp)) / 255);
//               dst[3] = (byte) (((alpha1 * b) + (alpha2 * bComp)) / 255);
//               dst += 4;
//            }
//            dst += dwAdd;
//         }
//
//         free(lpbAlloc);
//      }
//   }

   void dib::SetIconMask(::visual::icon * picon, int32_t cx, int32_t cy)
   {
      create(cx, cy);

      if(cx <= 0 || cy <= 0)
         return;

    
    

      // White blend dib
      dib dib1(get_app());
      dib1.create(cx, cy);
      dib1.set(255, 255, 255);

      dib1.m_spgraphics->DrawIcon(
         0, 0,
         picon,
         cx, cy,
         0,
         NULL,
         DI_IMAGE | DI_MASK);
    
      // Black blend dib
      ::draw2d::dib_sp spdib2(allocer());
      spdib2->create(cx, cy);
      spdib2->Fill(0, 0, 0, 0);

      spdib2->get_graphics()->DrawIcon(
         0, 0,
         picon,
         cx, cy,
         0,
         NULL,
         DI_IMAGE | DI_MASK);

      // Mask dib
      dib dibM(get_app());
      dibM.create(cx, cy);

      dibM.m_spgraphics->DrawIcon(
         0, 0,
         picon,
         cx, cy,
         0,
         NULL,
         DI_MASK);
    
      BYTE * r1=(BYTE*)dib1.m_pcolorref;
      BYTE * r2=(BYTE*)spdib2->get_data();
      BYTE * srcM=(BYTE*)dibM.m_pcolorref;
      BYTE * dest=(BYTE*)m_pcolorref;
      int32_t iSize = cx*cy;
    
      BYTE b;
      BYTE bMax;
         while ( iSize-- > 0)
         {
            if(srcM[0] == 255)
            {
               bMax = 0;
            }
            else
            {
               bMax = 0;
               b =(BYTE)(r1[0]  - r2[0]);
               bMax = MAX(b, bMax);
               b =(BYTE)(r1[1]  - r2[1]);
               bMax = MAX(b, bMax);
               b =(BYTE)(r1[2]  - r2[2]); 
               bMax = MAX(b, bMax);
               bMax = 255 - bMax;
            }
            dest[0]  =  bMax;
            dest[1]  =  bMax;
            dest[2]  =  bMax;
            dest     += 4;
            srcM     += 4;
            r1       += 4;
            r2       += 4;
         }
      
    
    
   }

   //void dib::rotate(::draw2d::dib * pdib, double dAngle, double dScale)
   //{
   //  // ::draw2d::dib_sp spdib(allocer());
   ////   spdib->Paste(this);

   //   int32_t cx = this->cx;
   //   int32_t cy = this->cy;

   //   int32_t l = MAX(cx, cy);

   //   
   //   int32_t jmax = MIN(l, cy / 2);
   //   int32_t jmin = - jmax;
   //   int32_t imax = MIN(l, cx / 2);
   //   int32_t imin = - imax;


   //   int32_t joff = cy / 2;
   //   int32_t ioff = cx / 2;

   //   //int32_t iAngle = iStep % 360;
   //   //int32_t iAngle = iStep;
   //   //int32_t iAngle = 1;
   //   //int32_t k = 0;

   ///*     for ( int32_t j=jmin; j<jmax; j++ )
   //   {
   //      for ( int32_t i=imin; i<imax; i++ )
   //      {
   //         int32_t x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         x=int32_t(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //         y=int32_t(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            spdib->m_pcolorref[abs(y%cy)*cx+abs(x%cx)];
   //         //k++;
   //      }
   //      (j+joff)*cx+(i+ioff)
   //   }*/

   //   int32_t k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int32_t cx1 = cx - 1;
   //   int32_t cy1 = cy - 1;
   //     for ( int32_t j=jmin; j<jmax; j++ )
   //   {
   //      for ( int32_t i=imin; i<imax; i++ )
   //      {
   //         int32_t x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int32_t(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((int32_t(dSin * i + dCos * j) + joff) % cy);

   //         x = (int32_t) abs((dCos * i - dSin * j) + ioff);
   //         y = (int32_t) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }


   //         
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pdib->m_pcolorref[y * cx + x];
   //         k++;
   //      }
   //   }
   //}


   //void dib::Rotate034(::draw2d::dib * pdib, double dAngle, double dScale)
   //{
   //  
   //   int32_t cx = this->cx;
   //   int32_t cy = this->cy;

   //   int32_t l = MAX(cx, cy);

   //   
   //   int32_t jmax = MIN(l, cy / 2);
   //   int32_t jmin = - jmax;
   //   int32_t imax = MIN(l, cx / 2);
   //   int32_t imin = - imax;


   //   if((cy % 2) == 1)
   //      jmax++;

   //   if((cx % 2) == 1)
   //      imax++;
   //   
   //   int32_t joff = cy / 2;
   //   int32_t ioff = cx / 2;

   //   
   //   int32_t k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int32_t cx1 = cx - 1;
   //   int32_t cy1 = cy - 1;
   //     for ( int32_t j=jmin; j<jmax; j++ )
   //   {
   //      for ( int32_t i=imin; i<imax; i++ )
   //      {
   //         int32_t x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int32_t(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((int32_t(dSin * i + dCos * j) + joff) % cy);

   //         x = (int32_t) abs((dCos * i - dSin * j) + ioff);
   //         y = (int32_t) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }


   //         
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pdib->m_pcolorref[y * cx + x];
   //         k++;
   //      }
   //   }
   //}

   //void dib::rotate(
   //   ::draw2d::dib * pdib,
   //   const RECT & rect,
   //   double dAngle, 
   //   double dScale)
   //{
   //  // ::draw2d::dib_sp spdib(allocer());
   ////   spdib->Paste(this);



   //   rect rect(lpcrect);

   //   int32_t cx = rect.width();
   //   int32_t cy = rect.height();

   //   int32_t l = MAX(cx, cy);
   //   
   //   int32_t jmax = MIN(l, cy / 2);
   //   int32_t jmin = - jmax;
   //   int32_t imax = MIN(l, cx / 2);
   //   int32_t imin = - imax;


   //   int32_t joff = cy / 2 + rect.left;
   //   int32_t ioff = cx / 2 + rect.top;

   //   //int32_t iAngle = iStep % 360;
   //   //int32_t iAngle = iStep;
   //   //int32_t iAngle = 1;
   //   //int32_t k = 0;

   ///*     for ( int32_t j=jmin; j<jmax; j++ )
   //   {
   //      for ( int32_t i=imin; i<imax; i++ )
   //      {
   //         int32_t x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         x=int32_t(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //         y=int32_t(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            spdib->m_pcolorref[abs(y%cy)*cx+abs(x%cx)];
   //         //k++;
   //      }
   //      (j+joff)*cx+(i+ioff)
   //   }*/

   //   int32_t k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int32_t cx1 = cx - 1;
   //   int32_t cy1 = cy - 1;
   //     for ( int32_t j=jmin; j<jmax; j++ )
   //   {
   //      for ( int32_t i=imin; i<imax; i++ )
   //      {
   //         int32_t x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int32_t(dCos * i - dSin * j) + ioff) % cx);
   //         //y=abs((int32_t(dSin * i + dCos * j) + joff) % cy);

   //         x = (int32_t) abs((dCos * i - dSin * j) + ioff);
   //         y = (int32_t) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }


   //         
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pdib->m_pcolorref[y * cx + x];
   //         k++;
   //      }
   //   }
   //}

   ///*int32_t dib::cos(int32_t i, int32_t iAngle)
   //{
   //   return (int32_t) (((_int64) i * CosN[iAngle]) >> 32);
   //}

   //int32_t dib::sin(int32_t i, int32_t iAngle)
   //{
   //   return (int32_t) (((_int64) i * SinN[iAngle]) >> 32);
   //}*/




   //void dib::Fill (int32_t A, int32_t R, int32_t G, int32_t B )
   //{
   //   COLORREF color = RGB ( B, G, R ) | (A << 24);
   //   int32_t size=cx*cy;

   //   COLORREF * pcr;

   //   int64_t iSize32 = size / 32;
   //   int32_t i;
   //   for (i=0; i < iSize32; i+=32 )
   //   {
   //      pcr = &m_pcolorref[i];
   //      pcr[0] = color;
   //      pcr[1] = color;
   //      pcr[2] = color;
   //      pcr[3] = color;
   //      pcr[4] = color;
   //      pcr[5] = color;
   //      pcr[6] = color;
   //      pcr[7] = color;
   //      pcr[8] = color;
   //      pcr[9] = color;
   //      pcr[10] = color;
   //      pcr[11] = color;
   //      pcr[12] = color;
   //      pcr[13] = color;
   //      pcr[14] = color;
   //      pcr[15] = color;
   //      pcr[16] = color;
   //      pcr[17] = color;
   //      pcr[18] = color;
   //      pcr[19] = color;
   //      pcr[20] = color;
   //      pcr[21] = color;
   //      pcr[22] = color;
   //      pcr[23] = color;
   //      pcr[24] = color;
   //      pcr[25] = color;
   //      pcr[26] = color;
   //      pcr[27] = color;
   //      pcr[28] = color;
   //      pcr[29] = color;
   //      pcr[30] = color;
   //      pcr[31] = color;
   //   }

   //   for (i=0; i<size; i++ )
   //   {
   //      m_pcolorref[i]=color;
   //   }

   //}

   //COLORREF dib::GetAverageColor()
   //{
   //   double dR = 0.0;
   //   double dG = 0.0;
   //   double dB = 0.0;
   //   int32_t iRLine;
   //   int32_t iGLine;
   //   int32_t iBLine;
   //   double dDiv = cx * cy;
   //   if(dDiv > 0)
   //   {
   //      LPBYTE lpb = (LPBYTE) m_pcolorref;
   //      for (int32_t y = 0; y < cy; y++)
   //      {
   //         iRLine = 0;
   //         iGLine = 0;
   //         iBLine = 0;
   //         for (int32_t x = 0; x < cx; x++)
   //         {
   //            iRLine += lpb[2];
   //            iGLine += lpb[1];
   //            iBLine += lpb[0];
   //            lpb += 4;
   //         }
   //         dR += iRLine / dDiv;
   //         dG += iGLine / dDiv;
   //         dB += iBLine / dDiv;
   //      }
   //      int32_t iR = (int32_t) dR;
   //      int32_t iG = (int32_t) dG;
   //      int32_t iB = (int32_t) dB;
   //      return RGB(iR, iG, iB);
   //   }
   //   else
   //   {
   //      return 0;
   //   }
   //   
   //}


   //void dib::xor(::draw2d::dib * pdib)
   //{
   //   if(cx != pdib->cx
   //   || cy != pdib->cy)
   //   {
   //      return;
   //   }
   //   int32_t iCount = cx * cy;
   //   LPDWORD lpd1 = (LPDWORD) m_pcolorref;
   //   LPDWORD lpd2 = (LPDWORD) pdib->m_pcolorref;
   //   for(int32_t i = 0; i < iCount; i++)
   //   {
   //      *lpd1 = *lpd1 ^ *lpd2;
   //      lpd1++;
   //      lpd2++;
   //   }
   //}

   //void dib::create_frame(class size size, int32_t iFrameCount)
   //{
   //   int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
   //   int32_t iFrameWidth = size.cx / iSliceCount;
   //   int32_t iFrameHeight = size.cy / iSliceCount;
   //   create(iFrameWidth, iFrameHeight);
   //}

   //void dib::set_frame1(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   //{
   //   int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int32_t iFrameWidth = cx / iSliceCount;
   //   int32_t iFrameHeight = cy / iSliceCount;
   //   int32_t iX = iFrame % iSliceCount;
   //   int32_t iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int32_t y = 0; y < iFrameHeight; y++)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(int32_t x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine = *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void dib::set_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   //{
   //   if(lpdata == NULL)
   //      return;
   //   int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int32_t iFrameWidth = cx / iSliceCount;
   //   int32_t iFrameHeight = cy / iSliceCount;
   //   int32_t iX = iFrame % iSliceCount;
   //   int32_t iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int32_t y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(int32_t x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine = *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void dib::xor_dib_frame2(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   //{
   //   if(lpdata == NULL)
   //      return;
   //   int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int32_t iFrameWidth = cx / iSliceCount;
   //   int32_t iFrameHeight = cy / iSliceCount;
   //   int32_t iX = iFrame % iSliceCount;
   //   int32_t iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int32_t y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * cx];
   //      for(int32_t x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine ^= *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void dib::get_frame(void * lpdata, int32_t iFrame, int32_t iFrameCount)
   //{
   //   int32_t iSliceCount = (int32_t) sqrt((double) iFrameCount);
   //   int32_t iFrameWidth = cx / iSliceCount;
   //   int32_t iFrameHeight = cy / iSliceCount;
   //   int32_t iX = iFrame % iSliceCount;
   //   int32_t iY = iFrame / iSliceCount;
   //   COLORREF * lpSrc = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight *  cx];
   //   COLORREF * lpDest = (COLORREF *) lpdata;
   //   COLORREF * lpSrcLine;
   //   for(int32_t y = 0; y < iFrameHeight; y++)
   //   {
   //      lpSrcLine = &lpSrc[y * cx];
   //      for(int32_t x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDest = *lpSrcLine;
   //          lpDest++;
   //          lpSrcLine++;
   //      }
   //   }
   //}

   //bool dib::is_rgb_black()
   //{
   //   int32_t iSize = cx * cy;
   //   COLORREF * lp = m_pcolorref;
   //   for(int32_t i = 0; i < iSize; i++)
   //   {
   //      if((*lp & 0x00FFFFFF) != 0)
   //         return false;
   //      lp++;
   //   }
   //   return true;
   //}

   //void dib::DivideRGB(int32_t iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int32_t iCount = cx * cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   int32_t i = 0;
   //   int32_t iCount1 = iCount - iCount % 8;
   //   for(; i < iCount1; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;

   //      lp[4] /= (byte) iDivide;
   //      lp[5] /= (byte) iDivide;
   //      lp[6] /= (byte) iDivide;

   //      lp[8] /= (byte) iDivide;
   //      lp[9] /= (byte) iDivide;
   //      lp[10] /= (byte) iDivide;

   //      lp[12] /= (byte) iDivide;
   //      lp[13] /= (byte) iDivide;
   //      lp[14] /= (byte) iDivide;

   //      lp[16] /= (byte) iDivide;
   //      lp[17] /= (byte) iDivide;
   //      lp[28] /= (byte) iDivide;

   //      lp[20] /= (byte) iDivide;
   //      lp[21] /= (byte) iDivide;
   //      lp[22] /= (byte) iDivide;

   //      lp[24] /= (byte) iDivide;
   //      lp[25] /= (byte) iDivide;
   //      lp[26] /= (byte) iDivide;

   //      lp[28] /= (byte) iDivide;
   //      lp[29] /= (byte) iDivide;
   //      lp[30] /= (byte) iDivide;

   //      lp += 4 * 8;
   //   }
   //   for(; i < iCount; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}

   //void dib::DivideARGB(int32_t iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int32_t iCount = cx * cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   for(int32_t i = 0; i < iCount; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}

   //void dib::DivideA(int32_t iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int32_t iCount = cx * cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   for(int32_t i = 0; i < iCount; i++)
   //   {
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}


   void dib::stretch_dib(::draw2d::dib * pdib)
   {

      if (area() <= 0 || pdib->area() <= 0)
         return;

      Gdiplus::RectF rectDest(0, 0, (Gdiplus::REAL) m_size.cx, (Gdiplus::REAL) m_size.cy);

      Gdiplus::RectF rectSource(0, 0, (Gdiplus::REAL) pdib->m_size.cx, (Gdiplus::REAL) pdib->m_size.cy);

      unmap();
      pdib->unmap();

      m_spgraphics->set_alpha_mode(::draw2d::alpha_mode_set);

      ((Gdiplus::Graphics * ) m_spgraphics->get_os_data())->DrawImage(((Gdiplus::Bitmap *)pdib->get_bitmap()->get_os_data()), rectDest, rectSource, Gdiplus::UnitPixel);

   }


   ::draw2d::graphics * dib::get_graphics() const
   {
      ((dib *) this)->unmap();

 
      return m_spgraphics;

   }


  // double dib::pi()
  // {
  //    return dPi;
  // }

  // void dib::fill_channel(int32_t intensity, visual::rgba::echannel echannel)
  // {
  //     int32_t offset = ((int32_t)echannel) % 4;
  //    int32_t size=m_size.cx*cy;

  //    BYTE * pb;

  //    int32_t iSize32 = size;
  //    int32_t i;
  //    for (i=0; i < iSize32; i+=32 )
  //    {
  //       pb = ((BYTE * ) &m_pcolorref[i]) + offset;
  //       pb[0 * 4] = (byte) intensity;
  //       pb[1 * 4] = (byte) intensity;
  //       pb[2 * 4] = (byte) intensity;
  //       pb[3 * 4] = (byte) intensity;
  //       pb[4 * 4] = (byte) intensity;
  //       pb[5 * 4] = (byte) intensity;
  //       pb[6 * 4] = (byte) intensity;
  //       pb[7 * 4] = (byte) intensity;
  //       pb[8 * 4] = (byte) intensity;
  //       pb[9 * 4] = (byte) intensity;
  //       pb[10 * 4] = (byte) intensity;
  //       pb[11 * 4] = (byte) intensity;
  //       pb[12 * 4] = (byte) intensity;
  //       pb[13 * 4] = (byte) intensity;
  //       pb[14 * 4] = (byte) intensity;
  //       pb[15 * 4] = (byte) intensity;
  //       pb[16 * 4] = (byte) intensity;
  //       pb[17 * 4] = (byte) intensity;
  //       pb[18 * 4] = (byte) intensity;
  //       pb[19 * 4] = (byte) intensity;
  //       pb[20 * 4] = (byte) intensity;
  //       pb[21 * 4] = (byte) intensity;
  //       pb[22 * 4] = (byte) intensity;
  //       pb[23 * 4] = (byte) intensity;
  //       pb[24 * 4] = (byte) intensity;
  //       pb[25 * 4] = (byte) intensity;
  //       pb[26 * 4] = (byte) intensity;
  //       pb[27 * 4] = (byte) intensity;
  //       pb[28 * 4] = (byte) intensity;
  //       pb[29 * 4] = (byte) intensity;
  //       pb[30 * 4] = (byte) intensity;
  //       pb[31 * 4] = (byte) intensity;
  //    }

  //    for (; i<size; i++ )
  //    {
  //       *(((BYTE * ) &m_pcolorref[i]) + offset) = (byte) intensity;
  //    }
  //}


  // int32_t dib::cos(int32_t i, int32_t iAngle)
  // {
  //    return (int32_t) (((_int64) i * CosN[iAngle]) >> 31);
  // }

  // int32_t dib::sin(int32_t i, int32_t iAngle)
  // {
  //    return (int32_t) (((_int64) i * SinN[iAngle]) >> 31);
  // }

  // int32_t dib::cos10(int32_t i, int32_t iAngle)
  // {
  //    return (int32_t) (((_int64) i * Cos10N[iAngle]) >> 34);
  // }

  // int32_t dib::sin10(int32_t i, int32_t iAngle)
  // {
  //    return (int32_t) (((_int64) i * Sin10N[iAngle]) >> 34);
  // }

/*   int32_t dib::width()
   {
      return m_size.cx;
   }

   int32_t dib::height()
   {
      return cy;
   }
   */
#undef new

#define NO_SCREEN_PRE_MULTIPLY_ALPHA 0

   bool dib::update_window(::aura::draw_interface * pwnd,signal_details * pobj,bool bTransferBuffer)
   {


      rect64 rectWindow;

      pwnd->GetWindowRect(rectWindow);

#if !NO_SCREEN_PRE_MULTIPLY_ALPHA
      if(bTransferBuffer && pwnd->is_composite() && !m_bReduced)
      {

         m_bReduced = true;

         m_spgraphics->SetViewportOrg(0,0);

         map();

         //pre_multiply_alpha((unsigned int *) get_data(),m_size.cx,m_size.cy,m_size.cx * 4);

         BYTE *dstR=(BYTE*)get_data();
         BYTE *dstG=dstR + 1;
         BYTE *dstB=dstR + 2;
         BYTE *dstA=dstR + 3;
         int64_t size = area() * 4;


         // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255
//#if defined(_OPENMP)
//         #pragma omp parallel num_threads(3)
//                  {
//         
//                     BYTE *dst = dstR + omp_get_thread_num();
//      #pragma omp parallel for
//                     for(index i = 0; i < size; i+=4)
//                     {
//                           dst[i] = LOBYTE(((int32_t)dst[i] * (int32_t)dstA[i]) >> 8);
//                     }
//                  }
//
////#pragma omp parallel num_threads(4)
////         {
////
////            if(omp_get_thread_num() == 3)
////            {
////               COLORREF *dst = get_data();
////#pragma omp parallel for
////               for(index i = 0; i < size; i+=4)
////               {
////                  if(dstA[i] <= 3)
////                  {
////                     dst[i>>2] = 0;
////                  }
////               }
////            }
////            else
////            {
////               BYTE *dst = dstR + omp_get_thread_num();
////#pragma omp parallel for
////               for(index i = 0; i < size; i+=4)
////               {
////                  if(dstA[i] > 3)
////                  {
////                     dst[i] = LOBYTE(((int32_t)dst[i] * (int32_t)dstA[i]) >> 8);
////                  }
////               }
////            }
////         }
//#else
//         for(index i = 0; i < size; i+=4)
//         {
//            dstR[i] = LOBYTE(((int32_t)dstR[i] * (int32_t)dstA[i]) >> 8);
//            dstG[i] = LOBYTE(((int32_t)dstG[i] * (int32_t)dstA[i]) >> 8);
//            dstB[i] = LOBYTE(((int32_t)dstB[i] * (int32_t)dstA[i]) >> 8);
//         }
//#endif
//         
      }
#endif 
      rect rect(rectWindow);

      m_pauraapp->window_graphics_update_window(pwnd->get_window_graphics(),pwnd->get_handle(),m_pcolorref,rect,m_size.cx,m_size.cy,m_iScan,bTransferBuffer);

      return true;

   }


   bool dib::print_window(::aura::draw_interface * pwnd,signal_details * pobj)
   {

      SCAST_PTR(::message::base, pbase, pobj);

      if(pbase->m_wparam == NULL)
         return false;

      m_spgraphics->attach((HDC) pbase->m_wparam);

      rect rectx;

      ::draw2d::bitmap * pbitmap = m_spgraphics->get_current_bitmap();

      ::GetCurrentObject((HDC) pbase->m_wparam, OBJ_BITMAP);

      //      uint32_t dw = ::GetLastError();
      class size size = pbitmap->get_size();

      rectx.left = 0;
      rectx.top = 0;
      rectx.right = size.cx;
      rectx.bottom = size.cy;

      try
      {
         
         rect rectWindow;
         
         pwnd->GetWindowRect(rectWindow);

         ::draw2d::dib_sp dib(allocer());

         if(!dib->create(rectWindow.bottom_right()))
            return false;

         ::draw2d::graphics * pdc = dib->get_graphics();

         if(pdc->get_os_data() == NULL)
            return false;

         rect rectPaint;
         rect rectUpdate;
         rectUpdate = rectWindow;
         rectPaint = rectWindow;
         rectPaint.offset(-rectPaint.top_left());
         m_spgraphics->SelectClipRgn(NULL);
         pwnd->_001OnDeferPaintLayeredWindowBackground(dib);
         m_spgraphics->SelectClipRgn(NULL);
         m_spgraphics-> SetViewportOrg(point(0, 0));
         pwnd->_000OnDraw(dib);
         m_spgraphics->SetViewportOrg(point(0, 0));
         //(dynamic_cast<::win::graphics * >(pdc))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
         m_spgraphics->SelectClipRgn(NULL);
         m_spgraphics->SetViewportOrg(point(0, 0));

         m_spgraphics->SelectClipRgn( NULL);
         m_spgraphics->BitBlt(rectPaint.left, rectPaint.top, 
            rectPaint.width(), rectPaint.height(),
            pdc, rectUpdate.left, rectUpdate.top,
            SRCCOPY);

      }
      catch(...)
      {
      }
      m_spgraphics->FillSolidRect(rectx, RGB(255, 255, 255));
      pobj->m_bRet = true;
      pbase->set_lresult(0);

      return true;

   }



#define new AURA_NEW

   void dib::map(bool bApplyTransform) const
   {
      UNREFERENCED_PARAMETER(bApplyTransform);

      if (!m_bMapped)
      {

         ((dib*)this)->m_bMapped = true;

      }

//      if (m_spgraphics.is_null())
  //       return;

    //  m_spgraphics->flush();

   }


   void dib::unmap() const
   {
      if (m_bMapped)
      {
         if (m_spgraphics.is_set())
         {
            ((dib*)this)->m_pt = m_spgraphics->GetViewportOrg();

         }

         ((dib*)this)->m_bMapped = false;

      }
//      if (m_spgraphics.is_null())
  //       return;

    //  m_spgraphics->flush();

   }


} // namespace draw2d_gdiplus

