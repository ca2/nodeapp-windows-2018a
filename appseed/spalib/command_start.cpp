#include "StdAfx.h"

#include <Winternl.h>


/*typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(
    HANDLE ProcessHandle,
    DWORD ProcessInformationClass, /* can't be bothered defining the whole enum */
  /*  PVOID ProcessInformation,
    DWORD ProcessInformationLength,
    PDWORD ReturnLength
    );

typedef struct _PROCESS_BASIC_INFORMATION
{
    ...
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;*/


typedef PVOID * PPVOID;

/*typedef struct _PEB1
{
   BOOLEAN InheritedAddressSpace; 
   BOOLEAN ReadImageFileExecOptions; 
   BOOLEAN BeingDebugged; 
   BOOLEAN Spare; 
   HANDLE Mutant;
   PVOID ImageBaseAddress; 
   PPEB_LDR_DATA LoaderData; 
   PRTL_USER_PROCESS_PARAMETERS ProcessParameters; 
} PEB1, *PPEB1;*/

/*typedef struct _PEB2 {
_PEB1 peb1;
PVOID SubSystemData; 
PVOID ProcessHeap;
PVOID FastPebLock; 
PPEBLOCKROUTINE FastPebLockRoutine; 
PPEBLOCKROUTINE FastPebUnlockRoutine;
ULONG EnvironmentUpdateCount; 
PPVOID KernelCallbackTable; 
PVOID EventLogSection; 
PVOID EventLog; 
PPEB_FREE_BLOCK FreeList; 
ULONG TlsExpansionCounter; 
PVOID TlsBitmap; 
ULONG TlsBitmapBits[0x2]; 
PVOID ReadOnlySharedMemoryBase; 
PVOID ReadOnlySharedMemoryHeap; 
PPVOID ReadOnlyStaticServerData;
PVOID AnsiCodePageData; 
PVOID OemCodePageData; 
PVOID UnicodeCaseTableData; 
ULONG NumberOfProcessors; 
ULONG NtGlobalFlag; 
BYTE Spare2[0x4]; 
LARGE_INTEGER CriticalSectionTimeout; 
ULONG HeapSegmentReserve; 
ULONG HeapSegmentCommit; 
ULONG HeapDeCommitTotalFreeThreshold; 
ULONG HeapDeCommitFreeBlockThreshold; 
ULONG NumberOfHeaps; 
ULONG MaximumNumberOfHeaps; 
PPVOID *ProcessHeaps; 
PVOID GdiSharedHandleTable; 
PVOID ProcessStarterHelper; 
PVOID GdiDCAttributeList; 
PVOID LoaderLock; 
ULONG OSMajorVersion; 
ULONG OSMinorVersion; 
ULONG OSBuildNumber; 
ULONG OSPlatformId; 
ULONG ImageSubSystem; 
ULONG ImageSubSystemMajorVersion; 
ULONG ImageSubSystemMinorVersion; 
ULONG GdiHandleBuffer[0x22]; 
ULONG PostProcessInitRoutine; 
ULONG TlsExpansionBitmap; 
BYTE TlsExpansionBitmapBits[0x80]; 
ULONG SessionId;

} PEB, *PPEB;*/


/*typedef struct _RTL_USER_PROCESS_PARAMETERS1
{
    ULONG MaximumLength;
    ULONG Length;
    ULONG Flags;
    ULONG DebugFlags;
    PVOID ConsoleHandle;
    ULONG ConsoleFlags;
    HANDLE StdInputHandle;
    HANDLE StdOutputHandle;
    HANDLE StdErrorHandle;
    /* +0x24 */ /*UNICODE_STRING CurrentDirectoryPath;
    HANDLE CurrentDirectoryHandle;
    /* +0x30 */ //UNICODE_STRING DllPath;
    /* +0x38 */ //UNICODE_STRING ImagePathName;
    /* +0x40 */// UNICODE_STRING CommandLine;
    /*  ... more stuff you probably won't care about */
//} RTL_USER_PROCESS_PARAMETERS1, * RTL_USER_PROCESS_PARAMETERS1;


/*typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;*/

typedef struct _PROCESS_BASIC_INFORMATION64 {
    unsigned __int64 Reserved1;
    unsigned __int64  PebBaseAddress;
    unsigned __int64  Reserved2[2];
    unsigned __int64  UniqueProcessId;
    unsigned __int64  Reserved3;
} PROCESS_BASIC_INFORMATION64;

typedef NTSTATUS (NTAPI *_NtQueryInformationProcess)(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, DWORD ProcessInformationLength, PDWORD ReturnLength);


std::string get_display_error(DWORD NTStatusMessage);

PPEB GetPebAddress(HANDLE handleProcess)
{
   _NtQueryInformationProcess NtQueryInformationProcess = (_NtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
   PROCESS_BASIC_INFORMATION pbi;
   memset(&pbi, 0, sizeof(pbi));
   DWORD dwInLen = sizeof(pbi);
   DWORD dwOutLen = 0xffffffff;
   DWORD dwStatus = NtQueryInformationProcess(handleProcess, ProcessBasicInformation, &pbi, dwInLen, &dwOutLen);
   std::string strError = get_display_error(dwStatus);
   if((dwStatus & 3) == 3)
   {
      return NULL;
   }
   return pbi.PebBaseAddress;
}


std::string get_display_error(DWORD NTStatusMessage)
{
   LPVOID lpMessageBuffer;
   HMODULE Hand = LoadLibrary("NTDLL.DLL");
   
   FormatMessage( 
       FORMAT_MESSAGE_ALLOCATE_BUFFER | 
       FORMAT_MESSAGE_FROM_SYSTEM | 
       FORMAT_MESSAGE_FROM_HMODULE,
       Hand, 
       NTStatusMessage,  
       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
       (LPTSTR) &lpMessageBuffer,  
       0,  
       NULL );

   // Now display the string.

   std::string str = (LPTSTR) lpMessageBuffer;

   // Free the buffer allocated by the system.
   LocalFree( lpMessageBuffer ); 
   FreeLibrary(Hand);
   return str;
}


std::string get_command_line(HANDLE handleProcess)
{

   PPEB ppeb = GetPebAddress(handleProcess);

   PRTL_USER_PROCESS_PARAMETERS pparam1 = NULL;

   if(!ReadProcessMemory(handleProcess, (PCHAR)&ppeb->ProcessParameters, &pparam1, sizeof(PRTL_USER_PROCESS_PARAMETERS), NULL) || pparam1 == NULL)
   {
      printf("Could not read the address of ProcessParameters!\n");
      return "";
   }


   _UNICODE_STRING ustrCommandLine;

   if(!ReadProcessMemory(handleProcess, (PCHAR)&pparam1->CommandLine, &ustrCommandLine, sizeof(ustrCommandLine), NULL))
   {
      printf("Could not read CommandLine!\n");
      return "";
   }

   /* allocate memory to hold the command line */
   WCHAR * commandLineContents = (WCHAR *)malloc(ustrCommandLine.Length + sizeof(WCHAR));
   /* read the command line */
   if (!ReadProcessMemory(handleProcess, ustrCommandLine.Buffer, commandLineContents, ustrCommandLine.Length, NULL))
   {
      printf("Could not read the command line string!\n");
      return "";
   }
   commandLineContents[ustrCommandLine.Length / sizeof(WCHAR)] = L'\0';
   std::string str = u8(commandLineContents);
   free(commandLineContents);
   return str;
}

int run_file(const char * pszFile, int nCmdShow);

HANDLE g_hmutexInstall = NULL;

stringa g_straTerminateProcesses;
stringa g_straRestartCommandLine;
std::string g_strLastHost;
std::string g_strCurrentHost;
bool g_bStarterStart = false;
MSG g_msg;
bool g_bAdmin = false;
std::string g_strId = "";
std::string g_strPlatform = "";

SPALIB_API bool spa_get_admin()
{
   return g_bAdmin;
}

SPALIB_API void spa_set_admin(bool bSet)
{
   g_bAdmin = bSet;
}

SPALIB_API std::string spa_get_id()
{
   return g_strId;
}

SPALIB_API void spa_set_id(const char * psz)
{
   g_strId = psz;
}

SPALIB_API std::string spa_get_platform()
{
#ifdef _X86_
   return "x86";
#else
   return "x64";
#endif
}

//DWORD WINAPI ThreadProcCommandStart(LPVOID lpParam)
//{
//      g_hmutexInstall = NULL;
//      SECURITY_ATTRIBUTES MutexAttributes;
//      ZeroMemory( &MutexAttributes, sizeof(MutexAttributes) );
//      MutexAttributes.nLength = sizeof( MutexAttributes );
//      MutexAttributes.bInheritHandle = FALSE; // object uninheritable
//      // declare and initialize a security descriptor
//      SECURITY_DESCRIPTOR SD;
//      BOOL bInitOk = InitializeSecurityDescriptor(
//                        &SD,
//                        SECURITY_DESCRIPTOR_REVISION );
//      if ( bInitOk )
//      {
//         // give the security descriptor a Null Dacl
//         // done using the  "TRUE, (PACL)NULL" here
//         BOOL bSetOk = SetSecurityDescriptorDacl( &SD,
//                                               TRUE,
//                                               (PACL)NULL,
//                                               FALSE );
//         if ( bSetOk )
//         {
//            // Make the security attributes point
//            // to the security descriptor
//            MutexAttributes.lpSecurityDescriptor = &SD;
//            g_hmutexInstall = ::CreateMutex(&MutexAttributes, FALSE, "Global\\ca2::fontopus::ccvotagus_ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
//            if(::GetLastError() == ERROR_ALREADY_EXISTS)
//            {
//               trace("another instance of spa is already running");
//               exit(-202);
//               return -202;
//            }
//         }
//         else
//         {
//            trace("error when trying to analyze if another instance of spa is already running");
//            exit(-200);
//            return -200;
//         }
//      }
//      else
//      {
//         trace("error when trying to analyze if another instance of spa is already running");
//         exit(-201);
//         return -201;
//      }
//
//      //::MessageBox(g_hwnd, "Start", "Start", MB_OK);
//      keep_true keeptrueInstalling(g_bInstalling);
//      g_dwInstallStartTime = ::GetTickCount();
//install_begin:;
//   {
//      g_strSpa.clear();
//      g_iTotalGzLen = 0;
//      g_NeedRestartBecauseOfReservedFile = false;
//      g_NeedRestartFatalError = false;
//      int iFileError = 0;
//
//      std::string strUrl;
//      trace(("get application name from server http://spaignition.api.veriterse.net/ using id \"" + g_strStart + "\" ").c_str());
//      strUrl = "http://spaignition.api.veriterse.net/query?node=install_application&id=";
//      strUrl += g_strStart;
//      strUrl += "&key=name";
//
//      std::string strName;
//      int iRetry = 0;
//      while(true)
//      {
//         strName = ms_get(strUrl.c_str());
//         if(strName.length() > 0)
//            break;
//         else if(iRetry < 84)
//            iRetry++;
//         else
//         {
//            trace("main server seems to be unavailable. could not get application name");
//            exit(-1);
//            return -1;
//         }
//         trace_add(".");
//         Sleep(484);
//      }
//      strName = "Installing " + strName;
//      g_strTitle = strName;
//
//      trace(("***" + strName).c_str());
//      
//      iRetry = 0;
//   RetryBuildNumber:
//      if(g_strBuildResource.length() == 19) // heuristically valid
//      {
//         trace("***Internal build number");
//         g_strBuild = g_strBuildResource;
//      }
//      else
//      {
//         trace("***Getting build number");
//         if(iRetry > 10)
//         {
//            trace("could not get build number. exiting");
//            exit(-1);
//            return -1;
//         }
//         iRetry++;
//         g_strBuild = ms_get("http://spaignition.api.veriterse.net/query?node=build");
//         str_trim(g_strBuild);
//         if(g_strBuild.length() != 19)
//         {
//            Sleep(1984);
//            goto RetryBuildNumber;
//         }
//      }
//      trace(g_strBuild.c_str());
//
//
//      int iHostRetry = 0;
//   RetryHost:
//      if(iHostRetry > 84)
//      {
//         exit(-1);
//         return -1;
//      }
//      else if(iHostRetry > 0)
//      {
//         trace("Retrying to guess host...");
//      }
//      iHostRetry = 1;
//      if(g_strLastHost.empty())
//      {
//         g_strLastHost = g_strCurrentHost;
//      }
//      else
//      {
//         g_strLastHost = g_strCurrentHost + ";" + g_strLastHost;
//      }
//      strUrl = "http://spaignition.api.veriterse.net/query?node=spa_host";
//      if(!g_strLastHost.empty())
//      {
//         strUrl += "&last_host=" + g_strLastHost;
//      }
//      trace("***Guessing fastest mirror");
//      int iGuessRetry = 0;
//      trace(".");
//      std::string strSpaHost;
//      while(iGuessRetry < 30)
//      {
//         strSpaHost = ms_get(strUrl.c_str());
//         if(strSpaHost.empty())
//         {
//            if(g_strLastHost.empty())
//            {
//               trace_add(".");
//               Sleep(484);
//               iGuessRetry++;
//            }
//            else
//            {
//               strUrl = "http://spaignition.api.veriterse.net/query?node=spa_host";
//            }
//         }
//         else
//         {
//            break;
//         }
//      }
//      if(g_strCurrentHost == strSpaHost)
//      {
//         g_strLastHost.clear();
//         goto RetryHost;
//      }
//      g_strCurrentHost = strSpaHost;
//      trace(("got server: " + strSpaHost).c_str());
//      g_strInstall = "http://" + strSpaHost + "/stage/";
//      g_strInstallGz = g_strInstall;
//      trace(strSpaHost.c_str());
//
//      
//      char path[1024 * 4];
//      ::GetModuleFileNameA(NULL, path, sizeof(path));
//      int iRetryDeleteSpa = 0;
//      std::string strFile;
//
//      HKEY hkey;
//
//      strUrl = "http://spaignition.api.veriterse.net/install_filter_list";
//      std::string strInstallFilterList = ms_get(strUrl.c_str());
//      XNode nodeInstallFilter;
//      nodeInstallFilter.Load(strInstallFilterList.c_str());
//      strUrl = "http://spaignition.api.veriterse.net/query?node=install_application&id=";
//      strUrl += g_strStart;
//      strUrl += "&key=install_filter";
//      std::string strInstallFilter = ms_get(strUrl.c_str());
//      for(unsigned int ui = 0; ui < nodeInstallFilter.childs.size(); ui++)
//      {
//         LPXNode lpchild = nodeInstallFilter.childs[ui];
//         std::string strId;
//         strId = lpchild->GetAttrValue("id");
//         std::string strFilter;
//         strFilter = "|" + strId + "|"; // ex. "|multimedia|"
//         if(strInstallFilter.find(strFilter) != std::string::npos)
//         {
//            std::string strKey;
//            strKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\ca2_fontopus_votagus_" + strId;
//            
//            if(::RegCreateKey(HKEY_LOCAL_MACHINE,
//               strKey.c_str(),
//               &hkey) == ERROR_SUCCESS)
//            {
//               ::RegSetValueEx(
//                  hkey,
//                  "DisplayIcon",
//                  0,
//                  REG_SZ, 
//                  (const BYTE *) strFile.c_str(),
//                  strFile.length());
//               std::string strDisplayName;
//               strDisplayName = "ca2 fontopus votagus - ";
//               strKey = "install_filter_title_" + strId;
//               strDisplayName += load_string(strKey.c_str(), strId.c_str());
//               ::RegSetValueEx(
//                  hkey,
//                  "DisplayName",
//                  0,
//                  REG_SZ, 
//                  (const BYTE *) strDisplayName.c_str(),
//                  strDisplayName.length());
//               ::RegSetValueEx(
//                  hkey,
//                  "UninstallString",
//                  0,
//                  REG_SZ, 
//                  (const BYTE *) strInstallFilter.c_str(),
//                  strInstallFilter.length());
//               ::RegSetValueEx(
//                  hkey,
//                  "ModifyString",
//                  0,
//                  REG_SZ, 
//                  (const BYTE *) strInstallFilter.c_str(),
//                  strInstallFilter.length());
//            }
//         }
//      }
//
//
//
//      stringa straFileList;
//      strintmap mapLen;
//      strstrmap mapMd5;
//      strintmap mapGzLen;
//
//
//
//      XNode nodeInstall;
//
//      nodeInstall.Load(file::get_contents(dir::appdata("spa_install.xml").c_str()).c_str());
//
//      LPXNode lpnodeInstalled = nodeInstall.GetChild("installed");
//
//
//
//      trace("***Downloading file list.");
//      
//      std::string strBuild;
//      strBuild = str_replace(g_strBuild.c_str(), " ", "_");
//      strBuild = str_replace(strBuild.c_str(), ":", "-");
//      std::string strIndexPath;
//      if(!ca2_fy_url(strIndexPath, ("app/stage/metastage/index-"+strBuild+".spa.bz").c_str(), false, -1, NULL, -1, true))
//      {
//         trace("Failed to download file list!");
//         trace("Going to retry host...");
//         Sleep(584);
//         goto RetryHost;
//      }
//      strUrl = "http://" + strSpaHost + "/stage/app/stage/metastage/index-"+strBuild+".md5";
//      std::string strCgclIndexMd5 = ms_get(strUrl.c_str(), false);
//      if(strCgclIndexMd5.length() != 32 
//      || file::md5(strIndexPath.c_str()) != strCgclIndexMd5)
//      {
//         trace("Invalid file list!");
//         trace("Going to retry host...");
//         Sleep(584);
//         goto RetryHost;
//      }
//      else
//      {
//         Sleep(584);
//      }
//      ParseIndexFile(strIndexPath.c_str(), mapLen, mapMd5, mapGzLen);
//
//
//      if(!GetFileList(straFileList, ("app/stage/metastage/" + g_strStart + ".spa").c_str(), mapLen, mapGzLen, mapMd5))
//      {
//            trace("Failed to download file list!");
//            trace("Going to retry host...");
//            Sleep(584);
//            goto RetryHost;
//      }
//      if(lpnodeInstalled != NULL)
//      {
//         for(unsigned int ui = 0; ui < lpnodeInstalled->childs.size(); ui++)
//         {
//            std::string strId = lpnodeInstalled->childs[ui]->GetAttrValue("id");
//            if(strcmp(strId.c_str(), g_strStart.c_str()) != 0)
//            {
//               GetFileList(straFileList, ("app/stage/metastage/" + strId + ".spa").c_str(), mapLen, mapGzLen, mapMd5);
//            }
//         }
//      }
//     std::string strStart;
//     XNode nodeSpaStart;
//     nodeSpaStart.Load(file::get_contents(dir::appdata("spa_start.xml").c_str()).c_str());
//     for(unsigned int ui = 0; ui < nodeSpaStart.childs.size(); ui++)
//     {
//	     strStart = nodeSpaStart.childs[ui]->GetAttrValue("id");
//	     bool bGet = true;
//        if(lpnodeInstalled != NULL)
//        {
//           for(unsigned int ui = 0; ui < lpnodeInstalled->childs.size(); ui++)
//           {
//              std::string strId = lpnodeInstalled->childs[ui]->GetAttrValue("id");
//              if(strId == strStart)
//              {
//                 bGet = false;
//              }
//           }
//        }
//
//        if(bGet && g_strStart == strStart)
//        {
//            bGet = false;
//        }
//		   if(bGet)
//		   {
//			   std::string strGet;
//			   strGet = "app/stage/metastage/";
//			   strGet += strStart;
//			   strGet += ".spa";
//			   GetFileList(straFileList, strGet.c_str(), mapLen, mapGzLen, mapMd5);
//		   }
//         }
//      
//
//      trace("***Downloading files.");
//      if(g_bInternetInstall)
//      {
//         DownloadFileList(straFileList, mapLen, mapMd5, mapGzLen);
//      }
//      else
//      {
//         UncompressFileList(straFileList, mapMd5);
//      }
//
//      /*iRetry = 0;
//      while(is_there_application_opened())
//      {
//         machine_signalize_close_application();
//         if(iRetry == 0)
//         {
//            trace("***Suggesting ca2 applications to close...");
//            trace("Installation will continue after ca2 applications have been closed.");
//            trace("You may force closing them by yourself if they do not accept suggestion.");
//            trace("Waiting for applications to close...");
//            trace(".");
//         }
//         trace(".");
//         iRetry++;
//         Sleep(1984);
//      }*/
//      
//
//      /*if(stringa.size() > 0)
//      {
//         std::string strUrl;
//         strUrl = stringa[0];
//         std::string strStage;
//         
//         strStage = dir::path(dir::stage().c_str(), "ca2\\stage\\ca2\\fontopus\\app\\main\\front\\Release\\bergedgeapp.exe");
//         std::string strParam;
//         strParam = "start=" + file::name(strUrl.c_str());
//         int i = (int) ::ShellExecute(g_hwnd, "open", strStage.c_str(), strParam.c_str(), dir::name(strStage.c_str()).c_str(), SW_SHOWNORMAL);
//         if(i >= 32)
//         {
//            ::PostMessage(g_hwnd, WM_CLOSE, 0, 0);
//         }
//      }*/
//
//      
//      std::vector < DWORD > dwa;
//
//      dll_processes(dwa, g_straTerminateProcesses, (dir::ca2("stage\\x86\\ca2.dll").c_str()));
//
//      if(g_straTerminateProcesses.size() > 0)
//      {
//         std::string strCommand;
//         for(int i = 0; i < g_straTerminateProcesses.size(); i++)
//         {
//            if(i >= 1)
//            {
//               strCommand += ";";
//            }
//            std::string str;
//            str = "Should ca2 Terminate and try to restart process \"";
//            str += g_straTerminateProcesses.at(i);
//            str += "\"?";
//            strCommand += str;
//            if(::MessageBox(NULL, str.c_str(), "need to terminate process to install", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
//            {
//               HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE |
//                            PROCESS_VM_READ,
//                            FALSE, dwa[i] );
//               if(hProcess == NULL)
//               {
//                  ::MessageBox(NULL, "Failed to terminate process","Failed to terminate process", MB_OK);
//                  return -1;
//               }
//
//               g_straRestartCommandLine.push_back(get_command_line(hProcess));
//
//               if(!TerminateProcess(hProcess, -1))
//               {
//                  ::MessageBox(NULL, "Failed to terminate process", "Failed to terminate process", MB_OK);
//                  return -1;
//               }
//            }
//            else
//            {
//               return -1;
//            }
//
//         }
//         file::put_contents("C:\\ca2\\machine\\on_after_spaadmin.txt", strCommand.c_str());
//      }
//
//      if(g_straRestartCommandLine.size() > 0)
//      {
//
//         HWND hwndSpaBoot = ::FindWindow(NULL, "ca2::fontopus::ccvotagus::spaboot:callback_window");
//
//         if(hwndSpaBoot != NULL)
//         {
//            std::string str = g_straRestartCommandLine.encode_v16();
//            COPYDATASTRUCT cds;
//            memset(&cds, 0, sizeof(cds));
//            cds.dwData = 15111984;
//            cds.cbData = str.length();
//            cds.lpData = (PVOID) str.c_str();
//            ::SendMessageA(hwndSpaBoot, WM_COPYDATA, NULL, (LPARAM) &cds);
//         }
//      }
//
//
//      CopyFileList(straFileList);
//
//      iRetry = 0;
//      while((!machine_unsignalize_close_application()
//        || machine_check_close_application(true))&& iRetry < 840)
//      {
//         if(iRetry == 0)
//         {
//            trace("Enabling applications to run.");
//         }
//         iRetry++;
//         Sleep(484);
//         if((iRetry % 33) == 0)
//         {
//            trace(".");
//         }
//         else
//         {
//            trace_add(".");
//         }
//      }
//      trace("***executing installer.");
//
//      if(g_NeedRestartBecauseOfReservedFile
//      || g_NeedRestartFatalError)
//      {
//	      add_spa_start(g_strStart.c_str());
//
//         if(::MessageBox(NULL, "The computer need to be restarted!!\n\nDo you want to restart now?\n\nWe recommend you to close all other applications first and then agree with this question using the buttons below.", "spa - Restart Needed!!", MB_ICONEXCLAMATION | MB_YESNO)
//            == IDYES)
//         {
//            m_reboot();
//         }
//      }
//      else
//      {
//         if(!ignit_phase2())
//         {
//            trace("***Going to ebing!!");
//            Sleep(1984);
//            goto install_begin;
//         }
//      }
//   }
//   exit(0);
//   return 0;
//}
