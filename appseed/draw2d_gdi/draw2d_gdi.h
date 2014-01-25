#pragma once


#include "app/appseed/base/base/base/base.h"



#ifdef _DRAW2D_GDI_LIBRARY
    #define CLASS_DECL_DRAW2D_GDI  _declspec(dllexport)
#else
    #define CLASS_DECL_DRAW2D_GDI  _declspec(dllimport)
#endif


#include "draw2d_gdi_factory_exchange.h"


#include "draw2d_gdi_object.h"
#include "draw2d_gdi_pen.h"
#include "draw2d_gdi_bitmap.h"
#include "draw2d_gdi_brush.h"
#include "draw2d_gdi_font.h"
#include "draw2d_gdi_palette.h"
#include "draw2d_gdi_region.h"
#include "draw2d_gdi_dib.h"
//#include "draw2d_gdi_path.h"


#include "draw2d_gdi_graphics.h"


#pragma comment(lib, "Msimg32.lib") 


#define GDI_GRAPHICS(pgraphics) (dynamic_cast < ::draw2d_gdi::graphics * > (dynamic_cast < ::draw2d::graphics * > (pgraphics)))
#define GDI_HDC(pgraphics) ((dynamic_cast < ::draw2d_gdi::graphics * > (dynamic_cast < ::draw2d::graphics * > (pgraphics)))->get_handle())
#define GDI_DIB(pdib) (dynamic_cast < ::draw2d_gdi::dib * > (dynamic_cast < ::draw2d::dib * > (pdib)))
#define GDI_BRUSH(pbrush) (dynamic_cast < ::draw2d_gdi::brush * > (dynamic_cast < ::draw2d::brush * > (pbrush)))
#define GDI_PEN(ppen) (dynamic_cast < ::draw2d_gdi::pen * > (dynamic_cast < ::draw2d::pen * > (ppen)))
#define GDI_FONT(pfont) (dynamic_cast < ::draw2d_gdi::font * > (dynamic_cast < ::draw2d::font * > (pfont)))
#define GDI_BITMAP(pbitmap) (dynamic_cast < ::draw2d_gdi::bitmap * > (dynamic_cast < ::draw2d::bitmap * > (pbitmap)))
#define GDI_REGION(pregion) (dynamic_cast < ::draw2d_gdi::region * > (dynamic_cast < ::draw2d::region * > (pregion)))


#define argb_invert(cr) ARGB(argb_get_a_value(cr),argb_get_b_value(cr),argb_get_g_value(cr),argb_get_r_value(cr))