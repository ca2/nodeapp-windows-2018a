#include "framework.h"

#undef new

namespace draw2d_gdiplus
{


   font::font(sp(::aura::application) papp) :
   element(papp)
   {

      m_pfont     = NULL;
      m_bUpdated  = false;

   }

   font::font(const font & font):
      element(font.get_app())
   {

      m_pfont     = NULL;
      m_bUpdated  = false;

      ::draw2d::font::operator = (font);

   }


   font::~font()
   { 

      if(m_pfont != NULL)
      {
         delete m_pfont;
         m_pfont = NULL;
      }
   
   }

//   font::operator HFONT() const
  // {
    //  return (HFONT)(this == NULL ? NULL : get_handle()); 
   //}
   //font* font::from_handle(sp(::aura::application) papp, HFONT hFont)
   //{
     // return dynamic_cast < font * > (::draw2d_gdiplus::object::from_handle(papp, hFont)); 
   //}
/*   bool font::CreateFontIndirect(const LOGFONTW* lpLogFont)
   { 

      m_strFontFamilyName = lpLogFont->lfFaceName;
      if(lpLogFont->lfHeight < 0)
         m_dFontSize = (int32_t) abs(lpLogFont->lfHeight);
      else
         m_dFontSize = lpLogFont->lfHeight / 10.0;
      m_eunitFontSize = ::draw2d::unit_point;
      m_bBold = lpLogFont->lfWeight >= 500;
      m_bItalic = lpLogFont->lfItalic != FALSE;
      m_bStrikeout = lpLogFont->lfStrikeOut != FALSE;

      return TRUE;
      //return attach(::CreateFontIndirect(lpLogFont)); 
   }
   bool font::CreateFont(int32_t nHeight, int32_t nWidth, int32_t nEscapement,
      int32_t nOrientation, int32_t nWeight, BYTE bItalic, BYTE bUnderline,
      BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
      BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
      const char * lpszFacename)
   { 
      return FALSE;
      //return attach(::CreateFont(nHeight, nWidth, nEscapement,
   //nOrientation, nWeight, bItalic, bUnderline, cStrikeOut,
   //nCharSet, nOutPrecision, nClipPrecision, nQuality,
   //nPitchAndFamily, lpszFacename)); 
   }
   
   int32_t font::GetLogFont(LOGFONTW* pLogFont)
   { 

      return 0;
   //   return ::GetObject(get_handle(), sizeof(LOGFONTW), pLogFont);

   }*/


      /////////////////////////////////////////////////////////////////////////////

   void font::construct(const ::draw2d::font & fontParam)
      {
         class font & font = const_cast < ::draw2d_gdiplus::font & > (dynamic_cast < const ::draw2d_gdiplus::font & > (fontParam));
         if(font.m_pfont == NULL)
         {
            if(m_pfont != NULL)
            {
               delete m_pfont;
               m_pfont = NULL;
            }
         }
         else
         {
            m_pfont = font.m_pfont->Clone();
         }
//         if(get_handle() != NULL)
  //          delete_object();
    //     if(font.get_handle() != NULL)
      /*   {
            LOGFONTW lf;
            memset(&lf, 0, sizeof(lf));
            font.GetLogFont(&lf);
            CreateFontIndirect(&lf);
         }*/
      }


      void font::dump(dump_context & dumpcontext) const
      {
         ::draw2d::object::dump(dumpcontext);

/*         if (get_handle() == NULL)
            return;

         if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_FONT)
         {
            // not a valid GDI object
            dumpcontext << "has ILLEGAL HFONT!";
            return;
         }

         LOGFONTW lf;
         VERIFY(GetObject(sizeof(lf), &lf));
         dumpcontext << "lf.lfHeight = " << lf.lfHeight;
         dumpcontext << "\nlf.lfWidth = " << lf.lfWidth;
         dumpcontext << "\nlf.lfEscapement = " << lf.lfEscapement;
         dumpcontext << "\nlf.lfOrientation = " << lf.lfOrientation;
         dumpcontext << "\nlf.lfWeight = " << lf.lfWeight;
         dumpcontext << "\nlf.lfItalic = " << (int32_t)lf.lfItalic;
         dumpcontext << "\nlf.lfUnderline = " << (int32_t)lf.lfUnderline;
         dumpcontext << "\nlf.lfStrikeOut = " << (int32_t)lf.lfStrikeOut;
         dumpcontext << "\nlf.lfCharSet = " << (int32_t)lf.lfCharSet;
         dumpcontext << "\nlf.lfOutPrecision = " << (int32_t)lf.lfOutPrecision;
         dumpcontext << "\nlf.lfClipPrecision = " << (int32_t)lf.lfClipPrecision;
         dumpcontext << "\nlf.lfQuality = " << (int32_t)lf.lfQuality;
         dumpcontext << "\nlf.lfPitchAndFamily = " << (int32_t)lf.lfPitchAndFamily;
         dumpcontext << "\nlf.lfFaceName = " << (const char *)lf.lfFaceName;

         dumpcontext << "\n";*/
      }




   void * font::get_os_data() const
   {
      
      if(m_pfont == NULL || !m_bUpdated)
      {
         if(m_pfont != NULL)
         {
            try
            {
               delete ((font *) this)->m_pfont;
            }
            catch(...)
            {
            }
            ((font *) this)->m_pfont = NULL;
         }


         int32_t iStyle = 0;

         if(m_iFontWeight >= 600)
         {
            iStyle |= (int32_t) Gdiplus::FontStyleBold;
         }

         if(m_bItalic)
         {
            iStyle |= (int32_t) Gdiplus::FontStyleItalic;
         }

         if(m_bUnderline)
         {
            iStyle |= (int32_t) Gdiplus::FontStyleUnderline;
         }

         if(m_bStrikeout)
         {
            iStyle |= (int32_t) Gdiplus::FontStyleStrikeout;
         }

         Gdiplus::Unit unit;

         switch(m_eunitFontSize)
         {
         case ::draw2d::unit_pixel:
            unit = Gdiplus::UnitPixel;
            break;
         case ::draw2d::unit_point:
            unit = Gdiplus::UnitPoint;
            break;
         default:
            unit = Gdiplus::UnitPoint;
            break;
         };
         
         //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

         ((font *) this)->m_pfont = new Gdiplus::Font(
               ::str::international::utf8_to_unicode(m_strFontFamilyName),
               (Gdiplus::REAL) m_dFontSize,
               iStyle,
               unit);

      }


      if(m_pfont != NULL)
      {
         ((font *) this)->m_bUpdated = true;
      }

      return (void *) (Gdiplus::Font *) m_pfont;

   }

} // namespace draw2d_gdiplus