#pragma once


namespace draw2d_gdi
{


   class CLASS_DECL_DRAW2D_GDI palette : 
      virtual public ::draw2d_gdi::object,
      virtual public ::draw2d::palette
   {
   public:


      palette(base_application * papp);
      virtual ~palette();


      operator HPALETTE() const;
      int GetEntryCount();

      static ::draw2d::palette * from_handle(base_application * papp, HPALETTE hPalette);

      bool CreatePalette(LPLOGPALETTE lpLogPalette);
      bool CreateHalftonePalette(::draw2d::graphics * pgraphics);


      UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
      UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);

      void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);
      UINT GetNearestPaletteIndex(COLORREF crColor) const;
      bool ResizePalette(UINT nNumEntries);

      
   };


} // namespace draw2d_gdi



