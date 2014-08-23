#include "framework.h"


namespace draw2d_gdi
{


   font::font(sp(::aura::application) papp) :
      element(papp)
   { 

   }


   font::~font()
   {

   }


   font::operator HFONT() const
   {

      return (HFONT)(this == NULL ? NULL : get_handle()); 

   }


   bool font::CreateFontIndirect(const LOGFONTW * lpLogFont)
   { 

      return Attach(::CreateFontIndirectW(lpLogFont)); 

   }


   bool font::CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision, BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily, const char * lpszFacename)
   { 

      return Attach(::CreateFont(nHeight, nWidth, nEscapement, nOrientation, nWeight, bItalic, bUnderline, cStrikeOut, nCharSet, nOutPrecision, nClipPrecision, nQuality,  nPitchAndFamily, lpszFacename)); 

   }


   int font::GetLogFont(LOGFONTW * pLogFont)
   { 

      return ::GetObjectW(get_handle(), sizeof(LOGFONTW), pLogFont);

   }


   void font::construct(const ::draw2d::font & fontParam)
   {

      class font & font = const_cast < ::draw2d_gdi::font & > (dynamic_cast < const ::draw2d_gdi::font & > (fontParam));

      if(get_handle() != NULL)
         destroy();

      if(font.get_handle() != NULL)
      {
         LOGFONTW lf;
         memset(&lf, 0, sizeof(lf));
         font.GetLogFont(&lf);
         CreateFontIndirect(&lf);
      }

   }

#ifdef _DEBUG

   void font::dump(dump_context & dumpcontext) const
   {
      ::draw2d::object::dump(dumpcontext);

      if (get_handle() == NULL)
         return;

      /*         if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_FONT)
      {
      // not a valid GDI object
      dumpcontext << "has ILLEGAL HFONT!";
      return;
      }*/

      LOGFONTW lf;
      VERIFY(get_object(sizeof(lf), &lf));
      dumpcontext << "lf.lfHeight = " << lf.lfHeight;
      dumpcontext << "\nlf.lfWidth = " << lf.lfWidth;
      dumpcontext << "\nlf.lfEscapement = " << lf.lfEscapement;
      dumpcontext << "\nlf.lfOrientation = " << lf.lfOrientation;
      dumpcontext << "\nlf.lfWeight = " << lf.lfWeight;
      dumpcontext << "\nlf.lfItalic = " << (int)lf.lfItalic;
      dumpcontext << "\nlf.lfUnderline = " << (int)lf.lfUnderline;
      dumpcontext << "\nlf.lfStrikeOut = " << (int)lf.lfStrikeOut;
      dumpcontext << "\nlf.lfCharSet = " << (int)lf.lfCharSet;
      dumpcontext << "\nlf.lfOutPrecision = " << (int)lf.lfOutPrecision;
      dumpcontext << "\nlf.lfClipPrecision = " << (int)lf.lfClipPrecision;
      dumpcontext << "\nlf.lfQuality = " << (int)lf.lfQuality;
      dumpcontext << "\nlf.lfPitchAndFamily = " << (int)lf.lfPitchAndFamily;
      dumpcontext << "\nlf.lfFaceName = " << (const char *)lf.lfFaceName;

      dumpcontext << "\n";
   }
#endif

   /////////////////////////////////////////////////////////////////////////////
   // out-of-line ::draw2d::brush, font, etc. helpers

   // nPointSize is actually scaled 10x
   bool font::CreatePointFont(int nPointSize, const char * lpszFaceName, ::draw2d::graphics * pgraphics)
   {
      
      LOGFONTW logFont;
      
      ZERO(logFont);

      logFont.lfCharSet = DEFAULT_CHARSET;

      logFont.lfHeight = nPointSize;

      wstring wstr(lpszFaceName);

      wstr = wstr.substr(0, sizeof(logFont.lfFaceName));

      wcsncpy(logFont.lfFaceName, wstr, sizeof(logFont.lfFaceName) / sizeof(wchar_t));

      return CreatePointFontIndirect(&logFont, pgraphics);

   }

   // pLogFont->nHeight is interpreted as PointSize * 10
   bool font::CreatePointFontIndirect(const LOGFONTW* lpLogFont, ::draw2d::graphics * pgraphics)
   {
      HDC hDC;
      if (pgraphics != NULL)
      {
         ASSERT_VALID(pgraphics);
         ASSERT((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle2() != NULL);
         hDC = (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle2();
      }
      else
         hDC = ::GetDC(NULL);

      // convert nPointSize to logical units based on pgraphics
      LOGFONTW logFont = *lpLogFont;
      POINT pt;
      // 72 points/inch, 10 decipoints/point
      pt.y = ::MulDiv(::GetDeviceCaps(hDC, LOGPIXELSY), logFont.lfHeight, 720);
      pt.x = 0;
      ::DPtoLP(hDC, &pt, 1);
      POINT ptOrg = { 0, 0 };
      ::DPtoLP(hDC, &ptOrg, 1);
      logFont.lfHeight = -abs(pt.y - ptOrg.y);

      if (pgraphics == NULL)
         ReleaseDC(NULL, hDC);


      logFont.lfQuality = ANTIALIASED_QUALITY;

      return CreateFontIndirect(&logFont);

   }


   bool font::create()
   {

      ::draw2d_gdi::object::create();

      if(m_eunitFontSize == ::draw2d::unit_pixel)
      {
      
         CreatePointFont((int32_t) (m_dFontSize * 720.0 / 96.0), m_strFontFamilyName, NULL);

      }
      else
      {
         CreatePointFont((int32_t) (m_dFontSize * 10.0), m_strFontFamilyName, NULL);
      }

      return true;
      
   }

} // namespace draw2d_gdi







