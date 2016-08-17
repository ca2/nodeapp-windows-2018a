#pragma once


struct spa_canvas_toolbox;


class a_spa;


class SPALIB_API spa_canvas
{
public:


   a_spa *                    m_pspa;

   spa_canvas_toolbox *       m_ptoolbox;

   string                     m_strLastStatus;

   int                        m_cx;

   int                        m_cy;

   COLORREF *                 m_pcolorref;

   HBITMAP                    m_hbitmap;
   
   HDC                        m_hdc;

   HBRUSH                     m_hbrushBk;

   HBITMAP                    m_hbmAlpha;
   
   HDC                        m_hdcAlpha;

   bool                       m_bDraw;


   spa_canvas(a_spa * pspa, int cx, int cy);
   ~spa_canvas();

   bool update_layered_window();

   void init_toolbox();

};


//SPALIB_API HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdcParam);
//SPALIB_API BOOL TextOutU(HDC hdc, int x, int y, const char * pszUtf8, int iSize);
SPALIB_API void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr);
//SPALIB_API HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdc);



LONG width(LPCRECT lpcrect);
LONG height(LPCRECT lpcrect);
