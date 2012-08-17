#pragma once

namespace backup
{

   class CLASS_DECL_CA2_BACKUP view :
      public production::view
   {
   public:
	   view(::ca::application * papp);
      virtual ~view();

      virtual production_class * create_production_class();
   };


} // namespace backup