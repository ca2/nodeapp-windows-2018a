#pragma once


namespace win
{


   class CLASS_DECL_win font : 
      virtual public ::ca::font
   {
   public:

      
      Gdiplus::Font *   m_pfont;


      font(::ca::application * papp);
      virtual ~font();


      virtual void * get_os_data() const;


      virtual void font::construct(const ::ca::font & fontParam);

      /*
      virtual bool CreateFontIndirect(const LOGFONT* lpLogFont);
      virtual bool CreateFont(int32_t nHeight, int32_t nWidth, int32_t nEscapement,
            int32_t nOrientation, int32_t nWeight, BYTE bItalic, BYTE bUnderline,
            BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
            BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
            const char * lpszFacename);
      virtual bool CreatePointFont(int32_t nPointSize, const char * lpszFaceName, ::ca::graphics * pgraphics = NULL);
      virtual bool CreatePointFontIndirect(const LOGFONT* lpLogFont, ::ca::graphics * pgraphics = NULL);
      */

      //virtual int32_t GetLogFont(LOGFONT* pLogFont);

      virtual void dump(dump_context & dumpcontext) const;


   };


} // namespace win



