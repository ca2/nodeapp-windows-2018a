#pragma once


#include "core.h"


#ifdef _CA2_DRAW2D_GDIPLUS_LIBRARY
    #define CLASS_DECL_DRAW2D_GDIPLUS  _declspec(dllexport)
#else
    #define CLASS_DECL_DRAW2D_GDIPLUS  _declspec(dllimport)
#endif


#include "draw2d_gdiplus_factory_exchange.h"


#include "draw2d_gdiplus_object.h"
#include "draw2d_gdiplus_pen.h"
#include "draw2d_gdiplus_bitmap.h"
#include "draw2d_gdiplus_brush.h"
#include "draw2d_gdiplus_font.h"
#include "draw2d_gdiplus_palette.h"
#include "draw2d_gdiplus_region.h"
#include "draw2d_gdiplus_dib.h"
#include "draw2d_gdiplus_path.h"


#include "draw2d_gdiplus_graphics.h"


#pragma comment(lib, "Msimg32.lib") 


#define GDIPLUS_GRAPHICS(pgraphics) (dynamic_cast < ::draw2d_gdiplus::graphics * > (dynamic_cast < ::draw2d::graphics * > (pgraphics)))
#define GDIPLUS_HDC(pgraphics) ((dynamic_cast < ::draw2d_gdiplus::graphics * > (dynamic_cast < ::draw2d::graphics * > (pgraphics)))->get_handle())
