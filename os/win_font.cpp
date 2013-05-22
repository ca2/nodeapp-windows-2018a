#include "framework.h"

#undef new

namespace win
{


   font::font(sp(::ca::application) papp) :
   ca(papp)
   {

      m_pfont     = ::null();
      m_bUpdated  = false;

   }


   font::~font()
   { 

      if(m_pfont != ::null())
      {
         delete m_pfont;
         m_pfont = ::null();
      }
   
   }

//   font::operator HFONT() const
  // {
    //  return (HFONT)(this == ::null() ? ::null() : get_handle()); 
   //}
   //font* font::from_handle(sp(::ca::application) papp, HFONT hFont)
   //{
     // return dynamic_cast < font * > (::win::graphics_object::from_handle(papp, hFont)); 
   //}
/*   bool font::CreateFontIndirect(const LOGFONT* lpLogFont)
   { 

      m_strFontFamilyName = lpLogFont->lfFaceName;
      if(lpLogFont->lfHeight < 0)
         m_dFontSize = (int32_t) abs(lpLogFont->lfHeight);
      else
         m_dFontSize = lpLogFont->lfHeight / 10.0;
      m_eunitFontSize = ::ca::unit_point;
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
   
   int32_t font::GetLogFont(LOGFONT* pLogFont)
   { 

      return 0;
   //   return ::GetObject(get_handle(), sizeof(LOGFONT), pLogFont);

   }*/


      /////////////////////////////////////////////////////////////////////////////

   void font::construct(const ::ca::font & fontParam)
      {
         class font & font = const_cast < ::win::font & > (dynamic_cast < const ::win::font & > (fontParam));
         if(font.m_pfont == ::null())
         {
            if(m_pfont != ::null())
            {
               delete m_pfont;
               m_pfont = ::null();
            }
         }
         else
         {
            m_pfont = font.m_pfont->Clone();
         }
//         if(get_handle() != ::null())
  //          delete_object();
    //     if(font.get_handle() != ::null())
      /*   {
            LOGFONT lf;
            memset(&lf, 0, sizeof(lf));
            font.GetLogFont(&lf);
            CreateFontIndirect(&lf);
         }*/
      }


      void font::dump(dump_context & dumpcontext) const
      {
         ::ca::graphics_object::dump(dumpcontext);

/*         if (get_handle() == ::null())
            return;

         if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_FONT)
         {
            // not a valid GDI object
            dumpcontext << "has ILLEGAL HFONT!";
            return;
         }

         LOGFONT lf;
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
      
      if(m_pfont == ::null() || !m_bUpdated)
      {
         if(m_pfont != ::null())
         {
            try
            {
               delete ((font *) this)->m_pfont;
            }
            catch(...)
            {
            }
            ((font *) this)->m_pfont = ::null();
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
         case ::ca::unit_pixel:
            unit = Gdiplus::UnitPixel;
            break;
         case ::ca::unit_point:
            unit = Gdiplus::UnitPoint;
            break;
         default:
            unit = Gdiplus::UnitPoint;
            break;
         };
         
         //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

         ((font *) this)->m_pfont = new Gdiplus::Font(
               ::ca::international::utf8_to_unicode(m_strFontFamilyName),
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

} // namespace win