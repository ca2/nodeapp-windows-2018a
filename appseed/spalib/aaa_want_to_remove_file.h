#pragma once





class file
{
public:
   SPALIB_API static bool exists(const char * path);
   inline static bool exists(const wstring & wstr){ return exists(u8(wstr).c_str()); }
   inline static bool exists(const wchar_t * pwsz) {return exists(u8(pwsz).c_str());}
   inline static bool exists(const string & str){ return exists(str.c_str()); }

   SPALIB_API static string get_contents(const char * path);
   SPALIB_API static void put_contents(const char * path, const char * contents);
   SPALIB_API static string md5(const char * path);
   SPALIB_API static int length(const char * path);
   SPALIB_API static string name(const char * path);
   SPALIB_API static string title(const char * path);
   SPALIB_API static string module_path();




   SPALIB_API static bool dtf(const char * pszFile, const char * pszDir);
   SPALIB_API static bool ftd(const char * pszDir, const char * pszFile);
   SPALIB_API static void is_valid_fileset(const char * pszFile);
   // 'n' (natural) terminated ascii number, example: 245765487n
   //static void write_n_number(FILE * pfile, class md5 * pctx, int iNumber);
   SPALIB_API static void read_n_number(FILE * pfile, class md5 * pctx, int & iNumber);
   //static void write_ex1_string(FILE * pfile, class md5 * pctx, string & str);
   SPALIB_API static void read_ex1_string(FILE * pfile, class md5 * pctx, string & str);

};

SPALIB_API bool read_resource_as_file(const char * pszFile, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
//SPALIB_API string get_temp_file_name_dup(const char * pszName,const char * pszExtension);
SPALIB_API int bzuncompress(LPCTSTR lpcszUncompressed, LPCTSTR lpcszGzFileCompressed);


void dll_processes(std::vector < DWORD > & dwa, stringa & straProcesses, const char * pszDll);






