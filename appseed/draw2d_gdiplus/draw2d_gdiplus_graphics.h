#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS graphics :
      virtual public ::draw2d::graphics
   {
   public:


      ::Gdiplus::Matrix *           m_pm;
      ::Gdiplus::Graphics *         m_pgraphics;
      ::Gdiplus::GraphicsPath *     m_ppath;
      ::Gdiplus::GraphicsPath *     m_ppathPaint;
      HDC                           m_hdc;
      HDC                           m_hdcGraphics;


      graphics(::aura::application * papp);
      virtual ~graphics();


      Gdiplus::Graphics & g()
      {
         if(m_pgraphics == NULL)
         {
            m_pgraphics = ::Gdiplus::Graphics::FromHDC(get_handle1());
         }
         return *m_pgraphics;
      }

//      ::window_sp GetWindow() override;

      virtual bool attach(void * pgraphics) override;   // attach/detach affects only the Output DC
      virtual void * detach() override;

      virtual bool Attach(HDC hdc) override;   // attach/detach affects only the Output DC
      virtual HDC Detach() override;
      virtual bool AttachPrinter(HDC hdc) override;
      virtual HDC DetachPrinter() override;


      //virtual void SetAttribDC(HDC hDC) override;  // set the Attribute DC
      //virtual void SetOutputDC(HDC hDC) override;  // set the Output DC
      //virtual void ReleaseAttribDC() override;     // Release the Attribute DC
      //virtual void ReleaseOutputDC() override;     // Release the Output DC

      bool IsPrinting() override;            // TRUE if being used for printing

      ::draw2d::pen_sp     get_current_pen() override;
      ::draw2d::brush_sp   get_current_brush() override;
      ::draw2d::palette_sp get_current_palette() override;
      ::draw2d::font_sp    get_current_font() override;
      ::draw2d::bitmap_sp  get_current_bitmap() override;


      Gdiplus::Pen *       gdiplus_pen();
      Gdiplus::Brush *     gdiplus_brush();
      Gdiplus::Font *      gdiplus_font();

      // for bidi and mirrored localization
      uint32_t GetLayout() override;
      uint32_t SetLayout(uint32_t dwLayout) override;


      HDC get_hdc();
      void release_hdc(HDC hdc);


      virtual f64 get_dpix() override;
      virtual f64 get_dpiy() override;

      // Constructors
      bool CreateDC(const char * lpszDriverName, const char * lpszDeviceName,
                    const char * lpszOutput, const void * lpInitData) override;
      bool CreateIC(const char * lpszDriverName, const char * lpszDeviceName,
                    const char * lpszOutput, const void * lpInitData) override;
      bool CreateCompatibleDC(::draw2d::graphics * pgraphics) override;

      bool DeleteDC() override;

      // Device-Context Functions
      virtual i32 SaveDC() override;
      virtual bool RestoreDC(i32 nSavedDC) override;
      i32 GetDeviceCaps(i32 nIndex) override;
      UINT SetBoundsRect(const RECT &  lpRectBounds, UINT flags) override;
      UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags) override;
      bool ResetDC(const DEVMODE* lpDevMode) override;

      // Drawing-Tool Functions
      point GetBrushOrg() override;
      point SetBrushOrg(i32 x, i32 y) override;
      point SetBrushOrg(POINT point) override;
      i32 EnumObjects(i32 nObjectType, i32 (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData) override;


      virtual ::draw2d::object* SelectStockObject(i32 nIndex) override;
      ::draw2d::pen* SelectObject(::draw2d::pen* pPen) override;
      ::draw2d::brush* SelectObject(::draw2d::brush* pBrush) override;
      virtual ::draw2d::font* SelectObject(::draw2d::font* pFont) override;
      ::draw2d::bitmap* SelectObject(::draw2d::bitmap* pBitmap) override;
      i32 SelectObject(::draw2d::region* pRgn) override;       // special return for regions


      COLORREF GetNearestColor(COLORREF crColor) override;
      ::draw2d::palette* SelectPalette(::draw2d::palette* pPalette, bool bForceBackground) override;
      UINT RealizePalette() override;
      void UpdateColors() override;

      i32 GetPolyFillMode() override;
      i32 GetROP2() override;
      i32 GetStretchBltMode() override;

      i32 SetPolyFillMode(i32 nPolyFillMode) override;
      i32 SetROP2(i32 nDrawMode) override;
      i32 SetStretchBltMode(i32 nStretchMode) override;

      bool GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) override;
      bool SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust) override;

#if (_WIN32_WINNT >= 0x0500)

      COLORREF GetDCBrushColor() override;
      COLORREF SetDCBrushColor(COLORREF crColor) override;

      COLORREF GetDCPenColor() override;
      COLORREF SetDCPenColor(COLORREF crColor) override;

#endif

      // Graphics mode
      i32 SetGraphicsMode(i32 iMode) override;
      i32 GetGraphicsMode() override;

      // World transform
      bool SetWorldTransform(const XFORM* pXform) override;
      bool ModifyWorldTransform(const XFORM* pXform,uint32_t iMode) override;
      bool GetWorldTransform(XFORM* pXform) override;

      // Mapping Functions
      virtual i32 GetMapMode() override;
      virtual point GetViewportOrg() override;
      virtual i32 SetMapMode(i32 nMapMode) override;
      // Viewport Origin
      virtual point SetViewportOrg(i32 x, i32 y) override;
      virtual point SetViewportOrg(POINT point) override;
      virtual point OffsetViewportOrg(i32 nWidth, i32 nHeight) override;

      // Viewport Extent
      virtual size GetViewportExt() override;
      virtual size SetViewportExt(i32 cx, i32 cy) override;
      virtual size SetViewportExt(SIZE size) override;
      virtual size ScaleViewportExt(i32 xNum, i32 xDenom, i32 yNum, i32 yDenom) override;

      // Window Origin
      point GetWindowOrg() override;
      point SetWindowOrg(i32 x, i32 y) override;
      point SetWindowOrg(POINT point) override;
      point OffsetWindowOrg(i32 nWidth, i32 nHeight) override;

      // Window extent
      size GetWindowExt() override;
      virtual size SetWindowExt(i32 cx, i32 cy) override;
      size SetWindowExt(SIZE size) override;
      virtual size ScaleWindowExt(i32 xNum, i32 xDenom, i32 yNum, i32 yDenom) override;

      // Coordinate Functions
      void DPtoLP(LPPOINT lpPoints,count nCount = 1) override;
      void DPtoLP(LPRECT lpRect) override;
      void DPtoLP(LPSIZE lpSize) override;
      void LPtoDP(LPPOINT lpPoints,count nCount = 1) override;
      void LPtoDP(LPRECT lpRect) override;
      void LPtoDP(LPSIZE lpSize) override;

      // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      void DPtoHIMETRIC(LPSIZE lpSize) override;
      void LPtoHIMETRIC(LPSIZE lpSize) override;
      void HIMETRICtoDP(LPSIZE lpSize) override;
      void HIMETRICtoLP(LPSIZE lpSize) override;

      // Region Functions
      bool FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush) override;
      bool FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, i32 nWidth, i32 nHeight) override;
      bool InvertRgn(::draw2d::region* pRgn) override;
      bool PaintRgn(::draw2d::region* pRgn) override;

      // Clipping Functions
      virtual i32 GetClipBox(LPRECT lpRect) override;
      virtual bool PtVisible(i32 x, i32 y) override;
      bool PtVisible(POINT point) override;
      virtual bool RectVisible(const RECT &  lpRect) override;
      i32 SelectClipRgn(::draw2d::region* pRgn) override;
      i32 ExcludeClipRect(i32 x1, i32 y1, i32 x2, i32 y2) override;
      i32 ExcludeClipRect(const RECT &  lpRect) override;
//      i32 ExcludeUpdateRgn(::user::primitive * pwindow) override;
      i32 IntersectClipRect(i32 x1, i32 y1, i32 x2, i32 y2) override;
      i32 IntersectClipRect(const RECT &  lpRect) override;
      i32 OffsetClipRgn(i32 x, i32 y) override;
      i32 OffsetClipRgn(SIZE size) override;
      i32 SelectClipRgn(::draw2d::region* pRgn, i32 nMode) override;

      // Line-Output Functions
      pointd current_position() override;
//      point move_to(i32 x, i32 y) override;
      //    point move_to(POINT point) override;
      using ::draw2d::graphics::line_to;
      bool line_to(LPCPOINT ppt) override;
      bool line_to(LPCPOINTD ppt) override;
      //  bool line_to(POINT point) override;
      bool Polyline(const POINT* lpPoints,count nCount) override;

      using ::draw2d::graphics::Arc;
      bool Arc(i32 x1,i32 y1,i32 x2,i32 y2,i32 x3,i32 y3,i32 x4,i32 y4) override;
      bool Arc(f64 x1,f64 y1,f64 x2,f64 y2,f64 x3,f64 y3,f64 x4,f64 y4) override;

      bool Arc(i32 x1,i32 y1,i32 x2,i32 y2,f64 start, f64 extends) override;
      bool Arc(f64 x1,f64 y1,f64 x2,f64 y2,f64 start, f64 extends) override;

      bool AngleArc(i32 x,i32 y,i32 nRadius,float fStartAngle,float fSweepAngle) override;
      bool ArcTo(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, i32 x4, i32 y4) override;
      bool ArcTo(const RECT &  lpRect, POINT ptStart, POINT ptEnd) override;
      i32 GetArcDirection() override;
      i32 SetArcDirection(i32 nArcDirection) override;

      bool PolyDraw(const POINT* lpPoints, const BYTE* lpTypes,count nCount) override;
      bool PolylineTo(const POINT* lpPoints,count nCount) override;
      bool PolyPolyline(const POINT* lpPoints, const INT * lpPolyPoints,count nCount) override;

      bool PolyBezier(const POINT* lpPoints,count nCount) override;
      bool PolyBezierTo(const POINT* lpPoints,count nCount) override;


      void frame_rect(LPCRECT prect, ::draw2d::brush* pBrush) override;
      void invert_rect(LPCRECT prect) override;
      bool DrawIcon(i32 x, i32 y, ::visual::icon * picon) override;
      bool DrawIcon(POINT point, ::visual::icon * picon) override;
      bool DrawIcon(i32 x, i32 y, ::visual::icon * picon, i32 cx, i32 cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags) override;
      bool DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags,
                     HBRUSH hBrush = NULL) override;
      bool DrawState(point pt, size size, ::draw2d::bitmap* pBitmap, UINT nFlags,
                     ::draw2d::brush* pBrush = NULL) override;
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
                     HBRUSH hBrush = NULL) override;
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
                     ::draw2d::brush* pBrush = NULL) override;
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
                     bool bPrefixText = TRUE, i32 nTextLen = 0, HBRUSH hBrush = NULL) override;
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
                     bool bPrefixText = TRUE, i32 nTextLen = 0, ::draw2d::brush* pBrush = NULL) override;
      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
                     LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL) override;
      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
                     LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush = NULL) override;

      // Ellipse and Polygon Functions
      bool Chord(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
                 i32 x4, i32 y4) override;
      bool Chord(const RECT &  lpRect, POINT ptStart, POINT ptEnd) override;
      void DrawFocusRect(const RECT &  lpRect) override;

      bool DrawEllipse(i32 x1, i32 y1, i32 x2, i32 y2) override;
      bool DrawEllipse(const RECT &  lpRect) override;
      bool FillEllipse(i32 x1, i32 y1, i32 x2, i32 y2) override;
      bool FillEllipse(const RECT &  lpRect) override;

      bool DrawEllipse(f64 x1,f64 y1,f64 x2,f64 y2) override;
      bool DrawEllipse(const RECTD & lpRect) override;
      bool FillEllipse(f64 x1,f64 y1,f64 x2,f64 y2) override;
      bool FillEllipse(const RECTD & lpRect) override;

      bool Pie(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, i32 x4, i32 y4) override;
      bool Pie(const RECT &  lpRect, POINT ptStart, POINT ptEnd) override;

      bool draw_polygon(LPCPOINT ppoints, count nCount) override;
      bool draw_polygon(LPCPOINTD ppoints, count nCount) override;
      bool fill_polygon(LPCPOINT ppoints, count nCount) override;
      bool fill_polygon(LPCPOINTD ppoints, count nCount) override;
      using ::draw2d::graphics::rectangle;
      using ::draw2d::graphics::draw_rect;
      using ::draw2d::graphics::fill_rect;
      virtual bool rectangle(LPCRECT lpcrect) override;
      virtual bool rectangle(LPCRECTD lpcrect) override;
      virtual bool draw_rect(LPCRECT lpcrect, ::draw2d::pen * ppen) override;
      virtual bool draw_rect(LPCRECTD lpcrect, ::draw2d::pen * ppen) override;
      virtual bool fill_rect(LPCRECT lpcrect, ::draw2d::brush * pbrush) override;
      virtual bool fill_rect(LPCRECTD lpcrect, ::draw2d::brush * pbrush) override;
      bool round_rect(LPCRECT lpcrect, LPCPOINT lppt) override;
      bool round_rect(LPCRECTD lpcrect, LPCPOINTD lppt) override;

      // Bitmap Functions
      bool PatBlt(i32 x, i32 y, i32 nWidth, i32 nHeight, uint32_t dwRop) override;
      bool BitBltRaw(i32 x, i32 y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc,
                     i32 xSrc, i32 ySrc, uint32_t dwRop) override;
      bool StretchBlt(i32 x, i32 y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc,
                      i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight, uint32_t dwRop) override;
      COLORREF GetPixel(i32 x, i32 y) override;
      COLORREF GetPixel(POINT point) override;
      COLORREF SetPixel(i32 x, i32 y, COLORREF crColor) override;
      COLORREF SetPixel(POINT point, COLORREF crColor) override;
      bool FloodFill(i32 x, i32 y, COLORREF crColor) override;
      bool ExtFloodFill(i32 x, i32 y, COLORREF crColor, UINT nFillType) override;
      bool MaskBlt(i32 x, i32 y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc,
                   i32 xSrc, i32 ySrc, ::draw2d::bitmap& maskBitmap, i32 xMask, i32 yMask,
                   uint32_t dwRop) override;
      bool PlgBlt(LPPOINT lpPoint, ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc,
                  i32 nWidth, i32 nHeight, ::draw2d::bitmap& maskBitmap, i32 xMask, i32 yMask) override;
      bool SetPixelV(i32 x, i32 y, COLORREF crColor) override;
      bool SetPixelV(POINT point, COLORREF crColor) override;
      bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices,
                        void * pMesh, ULONG nMeshElements, uint32_t dwMode) override;
      bool TransparentBlt(i32 xDest, i32 yDest, i32 nDestWidth, i32 nDestHeight,
                          ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight,
                          UINT clrTransparent) override;

      virtual bool alpha_blendRaw(i32 xDest, i32 yDest, i32 nDestWidth, i32 nDestHeight, ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight, f64 dOpacity) override;

      /*bool alpha_blend(i32 xDest, i32 yDest, i32 nDestWidth, i32 nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc, i32 nSrcWidth, i32 nSrcHeight,
        BLENDFUNCTION blend) override;*/

      // Text Functions
      virtual bool TextOutRaw(f64 x, f64 y, const char * lpszString, strsize nCount) override;
      virtual bool ExtTextOut(i32 x, i32 y, UINT nOptions, const RECT &  lpRect, const char * lpszString,strsize nCount, LPINT lpDxWidths) override;
      virtual bool ExtTextOut(i32 x, i32 y, UINT nOptions, const RECT &  lpRect, const string & str, LPINT lpDxWidths) override;
      virtual size TabbedTextOut(i32 x, i32 y, const char * lpszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions, i32 nTabOrigin) override;
      virtual size TabbedTextOut(i32 x, i32 y, const string & str,count nTabPositions, LPINT lpnTabStopPositions, i32 nTabOrigin) override;



      virtual bool draw_text(const char * lpszString,strsize nCount,const RECT & lpRect,UINT nFormat) override;
      virtual bool draw_text(const string & str,const RECT & lpRect,UINT nFormat) override;

      virtual bool draw_text_ex(const char * lpszString,strsize nCount,const RECT & lpRect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams) override;
      virtual bool draw_text_ex(const string & str,const RECT & lpRect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams) override;

      virtual bool draw_text(const char * lpszString,strsize nCount,const RECTD & rect,UINT nFormat) override;
      virtual bool draw_text(const string & str,const RECTD & rect,UINT nFormat) override;

      virtual bool draw_text_ex(const char * lpszString,strsize nCount,const RECTD & rect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams) override;
      virtual bool draw_text_ex(const string & str,const RECTD & rect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams) override;

      virtual ::count GetEachCharTextExtent(array < size > & sizea, const string & str, strsize iStart = 0, strsize iEnd = -1) override;
      virtual sized GetTextExtent(const char * lpszString, strsize nCount, strsize iIndex) override;
      virtual sized GetTextBegin(const char * lpszString, strsize nCount, strsize iIndex) override;
      virtual sized GetTextExtent(const char * lpszString, strsize nCount) override;
      virtual sized GetTextExtent(const string & str) override;
      virtual bool GetTextExtent(sized & size, const char * lpszString, strsize nCount, strsize iIndex) override;
      virtual bool GetTextExtent(sized & size, const char * lpszString, strsize nCount) override;
      virtual bool GetTextExtent(sized & size, const string & str) override;
      virtual size GetOutputTextExtent(const char * lpszString, strsize nCount) override;
      virtual size GetOutputTextExtent(const string & str) override;
      virtual size GetTabbedTextExtent(const char * lpszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions) override;
      virtual size GetTabbedTextExtent(const string & str,count nTabPositions, LPINT lpnTabStopPositions) override;
      virtual size GetOutputTabbedTextExtent(const char * lpszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions) override;
      virtual size GetOutputTabbedTextExtent(const string & str,count nTabPositions, LPINT lpnTabStopPositions) override;
      virtual bool GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, i32), LPARAM lpData, i32 nCount, i32 x, i32 y, i32 nWidth, i32 nHeight) override;
      virtual UINT GetTextAlign() override;
      virtual UINT SetTextAlign(UINT nFlags) override;
      virtual i32 GetTextFace(count nCount, LPTSTR lpszFacename) override;
      virtual i32 GetTextFace(string & rString) override;
      virtual bool get_text_metrics(::draw2d::text_metric * lpMetrics) override;
      virtual bool get_output_text_metrics(::draw2d::text_metric * lpMetrics) override;
      virtual i32 SetTextJustification(i32 nBreakExtra, i32 nBreakCount) override;
      virtual i32 GetTextCharacterExtra() override;
      virtual i32 SetTextCharacterExtra(i32 nCharExtra) override;

      virtual uint32_t GetCharacterPlacement(string & str, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) override;
      virtual uint32_t GetCharacterPlacement(const char * lpString, strsize nCount, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) override;

#if (_WIN32_WINNT >= 0x0500)

      bool GetTextExtentExPointI(LPWORD pgiIn, i32 cgi, i32 nMaxExtent, LPINT lpnFit, LPINT alpDx, __out_opt LPSIZE lpSize) override;
      bool GetTextExtentPointI(LPWORD pgiIn, i32 cgi, __out_opt LPSIZE lpSize) override;

#endif



      // Advanced Drawing
      bool DrawEdge(const RECT & lpRect,UINT nEdge,UINT nFlags) override;
      bool DrawFrameControl(const RECT & lpRect,UINT nType,UINT nState) override;

      // Scrolling Functions
      bool ScrollDC(i32 dx, i32 dy, const RECT &  lpRectScroll, const RECT &  lpRectClip,
                    ::draw2d::region* pRgnUpdate, LPRECT lpRectUpdate) override;

      // font Functions
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) override;
      bool GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) override;
      uint32_t SetMapperFlags(uint32_t dwFlag) override;
      size GetAspectRatioFilter() override;

      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) override;
      uint32_t GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData, uint32_t cbData) override;
      i32 GetKerningPairs(i32 nPairs, LPKERNINGPAIR lpkrnpair) override;
      UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm) override;
      uint32_t GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
                               uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) override;

      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
                            LPABCFLOAT lpABCF) override;
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar,
                        float* lpFloatBuffer) override;

      uint32_t GetFontLanguageInfo() override;

#if (_WIN32_WINNT >= 0x0500)

      bool GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) override;
      bool GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) override;

#endif

      virtual i32 Escape(i32 nEscape, i32 nCount, const char * lpszInData, LPVOID lpOutData) override;
      i32 Escape(i32 nEscape, i32 nInputSize,  const char * lpszInputData,i32 nOutputSize, char * lpszOutputData) override;
      i32 DrawEscape(i32 nEscape, i32 nInputSize, const char * lpszInputData) override;

      // Escape helpers
      i32 StartDoc(const char * lpszDocName) override;  // old Win3.0 version
      i32 StartDoc(LPDOCINFO lpDocInfo) override;
      i32 StartPage() override;
      i32 EndPage() override;
      i32 SetAbortProc(bool (CALLBACK* lpfn)(HDC, i32)) override;
      i32 AbortDoc() override;
      i32 EndDoc() override;

      // MetaFile Functions
      bool PlayMetaFile(HMETAFILE hMF) override;
      bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT &  lpBounds) override;
      bool AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData) override;
      // can be used for enhanced metafiles only

      bool abort_path() override;
      bool begin_path() override;
      bool close_figure() override;
      bool end_path() override;
      bool fill_path() override;
      bool flatten_path() override;
      bool stroke_and_fill_path() override;
      bool stroke_path() override;
      bool widen_path() override;

      bool draw_path(::draw2d::path * ppath) override;
      bool draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen) override;
      bool fill_path(::draw2d::path * ppath) override;
      bool fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrush) override;


      float GetMiterLimit() override;
      bool SetMiterLimit(float fMiterLimit) override;
      i32 GetPath(LPPOINT lpPoints, LPBYTE lpTypes,count nCount) override;
      bool SelectClipPath(i32 nMode) override;

      // Misc Helper Functions
      static ::draw2d::brush* GetHalftoneBrush(::aura::application * papp);
      void DrawDragRect(const RECT &  lpRect, SIZE size,
                        const RECT &  lpRectLast, SIZE sizeLast,
                        ::draw2d::brush* pBrush = NULL, ::draw2d::brush* pBrushLast = NULL) override;

      void fill_solid_rect(LPCRECT lpcrect, COLORREF cr) override;
      void fill_solid_rect(LPCRECTD lpcrect, COLORREF cr) override;

      void draw3d_rect(LPCRECT lpcrect, COLORREF crTopLeft, COLORREF crBottomRight) override;
      void draw3d_rect(LPCRECTD lpcrect, COLORREF crTopLeft, COLORREF crBottomRight) override;

#ifdef DEBUG
      virtual void assert_valid() const override;
      virtual void dump(dump_context & dumpcontext) const override;
#endif

//      HGDIOBJ SelectObject(HGDIOBJ) override;      // do not use for regions

      virtual void set_alpha_mode(::draw2d::e_alpha_mode ealphamode) override;

      virtual void set_smooth_mode(::draw2d::e_smooth_mode esmoothmode) override;

      virtual void set_text_rendering(::draw2d::e_text_rendering etextrendering) override;

      virtual void * get_os_data() const override;
      virtual void * get_os_data_ex(int i) const override;
      virtual void release_os_data_ex(int i, void *p) override;
      virtual HDC get_handle() const;
      virtual HDC get_handle1() const;
      virtual HDC get_handle2() const;

//      virtual bool attach(void * pdata) override;


      virtual Gdiplus::FillMode gdiplus_get_fill_mode();

      bool blur(bool bExpand, f64 dRadius, const RECT & rect) override;


      virtual bool flush() override;
      virtual bool sync_flush() override;

      virtual bool draw_line(LPCPOINTD lppt1, LPCPOINTD lppt2, ::draw2d::pen * ppen) override;
      virtual bool draw_line(LPCPOINT lppt1, LPCPOINT lppt2, ::draw2d::pen * ppen) override;

      virtual void enum_fonts(::draw2d::font::enum_item_array & itema) override;

      virtual bool prefer_mapped_dib_on_mix() override;


      virtual bool TextOutAlphaBlend(f64 x, f64 y, const char * lpszString, strsize nCount) override;
      virtual bool BitBltAlphaBlend(i32 x, i32 y, i32 nWidth, i32 nHeight, ::draw2d::graphics * pgraphicsSrc, i32 xSrc, i32 ySrc, uint32_t dwRop) override;


   };


} // namespace draw2d_gdiplus


