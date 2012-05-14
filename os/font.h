#pragma once

#undef new

#include <GdiPlus.h>


namespace win
{

   class CLASS_DECL_win font : 
      virtual public ::ca::font
   {
   public:

      
      Gdiplus::Font *   m_pfont;


      font(::ca::application * papp);
      virtual ~font();


      virtual INT_PTR get_os_data() const;


      virtual void font::construct(const ::ca::font & fontParam);

      virtual BOOL CreateFontIndirect(const LOGFONT* lpLogFont);
      virtual BOOL CreateFont(int nHeight, int nWidth, int nEscapement,
            int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
            BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
            BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
            const char * lpszFacename);
      virtual BOOL CreatePointFont(int nPointSize, const char * lpszFaceName, ::ca::graphics * pgraphics = NULL);
      virtual BOOL CreatePointFontIndirect(const LOGFONT* lpLogFont, ::ca::graphics * pgraphics = NULL);

      virtual int GetLogFont(LOGFONT* pLogFont);

         virtual void dump(dump_context & dumpcontext) const;


   };

} // namespace win

#define new DEBUG_NEW