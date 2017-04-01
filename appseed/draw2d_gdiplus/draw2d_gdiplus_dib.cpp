

namespace draw2d_gdiplus
{


   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////


   dib::dib(::aura::application * papp) :
      object(papp)
   {

      m_sizeWnd.cx = 0;
      m_sizeWnd.cy = 0;
      m_hbitmap = NULL;

      ZERO(m_bitmapinfo);

   }


   void    dib::construct(int32_t cx, int32_t cy)
   {
      m_pcolorref = NULL;
      cx = 0;
      cy = 0;
      create(cx, cy);
   }


   dib::~dib()
   {

      destroy();

   }


   ::draw2d::bitmap_sp dib::get_bitmap() const
   {

      return m_spbitmap;

   }


   ::draw2d::bitmap_sp dib::detach_bitmap()
   {

      return m_spbitmap.detach();

   }


   bool dib::create(class size size)
   {

      return create(size.cx, size.cy);

   }


   bool dib::create(int32_t width, int32_t height)
   {

      if (width <= 0 || height <= 0)
         return false;

      if (m_spbitmap.is_set()
         && m_spbitmap->get_os_data() != NULL
         && width == m_size.cx
         && height == m_size.cy)
         return true;

      destroy();

      m_info = {};

      m_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
      m_info.bmiHeader.biWidth = width;
      m_info.bmiHeader.biHeight = -height;
      m_info.bmiHeader.biPlanes = 1;
      m_info.bmiHeader.biBitCount = 32;
      m_info.bmiHeader.biCompression = BI_RGB;
      m_info.bmiHeader.biSizeImage = width * height * 4;

      m_spbitmap.alloc(allocer());
      m_spgraphics.alloc(allocer());

      if (m_spbitmap.is_null())
      {

         m_size.cx = 0;

         m_size.cy = 0;

         m_iScan = 0;

         return false;

      }

      if (!m_spbitmap->CreateDIBSection(NULL, &m_info, DIB_RGB_COLORS, (void **)&m_pcolorref, &m_iScan, NULL, 0))
      {

         m_size.cx = 0;

         m_size.cy = 0;

         m_iScan = 0;

         return false;

      }

      if (m_spbitmap->get_os_data() == NULL)
      {

         destroy();

         return false;

      }

      m_spgraphics->SelectObject(m_spbitmap);

      m_spgraphics->m_pdib = this;

      m_size.cx = width;

      m_size.cy = height;

      m_iScan = m_spbitmap->m_iStride;

      return true;

   }


   bool dib::dc_select(bool bSelect)
   {

      return true;

   }


   bool dib::create(::draw2d::graphics * pgraphics)
   {

      ::draw2d::bitmap * pbitmap = (dynamic_cast<::draw2d_gdiplus::graphics *>(pgraphics))->get_current_bitmap();

      if (pbitmap == NULL)
         return FALSE;

      if (!create(pbitmap->get_size()))
      {
         return FALSE;
      }

      from(pgraphics);

      return TRUE;

   }


   bool dib::destroy()
   {

      m_spbitmap.release();

      m_spgraphics.release();

      m_size = {};
      m_iScan = 0;
      m_pcolorref = NULL;

      return true;

   }


   bool dib::to(::draw2d::graphics * pgraphics, point pt, class size size, point ptSrc)
   {

      return pgraphics->BitBlt(pt, size, get_graphics(), ptSrc);

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

      if (!create(size))
      {

         GDIPLUS_GRAPHICS(pgraphics)->SelectObject(pbitmap);

         return false;

      }

      bool bOk = GetDIBits(GDIPLUS_HDC(pgraphics), (HBITMAP)pbitmap->get_os_data(), 0, m_size.cy, m_pcolorref, &(m_info), DIB_RGB_COLORS) != FALSE;

      GDIPLUS_GRAPHICS(pgraphics)->SelectObject(pbitmap);

      return bOk;

   }


   bool dib::from(point ptDest, ::draw2d::graphics * pgraphics, point pt, class size sz)
   {

      if (m_spgraphics.is_null())
         return false;

      return m_spgraphics->BitBlt(ptDest, sz, pgraphics, pt, SRCCOPY) != FALSE;

   }



   void dib::SetIconMask(::visual::icon * picon, int32_t cx, int32_t cy)
   {

      create(cx, cy);

      if (cx <= 0 || cy <= 0)
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

      BYTE * r1 = (BYTE*)dib1.m_pcolorref;
      BYTE * r2 = (BYTE*)spdib2->get_data();
      BYTE * srcM = (BYTE*)dibM.m_pcolorref;
      BYTE * dest = (BYTE*)m_pcolorref;
      int32_t iSize = cx*cy;

      BYTE b;
      BYTE bMax;
      while (iSize-- > 0)
      {
         if (srcM[0] == 255)
         {
            bMax = 0;
         }
         else
         {
            bMax = 0;
            b = (BYTE)(r1[0] - r2[0]);
            bMax = MAX(b, bMax);
            b = (BYTE)(r1[1] - r2[1]);
            bMax = MAX(b, bMax);
            b = (BYTE)(r1[2] - r2[2]);
            bMax = MAX(b, bMax);
            bMax = 255 - bMax;
         }
         dest[0] = bMax;
         dest[1] = bMax;
         dest[2] = bMax;
         dest += 4;
         srcM += 4;
         r1 += 4;
         r2 += 4;
      }



   }


   void dib::stretch_dib(::draw2d::dib * pdib)
   {

      if (area() <= 0 || pdib->area() <= 0)
      {

         return;

      }

      Gdiplus::Rect rectDest(0, 0, m_size.cx, m_size.cy);

      unmap();

      pdib->unmap();

      m_spgraphics->set_alpha_mode(::draw2d::alpha_mode_set);

      m_spgraphics->SetStretchBltMode(HALFTONE);

      ((Gdiplus::Graphics *) m_spgraphics->get_os_data())->DrawImage(((Gdiplus::Bitmap *)pdib->get_bitmap()->get_os_data()), rectDest, 0, 0, pdib->m_size.cx, pdib->m_size.cy, Gdiplus::UnitPixel);

   }


   ::draw2d::graphics * dib::get_graphics() const
   {

      ((dib *) this)->unmap();

      return m_spgraphics;

   }


   void dib::map(bool bApplyTransform) const
   {

      UNREFERENCED_PARAMETER(bApplyTransform);

      if (!m_bMapped)
      {

         ((dib*)this)->m_bMapped = true;

      }

   }


   void dib::unmap() const
   {

      if (m_bMapped)
      {

         if (m_spgraphics.is_set())
         {

            ((dib*)this)->m_pt = m_spgraphics->GetViewportOrg();

            m_spgraphics->m_spbitmap = m_spbitmap;

         }

         ((dib*)this)->m_bMapped = false;

      }

   }


} // namespace draw2d_gdiplus





