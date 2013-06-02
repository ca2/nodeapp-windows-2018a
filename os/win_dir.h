#pragma once


namespace win
{

   class CLASS_DECL_win path :
      virtual public ::ca::path
   {
   public:
      path(sp(::ca::application) papp);
      virtual bool  is_equal(const char * lpszFilPathA, const char * lpszFilPathB);
   };

   class CLASS_DECL_win dir :
      virtual public ::ca::dir::system
   {
   public:




      string      m_strCa2;
      string      m_strCommonAppData;
      string      m_strProfile;
      string      m_strAppData;
      string      m_strPrograms;
      string      m_strCommonPrograms;


      dir(sp(::ca::application) papp);


      using ::ca::dir::system::path;
      virtual class ::ca::path & path();
      virtual string path(const char * pszFolder, strsize iLenFolder, const char * pszRelative, strsize iLenRelative, const char * psz2, strsize iLen2, bool bUrl);

      virtual string relpath(const char * lpcszSource, const char * lpcszRelative, const char * lpcsz2 = NULL);
      virtual void  ls_pattern(sp(::ca::application) papp, const char * lpcsz, const char * lpcszPattern, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, array < bool, bool > * pbaIsDir = NULL, array < int64_t, int64_t > * piaSize = NULL);
      virtual void  ls(sp(::ca::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, array < bool, bool > * pbaIsDir = NULL, array < int64_t, int64_t > * piaSize = NULL);
      virtual void  rls_pattern(sp(::ca::application) papp, const char * lpcsz, const char * lpcszPattern, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, stringa * pstraRelative = NULL, array < bool, bool > * pbaIsDir = NULL, array < int64_t, int64_t > * piaSize = NULL, e_extract eextract = extract_first);
      virtual void  rls(sp(::ca::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, stringa * pstraRelative = NULL, e_extract eextract = extract_first);
      virtual void  rls_dir(sp(::ca::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL, stringa * pstraRelative = NULL);
      virtual void  ls_dir(sp(::ca::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL);
      virtual bool  has_subdir(sp(::ca::application) papp, const char * lpcsz);
      virtual void  ls_file(sp(::ca::application) papp, const char * lpcsz, stringa * pstraPath = NULL, stringa * pstraTitle = NULL);
      virtual bool  is(const char * lpcsz, sp(::ca::application) papp);
      virtual bool  is(const string & str, sp(::ca::application) papp);
      virtual bool  name_is(const string & str, sp(::ca::application) papp);
      virtual bool  is_inside(const char * lpcszDir, const char * lpcszPath, sp(::ca::application) papp);
      virtual bool  is_inside_time(const char * lpcsz, sp(::ca::application) papp);
      virtual void root_ones(stringa & stra, sp(::ca::application) papp);
      virtual bool mk(const char * lpcsz, sp(::ca::application) papp);
      virtual bool rm(sp(::ca::application) papp, const char * psz, bool bRecursive = true);
      

      virtual string name(const char * lpcsz);
      virtual string name(const string & str);

      virtual string time(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string stage(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string stageapp(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string netseed(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      
      // stage in ca2os spalib
      virtual string ca2(const char * lpcsz, const char * lpcsz2 = NULL);
      virtual string ca2();
      virtual string ca2(const string & str);
      virtual string ca2(const string & str, const string & str2);
      virtual string ca2(const char * lpcsz, const string & str2);
      virtual string ca2(const string & str, const char * lpcsz2);

      virtual string module(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string ca2module(const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual void time_square(string &str);
      virtual string time_log(const char * pszId);


      virtual string trash_that_is_not_trash(const char * psz);

      virtual string appdata(const char * lpcsz = NULL, const char * lpcsz2 = NULL);

      virtual string usersystemappdata(sp(::ca::application) papp, const char * lpcszPrefix, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string userappdata(sp(::ca::application) papp, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string userdata(sp(::ca::application) papp, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string userfolder(sp(::ca::application) papp, const char * lpcsz = NULL, const char * lpcsz2 = NULL);
      virtual string default_os_user_path_prefix(sp(::ca::application) papp);
      virtual string default_userappdata(sp(::ca::application) papp, const char * lpcszPrefix, const char * lpcszLogin , const char * pszRelativePath = NULL);
      virtual string default_userdata(sp(::ca::application) papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath = NULL);
      virtual string default_userfolder(sp(::ca::application) papp, const char * lpcszPrefix, const char * lpcszLogin, const char * pszRelativePath = NULL);
      virtual string userquicklaunch(sp(::ca::application) papp, const char * pszRelativePath = NULL, const char * lpcsz2 = NULL);
      virtual string userprograms(sp(::ca::application) papp, const char * pszRelativePath = NULL, const char * lpcsz2 = NULL);

      virtual string commonprograms(const char * pszRelativePath = NULL, const char * lpcsz2 = NULL);

      virtual bool initialize();
   protected:
      class path     m_path;
      string         m_strTimeFolder;
      string         m_strNetSeedFolder;
   };

} // namespace win