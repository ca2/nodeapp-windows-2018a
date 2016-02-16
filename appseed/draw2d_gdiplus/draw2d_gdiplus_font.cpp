#include "framework.h"

#undef new

namespace draw2d_gdiplus
{


   font::font(::aura::application * papp) :
      ::object(papp)
   {

      m_pfont     = NULL;
      m_bUpdated  = false;

   }

   font::font(const font & font):
      ::object(font.get_app())
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
      }


      void font::dump(dump_context & dumpcontext) const
      {
         ::draw2d::object::dump(dumpcontext);

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