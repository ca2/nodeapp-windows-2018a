#pragma once

namespace win
{

   class CLASS_DECL_win palette : 
      virtual public ::win::object,
      virtual public ::draw2d::palette
   {
   public:
      static palette* from_handle(sp(::ca2::application) papp, HPALETTE hPalette);

      palette(sp(::ca2::application) papp);
      bool CreatePalette(LPLOGPALETTE lpLogPalette);
      bool CreateHalftonePalette(::draw2d::graphics * pgraphics);

   // Attributes
      operator HPALETTE() const;
      int32_t GetEntryCount();
      UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
            LPPALETTEENTRY lpPaletteColors) const;
      UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
            LPPALETTEENTRY lpPaletteColors);

   // Operations
      void AnimatePalette(UINT nStartIndex, UINT nNumEntries,
            LPPALETTEENTRY lpPaletteColors);
      UINT GetNearestPaletteIndex(COLORREF crColor) const;
      bool ResizePalette(UINT nNumEntries);

   // Implementation
      virtual ~palette();
   };

} // namespace win