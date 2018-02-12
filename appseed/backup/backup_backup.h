#pragma once


namespace backup
{



   class CLASS_DECL_CA2_BACKUP backup : 
      public ::production::production 
   {
   public:



      backup(::aura::application * papp);
      virtual ~backup();

      virtual void run();

      bool hotcopy_repos(const char * psz);
      bool compress_repos(const char * psz);

      bool all_db_dump();
      bool db_copy();

      string get_new_repos_local_path(const char * psz);
      string get_new_db_local_path(const char * psz);




   };


} // namespace backup



