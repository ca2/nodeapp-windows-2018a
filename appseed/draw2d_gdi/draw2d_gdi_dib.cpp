#include "framework.h"


inline byte byte_clip(int32_t i)
{
   if(i >= 255)
      return 255;
   else if(i < 0)
      return 0;
   else
      return (byte) i;
}

namespace draw2d_gdi
{


   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////




   dib::dib(::aura::application * papp) :
      object(papp),
      m_spbitmap(allocer()),
      m_spgraphics(allocer())
   {

      m_hbitmapOriginal    = NULL;
      m_bMapped            = false;

   }


   ::draw2d::bitmap_sp dib::get_bitmap() const
   {

      return m_spbitmap;

   }


   ::draw2d::bitmap_sp dib::detach_bitmap()
   {

      return m_spbitmap.detach();

   }



   void    dib::construct (int cx,  int cy)
   {
      //m_pcolorref    = NULL;
//      size()         = class size(0, 0);
      create(cx, cy);
   }

   dib::~dib ()
   {
      Destroy ();
   }

   bool dib::create(class size size)
   {
      return create(size.cx, size.cy);
   }

   bool dib::create(int width, int height)
   {

      synch_lock ml(&draw2d_gdi_mutex());


      if(m_spbitmap.is_set()
            && m_spbitmap->get_os_data() != NULL
            && m_spgraphics.is_set()
            && m_spgraphics->get_os_data() != NULL
            && width == this->m_size.cx
            && height == this->m_size.cy)
         return true;

      Destroy();

      if(width <= 0 || height <= 0)
         return FALSE;

      ZERO(m_info);

      m_info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
      m_info.bmiHeader.biWidth         = width;
      m_info.bmiHeader.biHeight        =- height;
      m_info.bmiHeader.biPlanes        = 1;
      m_info.bmiHeader.biBitCount      = 32;
      m_info.bmiHeader.biCompression   = BI_RGB;
      m_info.bmiHeader.biSizeImage     = width * height * 4;

      if(m_spbitmap.m_p == NULL)
      {
         this->m_size.cx = 0;
         this->m_size.cy = 0;
         return FALSE;
      }

      //if(!m_spbitmap->CreateDIBSection(NULL, &m_info, DIB_RGB_COLORS, (void **) &m_pcolorref, &m_iScan, NULL, 0))
      if (!m_spbitmap->CreateDIBSection(NULL, width, height, DIB_RGB_COLORS, (void **)&m_pcolorref, &m_iScan, NULL, 0))
      {
         this->m_size.cx = 0;
         this->m_size.cy = 0;
         return FALSE;
      }

      if(m_spbitmap->get_os_data() == NULL)
      {

         Destroy();

         return false;

      }

      m_spgraphics->CreateCompatibleDC(NULL);

      m_spgraphics->m_pdibDraw2dGraphics = dynamic_cast < ::draw2d::dib * > (this);

      m_hbitmapOriginal = (HBITMAP) m_spgraphics->get_os_data_ex(::draw2d_gdi::graphics::data_bitmap);

      ::draw2d::bitmap * pbitmap = m_spgraphics->SelectObject(m_spbitmap);

      if(pbitmap == NULL || pbitmap->get_os_data() == NULL)
      {

         Destroy();

         return false;

      }

      this->m_size.cx = width;

      this->m_size.cy = height;

      return true;

   }


   bool dib::dc_select(bool bSelect)
   {
      if(bSelect)
      {
         return m_spgraphics->SelectObject(m_spbitmap) != NULL;
      }
      else
      {
         return m_spgraphics.cast < graphics >()->SelectObject(m_hbitmapOriginal) != NULL;
      }
   }

   bool dib::create(::draw2d::graphics * pgraphics)
   {
      ::draw2d_gdi::bitmap * pbitmap = dynamic_cast<::draw2d_gdi::bitmap * >(dynamic_cast<::draw2d_gdi::graphics * >(pgraphics)->get_current_bitmap().m_p);
      if(pbitmap == NULL)
         return FALSE;
      BITMAP bm;
      pbitmap->get_object(sizeof(bm), &bm);
      if(!create(bm.bmWidth, bm.bmHeight))
      {
         return FALSE;
      }
      from(pgraphics);
      return TRUE;
   }

   bool dib::Destroy ()
   {

      synch_lock ml(&draw2d_gdi_mutex());

      m_spbitmap->destroy();


      if(m_spgraphics.is_set() && m_spgraphics->is_set())
      {
         GDI_GRAPHICS(m_spgraphics.m_p)->SelectObject(m_hbitmapOriginal);
         m_spgraphics->DeleteDC();
      }

      m_size.cx = 0;
      m_size.cy = 0;
      m_iScan = 0;
      m_pcolorref    = NULL;

      return TRUE;
   }

   bool dib::to(::draw2d::graphics * pgraphics, point pt, class size size, point ptSrc)
   {
      return SetDIBitsToDevice(
             (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1(),
             pt.x, pt.y,
             size.cx, size.cy,
             ptSrc.x, ptSrc.y, ptSrc.y, this->m_size.cy - ptSrc.y,
             m_pcolorref, &m_info, 0)
             != FALSE;
   }

   bool dib::from(::draw2d::graphics * pgraphics)
   {

      ::draw2d::bitmap_sp bitmap(get_app());

      bitmap->CreateCompatibleBitmap(pgraphics, 1, 1);

      ::draw2d_gdi::bitmap * pbitmap = dynamic_cast < ::draw2d_gdi::bitmap * > (pgraphics->SelectObject(bitmap));

      if(pbitmap == NULL)
         return false;

      BITMAP bm;

      memset(&bm, 0, sizeof(bm));

      if(!pbitmap->GetBitmap(&bm))
      {
         pgraphics->SelectObject(pbitmap);
         return false;
      }

      if(!create(bm.bmWidth, bm.bmHeight))
      {
         pgraphics->SelectObject(pbitmap);
         return false;
      }

      bool bOk = GetDIBits(GDI_HDC(pgraphics), (HBITMAP) pbitmap->get_os_data(), 0, this->m_size.cy, m_pcolorref, &(m_info), DIB_RGB_COLORS) != FALSE;

      pgraphics->SelectObject(pbitmap);

      return bOk;
   }

   bool dib::from(point ptDest, ::draw2d::graphics * pgraphics, point pt, class size sz)
   {
      return m_spgraphics->BitBlt(ptDest.x, ptDest.y, sz.cx, sz.cy, pgraphics, pt.x, pt.y, SRCCOPY) != FALSE;
   }

   //void dib::Fill ( int R, int G, int B )
   //{
   //   COLORREF color=RGB ( B, G, R );
   //   int size=this->cx*this->cy;

   //   COLORREF * pcr;

   //   int iSize32 = size / 32;
   //   int i;
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

   //void dib::set_rgb(int R, int G, int B)
   //{
   //   int size=this->cx*this->cy;

   //   BYTE * pbyte = (BYTE *) m_pcolorref;

   //   int i;
   //   for (i=0; i<size; i++ )
   //   {
   //      *pbyte++ = (BYTE) R;
   //      *pbyte++ = (BYTE) G;
   //      *pbyte++ = (BYTE) B;
   //      pbyte++;
   //   }
   //}

   //void dib::ToAlpha(int i)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;

   //   while ( size-- )
   //   {
   //      dst[3] = dst[i];
   //      dst+=4;
   //   }
   //}

   //void dib::from_alpha()
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   __int64 size = area();

   //   while ( size-- )
   //   {
   //      dst[0] = dst[3];
   //      dst[1] = dst[3];
   //      dst[2] = dst[3];
   //      dst+=4;
   //   }
   //}

   ////DIB = DIB * SRC_ALPHA

   //void dib::mult_alpha(::draw2d::dib * pdibWork, bool bPreserveAlpha)
   //{
   //   return ::draw2d::dib::mult_alpha(NULL, true);
   //   /*::draw2d::dib_sp dibWork;

   //   if(pdibWork == NULL)
   //   {
   //      dibWork.alloc(allocer());
   //      pdibWork = dibWork;
   //   }

   //   pdibWork->create(width(), height());

   //   pdibWork->Fill(0, 0, 0, 0);

   //   pdibWork->channel_from(visual::rgba::channel_alpha, this);

   //   pdibWork->channel_invert(visual::rgba::channel_alpha);


   //   BLENDFUNCTION bf;

   //   bf.BlendOp = AC_SRC_OVER;
   //   bf.BlendFlags = 0;
   //   bf.SourceConstantAlpha = 255;
   //   bf.AlphaFormat = AC_SRC_ALPHA;

   //   get_graphics()->alpha_blend(size(), pdibWork->get_graphics(), bf);

   //   if(bPreserveAlpha)
   //   {

   //      pdibWork->channel_invert(visual::rgba::channel_alpha);

   //      channel_from(visual::rgba::channel_alpha, pdibWork);

   //   }
   //   */
   //}

   //void dib::Map(int ToRgb, int FromRgb)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;

   //   while ( size-- )
   //   {
   //      *dst = (byte) (*dst == FromRgb ? ToRgb : *dst);
   //      dst+=4;
   //   }
   //}


   //void dib::ToAlphaAndFill(int i, COLORREF cr)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;

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
   //   int size=this->cx*this->cy;

   //   uint32_t dwB = rgba_get_b(cr);
   //   uint32_t dwG = rgba_get_g(cr);
   //   uint32_t dwR = rgba_get_r(cr);

   //   while (size-- > 0)
   //   {
   //      dst[3] = dst[0];
   //      dst[0] = (BYTE)(((dwB * dst[3]) / 256) & 0xff);
   //      dst[1] = (BYTE)(((dwG * dst[3]) / 256) & 0xff);
   //      dst[2] = (BYTE)(((dwR * dst[3]) / 256) & 0xff);
   //      dst+=4;
   //   }
   //}


   //void dib::BitBlt(::draw2d::dib *pdib, int op)
   //{
   //   if(op == 123) // zero dest RGB, invert alpha, and OR src RGB
   //   {
   //      int isize=this->cx*this->cy;
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
   //#ifdef _AMD64_

   //      //x64
   //#else
   //      _asm
   //      {
   //         emms
   //         mov      eax, isize
   //         mov      ebx, lpbitsDest
   //         mov      ecx, lpbitsSrc
   //         movq     mm0, colorrefa
   //         movq     mm7, colorrefaN
   //   fill_loop:
   //         cmp      eax, 1
   //         jle      fill_last
   //         movq     mm1, [ebx]
   //         movq     mm2, [ecx]
   //         pandn    mm1, mm0
   //         pand     mm2, mm7
   //         por      mm1, mm2
   //         movq     [ebx], mm1
   //
   //         sub      eax, 2
   //         add      ebx, 8
   //         add      ecx, 8

   //         jmp      fill_loop

   //   fill_last:
   //         emms
   //      }
   //#endif
   //   }

   //}


   //void dib::Invert()
   //{
   //   int size=this->cx*this->cy;
   //   LPBYTE lpb = (LPBYTE) m_pcolorref;
   //   for ( int i=0; i<size; i++ )
   //   {
   //      lpb[0] = 255 - lpb[0];
   //      lpb[1] = 255 - lpb[1];
   //      lpb[2] = 255 - lpb[2];
   //      lpb += 4;
   //   }
   //}

   //void dib::channel_invert(visual::rgba::echannel echannel)
   //{
   //   __int64 size=area();
   //   LPBYTE lpb = (LPBYTE) m_pcolorref;
   //   lpb += ((int)echannel) % 4;
   //   for ( int i=0; i<size; i++ )
   //   {
   //      *lpb = 255 - *lpb;
   //      lpb += 4;
   //   }
   //}
   //void dib::channel_multiply(visual::rgba::echannel echannel, double dRate)
   //{
   //   if(dRate < 0)
   //      return;
   //   register __int64 size = area();
   //   LPBYTE lpb = (LPBYTE) get_data();
   //   lpb += ((int)echannel) % 4;
   //   register int iDiv = 256 * 256;
   //   register int iMul = (int) (dRate * ((double) iDiv));
   //   register int iRes;
   //   for(register __int64 i = 0; i < size; i++)
   //   {
   //      iRes = *lpb * iMul / iDiv;
   //      *lpb = (byte) (iRes > 255 ? 255 : iRes);
   //      lpb += 4;
   //   }
   //}

   //void dib::FillGlass ( int R, int G, int B, int A )
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
   //      dst+=4;
   //   }
   //}

   //void dib::FillStippledGlass ( int R, int G, int B )
   //{
   //   COLORREF color=RGB ( B, G, R );
   //   int w=this->cx;
   //   int h=this->cy;

   //   for ( int j=0; j<w; j++ )
   //   {
   //      for ( int i=0; i<h; i++ )
   //      {
   //         m_pcolorref[j*w+i]=((i+j)&0x1) ? m_pcolorref[j*w+i] : color;
   //      }
   //   }
   //}

   //void dib::copy(::draw2d::dib * pdib)
   //{
   //   // If DibSize Wrong Re-create dib
   //   if ( (pdib->cx!=this->cx) || (pdib->cy!=this->cy) )
   //      pdib->create ( this->cx, this->cy );
   //   // do copy
   //   memcpy ( pdib->m_pcolorref, m_pcolorref, this->cx*this->cy*4 );
   //}


   //void dib::Paste ( ::draw2d::dib * pdib )
   //{
   //   // If DibSize Wrong Re-create dib
   //   if ( (this->cx!=pdib->cx) || (this->cy!=pdib->cy) )
   //      create ( pdib->cx, pdib->cy );
   //   // do Paste
   //   memcpy ( m_pcolorref, pdib->m_pcolorref, this->cx*this->cy*4 );
   //}

   //bool dib::color_blend(COLORREF cr, BYTE bAlpha)
   //{

   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;

   //   uint32_t dwB = rgba_get_b(cr);
   //   uint32_t dwG = rgba_get_g(cr);
   //   uint32_t dwR = rgba_get_r(cr);
   //
   //   uint32_t dwB_ = dwB << 8;
   //   uint32_t dwG_ = dwG << 8;
   //   uint32_t dwR_ = dwR << 8;
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


   //void dib::Blend (::draw2d::dib * pdib, int A )
   //{
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;
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

   //bool dib::Blend(::draw2d::dib *pdib, ::draw2d::dib *pdibA, int A)
   //{
   //   if(size() != pdib->size() ||
   //      size() != pdibA->size())
   //      return false;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   BYTE *alf=(BYTE*)pdibA->m_pcolorref;
   //   int size=this->cx*this->cy;

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
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;
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
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      int Difference;
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
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;
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
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;
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
   //   if ( size()!=pdib->size() )
   //      return;

   //   BYTE *src=(BYTE*)pdib->m_pcolorref;
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size=this->cx*this->cy;
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

   //////////////////////////////////////////////////////////////////////
   // Rectangle Functions
   //////////////////////////////////////////////////////////////////////

   //void dib::copy (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to copy return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;
   //   // If DibSize Wrong Re-create dib
   //   if ( (dx!=pdib->cx) || (dy!=pdib->cy) )
   //      pdib->create ( dx, dy );

   //   // Prepare buffer Addresses
   //   COLORREF *src=m_pcolorref+(py*this->cx)+px;
   //   COLORREF *dst=pdib->m_pcolorref;

   //   // Do copy
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=this->cx;
   //      dst+=pdib->cx;
   //   }
   //}

   //void dib::PasteRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Paste return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   COLORREF *src=pdib->m_pcolorref+((py-y)*pdib->cx)+px-x;
   //   COLORREF *dst=m_pcolorref+(py*this->cx)+px;

   //   // Do Paste
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=pdib->cx;
   //      dst+=this->cx;
   //   }
   //}

   //void dib::FillRect ( int x, int y, int w, int h, int R, int G, int B )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<this->cx) ? w : this->cx-x;
   //   int dy=((y+h)<this->cy) ? h : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Fill return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*this->cx)+px;
   //   COLORREF color=RGB ( B, G, R );

   //   // Do Fill
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[i]=color;
   //      }
   //      dst+=this->cx;
   //   }
   //}

   //void dib::FillGlassRect ( int x, int y, int w, int h, int R, int G, int B, int A )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<this->cx) ? w : this->cx-x;
   //   int dy=((y+h)<this->cy) ? h : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do FillGlass
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //   }
   //}

   //void dib::FillStippledGlassRect ( int x, int y, int w, int h, int R, int G, int B )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<this->cx) ? w : this->cx-x;
   //   int dy=((y+h)<this->cy) ? h : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillStippledGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*this->cx)+px;
   //   COLORREF color=RGB ( B, G, R );

   //   // Do FillStippledGlass
   //   for ( int j=0; j<dy; j++ )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[i]=((i+j)&0x1) ? dst[i] : color;
   //      }
   //      dst+=this->cx;
   //   }
   //}

   //void dib::BlendRect (::draw2d::dib * pdib, int x, int y, int A )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Blend return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Blend
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::DarkenRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Darken return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Darken
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::DifferenceRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Difference return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Difference
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         int Difference;
   //         Difference=src[0]-dst[0];
   //         dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[1]-dst[1];
   //         dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[2]-dst[2];
   //         dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::LightenRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Lighten return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Lighten
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::MultiplyRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Multiply return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Multiply
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
   //         dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
   //         dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //void dib::ScreenRect (::draw2d::dib * pdib, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pdib->cx)<this->cx) ? pdib->cx : this->cx-x;
   //   int dy=((y+pdib->cy)<this->cy) ? pdib->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Screen return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pdib->m_pcolorref+(((py-y)*pdib->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Screen
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
   //         dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
   //         dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pdib->cx-dx)<<2;
   //   }
   //}

   //////////////////////////////////////////////////////////////////////
   // Line Functions
   //////////////////////////////////////////////////////////////////////

   /*void dib::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
   {
      int dx, dy, k1, k2, d, x, y;
      COLORREF color=RGB ( B, G, R );

      dx=x2-x1;
      dy=y2-y1;
      d=(dy<<1)-dx;
      k1=dy<<1;
      k2=(dy-dx)<<1;
      x=x1;
      y=y1;

      m_pcolorref[y*this->cx+x]=color;
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
         m_pcolorref[y*this->cx+x]=color;
      }
   }*/

//   void dib::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
//   {
//      int d, x, y, ax, ay, sx, sy, dx, dy;
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
//            m_pcolorref[y*this->cx+x]=color;
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
//            m_pcolorref[y*this->cx+x]=color;
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
//   void dib::LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A )
//   {
//      int d, x, y, ax, ay, sx, sy, dx, dy;
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
//            dst[(y*this->cx+x)<<2]=(BYTE)(((B-dst[(y*this->cx+x)<<2])*A+(dst[(y*this->cx+x)<<2]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+1]=(BYTE)(((G-dst[((y*this->cx+x)<<2)+1])*A+(dst[((y*this->cx+x)<<2)+1]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+2]=(BYTE)(((R-dst[((y*this->cx+x)<<2)+2])*A+(dst[((y*this->cx+x)<<2)+2]<<8))>>8);
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
//            dst[(y*this->cx+x)<<2]=(BYTE)(((B-dst[(y*this->cx+x)<<2])*A+(dst[(y*this->cx+x)<<2]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+1]=(BYTE)(((G-dst[((y*this->cx+x)<<2)+1])*A+(dst[((y*this->cx+x)<<2)+1]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+2]=(BYTE)(((R-dst[((y*this->cx+x)<<2)+2])*A+(dst[((y*this->cx+x)<<2)+2]<<8))>>8);
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
//   void dib::Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask)
//   {
//      COLORREF crFind = RGB(rgba_get_b(crMask), rgba_get_g(crMask), rgba_get_r(crMask));
//      COLORREF crSet = RGB(rgba_get_b(crInMask), rgba_get_g(crInMask), rgba_get_r(crInMask));
//      COLORREF crUnset  = RGB(rgba_get_b(crOutMask), rgba_get_g(crOutMask), rgba_get_r(crOutMask));
//
//      int size=this->cx*this->cy;
//
//      for ( int i=0; i<size; i++ )
//         if(m_pcolorref[i]== crFind)
//            m_pcolorref[i] = crSet;
//         else
//            m_pcolorref[i] = crUnset;
//
//   }
//
//   void dib::transparent_color(color color)
//   {
//      COLORREF crFind = color.get_rgb();
//      __int64 size = area();
//
//      for ( int i=0; i<size; i++ )
//         if((m_pcolorref[i] & 0x00ffffff) == crFind)
//            ((LPBYTE)&m_pcolorref[i])[3] = 255;
//         else
//            ((LPBYTE)&m_pcolorref[i])[3] = 0;
//   }
//
//   void dib::channel_mask(unsigned char uchFind, unsigned char uchSet, unsigned char uchUnset, visual::rgba::echannel echannel)
//   {
//      int size = this->cx * this->cy;
//      unsigned char * puch = (unsigned char * ) m_pcolorref;
//      puch += ((int) echannel) % 4;
//
//      for(int i = 0; i < size; i++)
//      {
//         if(*puch == uchFind)
//            *puch = uchSet;
//         else
//            *puch = uchUnset;
//         puch += 4;
//      }
//   }

   //uint32_t dib::GetPixel(int x, int y)
   //{
   //   uint32_t dw = *(m_pcolorref + x + (this->cy - y - 1) * this->cx);
   //   return RGB(rgba_get_b(dw), rgba_get_g(dw), rgba_get_r(dw));
   //}

   // too slow for animation on AMD XP Atlhon.
   // TOP SUGGESTION:
   // The gradient can't have more then 256 levels of the most bright color
   // (white). So creating a radial fill of radius 256 and then using fasting
   // stretching algorithms is much faster than calculating radial fill.
//   void dib::RadialFill(BYTE alpha, BYTE red, BYTE green, BYTE blue, int xCenter, int yCenter, int iRadius)
//   {
//      if (iRadius == 0)
//         return;
//      /*if(version == 0)
//      {
//
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= m_Size.cx) xU = m_Size.cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= m_Size.cy) yU = m_Size.cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * m_Size.cx));
//         uint32_t dwAdd = ((m_Size.cx - 1 - xU) + xL) * 4;
//         int size=m_Size.cx*m_Size.cy;
//         double iLevel;
//
//         int dx, dy;
//         int dx0, dy0;
//         int dx1, dy1;
//         int dx2, dy2;
//         int dx3, dy3;
//         int dx4, dy4;
//         int dx5, dy5;
//         int dx6, dy6;
//         int dx7, dy7;
//         int dx8, dy8;
//         int dx9, dy9;
//         int dxA, dyA;
//         int dxB, dyB;
//         int dxC, dyC;
//         int dxD, dyD;
//         int dxE, dyE;
//         int dxF, dyF;
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
//         int x, y;
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
//         LPBYTE lpb = (LPBYTE) (((int) lpbAlloc + 3) & ~3);
//
//
//         int x, y;
//         int b;
//
////         int r2 = iRadius * iRadius;
//
//         for(y = 0; y < iRadius; y++)
//         {
//            for(x = y; x < iRadius; x++)
//            {
//               b = (int) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
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
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= this->cx) xU = this->cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= this->cy) yU = this->cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * this->cx));
//         uint32_t dwAdd = ((this->cx - 1 - xU) + xL) * 4;
////         int size=this->cx*this->cy;
//
//         int dx, dy;
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
//      int xCenter, int yCenter, int iRadius)
//   {
//      if (iRadius == 0)
//         return;
//      /*if(version == 0)
//      {
//
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= m_Size.cx) xU = m_Size.cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= m_Size.cy) yU = m_Size.cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * m_Size.cx));
//         uint32_t dwAdd = ((m_Size.cx - 1 - xU) + xL) * 4;
//         int size=m_Size.cx*m_Size.cy;
//         double iLevel;
//
//         int dx, dy;
//         int dx0, dy0;
//         int dx1, dy1;
//         int dx2, dy2;
//         int dx3, dy3;
//         int dx4, dy4;
//         int dx5, dy5;
//         int dx6, dy6;
//         int dx7, dy7;
//         int dx8, dy8;
//         int dx9, dy9;
//         int dxA, dyA;
//         int dxB, dyB;
//         int dxC, dyC;
//         int dxD, dyD;
//         int dxE, dyE;
//         int dxF, dyF;
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
//         int x, y;
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
//         LPBYTE lpb = (LPBYTE) (((int) lpbAlloc + 3) & ~3);
//
//
//         int x, y;
//         int b;
//
////         int r2 = iRadius * iRadius;
//
//         for(y = 0; y < iRadius; y++)
//         {
//            for(x = y; x < iRadius; x++)
//            {
//               b = (int) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
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
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= this->cx) xU = this->cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= this->cy) yU = this->cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * this->cx));
//         uint32_t dwAdd = ((this->cx - 1 - xU) + xL) * 4;
////         int size=this->cx*this->cy;
//
//         int dx, dy;
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
//
   void dib::SetIconMask(::visual::icon * picon, int cx, int cy)
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
      int64_t iSize = area();

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
   //  // ::draw2d::dib_sp spdib(get_app());
   ////   spdib->Paste(this);

   //   int cx = this->cx;
   //   int cy = this->cy;

   //   int l = MAX(cx, cy);

   //
   //   int jmax = MIN(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = MIN(l, cx / 2);
   //   int imin = - imax;


   //   int joff = cy / 2;
   //   int ioff = cx / 2;

   //   //int iAngle = iStep % 360;
   //   //int iAngle = iStep;
   //   //int iAngle = 1;
   //   //int k = 0;

   ///*     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         x=int(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //         y=int(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            spdib->m_pcolorref[abs(y%this->cy)*this->cx+abs(x%this->cx)];
   //         //k++;
   //      }
   //      (j+joff)*cx+(i+ioff)
   //   }*/

   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = this->cx - 1;
   //   int cy1 = this->cy - 1;
   //     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % this->cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % this->cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / this->cx) % 2 == 0)
   //         {
   //            x %= this->cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % this->cx);
   //         }

   //         if((y / this->cy) % 2 == 0)
   //         {
   //            y %= this->cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % this->cy);
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
   //   int cx = this->cx;
   //   int cy = this->cy;

   //   int l = MAX(cx, cy);

   //
   //   int jmax = MIN(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = MIN(l, cx / 2);
   //   int imin = - imax;


   //   if((cy % 2) == 1)
   //      jmax++;

   //   if((cx % 2) == 1)
   //      imax++;
   //
   //   int joff = cy / 2;
   //   int ioff = cx / 2;

   //
   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = this->cx - 1;
   //   int cy1 = this->cy - 1;
   //     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % this->cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % this->cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / this->cx) % 2 == 0)
   //         {
   //            x %= this->cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % this->cx);
   //         }

   //         if((y / this->cy) % 2 == 0)
   //         {
   //            y %= this->cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % this->cy);
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
   //   LPCRECT lpcrect,
   //   double dAngle,
   //   double dScale)
   //{
   //  // ::draw2d::dib_sp spdib(get_app());
   ////   spdib->Paste(this);



   //   rect rect(lpcrect);

   //   int cx = rect.width();
   //   int cy = rect.height();

   //   int l = MAX(cx, cy);
   //
   //   int jmax = MIN(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = MIN(l, cx / 2);
   //   int imin = - imax;


   //   int joff = cy / 2 + rect.left;
   //   int ioff = cx / 2 + rect.top;

   //   //int iAngle = iStep % 360;
   //   //int iAngle = iStep;
   //   //int iAngle = 1;
   //   //int k = 0;

   ///*     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         x=int(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //         y=int(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            spdib->m_pcolorref[abs(y%this->cy)*this->cx+abs(x%this->cx)];
   //         //k++;
   //      }
   //      (j+joff)*cx+(i+ioff)
   //   }*/

   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = cx - 1;
   //   int cy1 = cy - 1;
   //     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % this->cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % this->cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

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
   //         m_pcolorref[(j+joff)*this->cx+(i+ioff)]=
   //            pdib->m_pcolorref[y * this->cx + x];
   //         k++;
   //      }
   //   }
   //}

   ///*int dib::cos(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * CosN[iAngle]) >> 32);
   //}

   //int dib::sin(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * SinN[iAngle]) >> 32);
   //}*/




   //void dib::Fill (int A, int R, int G, int B )
   //{
   //   COLORREF color = RGB ( B, G, R ) | (A << 24);
   //   int size=this->cx*this->cy;

   //   COLORREF * pcr;

   //   int iSize32 = size / 32;
   //   int i;
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
   //   int iRLine;
   //   int iGLine;
   //   int iBLine;
   //   double dDiv = this->cx * this->cy;
   //   if(dDiv > 0)
   //   {
   //      LPBYTE lpb = (LPBYTE) m_pcolorref;
   //      for (int y = 0; y < this->cy; y++)
   //      {
   //         iRLine = 0;
   //         iGLine = 0;
   //         iBLine = 0;
   //         for (int x = 0; x < this->cx; x++)
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
   //      int iR = (int) dR;
   //      int iG = (int) dG;
   //      int iB = (int) dB;
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
   //   if(this->cx != pdib->cx
   //   || this->cy != pdib->cy)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   LPDWORD lpd1 = (LPDWORD) m_pcolorref;
   //   LPDWORD lpd2 = (LPDWORD) pdib->m_pcolorref;
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      *lpd1 = *lpd1 ^ *lpd2;
   //      lpd1++;
   //      lpd2++;
   //   }
   //}

   //void dib::create_frame(class size size, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   int iFrameWidth = size.cx / iSliceCount;
   //   int iFrameHeight = size.cy / iSliceCount;
   //   create(iFrameWidth, iFrameHeight);
   //}

   //void dib::set_frame1(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * this->cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = 0; y < iFrameHeight; y++)
   //   {
   //      lpDestLine = &lpDest[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine = *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void dib::set_frame2(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   if(lpdata == NULL)
   //      return;
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * this->cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine = *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void dib::xor_dib_frame2(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   if(lpdata == NULL)
   //      return;
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * this->cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine ^= *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void dib::get_frame(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpSrc = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight *  this->cx];
   //   COLORREF * lpDest = (COLORREF *) lpdata;
   //   COLORREF * lpSrcLine;
   //   for(int y = 0; y < iFrameHeight; y++)
   //   {
   //      lpSrcLine = &lpSrc[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDest = *lpSrcLine;
   //          lpDest++;
   //          lpSrcLine++;
   //      }
   //   }
   //}

   //bool dib::is_rgb_black()
   //{
   //   int iSize = this->cx * this->cy;
   //   COLORREF * lp = m_pcolorref;
   //   for(int i = 0; i < iSize; i++)
   //   {
   //      if((*lp & 0x00FFFFFF) != 0)
   //         return false;
   //      lp++;
   //   }
   //   return true;
   //}

   //void dib::DivideRGB(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   int i = 0;
   //   int iCount1 = iCount - iCount % 8;
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

   //void dib::DivideARGB(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}

   //void dib::DivideA(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   LPBYTE lp = ((LPBYTE) m_pcolorref);
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}


   void dib::stretch_dib(::draw2d::dib * pdib)
   {
      ::StretchDIBits(
      GDI_HDC(m_spgraphics.m_p),
      0, 0,
      this->m_size.cx, this->m_size.cy,
      0, 0,
      pdib->m_size.cx, pdib->m_size.cy,
      pdib->m_pcolorref,
      &dynamic_cast < dib * > (pdib)->m_info,
      DIB_RGB_COLORS,
      SRCCOPY);
   }

   ::draw2d::graphics * dib::get_graphics() const
   {
      unmap();
      return m_spgraphics;
   }

   //double dib::pi()
   //{
   //   return dPi;
   //}

   // void dib::fill_channel(int intensity, visual::rgba::echannel echannel)
   // {
   //     int offset = ((int)echannel) % 4;
   //    int size=this->cx*this->cy;

   //    BYTE * pb;

   //    int iSize32 = size / 32;
   //    int i;
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

   //    for (i=0; i<size; i++ )
   //    {
   //       *(((BYTE * ) &m_pcolorref[i]) + offset) = (byte) intensity;
   //    }
   //}


   // int dib::cos(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * CosN[iAngle]) >> 31);
   // }

   // int dib::sin(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * SinN[iAngle]) >> 31);
   // }

   // int dib::cos10(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * Cos10N[iAngle]) >> 34);
   // }

   // int dib::sin10(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * Sin10N[iAngle]) >> 34);
   // }

   // int dib::width()
   // {
   //    return this->cx;
   // }

   // int dib::height()
   // {
   //    return this->cy;
   // }


   bool dib::update_window(::aura::draw_interface * pwnd, ::message::message * pobj, bool bTransferBuffer)
   {

      rect64 rectWindow;

      pwnd->GetWindowRect(rectWindow);

      m_spgraphics->SetViewportOrg(0, 0);

      map();


      BYTE *dst=(BYTE*)get_data();
      int64_t size = area();


      // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255

      /*
      while (size >= 8)
         {
            dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3])>> 8);
            dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3])>> 8);
            dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3])>> 8);

            dst[4+0] = LOBYTE(((int32_t)dst[4+0] * (int32_t)dst[4+3])>> 8);
            dst[4+1] = LOBYTE(((int32_t)dst[4+1] * (int32_t)dst[4+3])>> 8);
            dst[4+2] = LOBYTE(((int32_t)dst[4+2] * (int32_t)dst[4+3])>> 8);

            dst[8+0] = LOBYTE(((int32_t)dst[8+0] * (int32_t)dst[8+3])>> 8);
            dst[8+1] = LOBYTE(((int32_t)dst[8+1] * (int32_t)dst[8+3])>> 8);
            dst[8+2] = LOBYTE(((int32_t)dst[8+2] * (int32_t)dst[8+3])>> 8);

            dst[12+0] = LOBYTE(((int32_t)dst[12+0] * (int32_t)dst[12+3])>> 8);
            dst[12+1] = LOBYTE(((int32_t)dst[12+1] * (int32_t)dst[12+3])>> 8);
            dst[12+2] = LOBYTE(((int32_t)dst[12+2] * (int32_t)dst[12+3])>> 8);

            dst[16+0] = LOBYTE(((int32_t)dst[16+0] * (int32_t)dst[16+3])>> 8);
            dst[16+1] = LOBYTE(((int32_t)dst[16+1] * (int32_t)dst[16+3])>> 8);
            dst[16+2] = LOBYTE(((int32_t)dst[16+2] * (int32_t)dst[16+3])>> 8);

            dst[20+0] = LOBYTE(((int32_t)dst[20+0] * (int32_t)dst[20+3])>> 8);
            dst[20+1] = LOBYTE(((int32_t)dst[20+1] * (int32_t)dst[20+3])>> 8);
            dst[20+2] = LOBYTE(((int32_t)dst[20+2] * (int32_t)dst[20+3])>> 8);

            dst[24+0] = LOBYTE(((int32_t)dst[24+0] * (int32_t)dst[24+3])>> 8);
            dst[24+1] = LOBYTE(((int32_t)dst[24+1] * (int32_t)dst[24+3])>> 8);
            dst[24+2] = LOBYTE(((int32_t)dst[24+2] * (int32_t)dst[24+3])>> 8);

            dst[28+0] = LOBYTE(((int32_t)dst[28+0] * (int32_t)dst[28+3])>> 8);
            dst[28+1] = LOBYTE(((int32_t)dst[28+1] * (int32_t)dst[28+3])>> 8);
            dst[28+2] = LOBYTE(((int32_t)dst[28+2] * (int32_t)dst[28+3])>> 8);

            dst += 4 * 8;
            size -= 8;
         }
         while(size--)
         {
            dst[0] = LOBYTE(((int32_t)dst[0] * (int32_t)dst[3])>> 8);
            dst[1] = LOBYTE(((int32_t)dst[1] * (int32_t)dst[3])>> 8);
            dst[2] = LOBYTE(((int32_t)dst[2] * (int32_t)dst[3])>> 8);
            dst += 4;
         }
      */

      rect rect(rectWindow);

      //Application.window_graphics_update_window(pwnd->get_window_graphics(),pwnd->get_handle(),m_pcolorref,rect,m_size.cx,m_size.cy,m_iScan, bTransferBuffer);

      return true;

   }


   void dib::map(bool bApplyAlphaTransform) const
   {

      if(m_bMapped)
         return;

      if(m_pcolorref == NULL)
         return;


      GdiFlush();


      /*if(bApplyAlphaTransform)
      {

         GdiFlush();

         for(int y = 0; y < cy; y++)
         {
            byte * p = &((byte *) m_pcolorref)[scan * y];
            for(int x = 0; x < cx; x++)
            {
               if(p[3] == 0)
               {
                  p[0] = 255;
                  p[1] = 255;
                  p[2] = 255;
               }
               else
               {
                  p[0] = (p[0] * 255 / p[3]);
                  p[1] = (p[1] * 255 / p[3]);
                  p[2] = (p[2] * 255 / p[3]);
               }
               p += 4;
            }
         }

      }*/

      ((dib *) this)->m_bMapped = true;

   }

   void dib::unmap() const
   {

      if(!m_bMapped)
         return;

      if(m_pcolorref == NULL)
         return;

      int64_t iArea = area();

      byte * p = (byte *) m_pcolorref;

      GdiFlush();

      /*
      for(int y = 0; y < cy; y++)
      {
        byte * p = &((byte *) m_pcolorref)[scan * y];
        for(int x = 0; x < cx; x++)
        {
           p[0] = (p[0] * p[3] / 255);
           p[1] = (p[1] * p[3] / 255);
           p[2] = (p[2] * p[3] / 255);
           p += 4;
        }
      }

      */

      ((dib *) this)->m_bMapped = false;

   }


   bool dib::print_window(::aura::draw_interface * pwnd, ::message::message * pobj)
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

         ::draw2d::graphics * pgraphics = dib->get_graphics();

         if(pgraphics->get_os_data() == NULL)
            return false;

         rect rectPaint;
         rect rectUpdate;
         rectUpdate = rectWindow;
         rectPaint = rectWindow;
         rectPaint.offset(-rectPaint.top_left());
         m_spgraphics->SelectClipRgn(NULL);
         pwnd->_001OnDeferPaintLayeredWindowBackground(dib->get_graphics());
         m_spgraphics->SelectClipRgn(NULL);
         m_spgraphics-> SetViewportOrg(point(0, 0));
         pwnd->_000OnDraw(dib->get_graphics());
         m_spgraphics->SetViewportOrg(point(0, 0));
         //(dynamic_cast<::win::graphics * >(pgraphics))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
         m_spgraphics->SelectClipRgn(NULL);
         m_spgraphics->SetViewportOrg(point(0, 0));

         m_spgraphics->SelectClipRgn( NULL);
         m_spgraphics->BitBlt(rectPaint.left, rectPaint.top,
                              rectPaint.width(), rectPaint.height(),
                              pgraphics, rectUpdate.left, rectUpdate.top,
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

//
//procedure TAlphaBitmapWrapper.ProcessTransparency(const Alpha: Byte; TranspRect: TRect);
//var
//  LoopX : Integer;
//  PreMult : Single;
//  PQuad : PQuadColor;
//  LoopY: Integer;
//begin
//  GdiFlush; // Need to flush before any manipulation of bits
//  IntersectRect(TranspRect, TranspRect, Rect(0, 0, FWidth, FHeight)); // Clip to valid bounds
//
//  PreMult := Alpha / 255.0;
//  for LoopY := TranspRect.Top to TranspRect.Bottom - 1 do begin
//    PQuad := FQuads;
//    Inc(PQuad, LoopY);
//    for LoopX := TranspRect.Left to TranspRect.Right - 1 do begin
//      if PQuad.WrittenByGDI then begin
//        PQuad.SetAlpha(Alpha, PreMult);
//      end else begin
//        PQuad.Quad := 0;
//      end;
//      Inc(PQuad);
//    end;
//  end;
//end;

   bool dib::process_blend(COLORREF clr, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdibSrc)
   {

      ::GdiFlush();

      int32_t a = argb_get_a_value(clr);

      ::visual::rgba::echannel echannel;

      int32_t bTune;

      if(argb_get_g_value(clr) > argb_get_b_value(clr))
      {
         if(argb_get_b_value(clr) > argb_get_r_value(clr))
         {
            echannel = ::visual::rgba::channel_green;
            bTune = argb_get_g_value(clr);
         }
         else if(argb_get_g_value(clr) >  argb_get_r_value(clr))
         {
            echannel = ::visual::rgba::channel_green;
            bTune = argb_get_g_value(clr);
         }
         else
         {
            echannel = ::visual::rgba::channel_red;
            bTune = argb_get_r_value(clr);
         }
      }
      else if(argb_get_g_value(clr) > argb_get_r_value(clr))
      {
         echannel = ::visual::rgba::channel_blue;
         bTune = argb_get_b_value(clr);
      }
      else if(argb_get_r_value(clr) > argb_get_b_value(clr))
      {
         echannel = ::visual::rgba::channel_red;
         bTune = argb_get_r_value(clr);
      }
      else
      {
         echannel = ::visual::rgba::channel_blue;
         bTune = argb_get_b_value(clr);
      }

      int64_t size = area();

      byte * pb = (byte *) m_pcolorref;
      byte * pbTune = ((byte *) m_pcolorref) + ((int32_t) echannel);

      //bTune *= 255;

      bTune = 255  * 3;

      int32_t aTune = a * 255;

      int32_t r = argb_get_r_value(clr);
      int32_t g = argb_get_g_value(clr);
      int32_t b = argb_get_b_value(clr);

      if(bTune == 0)
      {

         r *= a;
         g *= a;
         b *= a;

         while(size > 0)
         {
            if(pb[3] == 0)
            {
               pb[0] = r * pb[0] / (255 * 255);
               pb[1] = g * pb[1] / (255 * 255);
               pb[2] = b * pb[2] / (255 * 255);
               pb[3] = a;
            }
            else
            {
               pb[3] = 0;
            }
            size--;
            pb+=4;
         }

      }
      else
      {
         if(ealphamode == ::draw2d::alpha_mode_blend)
         {
            while(size > 0)
            {
               //if(pb[3] == 0)
               {
                  pb[3] = byte_clip(a * (pb[0] + pb[1] + pb[2]) / bTune);
                  pb[0] = b * pb[3] / 255;
                  pb[1] = g * pb[3] / 255;
                  pb[2] = r * pb[3] / 255;
               }
               //else
               {
                  // pb[3] = 0;
               }
               size--;
               pb+=4;
               pbTune+=4;
            }
         }
         else
         {
            byte * ps = (byte *) pdibSrc->m_pcolorref;
            ps += pdibSrc->m_iScan * y + x * sizeof(COLORREF);
            int s = pdibSrc->m_iScan - m_size.cx * sizeof(COLORREF);
            int c = m_size.cx;

            rect r1(x, y,  x  + m_size.cx, y + m_size.cy);
            rect r2(0, 0, pdibSrc->m_size.cx, pdibSrc->m_size.cy);
            rect r3;

            r3.intersect(r1, r2);

            size = MIN(size, (int32_t) r3.area());
            while(size > 0)
            {
               if(pb[3] == 0)
               {
                  ps[3] = byte_clip(a * (pb[0] + pb[1] + pb[2]) / bTune);
                  ps[0] = (byte) b;
                  ps[1] = (byte) g;
                  ps[2] = (byte) r;
               }
               else
               {
                  /*                  pb[3] = ps[3];
                                    pb[2] = ps[2];
                                    pb[1] = ps[1];
                                    pb[0] = ps[0];
                                    */
               }
               c--;
               size--;
               pb+=4;
               pbTune+=4;
               ps+=4;
               if(c <= 0)
               {
                  c = m_size.cx;
                  ps += s;
               }
            }
         }
      }


      return true;

   }

   bool dib::process_blend(::draw2d::dib * pdib, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdibSrc)
   {

      if(pdib == NULL)
         return false;

      if(pdib->size() != size())
         return false;

      ::GdiFlush();

      int32_t size = (int32_t) area();

      byte * pdataDst = (byte *) m_pcolorref;

      byte * pdataSrc = (byte *) pdib->m_pcolorref;

      if(ealphamode == ::draw2d::alpha_mode_set)
      {
         byte * ps = (byte *) pdibSrc->m_pcolorref;
         ps += pdibSrc->m_iScan * y + x * sizeof(COLORREF);
         int s = pdibSrc->m_iScan - m_size.cx * sizeof(COLORREF);
         int c = m_size.cx;

         while(size > 0)
         {

            if(pdataDst[3] == 0)
            {
               ps[3] = pdataSrc[3] * pdataDst[1] / 255;
               ps[0] = pdataSrc[0] * ps[3] / 255;
               ps[1] = pdataSrc[1] * ps[3] / 255;
               ps[2] = pdataSrc[2] * ps[3] / 255;
            }
            else
            {
               //pdataDst[3] = ps[3];
               //pdataDst[2] = ps[2];
               //pdataDst[1] = ps[1];
               //pdataDst[0] = ps[0];

            }
            c--;
            size--;

            pdataDst+=4;
            pdataSrc+=4;
            ps+=4;
            if(c <= 0)
            {
               c = m_size.cx;
               ps+= s;
            }

         }

      }
      else
      {
         while(size > 0)
         {


            pdataDst[3] = pdataSrc[3] * pdataDst[1] / 255;
            pdataDst[0] = pdataSrc[0] * pdataDst[3] / 255;
            pdataDst[1] = pdataSrc[1] * pdataDst[3] / 255;
            pdataDst[2] = pdataSrc[2] * pdataDst[3] / 255;

            size--;
            pdataDst+=4;
            pdataSrc+=4;

         }

      }

      return true;

   }

   bool dib::process_blend(::draw2d::brush * pbrush, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdibSrc)
   {

      if(pbrush->m_etype == ::draw2d::brush::type_null)
      {
         return false;
      }
      if(pbrush->m_etype == ::draw2d::brush::type_solid)
      {
         process_blend(pbrush->m_cr, x, y, ealphamode, pdibSrc);
         GDI_BRUSH(pbrush)->m_bProcess = false;
         return true;
      }
      if(pbrush->m_etype == ::draw2d::brush::type_linear_gradient_point_color)
      {
         ::draw2d::dib_sp dib(allocer());
         dib->create(m_size);
         point p1 = pbrush->m_pt1;
         point p2 = pbrush->m_pt2;
         p1.offset(-x, -y);
         p2.offset(-x, -y);
         dib->gradient_fill(pbrush->m_cr1, pbrush->m_cr2, p1, p2);
         process_blend(dib, x, y, ealphamode, pdibSrc);
         GDI_BRUSH(pbrush)->m_bProcess = false;
         return true;
      }
      return false;
   }

   bool dib::process_initialize(::draw2d::brush * pbrush, bool bReset)
   {
      if(pbrush->m_etype == ::draw2d::brush::type_null)
      {
         return false;
      }
      if(pbrush->m_etype == ::draw2d::brush::type_solid)
      {
         GDI_BRUSH(pbrush)->m_bProcess = true;
         pbrush->m_bUpdated = false;
         if(bReset)
         {
            Fill(255, 0, 0, 0);
         }
         return true;
      }
      if(pbrush->m_etype == ::draw2d::brush::type_linear_gradient_point_color)
      {
         GDI_BRUSH(pbrush)->m_bProcess = true;
         pbrush->m_bUpdated = false;
         if(bReset)
         {
            Fill(255, 0, 0, 0);
         }
         return true;
      }
      return false;
   }


   bool dib::process_blend(::draw2d::pen * ppen, int32_t x, int32_t y, ::draw2d::e_alpha_mode ealphamode, ::draw2d::dib * pdib)
   {
      if(ppen->m_etype == ::draw2d::pen::type_null)
      {
         return false;
      }
      if(ppen->m_etype == ::draw2d::pen::type_solid)
      {
         process_blend(ppen->m_cr, x, y, ealphamode, pdib);
         GDI_PEN(ppen)->m_bProcess = false;
         return true;
      }
      return false;
   }


   bool dib::process_initialize(::draw2d::pen * ppen, bool bReset)
   {

      if(ppen->m_etype == ::draw2d::pen::type_null)
      {

         return false;

      }

      if(ppen->m_etype == ::draw2d::pen::type_solid)
      {

         GDI_PEN(ppen)->m_bProcess = true;

         ppen->m_bUpdated = false;

         if(bReset)
         {

            Fill(255, 0, 0, 0);

         }

         return true;

      }

      return false;

   }


   /*   bool dib::from(::draw2d::graphics * pgraphics, FIBITMAP *pfibitmap, bool bUnloadFI)
      {

         if(pfibitmap == NULL)
              return false;

         BITMAPINFO * pbi = FreeImage_GetInfo(pfibitmap);
         void * pdata = FreeImage_GetBits(pfibitmap);

         if(!create(pbi->bmiHeader.biWidth, pbi->bmiHeader.biHeight))
            return false;


         COLORREF * pcolorref = NULL;

         HBITMAP hbitmap = ::CreateDIBSection(NULL, &m_info, DIB_RGB_COLORS, (void **) &pcolorref, NULL, 0);

         if(hbitmap == NULL)
         {
            Destroy();
            return false;
         }

         HDC hdc = ::CreateCompatibleDC(NULL);

         if(pbi->bmiHeader.biHeight != SetDIBits(
            hdc,
            hbitmap,
            0,
            pbi->bmiHeader.biHeight,
            pdata,
            pbi,
            DIB_RGB_COLORS))
         {
            Destroy();
            if(bUnloadFI)
            {
               FreeImage_Unload(pfibitmap);
            }
            return false;
         }

         memcpy(m_pcolorref, pcolorref, (size_t) (area() * sizeof(COLORREF)));


         RGBQUAD bkcolor;

         if(pbi->bmiHeader.biBitCount == 32)
         {
         }
         else if(pbi->bmiHeader.biBitCount <= 24 && FreeImage_GetTransparencyCount(pfibitmap) <= 0)
         {
            fill_channel(0xff, ::visual::rgba::channel_alpha);
         }
         else if(FreeImage_GetBackgroundColor(pfibitmap, &bkcolor))
         {
            transparent_color(bkcolor);
         }

         if(bUnloadFI)
         {
            FreeImage_Unload(pfibitmap);
         }


         return true;
      }*/


   COLORREF dib::make_colorref(int32_t a, int32_t r, int32_t g, int32_t b)
   {

      return ARGB(a, b, g, r);

   }


} // namespace draw2d_gdi





