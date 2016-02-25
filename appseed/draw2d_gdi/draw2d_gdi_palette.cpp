#include "framework.h"


namespace draw2d_gdi
{


   palette::palette(::aura::application * papp) :
      ::object(papp)
   {
   
   }


   palette::~palette()
   {
   
   }


   
   palette::operator HPALETTE() const
   { 

      return (HPALETTE)(this == NULL ? NULL : get_os_data()); 

   }


   bool palette::CreatePalette(LPLOGPALETTE lpLogPalette)
   { 
      
      return Attach(::CreatePalette(lpLogPalette)); 
   
   }


   bool palette::CreateHalftonePalette(::draw2d::dib * pdib)
   {
      
      ASSERT(pgraphics != NULL && (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1() != NULL); 
      
      return Attach(::CreateHalftonePalette((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1())); 
   
   }


   UINT palette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
   { 
      
      ASSERT(get_os_data() != NULL); 
      
      return ::GetPaletteEntries((HPALETTE)get_os_data(), nStartIndex, nNumEntries, lpPaletteColors); 
   
   }
   
   
   UINT palette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
   { 
      
      ASSERT(get_os_data() != NULL);
      
      return ::SetPaletteEntries((HPALETTE)get_os_data(), nStartIndex, nNumEntries, lpPaletteColors); 
   
   }
   
   
   void palette::AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
   { 
      
      ASSERT(get_os_data() != NULL);
      
      ::AnimatePalette((HPALETTE)get_os_data(), nStartIndex, nNumEntries, lpPaletteColors); 
   
   }
   
   
   UINT palette::GetNearestPaletteIndex(COLORREF crColor) const
   {
      
      ASSERT(get_os_data() != NULL);
      
      return ::GetNearestPaletteIndex((HPALETTE)get_os_data(), crColor); 
   
   }


   bool palette::ResizePalette(UINT nNumEntries)
   { 
      
      ASSERT(get_os_data() != NULL); 
      
      return ::ResizePalette((HPALETTE)get_os_data(), nNumEntries) != FALSE; 
   
   }


   int palette::GetEntryCount()
   {
      
      ASSERT(get_os_data() != NULL); 
      
      WORD nEntries;
   
      get_object(sizeof(WORD), &nEntries);
      
      return (int)nEntries; 
   
   }


} // namespace draw2d_gdi





