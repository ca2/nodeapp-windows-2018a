#pragma once


namespace draw2d_gdi
{


   class CLASS_DECL_DRAW2D_GDI path:
      virtual public ::draw2d::path
   {
   public:


      path();
      path(sp(::axis::application) papp);
      virtual ~path();


      virtual void get_bounding_rect(LPRECT lprect,::draw2d::path::string_path & p);






   };




} // namespace draw2d






