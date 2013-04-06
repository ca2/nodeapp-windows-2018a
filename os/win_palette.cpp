#include "framework.h"

namespace win
{

   palette::palette(sp(::ca::application) papp) :
      ca(papp)
   { }
   palette::~palette()
   { }
   // palette
   palette::operator HPALETTE() const
   { 
      return (HPALETTE)(this == ::null() ? ::null() : get_os_data()); 
   }
   palette* palette::from_handle(sp(::ca::application) papp, HPALETTE hPalette)
   { 
      //return dynamic_cast < palette * > (::win::graphics_object::from_handle(papp, hPalette)); 
      return ::null();
   }
   bool palette::CreatePalette(LPLOGPALETTE lpLogPalette)
   { 
      //return attach(::CreatePalette(lpLogPalette)); 
      return FALSE;
   }
   bool palette::CreateHalftonePalette(::ca::graphics * pgraphics)
   {
      //ASSERT(pgraphics != ::null() && (dynamic_cast<::win::graphics * >(pgraphics))->get_handle1() != ::null()); 
      //return attach(::CreateHalftonePalette((dynamic_cast<::win::graphics * >(pgraphics))->get_handle1())); 
      return FALSE;

   }
   UINT palette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors) const
   { ASSERT(get_os_data() != ::null()); return ::GetPaletteEntries((HPALETTE)get_os_data(), nStartIndex,
   nNumEntries, lpPaletteColors); }
   UINT palette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors)
   { ASSERT(get_os_data() != ::null()); return ::SetPaletteEntries((HPALETTE)get_os_data(), nStartIndex,
   nNumEntries, lpPaletteColors); }
   void palette::AnimatePalette(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors)
   { ASSERT(get_os_data() != ::null()); ::AnimatePalette((HPALETTE)get_os_data(), nStartIndex, nNumEntries,
   lpPaletteColors); }
   UINT palette::GetNearestPaletteIndex(COLORREF crColor) const
   { ASSERT(get_os_data() != ::null()); return ::GetNearestPaletteIndex((HPALETTE)get_os_data(), crColor); }
   
   bool palette::ResizePalette(UINT nNumEntries)
   {
      ASSERT(get_os_data() != ::null()); 
      return ::ResizePalette((HPALETTE)get_os_data(), nNumEntries) != FALSE;
   }

   int32_t palette::GetEntryCount()
   { ASSERT(get_os_data() != ::null()); WORD nEntries;
   ::GetObject((HANDLE) get_os_data(), sizeof(WORD), &nEntries); return (int32_t)nEntries; }

} // namespace win