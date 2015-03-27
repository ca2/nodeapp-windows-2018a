#pragma once


SPALIB_API HFONT CreatePointFont(int nPointSize,const char * lpszFaceName,HDC hdc,bool bUnderline,int iWeight = FW_NORMAL);
// pLogFont->nHeight is interpreted as PointSize * 10
SPALIB_API HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdcParam);
SPALIB_API BOOL TextOutU(HDC hdc, int x, int y, const char * pszUtf8, int iSize);
SPALIB_API void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr);
SPALIB_API HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdc);


SPALIB_API void ca2_install_canvas_on_paint(HDC hdc, LPCRECT lpcrect, int iMode);