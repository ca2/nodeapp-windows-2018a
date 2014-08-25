#include "framework.h"
#include <Shlobj.h>
#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"
#include <codecvt>
#include <string.h>

#include <psapi.h>

#define WINDOWS
#include <tlhelp32.h>


int str_ends_dup(const char * psz,const char * pszSuffix);
string ca2_module_folder_dup()
;
bool dir_is(const char * path1);

bool eat_end_level_dup(string & str,INT iLevelCount,const char * lpSeparator);
INT str_ends_ci_dup(const char * psz,const char * pszSuffix)
{

   INT iLen = strlen(psz);

   INT iLenSuffix = strlen(pszSuffix);

   if(iLen < iLenSuffix)
      return false;

   return _stricmp(&psz[iLen - iLenSuffix],pszSuffix) == 0;

}

#define PATH_SEPARATOR "\\"
bool dir_mk(const char * lpcsz);

void reg_delete_tree_dup(HKEY hkey,const char * name);

// convert UTF-8 string to wstring
std::wstring from_utf8(const std::string& str)
{
   std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
   return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string to_utf8(const std::wstring& str)
{
   std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
   return myconv.to_bytes(str);
}
void str_replace_all(string &s,const string &search,const string &replace) {
   for(size_t pos = 0; ; pos += replace.length()) {
      // Locate the substring to replace
      pos = s.find(search,pos);
      if(pos == string::npos) break;
      // Replace by erasing and inserting
      s.erase(pos,search.length());
      s.insert(pos,replace);
   }
}

string dir_get_ca2_module_folder();
string dir_element(const char * path1=NULL,const char * path2=NULL,const char * path3=NULL,const char * path4=NULL);

class removal :
   virtual public ::aura::simple_app
{
public:

   
   HANDLE                     m_hmutex_app_removal;
   
   char *                     m_modpath;
   char *                     m_pszDllEnds;
   UINT *                    m_dwaProcess;
   INT                        m_iSizeProcess;
   HMODULE *                  m_hmodulea;
   INT                        m_iSizeModule;
   bool                       m_bInstallerInstalling;
   HINSTANCE m_hinstance;

   removal();
   virtual ~removal();
   
   bool is_user_using(const char * pszDll);

   INT cube_run(const char * id);

   bool are_there_user_files_in_use();

   bool is_user_using(UINT processid, const char * pszDll);

   virtual bool initialize();

   virtual bool finalize();

};


// if MSVC CRT is used
extern "C" int32_t WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int32_t nCmdShow)
{

   UNREFERENCED_PARAMETER(lpCmdLine);

   int iRet = s_main < removal >(hInstance,hPrevInstance,lpCmdLine,nCmdShow);

   return iRet;



}

// if MSVC CRT is used
//extern "C" INT WINAPI
//WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//   __in LPTSTR lpCmdLine, INT nCmdShow)
//{
//   // call shared/exported WinMain
//   removal app;
//
//   app.initialize();
//
//
//   app.finalize();
//
//   return 0;
//   
//}

// if MSVC CRT is stripped
/*extern "C" INT WinMainCRTStartup() \
{ 

   ExitProcess(simple_app::s_main < removal > ());

}*/

//extern bool g_bInstalling;
//extern stringa * g_pstraTrace;

removal::removal()
{
   m_hinstance             = ::GetModuleHandleA(NULL);
   m_hmutex_app_removal  = NULL;
   m_modpath               = NULL;
   m_pszDllEnds            = NULL;
   m_dwaProcess            = NULL;
   m_iSizeProcess          = 0;
   m_hmodulea              = NULL;
   m_iSizeModule           = 0;
   m_bInstallerInstalling  = false;
   //g_pstraTrace            = NULL;
}

removal::~removal()
{
}


// CreateLink - Uses the Shell's IShellLink and IPersistFile interfaces 
//              to create and store a shortcut to the specified object. 
//
// Returns the result of calling the member functions of the interfaces. 
//
// Parameters:
// lpszPathObj  - Address of a buffer that contains the path of the object,
//                including the file name.
// lpszPathLink - Address of a buffer that contains the path where the 
//                Shell link is to be stored, including the file name.
// lpszDesc     - Address of a buffer that contains a description of the 
//                Shell link, stored in the Comment field of the link
//                properties.


HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszIconPath = NULL, INT iIcon = 0) 
{ 
    HRESULT hres; 
    IShellLinkW* psl; 
 
    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf; 
 
        // set the path to the shortcut target and add the description. 
        psl->SetPath(lpszPathObj); 
        psl->SetDescription(lpszDesc); 
        if(lpszIconPath!= NULL)
        {
        psl->SetIconLocation(lpszIconPath, iIcon);
        }
 
        // Query IShellLink for the IPersistFile interface, used for saving the 
        // shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
 
        if (SUCCEEDED(hres)) 
        { 
            //WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
//            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 
            
            // Add code here to check return value from MultiByteWideChar 
            // for success.
 
            // Save the link by calling IPersistFile::Save. 
            hres = ppf->Save(lpszPathLink, TRUE); 
            ppf->Release(); 
        } 
        psl->Release(); 
    } 
    return hres; 

}


string dir_path(const char * path1,const char * path2,const char * path3= NULL,const char * path4= NULL,const char * path5= NULL)
{
   string str(path1);
   if(str.substr(str.length() - 1,1) == PATH_SEPARATOR)
   {
      str = str.substr(0,str.length() - 1);
   }
   if(path2 != NULL)
   {
      string strAdd(path2);
      if(strAdd.substr(0,1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
   if(path3 != NULL)
   {
      string strAdd(path3);
      if(strAdd.substr(0,1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
   if(path4 != NULL)
   {
      string strAdd(path4);
      if(strAdd.substr(0,1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
   if(path5 != NULL)
   {
      string strAdd(path5);
      if(strAdd.substr(0,1) != PATH_SEPARATOR)
      {
         strAdd = PATH_SEPARATOR + strAdd;
      }
      str += strAdd;
   }
#if defined(LINUX) || defined(APPLEOS)
   str_replace_all(str, "\\","/");
#else
   str_replace_all(str, "/","\\");
#endif
   return str;
}


string get_dir(const KNOWNFOLDERID & rfid, const char * lpcsz)
{

   wchar_t * buf = NULL;
   
   SHGetKnownFolderPath(rfid, 0, NULL, &buf);

   wstring wstr(buf);

   string str = dir_path(to_utf8(wstr).c_str(), lpcsz);

   CoTaskMemFree(buf);

   return str;

}




void my_system(const char * pszCmd)
{
STARTUPINFO si;
PROCESS_INFORMATION pi;

ZeroMemory(&si, sizeof(si));
si.cb = sizeof(si);
ZeroMemory(&pi, sizeof(pi));

si.dwFlags |= STARTF_USESHOWWINDOW;
si.wShowWindow = SW_HIDE;

if (CreateProcess(NULL, (char *) pszCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
{
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

}


void rmdir(const char * pszDir)
{
   my_system(("cmd.exe /C rmdir /S /Q \"" + string(pszDir) + "\"").c_str());
}

void rmdir_n_v(const char * pszDir)
{

   string str(pszDir);
   rmdir(str.c_str());

   str_replace_all(str, ":", "");

   string str2 = dir_path(get_dir(FOLDERID_LocalAppData, "Microsoft\\Windows\\Temporary Internet Files\\Virtualized").c_str(), str.c_str());
   rmdir(str2.c_str());

  
   str2 = dir_path(get_dir(FOLDERID_LocalAppData,"Microsoft\\Windows\\INetCache\\Virtualized").c_str(),str.c_str());
   rmdir(str2.c_str());

}


void g_n_rmdir_n_v(const KNOWNFOLDERID & rfid, const char * pszDir)
{

   string strDir = get_dir(rfid, pszDir);

   rmdir_n_v(strDir.c_str());

}




bool removal::initialize()
{


   m_hmutex_app_removal = ::CreateMutex(NULL, FALSE, "Global\\::ca2::fontopus::ccca2_spa_app_removal::7807e510-5579-11dd-ae16-0800200c7784");
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      ::MessageBox(NULL, "ca2 app.removal.exe is already running.\n\nPlease wait for app.removal to finish or close it - using Task Manager - Ctrl+Shift+ESC - to continue.", "app.install.exe is running!", MB_ICONEXCLAMATION);
      return false;
   }

   char szFile[MAX_PATH * 8];

   ::GetModuleFileName(NULL, szFile, sizeof(szFile));

   string strTargetDir = get_dir(FOLDERID_ProgramFilesX86, "ca2.app.removal");

   dir_mk(strTargetDir.c_str());

   string strTarget = dir_path(strTargetDir.c_str(),"app.removal.exe");

   if(::CopyFile(szFile,strTarget.c_str(),TRUE))
   {
      INT i = ::MessageBox(NULL,"Do you want to place a shortcut to ca2 app.removal in Desktop?\n\nProgram has already been copied to Program Files Folder.","app.removal installation",MB_ICONQUESTION | MB_YESNOCANCEL);

      if(i == IDCANCEL)
         return false;

      if(i == IDYES)
      {
         string strLink= get_dir(FOLDERID_Desktop, "ca2 app.removal Tool.lnk");
         wstring wstrTarget(from_utf8(strTarget));
         wstring wstrLink(from_utf8(strLink));
         // create shortcurt;
         CreateLink(wstrTarget.c_str(), wstrLink.c_str(), L"ca2 app.removal Tool", wstrTarget.c_str(), 0);
      }
   }


   my_system("taskkill /F /IM app.exe");
   my_system("taskkill /F /IM app.install.exe");
   my_system("taskkill /F /IM app.plugin.container.exe");
   my_system("taskkill /F /IM plugin-container.exe");
   my_system("taskkill /F /IM iexplore.exe");
   my_system("taskkill /F /IM firefox.exe");
   
   g_n_rmdir_n_v(FOLDERID_ProgramFilesX86, "ca2");
   g_n_rmdir_n_v(FOLDERID_ProgramFiles,"ca2");
   g_n_rmdir_n_v(FOLDERID_ProgramData, "ca2");

   g_n_rmdir_n_v(FOLDERID_LocalAppData, "VirtualStore\\Program Files (x86)\\ca2");
   g_n_rmdir_n_v(FOLDERID_LocalAppData,"VirtualStore\\Program Files\\ca2");
   g_n_rmdir_n_v(FOLDERID_LocalAppData, "VirtualStore\\ProgramData\\ca2");

   g_n_rmdir_n_v(FOLDERID_Profile,"ca2");
   g_n_rmdir_n_v(FOLDERID_LocalAppDataLow,"ca2");
   g_n_rmdir_n_v(FOLDERID_RoamingAppData, "ca2");
   g_n_rmdir_n_v(FOLDERID_LocalAppData,"ca2");

   g_n_rmdir_n_v(FOLDERID_LocalAppDataLow,"core");
   g_n_rmdir_n_v(FOLDERID_Profile,"core");
   g_n_rmdir_n_v(FOLDERID_RoamingAppData,"core");
   g_n_rmdir_n_v(FOLDERID_RoamingAppData,"core");

   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\iexca2.dll");
   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\iexca2.inf");
   ::DeleteFile("C:\\Windows\\Downloaded Program Files\\app.install.exe");

   ::reg_delete_tree_dup(HKEY_LOCAL_MACHINE, "SOFTWARE\\Wow6432Node\\MozillaPlugins\\@ca2.cc/npca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "Wow6432Node\\CLSID\\{CA211984-1984-1977-A861-F8AA2A7AEE4B}");
   ::reg_delete_tree_dup(HKEY_LOCAL_MACHINE, "SOFTWARE\\MozillaPlugins\\@ca2.cc/npca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "CLSID\\{CA211984-1984-1977-A861-F8AA2A7AEE4B}");

   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ca2os.ca2.fontopus.iexca2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ca2os.ca2.fontopus.iexca2.2");
   ::reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ca2_spaboot_file");

   //simple_message_box(NULL, "Hope Helped!", "Hope Helped!", MB_ICONINFORMATION);


   ::MessageBox(NULL,"app-removal has finished cleaning ca2 directories.","ca2 app-removal",MB_ICONINFORMATION);



   return false;

}


// non-thread safe
bool removal::is_user_using(UINT processid, const char * pszDll)
{

   HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
   MODULEENTRY32 me32;
   bool bFound = false;

   hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, processid );
   if( hModuleSnap == INVALID_HANDLE_VALUE )
   {
      return false;
   }

   me32.dwSize = sizeof(MODULEENTRY32);

   if(!Module32First(hModuleSnap, &me32))
   {
      ::CloseHandle(hModuleSnap);
      return false;
   }

   strcpy(m_pszDllEnds, "\\");
   strcat(m_pszDllEnds, pszDll);

   do
   {
      if(str_ends_ci_dup(me32.szModule, m_pszDllEnds) || _stricmp(me32.szModule, pszDll) == 0)
      {
         bFound = true;
         break;
      }

   }
   while(Module32Next(hModuleSnap, &me32));

   ::CloseHandle( hModuleSnap );
   return bFound;

}

// non-thread safe
bool removal::is_user_using(const char * pszDll)
{
   HANDLE hProcessSnap;
   PROCESSENTRY32 pe32;

   hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if(hProcessSnap == INVALID_HANDLE_VALUE)
   {
      return false;
   }
   
   pe32.dwSize = sizeof(PROCESSENTRY32);

   if(!Process32First(hProcessSnap, &pe32))
   {
      ::CloseHandle(hProcessSnap);
      return false;
   }

   bool bUsing = false;
   do
   {
      if(is_user_using(pe32.th32ProcessID, pszDll))
      {
         bUsing = true;
         break;
      }
   }
   while(Process32Next(hProcessSnap, &pe32));

  ::CloseHandle(hProcessSnap);

  return bUsing;

}




bool removal::are_there_user_files_in_use()
{
#ifdef X86
   if(is_user_using(dir_element("stage\\x86\\msvcp110d.dll")))
      return true;
   if(is_user_using(dir_element("stage\\x86\\msvcr110d.dll")))
      return true;
   if(is_user_using(dir_element("stage\\x86\\ca.dll")))
      return true;
   if(is_user_using(dir_element("stage\\x86\\ca2.dll")))
      return true;
   if(is_user_using(dir_element("stage\\x86\\ca2.dll")))
      return true;
#else
   if(is_user_using(dir_element("stage\\x64\\msvcp110d.dll").c_str()))
      return true;
   if(is_user_using(dir_element("stage\\x64\\msvcr110d.dll").c_str()))
      return true;
   if(is_user_using(dir_element("stage\\x64\\ca.dll").c_str()))
      return true;
   if(is_user_using(dir_element("stage\\x64\\ca2.dll").c_str()))
      return true;
   if(is_user_using(dir_element("stage\\x64\\ca2.dll").c_str()))
      return true;
#endif
   return false;
}


bool removal::finalize()
{

   return true;

}










string dir_element(const char * path1,const char * path2,const char * path3,const char * path4)
{

#ifdef WINDOWS

   if(path1 == NULL && path2 == NULL && path3 == NULL && path4 == NULL)
   {

      string str;


      str = dir_get_ca2_module_folder();
      int i = str.rfind('\\');
      if(i == string::npos)
         return "";
      i = str.rfind('\\', i -1);
      if(i == string::npos)
         return "";

      str =str.substr(0,i);

      return str;
   }
   else
   {

      return dir_path(dir_element().c_str(),path1,path2,path3,path4);

   }

#else

   string strRelative = ca2_module_folder_dup();

   eat_end_level_dup(strRelative,2,"/");

   string str = dir_path(getenv("HOME"),".core/appdata");

   return dir_path(dir_path(str.c_str(),"ca2",strRelative.c_str()).c_str(),path1,path2,path3,path4);

#endif

}



string dir_get_ca2_module_folder()
{
   wchar_t lpszModuleFolder[MAX_PATH * 8];
#if defined(METROWIN)

   return "";

   char lpszModuleFilePath[MAX_PATH * 8];

   HMODULE hmodule = ::LoadPackagedLibrary(L"ca.dll",0);

   if(hmodule == NULL)
      hmodule = ::LoadPackagedLibrary(L"spalib.dll",0);

   if(hmodule == NULL)
   {

      string buf;

      throw metrowin_todo();
      //HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_NO_ALIAS, NULL, wtostring(buf, 4096));
      //if(FAILED(hr))
      // throw "dir::get_ca2_module_folder_dup : SHGetKnownFolderPath failed";

      strcpy(lpszModuleFilePath,buf.c_str());

      if(lpszModuleFilePath[strlen(lpszModuleFilePath) - 1] == '\\'
         || lpszModuleFilePath[strlen(lpszModuleFilePath) - 1] == '/')
      {
         lpszModuleFilePath[strlen(lpszModuleFilePath) - 1] = '\0';
      }
      strcat(lpszModuleFilePath,"\\core\\");
#ifdef X86
      strcat(lpszModuleFilePath,"stage\\x86\\");
#else
      strcat(lpszModuleFilePath,"stage\\x64\\");
#endif

      strcpy(lpszModuleFolder,lpszModuleFilePath);

      return true;

   }

   throw metrowin_todo();
   //GetModuleFileName(hmodule, lpszModuleFilePath, sizeof(lpszModuleFilePath));

   // xxx   LPTSTR lpszModuleFileName;

   throw metrowin_todo();
   //GetFullPathName(lpszModuleFilePath, sizeof(lpszModuleFilePath), lpszModuleFolder, &lpszModuleFileName);

   throw metrowin_todo();
   //lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

   throw metrowin_todo();
   /*
   if(strlen(lpszModuleFolder) > 0)
   {

   if(lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '/')
   {

   lpszModuleFolder[strlen(lpszModuleFolder) - 1] = '\0';

   }

   }
   */

   return true;

#elif defined(WINDOWS)

   wchar_t lpszModuleFilePath[MAX_PATH * 8];

   HMODULE hmodule = ::GetModuleHandleA("core.dll");

   if(hmodule == NULL)
      hmodule = ::GetModuleHandleA("spalib.dll");

   if(hmodule == NULL)
   {

      SHGetSpecialFolderPathW(
         NULL,
         lpszModuleFilePath,
         CSIDL_PROGRAM_FILES,
         FALSE);
      if(lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '\\'
         || lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '/')
      {
         lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] = '\0';
      }
      wcscat(lpszModuleFilePath,L"\\ca2\\");
#ifdef X86
      wcscat(lpszModuleFilePath,L"stage\\x86\\");
#else
      wcscat(lpszModuleFilePath,L"stage\\x64\\");
#endif

      wcscpy(lpszModuleFolder,lpszModuleFilePath);

      return to_utf8(lpszModuleFolder);

   }

   if(!GetModuleFileNameW(hmodule,lpszModuleFilePath,sizeof(lpszModuleFilePath) / sizeof(wchar_t)))
      return "";

   LPWSTR lpszModuleFileName;

   if(!GetFullPathNameW(lpszModuleFilePath,sizeof(lpszModuleFilePath) / sizeof(wchar_t),lpszModuleFolder,&lpszModuleFileName))
      return "";

   lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';

   if(wcslen(lpszModuleFolder) > 0)
   {

      if(lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '\\' || lpszModuleFolder[wcslen(lpszModuleFolder) - 1] == '/')
      {

         lpszModuleFolder[wcslen(lpszModuleFolder) - 1] = '\0';

      }

   }


#else

   wcscpy(lpszModuleFolder,L"/ca2/");



#endif

   return to_utf8(lpszModuleFolder);

}




void reg_delete_tree_dup(HKEY hkey,const char * name)
{
   HKEY hkeySub = NULL;
   if(ERROR_SUCCESS == ::RegOpenKey(
      hkey,
      name,
      &hkeySub))
   {
      UINT dwAlloc = 1026 * 64;
      char * szKey = (char *)malloc(dwAlloc);
      UINT dwIndex = 0;
      while(ERROR_SUCCESS == ::RegEnumKey(hkeySub,dwIndex,szKey,dwAlloc))
      {
         reg_delete_tree_dup(hkeySub,szKey);
         dwIndex++;
      }
      free(szKey);
      ::RegCloseKey(hkeySub);
   }
   ::RegDeleteKey(hkey,name);
}


bool dir_mk(const char * lpcsz)
{

#ifdef WINDOWS

   if(dir_is(lpcsz))
      return true;

   string url(lpcsz);
   string tmp;
   string dir;
   int oldpos = -1;
   int pos = url.find("\\");
   string unc("\\\\?\\");
   while(pos >= 0)
   {
      tmp = url.substr(oldpos + 1,pos - oldpos - 1);
      dir += tmp + "\\";
      wstring wstr(from_utf8(unc) + from_utf8(dir));
      UINT dw = ::GetFileAttributesW(wstr.c_str());
      if(dw == INVALID_FILE_ATTRIBUTES)
      {
         ::CreateDirectoryW(wstr.c_str(),NULL);
      }
      oldpos = pos;
      pos = url.find("\\",oldpos + 1);

   }
   tmp = url.substr(oldpos + 1);
   dir += tmp + "\\";
   wstring wstr(from_utf8(unc) + from_utf8(dir));
   if(::GetFileAttributesW(wstr.c_str()) == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectoryW(wstr.c_str(),NULL);
   }
   return true;

#else

   // stat -> Sir And Arthur - Serenato
   struct stat st;

   string url(lpcsz);
   string tmp;
   string dir;
   INT oldpos = -1;
   INT pos = url.find("/");
   while(pos >= 0)
   {
      tmp = url.substr(oldpos + 1,pos - oldpos - 1);
      dir += tmp + "/";
      if(stat(dir,&st))
      {
         mkdir(dir,S_IRWXU | S_IRWXG | S_IRWXO);
      }
      oldpos = pos;
      pos = url.find("/",oldpos + 1);

   }
   tmp = url.substr(oldpos + 1);
   dir += tmp + "/";
   if(stat(dir,&st))
   {
      mkdir(dir,S_IRWXU | S_IRWXG | S_IRWXO);
   }
   return true;

#endif

}


string ca2_module_folder_dup()
{

   static string * s_pstrCalc = NULL;

   if(s_pstrCalc != NULL)
   {

      return *s_pstrCalc;

   }

   string str;

#ifdef WINDOWSEX

   wchar_t lpszModuleFilePath[MAX_PATH + 1];
   GetModuleFileNameW(::GetModuleHandleA("ca.dll"),lpszModuleFilePath,MAX_PATH + 1);
   wchar_t lpszModuleFolder[MAX_PATH + 1];
   LPWSTR lpszModuleFileName;
   GetFullPathNameW(lpszModuleFilePath,MAX_PATH + 1,lpszModuleFolder,&lpszModuleFileName);
   str = string(lpszModuleFolder,lpszModuleFileName - lpszModuleFolder);

#elif defined(LINUX)

   void * handle = dlopen("libcore.so",RTLD_NOW);

   if(handle == NULL)
      return "";

   link_map * plm;

   dlinfo(handle,RTLD_DI_LINKMAP,&plm);

   string strCa2ModuleFolder = ::dir::name(plm->l_name);

   dlclose(handle);

   str = strCa2ModuleFolder;

#elif defined(METROWIN)

   str = "";

#elif defined(APPLEOS)

   {

      char * pszCurDir = getcwd(NULL,0);

      string strCurDir = pszCurDir;

      free(pszCurDir);

      if(file_exists_dup(::dir::path(strCurDir,"libca2.dylib")))
      {
         return strCurDir;
      }


      str = ::dir::name(::dir::pathfind(getenv("DYLD_LIBRARY_PATH"),"libca2.dylib","rfs")); // readable - normal file - non zero sized

   }

#endif

   s_pstrCalc = new string(str);

   return *s_pstrCalc;


}




bool eat_end_level_dup(string & str,INT iLevelCount,const char * lpSeparator)
{

   int iLast = str.length() - 1;

   if(iLast < 0)
      return iLevelCount <= 0;

   while(str[iLast] == '/' || str[iLast] == '\\')
      iLast--;

   for(INT i = 0; i < iLevelCount; i++)
   {

      int iFind1 = str.rfind('/',iLast);

      int iFind2 = str.rfind('\\',iLast);

      int iFind = max(iFind1,iFind2);

      if(iFind >= iLast)
         return false;

      if(iFind < 0)
         return false;

      iLast = iFind;

      while(str[iLast] == '/' || str[iLast] == '\\')
         iLast--;

   }

   str = str.substr(0,iLast + 1);

   return true;

}


bool dir_is(const char * path1)
{

#ifdef WINDOWS
   string str;

   str = "\\\\?\\";
   str += path1;

   while(str_ends_dup(str.c_str(),"\\") || str_ends_dup(str.c_str(),"/"))
   {
      str = str.substr(0,str.length() - 1);
   }

   UINT dwFileAttributes = ::GetFileAttributesW(from_utf8(str).c_str());
   if(dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
      dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      return true;
   else
      return false;

#else

   // dedicaverse stat -> Sir And Arthur - Cesar Serenato

   struct stat st;

   if(stat(path1,&st))
      return false;

   if(!(st.st_mode & S_IFDIR))
      return false;

   return true;

#endif

}



int str_ends_dup(const char * psz,const char * pszSuffix)
{

   int iLen = strlen(psz);

   int iLenSuffix = strlen(pszSuffix);

   if(iLen < iLenSuffix)
      return false;

   return strcmp(&psz[iLen - iLenSuffix],pszSuffix) == 0;

}
