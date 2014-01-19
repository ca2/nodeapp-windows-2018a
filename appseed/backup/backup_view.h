#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP view :
      virtual public production::view
   {
   public:
	   
      
      view(sp(base_application) papp);
      virtual ~view();


      virtual ::production::production * create_production();


   };


} // namespace backup



