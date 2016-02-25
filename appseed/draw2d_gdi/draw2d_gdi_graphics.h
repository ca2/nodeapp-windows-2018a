#pragma once


namespace draw2d_gdi
{

   
   class object;


   class CLASS_DECL_DRAW2D_GDI graphics : 
      virtual public ::draw2d::graphics
   {
   public:


      enum e_data
      {
         data_graphics,
         data_bitmap,
         data_pen,
         data_brush,
         data_font,
         data_palette,
         data_region,
      };


      class draw_item
      {
      public:
         
         const POINT * lpPoints;
         const INT * lpPolyCounts;
         count nCount;

      };


      HDC                        m_hdc;

      ::draw2d::path_sp          m_sppath;

      HBITMAP                                            m_hbitmapOriginal;
      HPEN                                               m_hpenOriginal;
      HBRUSH                                             m_hbrushOriginal;
      HFONT                                              m_hfontOriginal;
      HRGN                                               m_hrgnOriginal;

      ptr_array < ::draw2d_gdi::object >                 m_ptraObject;

      //graphics();
      graphics(::aura::application * papp);
      virtual ~graphics();


      bool Attach(HDC hdc);
      HDC Detach();
     
      operator HDC() const;
      HDC get_handle1() const; // Always returns the Output DC
      HDC get_handle2() const; // Always returns the Output DC
      virtual void * get_os_data() const;
      virtual void * get_os_data_ex(int i) const;
      virtual HDC get_handle() const;


      ::draw2d::dib * dib_work(class size size, bool bReset);
      ::draw2d::dib * fill_dib_work(COLORREF clr, class size size, bool bReset);
     
      bool internal_fill_path(void (::draw2d_gdi::graphics::* pfnInternalSetPath)(void *), void * pparam, const RECT & lpcrect, ::draw2d::brush * pbrush);
      bool internal_stroke_path(void (::draw2d_gdi::graphics::* pfnInternalSetPath)(void *), void * pparam, const RECT & lpcrect, ::draw2d::pen * ppen);
      bool internal_fill_and_stroke_path(void(::draw2d_gdi::graphics::* pfnInternalSetPath)(void *),void * pparam,const RECT & lpcrect,::draw2d::brush * pbrush,::draw2d::pen * ppen);

      void internal_set_path(void * pparam);
      void internal_set_path_ellipse(void * pparam);
      void internal_set_path_rectangle(void * pparam);
      void internal_set_path_line(void * pparam);
      void internal_set_path_polygon(void * pparam);
      void internal_set_path_poly_polygon(void * pparam);

      bool IsPrinting() const;            // TRUE if being used for printing

      ::draw2d::pen_sp     get_current_pen() const;
      ::draw2d::brush_sp   get_current_brush() const;
      ::draw2d::palette_sp get_current_palette() const;
      ::draw2d::font_sp    get_current_font() const;
      ::draw2d::bitmap_sp  get_current_bitmap() const;

      // for bidi and mirrored localization
      uint32_t GetLayout() const;
      uint32_t SetLayout(uint32_t dwLayout);

      void set_original_object(int iType);

   // Constructors
      bool CreateDC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData);
      bool CreateIC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData);
      bool CreateCompatibleDC(::draw2d::dib * pdib);

      bool DeleteDC();

   // Device-Context Functions
      virtual int SaveDC();
      virtual bool RestoreDC(int nSavedDC);
      int GetDeviceCaps(int nIndex) const;
      UINT SetBoundsRect(const RECT & lpRectBounds, UINT flags);
      UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);
      bool ResetDC(const DEVMODE* lpDevMode);

   // Drawing-Tool Functions
      point GetBrushOrg() const;
      point SetBrushOrg(int x, int y);
      point SetBrushOrg(POINT point);
      int EnumObjects(int nObjectType,
            int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData);

   // Type-safe selection helpers
   public:
      //virtual ::draw2d::object* SelectStockObject(int nIndex);
      virtual ::draw2d::pen* SelectObject(::draw2d::pen* pPen);
      virtual ::draw2d::brush* SelectObject(::draw2d::brush* pBrush);
      virtual ::draw2d::font* SelectObject(::draw2d::font* pFont);
      virtual ::draw2d::bitmap* SelectObject(::draw2d::bitmap* pBitmap);
      virtual int SelectObject(::draw2d::region* pRgn);       // special return for regions

      virtual void on_select_object(::draw2d::object * pobjectParam);

      bool SelectFont(::draw2d::font * pfont);



      virtual void select_pen();
      virtual void select_brush();
      virtual void select_font();
      virtual void select_null_pen();
      virtual void select_null_brush();


   // color and color Palette Functions
      COLORREF GetNearestColor(COLORREF crColor) const;
      ::draw2d::palette* SelectPalette(::draw2d::palette* pPalette, bool bForceBackground);
      UINT RealizePalette();
      void UpdateColors();

   // Drawing-Attribute Functions
      COLORREF GetBkColor() const;
      int GetBkMode() const;
      int GetPolyFillMode() const;
      int GetROP2() const;
      int GetStretchBltMode() const;
      COLORREF GetTextColor() const;

      virtual COLORREF SetBkColor(COLORREF crColor);
      int SetBkMode(int nBkMode);
      int SetPolyFillMode(int nPolyFillMode);
      int SetROP2(int nDrawMode);
      int SetStretchBltMode(int nStretchMode);
      virtual COLORREF SetTextColor(COLORREF crColor);

      bool GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const;
      bool SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust);

   #if (_WIN32_WINNT >= 0x0500)

      COLORREF GetDCBrushColor() const;
      COLORREF SetDCBrushColor(COLORREF crColor);

      COLORREF GetDCPenColor() const;
      COLORREF SetDCPenColor(COLORREF crColor);

   #endif

      // Graphics mode
      int SetGraphicsMode(int iMode);
      int GetGraphicsMode() const;

      // World transform
      bool SetWorldTransform(const XFORM* pXform);
      bool ModifyWorldTransform(const XFORM* pXform,uint32_t iMode);
      bool GetWorldTransform(XFORM* pXform) const;

      // Mapping Functions
      virtual int GetMapMode() const;
      virtual point GetViewportOrg() const;
      virtual int SetMapMode(int nMapMode);
      // Viewport Origin
      virtual point SetViewportOrg(int x, int y);
      virtual point SetViewportOrg(POINT point);
      virtual point OffsetViewportOrg(int nWidth, int nHeight);

      // Viewport Extent
      virtual size GetViewportExt() const;
      virtual size SetViewportExt(int cx, int cy);
      virtual size SetViewportExt(SIZE size);
      virtual size ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);

      // Window Origin
      point GetWindowOrg() const;
      point SetWindowOrg(int x, int y);
      point SetWindowOrg(POINT point);
      point OffsetWindowOrg(int nWidth, int nHeight);

      // Window extent
      size GetWindowExt() const;
      virtual size SetWindowExt(int cx, int cy);
            size SetWindowExt(SIZE size);
      virtual size ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);

   // Coordinate Functions
      void DPtoLP(LPPOINT lpPoints, count nCount = 1) const;
      void DPtoLP(LPRECT lpRect) const;
      void DPtoLP(LPSIZE lpSize) const;
      void LPtoDP(LPPOINT lpPoints, count nCount = 1) const;
      void LPtoDP(LPRECT lpRect) const;
      void LPtoDP(LPSIZE lpSize) const;

   // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      void DPtoHIMETRIC(LPSIZE lpSize) const;
      void LPtoHIMETRIC(LPSIZE lpSize) const;
      void HIMETRICtoDP(LPSIZE lpSize) const;
      void HIMETRICtoLP(LPSIZE lpSize) const;

   // Region Functions
      bool FillRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush);
      bool FrameRgn(::draw2d::region* pRgn, ::draw2d::brush* pBrush, int nWidth, int nHeight);
      bool InvertRgn(::draw2d::region* pRgn);
      bool PaintRgn(::draw2d::region* pRgn);

   // Clipping Functions
      virtual int GetClipBox(LPRECT lpRect) const;
      virtual bool PtVisible(int x, int y) const;
            bool PtVisible(POINT point) const;
      virtual bool RectVisible(const RECT & lpRect) const;
            int SelectClipRgn(::draw2d::region* pRgn);
            int ExcludeClipRect(int x1, int y1, int x2, int y2);
            int ExcludeClipRect(const RECT & lpRect);
            int ExcludeUpdateRgn(::user::primitive * pwindow);
            int IntersectClipRect(int x1, int y1, int x2, int y2);
            int IntersectClipRect(const RECT & lpRect);
            int OffsetClipRgn(int x, int y);
            int OffsetClipRgn(SIZE size);
      int SelectClipRgn(::draw2d::region* pRgn, int nMode);

   // Line-Output Functions
      point GetCurrentPosition() const;
      point MoveTo(int x, int y);
      point MoveTo(POINT point);
      bool LineTo(int x, int y);
      bool LineTo(POINT point);
      bool Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool Arc(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      bool Polyline(const POINT* lpPoints,count nCount);

      bool AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle);
      bool ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool ArcTo(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      int GetArcDirection() const;
      int SetArcDirection(int nArcDirection);

      bool PolyDraw(const POINT* lpPoints, const BYTE* lpTypes,count nCount);
      bool PolylineTo(const POINT* lpPoints,count nCount);
      bool PolyPolyline(const POINT* lpPoints, const INT * lpPolyPoints,count nCount);

      bool PolyBezier(const POINT* lpPoints,count nCount);
      bool PolyBezierTo(const POINT* lpPoints,count nCount);

      

   // Simple Drawing Functions
      void FillRect(const RECT & lpRect, ::draw2d::brush* pBrush);
      void FrameRect(const RECT & lpRect, ::draw2d::brush* pBrush);
      void InvertRect(const RECT & lpRect);
      bool DrawIcon(int x, int y, ::visual::icon * picon);
      bool DrawIcon(POINT point, ::visual::icon * picon);
      bool DrawIcon(int x, int y, ::visual::icon * picon, int cx, int cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags);
      bool DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, ::draw2d::bitmap* pBitmap, UINT nFlags,
         ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int nTextLen = 0, HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int nTextLen = 0, ::draw2d::brush* pBrush = NULL);
      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
         LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
         LPARAM lData, UINT nFlags, ::draw2d::brush* pBrush = NULL);

      virtual bool DrawRect(const RECT & lpcrect, ::draw2d::pen * ppen);

   // Ellipse and Polygon Functions
      bool Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool Chord(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      void DrawFocusRect(const RECT & lpRect);

      bool Ellipse(const RECT & lpRect);
      bool DrawEllipse(const RECT & lpRect);
      bool FillEllipse(const RECT & lpRect);

      bool Rectangle(const RECT & lpRect);
      bool DrawRectangle(const RECT & lpRect);
      bool FillRectangle(const RECT & lpRect);

      bool Polygon(const POINT * lpPoints, count nCount);
      bool DrawPolygon(const POINT * lpPoints,count nCount);
      bool FillPolygon(const POINT * lpPoints,count nCount);

      bool Polygon(const POINTD * lpPoints,count nCount);
      bool DrawPolygon(const POINTD * lpPoints,count nCount);
      bool FillPolygon(const POINTD * lpPoints,count nCount);

      bool PolyPolygon(const POINT * lpPoints, const INT * lpPolyCounts,count nCount);
      bool DrawPolyPolygon(const POINT * lpPoints, const INT * lpPolyCounts,count nCount);
      bool FillPolyPolygon(const POINT * lpPoints, const INT * lpPolyCounts,count nCount);

      bool Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool Pie(const RECT & lpRect, POINT ptStart, POINT ptEnd);
      bool RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
      bool RoundRect(const RECT & lpRect, POINT point);

   // Bitmap Functions
      bool PatBlt(int x, int y, int nWidth, int nHeight, uint32_t dwRop);
      bool BitBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc,
         int xSrc, int ySrc, uint32_t dwRop);
      bool StretchBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc,
         int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, uint32_t dwRop);
      COLORREF GetPixel(int x, int y) const;
      COLORREF GetPixel(POINT point) const;
      COLORREF SetPixel(int x, int y, COLORREF crColor);
      COLORREF SetPixel(POINT point, COLORREF crColor);
      bool FloodFill(int x, int y, COLORREF crColor);
      bool ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
      bool MaskBlt(int x, int y, int nWidth, int nHeight, ::draw2d::graphics * pgraphicsSrc,
         int xSrc, int ySrc, ::draw2d::bitmap& maskBitmap, int xMask, int yMask,
         uint32_t dwRop);
      bool PlgBlt(LPPOINT lpPoint, ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc,
         int nWidth, int nHeight, ::draw2d::bitmap& maskBitmap, int xMask, int yMask);
      bool SetPixelV(int x, int y, COLORREF crColor);
      bool SetPixelV(POINT point, COLORREF crColor);
      bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices, 
        void * pMesh, ULONG nMeshElements, uint32_t dwMode);
      bool TransparentBlt(int xDest, int yDest, int nDestWidth, int nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
        UINT clrTransparent);
      /*bool alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
        BLENDFUNCTION blend);*/
      bool alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight,
        ::draw2d::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, 
        double dOpacity);

      virtual bool GetTextExtent(sized & size, const char * lpszString, strsize nCount, strsize iIndex) const;

   // Text Functions
      virtual bool TextOut(double x, double y, const char * lpszString, strsize nCount);
      virtual bool TextOut(int x, int y, const string & str);
      virtual bool ExtTextOut(int x, int y, UINT nOptions, const RECT & lpRect, const char * lpszString, count nCount, LPINT lpDxWidths);
      virtual bool ExtTextOut(int x, int y, UINT nOptions, const RECT & lpRect, const string & str, LPINT lpDxWidths);
      virtual size TabbedTextOut(int x, int y, const char * lpszString, strsize nCount, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin);
      virtual size TabbedTextOut(int x, int y, const string & str, count nTabPositions, LPINT lpnTabStopPositions, int32_t nTabOrigin);

      virtual int draw_text(const char * lpszString,strsize nCount,const RECT & lpRect, UINT nFormat);
      virtual int draw_text(const string & str,const RECT & lpRect,UINT nFormat);

      virtual int draw_text_ex(const char * lpszString,strsize nCount,const RECT & lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams);
      virtual int draw_text_ex(const string & str,const RECT & lpRect,UINT nFormat,LPDRAWTEXTPARAMS lpDTParams);

      virtual size GetTextExtent(const char * lpszString, strsize nCount) const;
      virtual size GetTextExtent(const string & str) const;
      virtual size GetOutputTextExtent(const char * lpszString,strsize nCount) const;
      virtual size GetOutputTextExtent(const string & str) const;
      virtual size GetTabbedTextExtent(const char * lpszString,strsize nCount, count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetTabbedTextExtent(const string & str, count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetOutputTabbedTextExtent(const char * lpszString,strsize nCount, count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual size GetOutputTabbedTextExtent(const string & str, count nTabPositions, LPINT lpnTabStopPositions) const;
      virtual bool GrayString(::draw2d::brush* pBrush, bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight);
      virtual UINT GetTextAlign() const;
      virtual UINT SetTextAlign(UINT nFlags);
      virtual int GetTextFace(count nCount, LPTSTR lpszFacename) const;
      virtual int GetTextFace(string & rString) const;
      
      virtual bool get_text_metrics(::draw2d::text_metric * lpMetrics) const;
      virtual bool get_output_text_metrics(::draw2d::text_metric * lpMetrics) const;
      
      virtual int SetTextJustification(int nBreakExtra, int nBreakCount);
      virtual int GetTextCharacterExtra() const;
      virtual int SetTextCharacterExtra(int nCharExtra);
      
      virtual uint32_t GetCharacterPlacement(const char * lpString, strsize nCount, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const;
      virtual uint32_t GetCharacterPlacement(string & str, strsize nMaxExtent, LPGCP_RESULTS lpResults, uint32_t dwFlags) const;

   #if (_WIN32_WINNT >= 0x0500)

      bool GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, __out_opt LPSIZE lpSize) const;
      bool GetTextExtentPointI(LPWORD pgiIn, int cgi, __out_opt LPSIZE lpSize) const;

   #endif



   // Advanced Drawing
      bool DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags);
      bool DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState);

   // Scrolling Functions
      bool ScrollDC(int dx, int dy, const RECT & lpRectScroll, const RECT & lpRectClip,
         ::draw2d::region* pRgnUpdate, LPRECT lpRectUpdate);

   // font Functions
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      bool GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      uint32_t SetMapperFlags(uint32_t dwFlag);
      size GetAspectRatioFilter() const;

      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
      uint32_t GetFontData(uint32_t dwTable, uint32_t dwOffset, LPVOID lpData, uint32_t cbData) const;
      int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const;
      UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRICW lpotm) const;
      uint32_t GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm, uint32_t cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;

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
      virtual int Escape(int nEscape, int nCount, const char * lpszInData, LPVOID lpOutData);
      virtual int Escape(int nEscape, int nInputSize, const char * lpszInputData, int nOutputSize, char * lpszOutputData);
      virtual int DrawEscape(int nEscape, int nInputSize, const char * lpszInputData);

      // Escape helpers
      virtual int StartDoc(const char * lpszDocName);  // old Win3.0 version
      virtual int StartDoc(LPDOCINFO lpDocInfo);
      virtual int StartPage();
      virtual int EndPage();
      virtual int SetAbortProc(bool (CALLBACK* lpfn)(HDC, int));
      virtual int AbortDoc();
      virtual int EndDoc();

   // MetaFile Functions
      virtual bool PlayMetaFile(HMETAFILE hMF);
      virtual bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, const RECT & lpBounds);
      virtual bool AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData);
         // can be used for enhanced metafiles only

   // Path Functions
      bool AbortPath();
      bool BeginPath();
      bool CloseFigure();
      bool EndPath();
      bool FlattenPath();
      bool FillPath();
      bool StrokeAndFillPath();
      bool StrokePath();
      bool FillPath(::draw2d::brush * pbrush);
      bool StrokeAndFillPath(::draw2d::brush * pbrush,::draw2d::pen * ppen);
      bool StrokePath(::draw2d::pen * ppen);
      bool WidenPath();
      float GetMiterLimit() const;
      bool SetMiterLimit(float fMiterLimit);
      int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, count nCount) const;
      bool SelectClipPath(int nMode);

   // Misc Helper Functions
      static ::draw2d::brush* GetHalftoneBrush(::aura::application * papp);
      void DrawDragRect(const RECT & lpRect, SIZE size,
         const RECT & lpRectLast, SIZE sizeLast, ::draw2d::brush* pBrush = NULL, ::draw2d::brush* pBrushLast = NULL);
      void FillSolidRect(const RECT64 * lpRect, COLORREF clr);
      void FillSolidRect(const RECT & lpRect, COLORREF clr);
      void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
      void Draw3dRect(const RECT & lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
      void Draw3dRect(int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight);

      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

      HGDIOBJ SelectObject(HGDIOBJ);      // do not use for regions

      bool set(::draw2d::path * ppath);
      bool set(::draw2d::path::element & pelement);
      bool set(::draw2d::path::line & pline);
      bool set(::draw2d::path::arc & parc);
      bool set(::draw2d::path::move & pmove);
      bool set(::draw2d::path::string_path & stringpath);
      bool draw_path(::draw2d::path * ppath);
      bool fill_path(::draw2d::path * ppath);
      bool draw_path(::draw2d::path * ppath, ::draw2d::pen * ppen);
      bool fill_path(::draw2d::path * ppath,::draw2d::brush * pbrush);

      bool select_path(::draw2d::path * ppath);


      virtual bool flush();

   };


} // namespace draw2d_gdi



