#pragma once

class CLASS_DECL_CA2_BACKUP backup_class : public production_class 
{
public:
   backup_class(::ca::application * papp);
   virtual ~backup_class();

   virtual int run();

   bool hotcopy_repos(const char * psz);
   bool compress_repos(const char * psz);

   bool all_db_dump();
   bool db_copy();

   string get_new_repos_local_path(const char * psz);
   string get_new_db_local_path(const char * psz);




};