#include "StdAfx.h"
#include <shlwapi.h>

class handle
{
public:
   handle(HANDLE handle) : m_handle(handle) {}
   HANDLE m_handle;
};


class close_handle :
   virtual public handle
{
public:
   close_handle(HANDLE handleParam) : handle(handleParam) {}
   virtual ~close_handle() { ::CloseHandle(m_handle); }
};


extern std::string get_starter_version();

int bzuncompress_dup(LPCTSTR lpcszUncompressed, LPCTSTR lpcszGzFileCompressed);
bool read_resource_as_file_dup(const char * pszFile, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
std::string get_temp_file_name_dup(const char * pszName, const char * pszExtension);
bool file_exists_dup(const char * path1);



HANDLE g_hmutexBoot = NULL;
DWORD g_dwPrepareSmallBell = 0;

void parse_spaboot(const char * psz);
bool parse_spaboot_start(const char * psz);
//SPALIB_API std::string read_resource_as_string(HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
//int gzuncompress(LPCTSTR lpcszUncompressed, LPCTSTR lpcszGzFileCompressed);
//SPALIB_API bool read_resource_as_file(const char * pszFile, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
//SPALIB_API std::string get_temp_file_name(const char * pszName, const char * pszExtension);

//int start();

//SPALIB_API void prepare_small_bell();
//SPALIB_API void defer_play_small_bell();
//SPALIB_API void play_small_bell();

int APIENTRY ca2_cube_install(const char * pszId)
{
   g_hmutexBoot = NULL;
   SECURITY_ATTRIBUTES MutexAttributes;
   ZeroMemory( &MutexAttributes, sizeof(MutexAttributes) );
   MutexAttributes.nLength = sizeof( MutexAttributes );
   MutexAttributes.bInheritHandle = FALSE; // object uninheritable
   // declare and initialize a security descriptor
   SECURITY_DESCRIPTOR SD;
   BOOL bInitOk = InitializeSecurityDescriptor(
                     &SD,
                     SECURITY_DESCRIPTOR_REVISION );
   if ( bInitOk )
   {
      // give the security descriptor a Null Dacl
      // done using the  "TRUE, (PACL)NULL" here
      BOOL bSetOk = SetSecurityDescriptorDacl( &SD,
                                            TRUE,
                                            (PACL)NULL,
                                            FALSE );
      if ( bSetOk )
      {
         // Make the security attributes point
         // to the security descriptor
         MutexAttributes.lpSecurityDescriptor = &SD;
         g_hmutexBoot = ::CreateMutex(&MutexAttributes, FALSE, "Global\\ca2::fontopus::ccvotagus_ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
         if(::GetLastError() == ERROR_ALREADY_EXISTS)
         {
            while(::GetLastError() == ERROR_ALREADY_EXISTS)
            {
               ::CloseHandle(g_hmutexBoot);
               Sleep(2000 + 1977);
               g_hmutexBoot = ::CreateMutex(&MutexAttributes, FALSE, "Global\\ca2::fontopus::ccvotagus_ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
            }
         }
         ::CloseHandle(g_hmutexBoot);
      }
      else
      {
         ::OutputDebugString("error when trying to analyze if another instance of spa is already running");
         Sleep(2000 + 1977);
         return -200;
      }
   }
   else
   {
      ::OutputDebugString("error when trying to analyze if another instance of spa is already running");
      Sleep(2000 + 1977);
      return -201;
   }


   spa_set_id(pszId);
   std::string strPlatform = spa_get_platform();
   std::string strSp = get_ca2_folder_dup() + "\\ca2\\stage\\"+strPlatform+"\\spaboot.exe";

   SHELLEXECUTEINFO sei;
   memset(&sei, 0, sizeof(sei));
   sei.cbSize = sizeof(sei);
   sei.fMask = SEE_MASK_NOCLOSEPROCESS;
   sei.lpFile = strSp.c_str();
   sei.nShow = SW_SHOWNORMAL;
   std::string strParameters;

   strParameters = "starter_start=";
   strParameters += pszId;
   strParameters += " background";
   sei.lpParameters =  strParameters.c_str();

   if(!::ShellExecuteEx(&sei))
   {
      return 1;
   }

   DWORD dwExitCode;
   int i = 1;
   while(true)
   {
      if(!GetExitCodeProcess(sei.hProcess, &dwExitCode))
         break;
      if(dwExitCode != STILL_ACTIVE)
         break;
      Sleep(200);
      i++;
   }
   std::string strInstall;



   /*std::string strUrl;
   std::string strApp;
   strUrl = "http://spaignition.ca2api.net/query?node=install_application&id=";
   strUrl += g_strId;
   strUrl += "&key=application";
   int iRetry = 0;
   while(true)
   {
      strApp = ms_get(strUrl.c_str());
      if(strApp.length() > 0)
         break;
      iRetry++;
      if(iRetry > 30)
         break;
      Sleep(2000 + 1977);
   }
   if(iRetry > 30)
   {
      return 1;
   }*/

   return 0;
}




bool file_exists(const char * path1)
{
   DWORD dwFileAttributes = ::GetFileAttributes(path1);
   if(dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
      (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
      return true;
   else
      return false;
}



void parse_spaboot(XNode & node)
{
   if(node.name == "spa" && node.childs.size() > 0)
   {
      LPXNode lpnode = &node;
      for(unsigned int ui = 0; ui < lpnode->childs.size(); ui++)
      {
         if(lpnode->childs[ui]->name == "index")
         {
            if(lpnode->childs[ui]->GetAttr("start") != NULL)
            {
               if(std::string(lpnode->childs[ui]->GetAttrValue("start")).length() > 0)
               {
                  spa_set_id(std::string(lpnode->childs[ui]->GetAttrValue("start")).c_str());
               }
            }
         }
      }
   }
}

void parse_spaboot(const char * psz)
{
   XNode node;
   node.Load(file::get_contents(psz).c_str());
   parse_spaboot(node);
}


bool parse_spaboot_start(XNode & node)
{
   int iOkCount = 0;
   std::string strInstalledBuild;
   std::string strRequestedBuild;
   if(node.name == "spa" && node.childs.size() > 0)
   {
      LPXNode lpnode = &node;
      for(unsigned int ui = 0; ui < lpnode->childs.size(); ui++)
      {
         if(lpnode->childs[ui]->name == "index")
         {
            if(lpnode->childs[ui]->GetAttr("start") != NULL)
            {
               if(std::string(lpnode->childs[ui]->GetAttrValue("start")).length() > 0)
               {
                  spa_set_id(std::string(lpnode->childs[ui]->GetAttrValue("start")).c_str());
                  iOkCount++;
               }
               else
               {
                  return false;
               }
            }
            else
            {
               return false;
            }
         }
         else
         {
            return false;
         }
         if(lpnode->childs[ui]->GetAttr("build") != NULL)
         {
            if(std::string(lpnode->childs[ui]->GetAttrValue("build")).length() > 0)
            {
               strInstalledBuild = file::get_contents((get_ca2_folder_dup() + "\\ca2\\app\\build.txt").c_str());
               if(strInstalledBuild.length() <= 0)
                  return false;
               strRequestedBuild = std::string(lpnode->childs[ui]->GetAttrValue("build")).c_str();
               if(strRequestedBuild.length() <= 0)
                  return false;
               iOkCount++;
            }
            else
            {
               return false;
            }
         }
         else
         {
            return false;
         }
      }
   }
   if(iOkCount < 3)
      return false;
   if(strRequestedBuild != strInstalledBuild)
      return false;
   if(!is_installed(spa_get_id().c_str()))
      return false;
   return true;
}

bool parse_spaboot_start(const char * psz)
{
   XNode node;
   node.Load(file::get_contents(psz).c_str());
   return parse_spaboot_start(node);
}

void trace(const char * psz)
{
   printf("%s", psz);
}

int spaboot_start(const char * pszVersion, const char * pszId)
{
   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   std::string strInstall;
   std::string strVersion = pszVersion;
   std::string strId = pszId;
   std::string strPlatform = spa_get_platform();
   if(strId == "_set_windesk")
   {
      strInstall = get_ca2_folder_dup() + "\\ca2\\stage\\" + strPlatform + "\\cubeapp.exe : app=winservice_1";
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      if(!::CreateProcess(NULL, (LPSTR)  strInstall.c_str(),
         NULL, NULL, FALSE, 0, NULL, NULL,
         &si, &pi))
      {
         return 1;
      }
   }
   else
   {
      strInstall = get_ca2_folder_dup() + "\\ca2\\stage\\" + strPlatform + "\\cubeapp.exe";
      strInstall += " : ";
      strInstall += "app=bergedge bergedge_start=";
      strInstall += strId;
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      if(!::CreateProcess(NULL, (LPSTR)  strInstall.c_str(),
         NULL, NULL, FALSE, 0, NULL, NULL,
         &si, &pi))
      {
         return 1;
      }
   }
   
   return 0;
}



int bzuncompress_dup(LPCTSTR lpcszUncompressed, LPCTSTR lpcszGzFileCompressed)
{
   char * g_pchGzUncompressBuffer = NULL;
   int g_iGzUncompressLen = 1024 * 256;
   if(g_pchGzUncompressBuffer == NULL)
   {
       g_pchGzUncompressBuffer = new char[g_iGzUncompressLen];
   }
   BZFILE * file = BZ2_bzopen(lpcszGzFileCompressed, "rb");
   if (file == NULL)
   {
      fprintf(stderr, "bzopen error\n");
      return -2;
   }
   std::string strUn(lpcszUncompressed);
//   strUn += ".tmp";
   FILE * fileUn = fopen(strUn.c_str(), "wb+");
   if (fileUn == NULL)
   {
      BZ2_bzclose(file);
      int err;
      _get_errno(&err);
      fprintf(stderr, "fopen error\n %d", err);

      return -1;
   }
   int uncomprLen;
   while((uncomprLen = BZ2_bzread(file, g_pchGzUncompressBuffer, g_iGzUncompressLen)) > 0)
   {
      fwrite(g_pchGzUncompressBuffer, 1, uncomprLen, fileUn);
   }
   fclose(fileUn);
   BZ2_bzclose(file);
//   ::CopyFile(strUn.c_str(), lpcszUncompressed, FALSE);
//   ::DeleteFile(strUn.c_str());
   return 0;
}



bool read_resource_as_file_dup(
   const char * pszFile,
   HINSTANCE hinst,
   UINT nID, 
   LPCTSTR lpcszType)
{
	HRSRC hrsrc = ::FindResource(
		hinst,
		MAKEINTRESOURCE(nID), 
		lpcszType);
	if(hrsrc == NULL)
		return false;
    HGLOBAL hres = ::LoadResource(hinst, hrsrc);
	if(hres == NULL)
	   return false;
   DWORD dwResSize = ::SizeofResource(hinst, hrsrc);

   if(hres != NULL)
   {
      bool bOk = false;
		UINT FAR* lpnRes = (UINT FAR*)::LockResource(hres);
      HANDLE hfile = ::CreateFile(pszFile,                // name of the write
                       GENERIC_WRITE,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       CREATE_ALWAYS,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);  
      DWORD dwLastError = ::GetLastError();
      if(hfile != INVALID_HANDLE_VALUE)
      {
         DWORD dwWritten = 0;
         ::WriteFile(hfile, lpnRes, dwResSize, &dwWritten, NULL);
         ::CloseHandle(hfile);
         bOk = dwWritten == dwResSize;
      }
		#ifndef WIN32 //Unlock Resource is obsolete in the Win32 API
			::UnlockResource(hres);
		#endif
            ::FreeResource(hres);
      return bOk;
	}
	return false;

}


std::string get_temp_file_name_dup(const char * pszName, const char * pszExtension)
{
   char lpPathBuffer[MAX_PATH * 16];
 // Get the temp path.
   DWORD dwRetVal = GetTempPath(sizeof(lpPathBuffer),     // length of the buffer
                        lpPathBuffer); // buffer for path 
   if (dwRetVal > sizeof(lpPathBuffer) || (dwRetVal == 0))
   {
      printf ("GetTempPath failed (%d)\n", GetLastError());
      return "";
   }
   std::string str;
   char buf[30];
   int iLen= strlen(lpPathBuffer);
   if(!(lpPathBuffer[iLen - 1] == '/'
      || lpPathBuffer[iLen - 1] == '\\'))
   {
      lpPathBuffer[iLen] = '\\';
      lpPathBuffer[iLen+1] = '\0';
   }
   for(int i = 0; i < (64 * 1024); i++)
   {
      sprintf(buf, "%d", i);
      str = lpPathBuffer;
      str += pszName;
      str += "-";
      str += buf;
      str += ".";
      str += pszExtension;
      if(file_exists_dup(str.c_str()))
      {
         if(::DeleteFileA(str.c_str()))
            return str;
      }
      else
      {
         return str;
      }
   }
   return "";
}

bool file_exists_dup(const char * path1)
{
   DWORD dwFileAttributes = ::GetFileAttributes(path1);
   if(dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
      (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
      return true;
   else
      return false;
}


int install_spa()
{
   std::string strPlatform;

#ifdef _X86_
   strPlatform = "x86";
#else
   strPlatform = "x64";
#endif

   DWORD dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2").c_str());
   if(dw == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory((get_ca2_folder_dup() + "\\ca2").c_str(), NULL);
   }
   dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\stage").c_str());
   if(dw == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\stage").c_str(), NULL);
   }
   dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\stage\\" + strPlatform).c_str());
   if(dw == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\stage\\" + strPlatform).c_str(), NULL);
   }
   dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\time").c_str());
   if(dw == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\time").c_str(), NULL);
   }
   dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\time\\bz").c_str());
   if(dw == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\time\\bz").c_str(), NULL);
   }
   dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\time\\bz\\stage").c_str());
   if(dw == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\time\\bz\\stage").c_str(), NULL);
   }
   dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\time\\bz\\stage\\" + strPlatform).c_str());
   if(dw == INVALID_FILE_ATTRIBUTES)
   {
      ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\time\\bz\\stage\\" + strPlatform).c_str(), NULL);
   }

   // ignore errors and try to go on

   bool bOk = true;

   bOk = bOk && read_resource_as_file_dup((get_ca2_folder_dup() + "\\ca2\\stage\\" + strPlatform + "\\spa_bspatch.dll").c_str(), ::GetModuleHandleA(ca2_browser_plugin_get_module_name()), ID_CGCLBOOT, "CA2SP");



   std::string strGz = get_ca2_folder_dup() + "\\ca2\\time\\bz\\stage\\" + strPlatform + "\\spalib.bz";
   bOk = bOk && read_resource_as_file_dup(strGz.c_str(), ::GetModuleHandleA(ca2_browser_plugin_get_module_name()), ID_CST, "CA2SP");
   std::string strSp = get_ca2_folder_dup() + "\\ca2\\stage\\" + strPlatform + "\\spalib.dll";
   SetDllDirectory((get_ca2_folder_dup() + "\\ca2\\stage\\" + strPlatform + "\\").c_str());
   bOk = bOk && !bzuncompress_dup(strSp.c_str(), strGz.c_str());




   strGz = get_ca2_folder_dup() + "\\ca2\\time\\bz\\stage\\" + strPlatform + "\\spaadmin.bz";
   bOk = bOk && read_resource_as_file(strGz.c_str(), ::GetModuleHandleA(ca2_browser_plugin_get_module_name()), ID_CSTBOOT, "CA2SP");
   strSp = dir::beforeca2() + "\\ca2\\stage\\" + strPlatform + "\\spaadmin.exe";
   bOk = bOk && !bzuncompress(strSp.c_str(), strGz.c_str());



   strGz = get_ca2_folder_dup() + "\\ca2\\time\\bz\\stage\\" + strPlatform + "\\spaboot.bz";
   bOk = bOk && read_resource_as_file(strGz.c_str(), ::GetModuleHandleA(ca2_browser_plugin_get_module_name()), ID_STARTACCA, "CA2SP");
   strSp = dir::beforeca2() + "\\ca2\\stage\\" + strPlatform + "\\spaboot.exe";
   bOk = bOk && !bzuncompress(strSp.c_str(), strGz.c_str());


   bOk = bOk && read_resource_as_file((get_ca2_folder_dup() + "\\ca2\\stage\\small_bell.mp3").c_str(), ::GetModuleHandleA("spalib.dll"), ID_MP3_SMALL_BELL, "MP3");



   //std::string strBuild = spalib_get_build();

   std::string strBuild = get_starter_version();

   update_spa_installed();
   if(is_spa_installed() && bOk)
   {
      dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\appdata").c_str());
      if(dw == INVALID_FILE_ATTRIBUTES)
      {
         ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\appdata").c_str(), NULL);
      }
      dw = GetFileAttributes((get_ca2_folder_dup() + "\\ca2\\appdata\\" + strPlatform).c_str());
      if(dw == INVALID_FILE_ATTRIBUTES)
      {
         ::CreateDirectory((get_ca2_folder_dup() + "\\ca2\\appdata\\" + strPlatform).c_str(), NULL);
      }
      file::put_contents(dir::path(dir::ca2().c_str(),("appdata\\" + strPlatform + "\\spa_build_" +strPlatform + ".txt").c_str()).c_str(),strBuild.c_str());
   }

   return 0;
}



