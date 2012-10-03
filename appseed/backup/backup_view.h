#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP view :
      public production::view
   {
   public:
	   
      
      view(::ca::application * papp);
      virtual ~view();


      virtual ::production::production * create_production();


   };


} // namespace backup



