#pragma once


namespace draw2d_gdiplus
{


   class CLASS_DECL_DRAW2D_GDIPLUS graphics : 
      virtual public ::draw2d::graphics
   {
   public:


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

//      ::window_sp GetWindow() const;

      virtual bool attach(void * pgraphics) override;   // attach/detach affects only the Output DC
      virtual void * detach() override;

      virtual bool Attach(HDC hdc);   // attach/detach affects only the Output DC
      virtual HDC Detach();
      virtual bool AttachPrinter(HDC hdc);
      virtual HDC DetachPrinter();


      virtual void SetAttribDC(HDC hDC);  // set the Attribute DC
      virtual void SetOutputDC(HDC hDC);  // set the Output DC
      virtual void ReleaseAttribDC();     // Release the Attribute DC
      virtual void ReleaseOutputDC();     // Release the Output DC

      bool IsPrinting() const;            // TRUE if being used for printing

      ::draw2d::pen_sp     get_current_pen() const;
      ::draw2d::brush_sp   get_current_brush() const;
      ::draw2d::palette_sp get_current_palette() const;
      ::draw2d::font_sp    get_current_font() const;
      ::draw2d::bitmap_sp  get_current_bitmap() const;


      Gdiplus::Pen *       gdiplus_pen();
      Gdiplus::Brush *     gdiplus_brush();
      Gdiplus::Font *      gdiplus_font();

      // for bidi and mirrored localization
      uint32_t GetLayout() const;
      uint32_t SetLayout(uint32_t dwLayout);


      HDC get_hdc();
      void release_hdc(HDC hdc);


      virtual double get_dpix() const;
      virtual double get_dpiy() const;

   // Constructors
      bool CreateDC(const char * lpszDriverName, const char * lpszDeviceName,
         const char * lpszOutput, const void * lpInitData);
      bool CreateIC(const char * lpszDriverName, const char * lpszDeviceName,
         const char * lpszOutput, const void * lpInitData);
      bool CreateCompatibleDC(::draw2d::graphics * pgraphics);

      bool DeleteDC();

   // Device-Context Functions
      virtual int32_t SaveDC();
      virtual bool RestoreDC(int32_t nSavedDC);
      int32_t GetDeviceCaps(int32_t nIndex) const;
      UINT SetBoundsRect(const RECT &  lpRectBounds, UINT flags);
      UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);
      bool ResetDC(const DEVMODE* lpDevMode);

   // Drawing-Tool Functions
      point GetBrushOrg() const;
      point SetBrushOrg(int32_t x, int32_t y);
      point SetBrushOrg(POINT point);
      int32_t EnumObjects(int32_t nObjectType,
            int32_t (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData);

   // Type-safe selection helpers
   public:
      virtual ::draw2d::object* SelectStockObject(int32_t nIndex);
      ::draw2d::pen* SelectObject(::draw2d::pen* pPen);
      ::draw2d::brush* SelectObject(::draw2d::brush* pBrush);
      virtual ::draw2d::font* SelectObject(::draw2d::font* pFont);
      ::draw2d::bitmap* SelectObject(::draw2d::bitmap* pBitmap);
      int32_t SelectObject(::draw2d::region* pRgn);       // special return for regions
      ::draw2d::object* SelectObject(::draw2d::object* pObject);
         // ::draw2d::object* provided so compiler doesn't use SelectObject(HGDIOBJ)

   // color and color Palette Functions
      COLORREF GetNearestColor(COLORREF crColor) const;
      ::draw2d::palette* SelectPalette(::draw2d::palette* pPalette, bool bForceBackground);
      UINT RealizePalette();
      void UpdateColors();

      int32_t GetPolyFillMode() const;
      int32_t GetROP2() const;
      int32_t GetStretchBltMode() const;

      int32_t SetPolyFillMode(int32_t nPolyFillMode);
      int32_t SetROP2(int32_t nDrawMode);
      int32_t SetStretchBltMode(int32_t nStretchMode);

      bool GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const;
      bool SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust);

   #if (_WIN32_WINNT >= 0x0500)

      COLORREF GetDCBrushColor() const;
      COLORREF SetDCBrushColor(COLORREF crColor);

      COLORREF GetDCPenColor() const;
      COLORREF SetDCPenColor(COLORREF crColor);

   #endif

      // Graphics mode
      int32_t SetGraphicsMode(int32_t iMode);
      int32_t GetGraphicsMode() const;

      // World transform
      bool SetWorldTransform(const XFORM* pXform);
      bool ModifyWorldTransform(const XFORM* pXform,uint32_t iMode);
      bool GetWorldTransform(XFORM* pXform) const;

      // Mapping Functions
      virtual int32_t GetMapMode() const;
      virtual point GetViewportOrg() const;
      virtual int32_t SetMapMode(int32_t nMapMode);
      // Viewport Origin
      virtual point SetViewportOrg(int32_t x, int32_t y);
      virtual point SetViewportOrg(POINT point);
      virtual point OffsetViewportOrg(int32_t nWidth, int32_t nHeight);

      // Viewport Extent
      virtual size GetViewportExt() const;
      virtual size SetViewportExt(int32_t cx, int32_t cy);
      virtual size SetViewportExt(SIZE size);
      virtual size ScaleViewportExt(int32_t xNum, int32_t xDenom, int32_t yNum, int32_t yDenom);

      // Window Origin
      point GetWindowOrg() const;
      point SetWindowOrg(int32_t x, int32_t y);
      point SetWindowOrg(POINT point);
      point OffsetWindowOrg(int32_t nWidth, int32_t nHeight);

      // Window extent
      size GetWindowExt() const;
      virtual size SetWindowExt(int32_t cx, int32_t cy);
            size SetWindowExt(SIZE size);
      virtual size ScaleWindowExt(int32_t xNum, int32_t xDenom, int32_t yNum, int32_t yDenom);

   // Coordinate Functions
      void DPtoLP(LPPOINT lpPoints,count nCount = 1) const;
      void DPtoLP(LPRECT lpRect) const;
      void DPtoLP(LPSIZE lpSize) const;
      void LPtoDP(LPPOINT lpPoints,count nCount = 1) const;
      void LPtoDP(LPRECT lpRect) const;
      void LPtoDP(LPSIZE lpSize) const;

   // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      void DPtoHIMETRIC(LPSIZE lpSize) const;
      void LPtoHIMETRIC(LPSIZE lpSize) const;
      void HIMETRICtoDP(LPSIZE lpSize) const;
      void HIMETRICtoLP(LPSIZE lpSize) const;

   // Region Functions
      bool FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush);
      bool FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, int32_t nWidth, int32_t nHeight);
      bool InvertRgn(::draw2d::region* pRgn);
      bool PaintRgn(::draw2d::region* pRgn);

   // Clipping Functions
      virtual int32_t GetClipBox(LPRECT lpRect) const;
      virtual bool PtVisible(int32_t x, int32_t y) const;
            bool PtVisible(POINT point) const;
      virtual bool RectVisible(const RECT &  lpRect) const;
            int32_t SelectClipRgn(::draw2d::region* pRgn);
            int32_t ExcludeClipRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
            int32_t ExcludeClipRect(const RECT &  lpRect);
            int32_t ExcludeUpdateRgn(::user::primitive * pwindow);
            int32_t IntersectClipRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
            int32_t IntersectClipRect(const RECT &  lpRect);
            int32_t OffsetClipRgn(int32_t x, int32_t y);
            int32_t OffsetClipRgn(SIZE size);
      int32_t SelectClipRgn(::draw2d::region* pRgn, int32_t nMode);

   // Line-Output Functions
      point GetCurrentPosition() const;
//      point MoveTo(int32_t x, int32_t y);
  //    point MoveTo(POINT point);
      bool LineTo(double x, double y);
      bool LineTo(int x,int y);
    //  bool LineTo(POINT point);
      bool Polyline(const POINT* lpPoints,count nCount);

      using ::draw2d::graphics::Arc;
      bool Arc(int32_t x1,int32_t y1,int32_t x2,int32_t y2,int32_t x3,int32_t y3,int32_t x4,int32_t y4);
      bool Arc(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4);

      bool Arc(int32_t x1,int32_t y1,int32_t x2,int32_t y2,double start, double extends);
      bool Arc(double x1,double y1,double x2,double y2,double start, double extends);

      bool AngleArc(int32_t x,int32_t y,int32_t nRadius,float fStartAngle,float fSweepAngle);
      bool ArcTo(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4);
      bool ArcTo(const RECT &  lpRect, POINT ptStart, POINT ptEnd);
      int32_t GetArcDirection() const;
      int32_t SetArcDirection(int32_t nArcDirection);

      bool PolyDraw(const POINT* lpPoints, const BYTE* lpTypes,count nCount);
      bool PolylineTo(const POINT* lpPoints,count nCount);
      bool PolyPolyline(const POINT* lpPoints, const INT * lpPolyPoints,count nCount);

      bool PolyBezier(const POINT* lpPoints,count nCount);
      bool PolyBezierTo(const POINT* lpPoints,count nCount);

   // Simple Drawing Functions
      void FillRect(const RECT &  lpRect, ::draw2d::brush* pBrush);
      void FrameRect(const RECT &  lpRect, ::draw2d::brush* pBrush);
      bool DrawRect(const RECT & rect, ::draw2d::pen * ppen);
      void InvertRect(const RECT &  lpRect);
      bool DrawIcon(int32_t x, int32_t y, ::visual::icon * picon);
      bool DrawIcon(POINT point, ::visual::icon * picon);
      bool DrawIcon(int32_t x, int32_t y, ::visual::icon * picon, int32_t cx, int32_t cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags);
      bool DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, ::draw2d::bitmap* pBitmap, UINT nFlags,
         ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int32_t nTextLen = 0, HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int32_t nTextLen = 0, ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
         LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
         LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush = NULL);

   // Ellipse and Polygon Functions
      bool Chord(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3,
         int32_t x4, int32_t y4);
      bool Chord(const RECT &  lpRect, POINT ptStart, POINT ptEnd);
      void DrawFocusRect(const RECT &  lpRect);
      
      bool DrawEllipse(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      bool DrawEllipse(const RECT &  lpRect);
      bool FillEllipse(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      bool FillEllipse(const RECT &  lpRect);

      bool DrawEllipse(double x1,double y1,double x2,double y2);
      bool DrawEllipse(const RECTD & lpRect);
      bool FillEllipse(double x1,double y1,double x2,double y2);
      bool FillEllipse(const RECTD & lpRect);

      bool Pie(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, int32_t x4, int32_t y4);
      bool Pie(const RECT &  lpRect, POINT ptStart, POINT ptEnd);
      bool fill_polygon(const POINTD * lpPoints,count nCount);
      bool fill_polygon(const POINT * lpPoints,count nCount);
      bool draw_polygon(const POINT * lpPoints,count nCount);
      bool draw_polygon(const POINTD * lpPoints,count nCount);
      bool Polygon(const POINT* lpPoints,count nCount);
      bool Polygon(const POINTD* lpPoints,count nCount);
      bool PolyPolygon(const POINT* lpPoints, const INT * lpPolyCounts,count nCount);
      bool Rectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      bool Rectangle(const RECT &  lpRect);
      virtual bool DrawRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool DrawRectangle(const RECT &  lpRect);
      virtual bool FillRectangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      virtual bool FillRectangle(const RECT &  lpRect);
      bool RoundRect(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);
      bool RoundRect(const RECT &  lpRect, POINT point);

   // Bitmap Functions
      bool PatBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, uint32_t dwRop);
      bool BitBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::draw2d::graphics * pgraphicsSrc,
         int32_t xSrc, int32_t ySrc, uint32_t dwRop);
      bool StretchBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::draw2d::graphics * pgraphicsSrc,
         int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, uint32_t dwRop);
      COLORREF GetPixel(int32_t x, int32_t y) const;
      COLORREF GetPixel(POINT point) const;
      COLORREF SetPixel(int32_t x, int32_t y, COLORREF crColor);
      COLORREF SetPixel(POINT point, COLORREF crColor);
      bool FloodFill(int32_t x, int32_t y, COLORREF crColor);
      bool ExtFloodFill(int32_t x, int32_t y, COLORREF crColor, UINT nFillType);
      bool MaskBlt(int32_t x, int32_t y, int32_t nWidth, int32_t nHeight, ::draw2d::graphics * pgraphicsSrc,
         int32_t xSrc, int32_t ySrc, ::draw2d::bitmap& maskBitmap, int32_t xMask, int32_t yMask,
         uint32_t dwRop);
      bool PlgBlt(LPPOINT lpPoint, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc,
         int32_t nWidth, int32_t nHeight, ::draw2d::bitmap& maskBitmap, int32_t xMask, int32_t yMask);
      bool SetPixelV(int32_t x, int32_t y, COLORREF crColor);
      bool SetPixelV(POINT point, COLORREF crColor);
      bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices, 
        void * pMesh, ULONG nMeshElements, uint32_t dwMode);
      bool TransparentBlt(int32_t xDest, int32_t yDest, int32_t nDestWidth, int32_t nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, 
        UINT clrTransparent);

      virtual bool alpha_blend(int32_t xDest, int32_t yDest, int32_t nDestWidth, int32_t nDestHeight, ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, double dOpacity);

      /*bool alpha_blend(int32_t xDest, int32_t yDest, int32_t nDestWidth, int32_t nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, int32_t xSrc, int32_t ySrc, int32_t nSrcWidth, int32_t nSrcHeight, 
        BLENDFUNCTION blend);*/

   // Text Functions
      virtual bool TextOut(int32_t x, int32_t y, const char * lpszString, strsize nCount);
      virtual bool TextOut(int32_t x, int32_t y, const string & str);
      virtual bool TextOut(double x, double y, const char * lpszString, strsize nCount);
      virtual bool TextOut(double x, double y, const string & str);
      virtual bool ExtTextOut(int32_t x, int32_t y, UINT nOptions, const RECT &  lpRect, const char * lpszString,strsize nCount, LPINT lpDxWidths);
      virtual bool ExtTextOut(int32_t x, int32_t y, UINT nOptions, const RECT &  lpRect, const string & str, LPINT lpDxWidths);
      virtual size TabbedTextOut(int32_t x, int32_t y, const char * lpszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin);
      virtual size TabbedTextOut(int32_t x, int32_t y, const string & str,count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin);

      virtual int32_t draw_text(const char * lpszString,strsize nCount,const RECT & lpRect,UINT nFormat);
      virtual int32_t draw_text(const string & str,const RECT & lpRect,UINT nFormat);

      virtual int32_t draw_text_ex(LPTSTR lpszString,strsize nCount,const RECT & lpRect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams);
      virtual int32_t draw_text_ex(const string & str,const RECT & lpRect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams);

      virtual int32_t draw_text(const char * lpszString,strsize nCount,const RECTD & rect,UINT nFormat);
      virtual int32_t draw_text(const string & str,const RECTD & rect,UINT nFormat);

      virtual int32_t draw_text_ex(LPTSTR lpszString,strsize nCount,const RECTD & rect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams);
      virtual int32_t draw_text_ex(const string & str,const RECTD & rect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams);

      virtual size GetTextExtent(const char * lpszString, strsize nCount, strsize iIndex) const;
      virtual size GetTextExtent(const char * lpszString, strsize nCount) const;
      virtual size GetTextExtent(const string & str) const;
      virtual bool GetTextExtent(sized & size, const char * lpszString, strsize nCount, strsize iIndex) const;
      virtual bool GetTextExtent(sized & size, const char * lpszString, strsize nCount) const;
      virtual bool GetTextExtent(sized & size, const string & str) const;
      virtual size GetOutputTextExtent(const char * lpszString, strsize nCount) const;
      virtual size GetOutputTextExtent(const string & str) const;
      virtual size GetTabbedTextExtent(const char * lpszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetTabbedTextExtent(const string & str,count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetOutputTabbedTextExtent(const char * lpszString, strsize nCount,count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetOutputTabbedTextExtent(const string & str,count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual bool GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int32_t), LPARAM lpData, int32_t nCount, int32_t x, int32_t y, int32_t nWidth, int32_t nHeight);
      virtual UINT GetTextAlign() const;
      virtual UINT SetTextAlign(UINT nFlags);
      virtual int32_t GetTextFace(count nCount, LPTSTR lpszFacename) const;
      virtual int32_t GetTextFace(string & rString) const;
      virtual bool get_text_metrics(::draw2d::text_metric * lpMetrics) const;
      virtual bool get_output_text_metrics(::draw2d::text_metric * lpMetrics) const;
      virtual int32_t SetTextJustification(int32_t nBreakExtra, int32_t nBreakCount);
      virtual int32_t GetTextCharacterExtra() const;
      virtual int32_t SetTextCharacterExtra(int32_t nCharExtra);

      virtual uint32_t GetCharacterPlacement(string & str, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const;
      virtual uint32_t GetCharacterPlacement(const char * lpString, strsize nCount, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const;

   #if (_WIN32_WINNT >= 0x0500)

      bool GetTextExtentExPointI(LPWORD pgiIn, int32_t cgi, int32_t nMaxExtent, LPINT lpnFit, LPINT alpDx, __out_opt LPSIZE lpSize) const;
      bool GetTextExtentPointI(LPWORD pgiIn, int32_t cgi, __out_opt LPSIZE lpSize) const;

   #endif



   // Advanced Drawing
      bool DrawEdge(const RECT & lpRect,UINT nEdge,UINT nFlags);
      bool DrawFrameControl(const RECT & lpRect,UINT nType,UINT nState);

   // Scrolling Functions
      bool ScrollDC(int32_t dx, int32_t dy, const RECT &  lpRectScroll, const RECT &  lpRectClip,
         ::draw2d::region* pRgnUpdate, LPRECT lpRectUpdate);

   // font Functions
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      bool GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      uint32_t SetMapperFlags(uint32_t dwFlag);
      size GetAspectRatioFilter() const;

      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
      uint32_t GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData, uint32_t cbData) const;
      int32_t GetKerningPairs(int32_t nPairs, LPKERNINGPAIR lpkrnpair) const;
      UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm) const;
      uint32_t GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
         uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;

      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
         LPABCFLOAT lpABCF) const;
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar,
         float* lpFloatBuffer) const;

      uint32_t GetFontLanguageInfo() const;

   #if (_WIN32_WINNT >= 0x0500)

      bool GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const;
      bool GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const;

   #endif

   // Printer/Device Escape Functions
      virtual int32_t Escape(int32_t nEscape, int32_t nCount, const char * lpszInData, LPVOID lpOutData);
      int32_t Escape(int32_t nEscape, int32_t nInputSize,  const char * lpszInputData,int32_t nOutputSize, char * lpszOutputData);
      int32_t DrawEscape(int32_t nEscape, int32_t nInputSize, const char * lpszInputData);

      // Escape helpers
      int32_t StartDoc(const char * lpszDocName);  // old Win3.0 version
      int32_t StartDoc(LPDOCINFO lpDocInfo);
      int32_t StartPage();
      int32_t EndPage();
      int32_t SetAbortProc(bool (CALLBACK* lpfn)(HDC, int32_t));
      int32_t AbortDoc();
      int32_t EndDoc();

   // MetaFile Functions
      bool PlayMetaFile(HMETAFILE hMF);
      bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT &  lpBounds);
      bool AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData);
         // can be used for enhanced metafiles only

   // Path Functions
      bool AbortPath();
      bool BeginPath();
      bool CloseFigure();
      bool EndPath();
      bool FillPath();
      bool FlattenPath();
      bool StrokeAndFillPath();
      bool StrokePath();
      bool WidenPath();

      bool draw_path(::draw2d::path * ppath);
      bool draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen);
      bool fill_path(::draw2d::path * ppath);
      bool fill_path(::draw2d::path * ppath, ::draw2d::brush * pbrush);


      float GetMiterLimit() const;
      bool SetMiterLimit(float fMiterLimit);
      int32_t GetPath(LPPOINT lpPoints, LPBYTE lpTypes,count nCount) const;
      bool SelectClipPath(int32_t nMode);

   // Misc Helper Functions
      static ::draw2d::brush* GetHalftoneBrush(::aura::application * papp);
      void DrawDragRect(const RECT &  lpRect, SIZE size,
         const RECT &  lpRectLast, SIZE sizeLast,
         ::draw2d::brush* pBrush = NULL, ::draw2d::brush* pBrushLast = NULL);
      void FillSolidRect(const RECT64 * lpRect, COLORREF clr);
      void FillSolidRect(const RECT &  lpRect, COLORREF clr);
      void FillSolidRect(int32_t x, int32_t y, int32_t cx, int32_t cy, COLORREF clr);
      void Draw3dRect(const RECT &  lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
      void Draw3dRect(int32_t x, int32_t y, int32_t cx, int32_t cy,
         COLORREF clrTopLeft, COLORREF clrBottomRight);




   // Implementation
   public:
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      HGDIOBJ SelectObject(HGDIOBJ);      // do not use for regions

      virtual void set_alpha_mode(::draw2d::e_alpha_mode ealphamode);

      virtual void set_smooth_mode(::draw2d::e_smooth_mode esmoothmode);

      virtual void set_text_rendering(::draw2d::e_text_rendering etextrendering);

      virtual void * get_os_data() const;
      virtual void * get_os_data_ex(int i) const;
      virtual void release_os_data_ex(int i, void *p);
      virtual HDC get_handle() const;
      virtual HDC get_handle1() const;
      virtual HDC get_handle2() const;

//      virtual bool attach(void * pdata);


      virtual Gdiplus::FillMode gdiplus_get_fill_mode();

      bool blur(bool bExpand, double dRadius, const RECT & rect);


      virtual bool flush();
      virtual bool sync_flush();

      virtual bool DrawLine(float x1, float y1, float x2, float y2, ::draw2d::pen * ppen);
      virtual bool DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, ::draw2d::pen * ppen);

      virtual void enum_fonts(stringa & straPath, stringa & stra) override;


   };


} // namespace draw2d_gdiplus


#define new AURA_NEW


