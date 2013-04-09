#pragma once


namespace backup
{


   class CLASS_DECL_CA2_BACKUP document :
      public ::production::document
   {
   public:
   
      
      document(sp(::ca::application) papp);
      virtual ~document();


   };


} // namespace backup


