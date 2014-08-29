#pragma once



namespace backup
{


   class CLASS_DECL_CA2_BACKUP frame : 
      public ::production::frame
   {
   	
   public:
	   frame(sp(::aura::application) papp);
      virtual ~frame();

   };


} // namespace backup



