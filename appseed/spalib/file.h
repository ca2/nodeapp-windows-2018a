#pragma once





class file
{
public:
   SPALIB_API static bool exists(const char * path);
   SPALIB_API static std::string get_contents(const char * path);
   SPALIB_API static void put_contents(const char * path, const char * contents);
   SPALIB_API static std::string md5(const char * path);
   SPALIB_API static int length(const char * path);
   SPALIB_API static std::string name(const char * path);
   SPALIB_API static std::string title(const char * path);
   SPALIB_API static std::string module_path();




   SPALIB_API static bool dtf(const char * pszFile, const char * pszDir);
   SPALIB_API static bool ftd(const char * pszDir, const char * pszFile);
   SPALIB_API static void is_valid_fileset(const char * pszFile);
   // 'n' (natural) terminated ascii number, example: 245765487n
   //static void write_n_number(FILE * pfile, class md5 * pctx, int iNumber);
   SPALIB_API static void read_n_number(FILE * pfile, class md5 * pctx, int & iNumber);
   //static void write_ex1_string(FILE * pfile, class md5 * pctx, std::string & str);
   SPALIB_API static void read_ex1_string(FILE * pfile, class md5 * pctx, std::string & str);

};

SPALIB_API bool read_resource_as_file(const char * pszFile, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
SPALIB_API std::string get_temp_file_name(const char * pszName, const char * pszExtension);
SPALIB_API int bzuncompress(LPCTSTR lpcszUncompressed, LPCTSTR lpcszGzFileCompressed);


void dll_processes(std::vector < DWORD > & dwa, stringa & straProcesses, const char * pszDll);