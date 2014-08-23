#include "framework.h"


namespace draw2d_gdi
{
   path::path()
   {


   }

   path::path(sp(::aura::application) papp):
      ::element(papp)
   {

      }

   path::~path()
   {

   }



   void path::get_bounding_rect(LPRECT lprect,::draw2d::path::string_path & p)
   {

      HDC hdc = ::CreateCompatibleDC(NULL);

      ::SelectObject(hdc, p.m_spfont->get_os_data());

      wstring wstr(p.m_strText);

      size size;

      if(::GetTextExtentPoint32W(hdc,wstr,(int)wstr.get_length(),&size))
      {
         lprect->left   = (LONG)MIN(p.m_x,lprect->left);

         lprect->top    = (LONG)MIN(p.m_y,lprect->top);

         lprect->right  = (LONG)MAX(p.m_x + size.cx,lprect->right);

         lprect->bottom = (LONG)MAX(p.m_y + size.cy,lprect->bottom);
      }


      ::ReleaseDC(NULL,hdc);

   }


} // namespace draw2d






