#pragma once

namespace win
{

   class CLASS_DECL_win folder_watch :
      virtual public ::ca::folder_watch,
      virtual public ::ca::thread
   {
   public:
      
      folder_watch(::ca::applicationsp papp);

      virtual bool watch(const char * pszPath);

      virtual e_action translate_os_action(int32_t iAction);

      virtual int32_t run();

   };


} // namespace win