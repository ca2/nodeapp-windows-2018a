#pragma once

namespace win
{

   class CLASS_DECL_win folder_watch :
      virtual public ::core::folder_watch,
      virtual public ::core::thread
   {
   public:
      
      folder_watch(sp(base_application) papp);

      virtual bool watch(const char * pszPath);

      virtual e_action translate_os_action(int32_t iAction);

      virtual int32_t run();

   };


} // namespace win