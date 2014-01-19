#pragma once

namespace production
{

   class CLASS_DECL_CA2_PRODUCTION view_update_hint :
      virtual public ::object
   {
   public:
      enum EOption
      {
         OptionNone,
         OptionClean,
         OptionBuild,
      };

      EOption  m_eoption;
      bool     m_bOption;
   };

} // namespace production