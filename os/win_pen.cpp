#include "framework.h"


#undef new


namespace win
{


   pen::pen(::ca::application * papp) :
      ca(papp)
   { 

      m_ppen = NULL;

   }

   pen::~pen()
   { 
      if(m_ppen != NULL)
      {
         delete m_ppen;
         m_ppen = NULL;
      }
   }

   /*bool pen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
   { return Attach(::CreatePen(nPenStyle, nWidth, crColor)); }
   bool pen::CreatePenIndirect(LPLOGPEN lpLogPen)
   { return Attach(::CreatePenIndirect(lpLogPen)); }
   bool pen::CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
      int nStyleCount, const DWORD* lpStyle)
   { return Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount,
   lpStyle)); }
   int pen::GetExtLogPen(EXTLOGPEN* pLogPen)
   { ASSERT(get_handle() != NULL);
   return ::GetObject(get_handle(), sizeof(EXTLOGPEN), pLogPen); }
   
   int pen::GetLogPen(LOGPEN* pLogPen)
   { 
      if(get_handle() == NULL)
         return 0;
      return ::GetObject(get_handle(), sizeof(LOGPEN), pLogPen); 
   }


   void pen::construct(int nPenStyle, double dWidth, COLORREF crColor)
   {
      
      if(m_ppen == NULL)
         delete m_ppen;

      m_nPenStyle    = nPenStyle;
      m_dWidth       = dWidth;
      m_crColor      = crColor;
      m_ppen         = new Gdiplus::Pen(Gdiplus::Color(crColor), dWidth);

   }

   void pen::construct(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
      int nStyleCount, const DWORD* lpStyle)
   {
      if (!Attach(::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount,
         lpStyle)))
         throw resource_exception();
   }*/

   /////////////////////////////////////////////////////////////////////////////


   void pen::dump(dump_context & dumpcontext) const
   {
      ::ca::graphics_object::dump(dumpcontext);

/*      if (get_handle() == NULL)
         return;

      if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_PEN)
      {
         // not a valid object
         dumpcontext << "has ILLEGAL HPEN!";
         return;
      }

      LOGPEN lp;
      VERIFY(GetObject(sizeof(lp), &lp));
      dumpcontext << "lgpn.lopnStyle = " << lp.lopnStyle;
      dumpcontext << "\nlgpn.lopnWidth.x (width) = " << lp.lopnWidth.x;
      dumpcontext << "\nlgpn.lopnColor = " << (void *)(dword_ptr)lp.lopnColor;

      dumpcontext << "\n";*/
   }





      // IMPLEMENT_DYNAMIC(resource_exception, base_exception)
      //resource_exception _simpleResourceException(FALSE, __IDS_RESOURCE_EXCEPTION);

      // IMPLEMENT_DYNAMIC(user_exception, base_exception)
      //user_exception _simpleUserException(FALSE, __IDS_USER_EXCEPTION);

      // IMPLEMENT_DYNCREATE(::ca::graphics_sp, ::radix::object)
      // IMPLEMENT_DYNAMIC(CClientDC, ::ca::graphics_sp)
      // IMPLEMENT_DYNAMIC(CWindowDC, ::ca::graphics_sp)
      // IMPLEMENT_DYNAMIC(CPaintDC, ::ca::graphics_sp)
      // IMPLEMENT_DYNCREATE(::ca::graphics_object, ::radix::object)

      // IMPLEMENT_DYNAMIC(pen, ::ca::graphics_object)
      // IMPLEMENT_DYNAMIC(::ca::brush, ::ca::graphics_object)
      // IMPLEMENT_DYNAMIC(::ca::font, ::ca::graphics_object)
      // IMPLEMENT_DYNAMIC(::ca::bitmap, ::ca::graphics_object)
      // IMPLEMENT_DYNAMIC(::ca::palette, ::ca::graphics_object)
      // IMPLEMENT_DYNAMIC(::ca::region, ::ca::graphics_object)

      /////////////////////////////////////////////////////////////////////////////
      // Standard exception processing


   /*void __get_gray_bitmap(::ca::application * papp, const ::ca::bitmap &rSrc, ::ca::bitmap *pDest, COLORREF crBackground)
   {
      ASSERT(pDest);
      ASSERT_KINDOF(::ca::bitmap, pDest);

      BITMAP bm;
      ::ca::graphics_sp graphicsMem, graphicsMask;
      COLORREF cr;
      ::ca::bitmap_sp bmpMask(papp);
      ::ca::bitmap *pOldMask, *pOldMem;
      const DWORD   CP_ROP = 0xE20746;
      ::ca::brush_sp brHighLight(papp, ::GetSysColor(COLOR_3DHIGHLIGHT)),
         brShadow(papp, ::GetSysColor(COLOR_3DSHADOW)), spbr;

      if(graphicsMem->CreateCompatibleDC(NULL) &&
         graphicsMask->CreateCompatibleDC(NULL))
      {
         const_cast<::ca::bitmap &>(rSrc).GetBitmap(&bm);
//         pDest->delete_object();
         if(pDest->CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL) &&
            bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL))
         {
            pOldMem = graphicsMem->SelectObject(const_cast<::ca::bitmap *>(&rSrc));
            pOldMask = graphicsMask->SelectObject(bmpMask);

            ASSERT(pOldMem && pOldMask);
            if(!pOldMem || !pOldMask)
               return;

            // Make the upper left corner pixel the "transparent" pixel
            cr = graphicsMem->SetBkColor(graphicsMem->GetPixel(0, 0));
            graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, SRCCOPY);
            // Make white pixels transparent too
            graphicsMem->SetBkColor(RGB(255, 255, 255));
            graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, NOTSRCERASE);

            if(graphicsMem->SelectObject(pDest))
            {
               graphicsMem->FillSolidRect(0, 0, bm.bmWidth, bm.bmHeight, crBackground);

               graphicsMem->SetBkColor(RGB(255, 255, 255));

               spbr = graphicsMem->SelectObject(brHighLight);
               graphicsMem->BitBlt(1, 1, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

               graphicsMem->SelectObject(brShadow);
               graphicsMem->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

               graphicsMem->SelectObject(spbr);

               graphicsMem->SetBkColor(cr);
            }
            graphicsMask->SelectObject(pOldMask);
            graphicsMem->SelectObject(pOldMem);
         }
      }
   }

   void __draw_gray_bitmap(::ca::application * papp, ::ca::graphics * pgraphics, int x, int y, const ::ca::bitmap &rSrc, COLORREF crBackground)
   {
      ASSERT(pgraphics);
      ASSERT_KINDOF(::ca::graphics_sp, pgraphics);

      BITMAP bm;
      ::ca::graphics_sp graphicsMem, graphicsMask;
      COLORREF cr;
      ::ca::bitmap_sp bmpMask(papp);
      ::ca::bitmap *pOldMask, *pOldMem;
      const DWORD   CP_ROP = 0xE20746;
      ::ca::brush_sp brHighLight(papp, ::GetSysColor(COLOR_3DHIGHLIGHT)),
         brShadow(papp, ::GetSysColor(COLOR_3DSHADOW)), spbr;

      if(graphicsMem->CreateCompatibleDC(pgraphics) &&
         graphicsMask->CreateCompatibleDC(pgraphics) &&
         const_cast<::ca::bitmap &>(rSrc).GetBitmap(&bm) &&
         bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL))
      {
         pOldMem = graphicsMem->SelectObject(const_cast<::ca::bitmap *>(&rSrc));
         pOldMask = graphicsMask->SelectObject(bmpMask);

         ASSERT(pOldMem && pOldMask);
         if(!pOldMem || !pOldMask)
            return;

         cr = graphicsMem->SetBkColor(graphicsMem->GetPixel(0, 0));
         graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, SRCCOPY);
         graphicsMem->SetBkColor(RGB(255, 255, 255));
         graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMem, 0, 0, NOTSRCERASE);

         pgraphics->FillSolidRect(x, y, bm.bmWidth, bm.bmHeight, crBackground);

         pgraphics->SetBkColor(RGB(255, 255, 255));

         spbr = pgraphics->SelectObject(brHighLight);
         pgraphics->BitBlt(x + 1, y + 1, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

         pgraphics->SelectObject(brShadow);
         pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, CP_ROP);

         pgraphics->SelectObject(spbr);

         pgraphics->SetBkColor(cr);
         graphicsMask->SelectObject(pOldMask);
      }
   }

   void __get_dithered_bitmap(::ca::application * papp, const ::ca::bitmap &rSrc, ::ca::bitmap *pDest, COLORREF cr1, COLORREF cr2)
   {
      ASSERT(pDest);
      ASSERT_KINDOF(::ca::bitmap, pDest);

      BITMAP bm;
      ::ca::graphics_sp graphicsSrc, graphicsMask, graphicsDest;
      COLORREF cr;
      ::ca::bitmap_sp bmpMask(papp);
      ::ca::bitmap *pOldMask, *pOldSrc;
      ::ca::brush brChecker;
      static const WORD wPat[8] = {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};

      if(graphicsSrc->CreateCompatibleDC(NULL) &&
         graphicsMask->CreateCompatibleDC(NULL) &&
         graphicsDest->CreateCompatibleDC(NULL))
      {
         if(const_cast<::ca::bitmap &>(rSrc).GetBitmap(&bm))
         {
            //pDest->delete_object();
            if(pDest->CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, NULL))
            {
               // create checker brush
               bmpMask->CreateBitmap(8, 8, 1, 1, wPat);
               brChecker.CreatePatternBrush(bmpMask);
               //bmpMask->delete_object();

               // Mask
               bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

               pOldSrc = graphicsSrc->SelectObject(const_cast<::ca::bitmap *>(&rSrc));
               pOldMask = graphicsMask->SelectObject(bmpMask);

               ASSERT(pOldSrc && pOldMask);
               if(!pOldSrc || !pOldMask)
                  return;

               // Make the upper left corner pixel of the source a "transparent" color
               cr = graphicsSrc->SetBkColor(graphicsSrc->GetPixel(0, 0));
               graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCCOPY);
               // Make white pixels of the source "transparent" too
               graphicsSrc->SetBkColor(RGB(255, 255, 255));
               graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCPAINT);
               graphicsSrc->SetBkColor(cr);

               // Checker the background with white and crBackground
               pDest = graphicsDest->SelectObject(pDest);
               ASSERT(pDest);
               if(pDest)
               {
                  cr1 = graphicsDest->SetTextColor(cr1);
                  cr2 = graphicsDest->SetBkColor(cr2);
                  graphicsDest->FillRect(rect(0, 0, bm.bmWidth, bm.bmHeight), &brChecker);
                  graphicsDest->SetTextColor(cr1);
                  graphicsDest->SetBkColor(cr2);

                  // Blt it
                  graphicsDest->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);
                  graphicsDest->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, SRCAND);
                  graphicsDest->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);
               }
               graphicsDest->SelectObject(pDest);
               graphicsMask->SelectObject(pOldMask);
               graphicsSrc->SelectObject(pOldSrc);
            }
         }
      }
   }

   void __draw_dithered_bitmap(::ca::application * papp, ::ca::graphics * pgraphics, int x, int y, const ::ca::bitmap &rSrc, COLORREF cr1, COLORREF cr2)
   {
      ASSERT(pgraphics);
      ASSERT_KINDOF(::ca::graphics_sp, pgraphics);

      BITMAP bm;
      ::ca::graphics_sp graphicsSrc, graphicsMask;
      COLORREF cr;
      ::ca::bitmap_sp bmpMask(papp);
      ::ca::bitmap *pOldMask, *pOldSrc;
      ::ca::brush brChecker;
      static const WORD wPat[8] = {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa};

      if(graphicsSrc->CreateCompatibleDC(pgraphics) &&
         graphicsMask->CreateCompatibleDC(pgraphics) &&
         const_cast<::ca::bitmap &>(rSrc).GetBitmap(&bm))
      {
         // create checker brush
         bmpMask->CreateBitmap(8, 8, 1, 1, wPat);
         brChecker.CreatePatternBrush(bmpMask);
         //bmpMask->delete_object();

         // Mask
         bmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

         pOldSrc = graphicsSrc->SelectObject(const_cast<::ca::bitmap *>(&rSrc));
         pOldMask = graphicsMask->SelectObject(bmpMask);

         ASSERT(pOldSrc && pOldMask);
         if(!pOldSrc || !pOldMask)
            return;

         // Make the upper left corner pixel of the source a "transparent" color
         cr = graphicsSrc->SetBkColor(graphicsSrc->GetPixel(0, 0));
         graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCCOPY);
         // Make white pixels of the source "transparent" too
         graphicsSrc->SetBkColor(RGB(255, 255, 255));
         graphicsMask->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCPAINT);
         graphicsSrc->SetBkColor(cr);

         // Checker the background with white and crBackground
         cr1 = pgraphics->SetTextColor(cr1);
         cr2 = pgraphics->SetBkColor(cr2);
         pgraphics->FillRect(rect(x, y, x + bm.bmWidth, y + bm.bmHeight), &brChecker);
         pgraphics->SetTextColor(cr1);
         pgraphics->SetBkColor(cr2);

         // Blt it
         pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);
         pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsMask, 0, 0, SRCAND);
         pgraphics->BitBlt(x, y, bm.bmWidth, bm.bmHeight, graphicsSrc, 0, 0, SRCINVERT);

         graphicsMask->SelectObject(pOldMask);
         graphicsSrc->SelectObject(pOldSrc);
      }
   }

*/

   int_ptr pen::get_os_data() const
   {
      
      if(m_ppen == NULL || !m_bUpdated)
      {
         if(m_ppen != NULL)
         {
            delete m_ppen;
         }
         ((pen *) this)->m_ppen = new Gdiplus::Pen(Gdiplus::Color(m_crPenColor), (Gdiplus::REAL) m_dPenWidth);
         if(m_nPenStyle & PS_JOIN_ROUND)
         {
            ((pen *) this)->m_ppen->SetLineJoin(Gdiplus::LineJoinRound);
         }
         if(m_nPenStyle & PS_ENDCAP_ROUND)
         {
            ((pen *) this)->m_ppen->SetEndCap(Gdiplus::LineCapRound);
         }
      }

      if(m_ppen != NULL)
      {
         ((pen *) this)->m_bUpdated = true;
      }

      return (int_ptr) (Gdiplus::Pen *) m_ppen;

   }


} // namespace win