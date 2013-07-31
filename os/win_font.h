#pragma once


namespace win
{


   class CLASS_DECL_win font : 
      virtual public ::draw2d::font
   {
   public:

      
      Gdiplus::Font *   m_pfont;


      font(sp(::ca2::application) papp);
      virtual ~font();


      virtual void * get_os_data() const;


      virtual void font::construct(const ::draw2d::font & fontParam);

      /*
      virtual bool CreateFontIndirect(const LOGFONT* lpLogFont);
      virtual bool CreateFont(int32_t nHeight, int32_t nWidth, int32_t nEscapement,
            int32_t nOrientation, int32_t nWeight, BYTE bItalic, BYTE bUnderline,
            BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
            BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
            const char * lpszFacename);
      virtual bool CreatePointFont(int32_t nPointSize, const char * lpszFaceName, ::draw2d::graphics * pgraphics = NULL);
      virtual bool CreatePointFontIndirect(const LOGFONT* lpLogFont, ::draw2d::graphics * pgraphics = NULL);
      */

      //virtual int32_t GetLogFont(LOGFONT* pLogFont);

      virtual void dump(dump_context & dumpcontext) const;


   };


} // namespace win



