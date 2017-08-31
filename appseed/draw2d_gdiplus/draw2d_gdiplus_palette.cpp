#include "framework.h"


namespace draw2d_gdiplus
{

   palette::palette(::aura::application * papp) :
      ::object(papp)
   { }
   palette::~palette()
   { }
   // palette
   palette::operator HPALETTE() const
   { 
      return (HPALETTE)(this == NULL ? NULL : get_os_data()); 
   }
   palette* palette::from_handle(::aura::application * papp, HPALETTE hPalette)
   { 
      //return dynamic_cast < palette * > (::draw2d_gdiplus::object::from_handle(papp, hPalette)); 
      return NULL;
   }
   bool palette::CreatePalette(LPLOGPALETTE lpLogPalette)
   { 
      //return attach(::CreatePalette(lpLogPalette)); 
      return FALSE;
   }
   bool palette::CreateHalftonePalette(::draw2d::dib * pdib)
   {
      //ASSERT(pgraphics != NULL && (dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1() != NULL); 
      //return attach(::CreateHalftonePalette((dynamic_cast<::draw2d_gdiplus::graphics * >(pgraphics))->get_handle1())); 
      return FALSE;

   }
   UINT palette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors) const
   { ASSERT(get_os_data() != NULL); return ::GetPaletteEntries((HPALETTE)get_os_data(), nStartIndex,
   nNumEntries, lpPaletteColors); }
   UINT palette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors)
   { ASSERT(get_os_data() != NULL); return ::SetPaletteEntries((HPALETTE)get_os_data(), nStartIndex,
   nNumEntries, lpPaletteColors); }
   void palette::AnimatePalette(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors)
   { ASSERT(get_os_data() != NULL); ::AnimatePalette((HPALETTE)get_os_data(), nStartIndex, nNumEntries,
   lpPaletteColors); }
   UINT palette::GetNearestPaletteIndex(COLORREF crColor) const
   { ASSERT(get_os_data() != NULL); return ::GetNearestPaletteIndex((HPALETTE)get_os_data(), crColor); }
   
   bool palette::ResizePalette(UINT nNumEntries)
   {
      ASSERT(get_os_data() != NULL); 
      return ::ResizePalette((HPALETTE)get_os_data(), nNumEntries) != FALSE;
   }

   int32_t palette::GetEntryCount()
   { ASSERT(get_os_data() != NULL); WORD nEntries;
   ::GetObject((HANDLE) get_os_data(), sizeof(WORD), &nEntries); return (int32_t)nEntries; }

} // namespace draw2d_gdiplus