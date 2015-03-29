#pragma once

#include <gdiplus.h>

using namespace Gdiplus;


SPALIB_API Font * CreatePointFont(double dPointSize,const wchar_t * lpszFaceName,bool bUnderline,bool bBold);
//SPALIB_API HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdcParam);
//SPALIB_API BOOL TextOutU(HDC hdc, int x, int y, const char * pszUtf8, int iSize);
SPALIB_API void FillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr);
//SPALIB_API HFONT CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hdc);


SPALIB_API void ca2_install_canvas_on_paint(HDC hdc, LPCRECT lpcrect, int iMode);

LONG width(LPCRECT lpcrect);
LONG height(LPCRECT lpcrect);
Rect make_rect(LPCRECT lpcrect);