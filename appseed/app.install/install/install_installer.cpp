#include "framework.h" // from "axis/net/net_sockets.h"
//#include "axis/net/net_sockets.h"
//#include "axis/compress/compress.h"
#ifdef WINDOWS
#include <Shlobj.h>
#endif

#if defined(WINDOWS) || defined(LINUX)
//#include <omp.h>
#else
extern int omp_get_thread_num();
#endif


#ifdef WINDOWS
//#include <WinInet.h>
//#include <Winternl.h>
#endif

#ifdef APPLEOS
//#include <sys/param.h>
//#include <mach-o/dyld.h>
#endif


CHAR szTitle[1024];					// The title bar text
CHAR szWindowClassSpaAdmin[1024];			// the main interaction_impl class name

//void ensure_trace_file();
// bool g_bInstalling = false;

typedef PVOID * PPVOID;






int32_t run_file(const char * pszFile, int32_t nCmdShow);



namespace install
{


   installer::installer(::aura::application * papp) :
      ::object(papp),

      m_mutex(papp),
      m_xmldocStringTable(papp),
      m_mutexOmp(papp)
   {
      
      m_psockethandler = new ::sockets::socket_handler(papp);

      m_daProgress.add(0.0);
      m_iaProgress.add(0);
      m_dAppInstallProgressBase = 0.0;
      m_bProgressModeAppInstall = false;
      m_bMsDownload              = false;
      m_dAnime                   = 0.0;
//      g_bInstalling              = false;
      m_pwindow                  = NULL;
      m_dProgressStart           = 0.0;
      m_dProgressEnd             = 0.0;
      m_bInstallSet              = false;
      m_bOfflineInstall          = false;
      m_bInternetInstall         = true;
      m_dProgress                = -1.0;
      m_dProgress1               = -1.0;
      m_dProgress2               = -1.0;
      m_bShow                    = true;
      m_oswindow                 = NULL;
      m_bForceUpdatedBuild       = false;
      m_bSynch                   = true;
      m_bStarterStart            = false;
      m_strPlatform              = "";
      m_bLaunchDesktopApplicationOnIgnitPhase2 = false;

#if CA2_PLATFORM_VERSION == CA2_BASIS

      m_strVersion = "basis";

#else

      m_strVersion = "stage";

#endif
      m_phttpsession = NULL;

   }


   installer::~installer()
   {

      ::aura::del(m_psockethandler);

   }

   void installer::new_progress_end(double dMilestone)
   {
      m_dProgressStart = m_dProgressEnd;
      m_dProgressEnd = dMilestone;
      set_progress(0.0);
   }


   uint32_t installer::run()
   {

      string_to_string mapMd5;
      string_to_intptr mapLen;


      m_dProgressStart = 0.0;
      m_dProgressEnd = 0.0;



      //SECURITY_ATTRIBUTES MutexAttributes;
      //ZeroMemory( &MutexAttributes, sizeof(MutexAttributes) );
      //MutexAttributes.nLength = sizeof( MutexAttributes );
      //MutexAttributes.bInheritHandle = FALSE; // object uninheritable
      // declare and initialize a security descriptor
      //SECURITY_DESCRIPTOR SD;
      /*      bool bInitOk = InitializeSecurityDescriptor(
      &SD,
      SECURITY_DESCRIPTOR_REVISION );
      if ( bInitOk )
      {
      // give the security descriptor a Null Dacl
      // done using the  "TRUE, (PACL)NULL" here
      bool bSetOk = SetSecurityDescriptorDacl( &SD,
      TRUE,
      (PACL)NULL,
      FALSE );
      if ( bSetOk )
      {
      // Make the security attributes point
      // to the security descriptor
      MutexAttributes.lpSecurityDescriptor = &SD;*/
      //g_hmutexInstall = ::CreateMutex(&MutexAttributes, FALSE, "Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
      /*g_hmutexInstall = ::CreateMutex(NULL, FALSE, "Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
      if(::GetLastError() == ERROR_ALREADY_EXISTS)
      {
      System.install().trace().rich_trace("another instance of spa is already running");
//      exit(-202);
      return -202;
      }*/
      /* }
      else
      {
      System.install().trace().rich_trace("error when trying to analyze if another instance of spa is already running");
//      exit(-200);
      return -200;
      }
      }
      else
      {
      System.install().trace().rich_trace("error when trying to analyze if another instance of spa is already running");
//      exit(-201);
      return -201;
      }*/

      //Sleep(500);

      if(System.install().is_installing_ca2())
         return -1;

      // ::simple_message_box(g_oswindow, "Start", "Start", MB_OK);

      //keep_true keeptrueInstalling(m_bInstalling);

      mutex mutexInstallingCa2(get_app(), false, "Global\\::ca::fontopus::ca2_spaboot_install_" + process_platform_dir_name2() + "::7807e510-5579-11dd-ae16-0800200c7784");

      System.install().trace().rich_trace("#----------");
      System.install().trace().rich_trace(":::::Thank you");
      System.install().trace().rich_trace("***Thank you");
      System.install().trace().rich_trace("Thank you");

      new_progress_end(0.01);

      set_progress(0.001);

      m_bProgressModeAppInstall = true;

      string strVersion(m_strVersion);

      if(strVersion.is_empty())
      {

#if CA2_PLATFORM_VERSION == CA2_BASIS

         strVersion = "basis";

#else

         strVersion = "stage";

#endif

      }

      System.install().m_strVersion = strVersion;

      //m_strSpaIgnitionBaseUrl = "http://" + strVersion + "-server.ca2.cc/api/spaignition";
      m_strSpaIgnitionBaseUrl = "https://ca2.cc/api/spaignition";

      string strSpaHost;

      int32_t iHostRetry = 0;

      stringa straMd5;
      int_array iaLen;

      int32_t iRet = ca2_build_version_etc(strSpaHost,iHostRetry,straMd5, iaLen);

      if (iRet < 0)
         return iRet;

      stringa straMd5AppInstall;

      straMd5.slice(straMd5AppInstall,0, straMd5.get_count() - 1);

      string strIndexMd5 = straMd5.last();

      set_progress(1.0);

      new_progress_end(0.02);

      System.install().app_install_get_extern_executable_path(m_strVersion, m_strBuild, &straMd5AppInstall, &iaLen, this, &mapMd5, &mapLen); // defer install install extern app.install.exe executable

      m_bProgressModeAppInstall = false;

      new_progress_end(0.1);


      //installation_lock_file_lock installationlockfilelock(get_app());

      m_dwInstallStartTime = ::get_tick_count();

install_begin:;

      {
         m_strLastHost = "";
         m_strSpa.remove_all();
         m_iTotalGzLen2 = 0;
         m_iProgressTotalGzLen2 = 0;
         m_NeedRestartBecauseOfReservedFile = false;
         m_NeedRestartFatalError = false;
//         int32_t iFileError = 0;

         int32_t iRet = 0;
         if(iRet < 0)
            return iRet;

//         set_progress(0.1);

         //int32_t iRetry = 0;
         //iRet = application_name();
         //if(iRet < 0)
           // return iRet;

         set_progress(0.2);

      RetryHost:

         if(iHostRetry > 0)
         {

            iRet= calc_host(strSpaHost,iHostRetry);

            if(iRet < 0)
               return iRet;

         }

         m_strCurrentHost = strSpaHost;

         System.install().trace().rich_trace(("got server: " + strSpaHost));

         string strBuild(m_strBuild);

         strBuild.replace(" ","_");

         strBuild.replace(":","-");

         //m_strInstall = "http://" + strSpaHost + "/ccvotagus/" + m_strVersion + "/" + strBuild + "/";
         //m_strInstall = "http://" + strSpaHost + "/ccvotagus/" + m_strVersion + "/";
         m_strInstall = "http://server.ca2.cc/ccvotagus/" + m_strVersion + "/";

         m_strInstallGz = m_strInstall;

         System.install().trace().rich_trace(strSpaHost);

         //dir::mk("C:\\ca");

         //dir::mk("C:\\core\\time");

         //dir::mk("C:\\core\\time\\ca");

         set_progress(0.3);

#ifdef WINDOWSEX
         string path;
         LPSTR lpsz =          path.GetBufferSetLength(1024 * 4);
         ::GetModuleFileNameA(NULL, (char *) (const char *) path, 1024 * 4);
         path.ReleaseBuffer();
#endif
//         int32_t iRetryDeleteSpa = 0;
         string strFile;

         string strUrl;

//#ifdef WINDOWSEX
//         HKEY hkey;
//
//
//         strUrl = m_strSpaIgnitionBaseUrl + "/install_filter_list";

         //property_set set1;

         //set1["disable_ca2_sessid"] = true;

         //set1["raw_http"] = true;

         //string strInstallFilterList = Application.http().get(strUrl, set1);
         //::xml::document nodeInstallFilter(get_app());
         //nodeInstallFilter.load(strInstallFilterList);
         //strUrl = m_strSpaIgnitionBaseUrl + "/query?node=install_application&id=";
         //strUrl += m_strApplicationId;
         //strUrl += "&key=install_filter";


         //property_set set2;

         //set2["disable_ca2_sessid"] = true;

         //set2["raw_http"] = true;

         //string strInstallFilter = Application.http().get(strUrl, set2);
         //for(int32_t ui = 0; ui < nodeInstallFilter.get_root()->get_children_count(); ui++)
         //{
         //   ::xml::node * lpchild = nodeInstallFilter.get_root()->child_at(ui);
         //   string strId;
         //   strId = lpchild->attr("id");
         //   string strFilter;
         //   strFilter = "|" + strId + "|"; // ex. "|multimedia|"
         //   if(strInstallFilter.find(strFilter) >= 0)
         //   {
         //      string strKey;
         //      strKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\ca2_" + strId;

         //      if(::RegCreateKey(HKEY_LOCAL_MACHINE,
         //         strKey,
         //         &hkey) == ERROR_SUCCESS)
         //      {
         //         ::RegSetValueEx(
         //            hkey,
         //            "DisplayIcon",
         //            0,
         //            REG_SZ,
         //            (const BYTE *) (const char *) strFile,
         //            (uint32_t) strFile.length());
         //         string strDisplayName;
         //         strDisplayName = "core - ";
         //         strKey = "install_filter_title_" + strId;
         //         strDisplayName += load_string(strKey, strId);
         //         ::RegSetValueEx(
         //            hkey,
         //            "DisplayName",
         //            0,
         //            REG_SZ,
         //            (const BYTE *) (const char *) strDisplayName,
         //            (uint32_t) strDisplayName.length());
         //         ::RegSetValueEx(
         //            hkey,
         //            "UninstallString",
         //            0,
         //            REG_SZ,
         //            (const BYTE *) (const char *) strInstallFilter,
         //            (uint32_t) strInstallFilter.length());
         //         ::RegSetValueEx(
         //            hkey,
         //            "ModifyString",
         //            0,
         //            REG_SZ,
         //            (const BYTE *) (const char *) strInstallFilter,
         //            (uint32_t) strInstallFilter.length());
         //      }
         //   }
         //}

//#endif // WINDOWS

         set_progress(0.4);

         ::file::patha straFileList;
         string_to_intptr mapGzLen;
         string_to_intptr mapFlag;

         if(iHostRetry > 0)
         {

            strUrl = "http://" + strSpaHost + "/ccvotagus/" + m_strVersion + "/app/stage/metastage/index-" + strBuild + ".md5";

            strIndexMd5 = http_get(strUrl,false);

         }

         ::xml::document nodeInstall(get_app());

         nodeInstall.load(file_as_string_dup(dir::appdata(process_platform_dir_name2()) / "install.xml"));

         ::xml::node * lpnodeVersion = nodeInstall.get_child(m_strVersion);

         System.install().trace().rich_trace("***Downloading file list.");

         string strIndexPath;

         bool bOkIndexPath = false;

         if(!ca2_fy_url(strIndexPath, ("app/stage/metastage/index-"+strBuild+".spa.bz"), true, -1, strIndexMd5, -1, true, &bOkIndexPath))
         {
            System.install().trace().rich_trace("Failed to download file list!");
            System.install().trace().rich_trace("Going to retry host...");
            Sleep(184);
            goto RetryHost;
         }

         if(iHostRetry > 0 || !bOkIndexPath)
         {

            if(strIndexMd5.length() != 32 || stricmp_dup(System.file().md5(strIndexPath),strIndexMd5) != 0)
            {
               System.install().trace().rich_trace("Invalid file list!");
               System.install().trace().rich_trace("Going to retry host...");
               Sleep(184);
               goto RetryHost;
            }

         }

         set_progress(0.5);

         ParseIndexFile(strIndexPath, mapLen, mapMd5, mapGzLen, mapFlag);

         set_progress(0.6);

         if(iHostRetry > 0)
         {

            appmatter_list();

         }

         set_progress(0.7);

         if(!GetFileList(straFileList, ("app/stage/metastage/" + m_strApplicationId + ".spa"), mapLen, mapGzLen, mapMd5, mapFlag))
         {
            System.install().trace().rich_trace("Failed to download file list!");
            System.install().trace().rich_trace("Going to retry host...");
            Sleep(500);
            goto RetryHost;
         }

         set_progress(0.8);

         for(int32_t i = 0; i < straFileList.get_count(); i++)
         {
            const char * psz = straFileList[i];
            ::OutputDebugStringA(psz);
            ::OutputDebugStringA("\r\n");
         }

         if(lpnodeVersion != NULL)
         {

            for(int32_t ui = 0; ui < lpnodeVersion->get_children_count(); ui++)
            {

               ::xml::node * lpnodeInstalled = lpnodeVersion->child_at(ui);

               if(!strcmp(lpnodeVersion->get_name(), "installed"))
               {

                  for(int32_t ui = 0; ui < lpnodeInstalled->get_children_count(); ui++)
                  {

                     ::xml::node * lpnodeType = lpnodeInstalled->child_at(ui);

                     for(int32_t ui = 0; ui < lpnodeType->get_children_count(); ui++)
                     {

                        string strId = lpnodeType->child_at(ui)->attr("id");

                        if(strcmp_dup(strId, m_strApplicationId) != 0)
                        {

                           GetFileList(straFileList, ("app/stage/metastage/" + strId + ".spa"), mapLen, mapGzLen, mapMd5, mapFlag);

                        }

                     }

                  }

               }

            }

         }


         {

            ::file::patha patha;

            patha.add_tokens(m_strAppMatterList, "\n");

            System.install().trace().rich_trace("***appmatter");

            for(int32_t i = 0; i < patha.get_count(); i++)
            {
               System.install().trace().rich_trace(patha[i]);
            }

            GetFileListEx(straFileList, patha, mapLen, mapGzLen, mapMd5, mapFlag);

         }

         /*string strType;
         string strStart;
         ::xml::node nodeSpaStart;
         nodeSpaStart.Load(file_as_string_dup(dir::appdata("spa_start.xml")));

         for(int32_t ui = 0; ui < nodeSpaStart.get_children_count(); ui++)
         {

            strType     = nodeSpaStart.child_at(ui)->attr("type");

            strStart    = nodeSpaStart.child_at(ui)->attr("id");

            bool bGet   = true;

            if(lpnodeInstalled != NULL)
            {

               for(int32_t ui = 0; ui < lpnodeInstalled->get_children_count(); ui++)
               {

                  ::xml::node * lpnodeType = lpnodeInstalled->child_at(ui);

                  if(lpnodeType->get_name() == strType)
                  {

                     for(int32_t ui = 0; ui < lpnodeType->get_children_count(); ui++)
                     {

                        string strId = lpnodeType->child_at(ui)->attr("id");

                        if(strId == strStart)
                        {

                           bGet = false;

                        }

                     }

                  }

               }

            }

            if(bGet && m_strApplicationId == strStart)
            {
               bGet = false;
            }
            if(bGet)
            {
               string strGet;
               strGet = "app/stage/metastage/";
               strGet += strStart;
               strGet += ".spa";
               GetFileList(straFileList, strGet, mapLen, mapGzLen, mapMd5, mapFlag);
            }
         }*/

         for(int32_t i = 0; i < straFileList.get_count(); i++)
         {
            ::OutputDebugStringA(straFileList[i]);
            ::OutputDebugStringA("\r\n");
         }

         set_progress(0.9);

         m_straTerminateProcesses.remove_all();

         uint_array dwa;
#ifndef METROWIN
         dll_processes(dwa, m_straTerminateProcesses, dir::stage(process_platform_dir_name2()) / "axis.dll");
         dll_processes(dwa,m_straTerminateProcesses,dir::stage(process_platform_dir_name2()) / "core.dll");
         dll_processes(dwa,m_straTerminateProcesses,dir::stage(process_platform_dir_name2()) / "os.dll");
         //dll_processes(dwa, m_straTerminateProcesses, dir::element("stage\\x86\\npca2.dll"));
         //dll_processes(dwa, m_straTerminateProcesses, dir::element("stage\\x86\\iexca2.dll"));
         // TODO: simular virtualmente a cópia dos arquivos também, se tiver aquivo travado, também retornar
#endif


#ifdef WINDOWSEX

         m_straRestartCommandLine.remove_all();
         m_straRestartProcess.remove_all();

         if(m_straTerminateProcesses.get_count() > 0)
         {
            if(!m_bStarterStart)
               return 0;
            string strCommand;
            bool bAsk;
            bool bRestart;
            for(int32_t i = 0; i < m_straTerminateProcesses.get_count(); i++)
            {
               if(i >= 1)
               {
                  strCommand += ";";
               }
               bAsk = true;
               bRestart = true;
               string strPath = m_straTerminateProcesses.element_at(i);
               if(str_ends_ci_dup(strPath, "\\app.install.exe") && stristr_dup(strPath, "\\core\\") != NULL)
               {
                  bAsk = false;
                  bRestart = false;
               }
               else if(str_ends_ci_dup(strPath, ".exe")  && stristr_dup(strPath, "\\app.install.exe") != NULL)
               {
                  bAsk = false;
                  bRestart = false;
               }
               else if(file_is_equal_path(strPath, file_get_mozilla_firefox_plugin_container_path()))
               {
                  bAsk = false;
                  bRestart = false;
               }

               bAsk = false;
               bRestart = false;

               string str;
               str = "Should ca Terminate and try to restart process \"";
               str += strPath;
               str += "\"?";
               strCommand += str;
               if(!bAsk || ::simple_message_box(NULL,str,"need to terminate process to install",MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
               {
                  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE | PROCESS_VM_READ, FALSE, dwa[i]);
                  if(hProcess == NULL)
                  {
                     ::CloseHandle(hProcess);
                     str = "Failed to open process for termination - ";
                     str += m_straTerminateProcesses.element_at(i);
                     str += ".";
                     ::simple_message_box(NULL,"Failed to open process for termination",str,MB_OK);

                     return -1;
                  }

                  if(bRestart)
                  {
                     m_straRestartCommandLine.add(get_command_line(hProcess));
                     m_straRestartProcess.add(m_straTerminateProcesses.element_at(i));
                  }


                  //Sleep(15 * 1000);
                  //Sleep(15 * 1000);
                  //Sleep(15 * 1000);

                  TerminateProcess(hProcess, -1);

                  int32_t iRetry = 49;
                  DWORD dwExitCode = (uint32_t) -1;
                  while(::GetExitCodeProcess(hProcess, &dwExitCode) && dwExitCode == STILL_ACTIVE && iRetry > 0)
                  {
                     Sleep(184);
                     iRetry--;
                  }
                  if(iRetry <= 0)
                  {
                     ::CloseHandle(hProcess);
                     str = "Timeout while waiting for process - ";
                     str += m_straTerminateProcesses.element_at(i);
                     str += " - to exit.";
                     ::simple_message_box(NULL,"Failed to open process for termination",str,MB_OK);
                     return -1;
                  }
                  ::CloseHandle(hProcess);
               }
               else
               {
                  return -1;
               }

            }
            //file_put_contents_dup("C:\\core\\machine\\on_after_spaadmin.txt", strCommand);
         }

         if(m_straRestartCommandLine.get_count() > 0)
         {

            oswindow oswindowSpaBoot = ::FindWindow(NULL, "::core::spaboot:callback_window");

            if(oswindowSpaBoot != NULL)
            {
               string str = m_straRestartCommandLine.encode_v16();
               COPYDATASTRUCT cds;
               memset_dup(&cds, 0, sizeof(cds));
               cds.dwData = 15111984;
               cds.cbData = (uint32_t) str.length();
               cds.lpData = (PVOID) (const char *) str;
               ::SendMessageA(oswindowSpaBoot, WM_COPYDATA, (WPARAM) 0, (LPARAM) &cds);
            }

         }


#else

         throw 0;

#endif

         new_progress_end(0.49);
         if(m_bInternetInstall)
         {
            if (file_exists_dup(::dir::system() / "config\\plugin\\do_not_download_file_list.txt"))
            {
               Sleep(15000);
            }
            else
            {
               download_file_list(straFileList, mapLen, mapMd5, mapGzLen, mapFlag);
            }

         }
         else
         {
            UncompressFileList(straFileList, mapMd5);
         }




         System.install().set_ca2_updated(m_strBuild);

         //new_progress_end(0.8);
         //CopyFileList(straFileList, mapFlag);

         new_progress_end(1.0);
         int iRetry = 0;
         while((!machine_unsignalize_close_application()
            || machine_check_close_application(true))&& iRetry < 840)
         {
            if(iRetry == 0)
            {
               System.install().trace().rich_trace("Enabling applications to run.");
            }
            iRetry++;
            Sleep(184);
            if((iRetry % 33) == 0)
            {
               System.install().trace().rich_trace(".");
            }
            else
            {
               System.install().trace().trace_add(".");
            }
         }
         System.install().trace().rich_trace("***Installing application.");

         if(m_NeedRestartBecauseOfReservedFile
            || m_NeedRestartFatalError)
         {
            add_spa_start(m_strCommandLine);

            if(::simple_message_box(NULL,"The computer need to be restarted!!\n\nDo you want to restart now?\n\nWe recommend you to close all other applications first and then agree with this question using the buttons below.","spa - Restart Needed!!",MB_ICONEXCLAMATION | MB_YESNO)
               == IDYES)
            {
               m_reboot();
            }
         }
         else
         {
            if(!ignit_phase2())
            {
               System.install().trace().rich_trace("***Going to ebing!!");
               Sleep(2000);
               goto install_begin;
            }
         }
      }
      return 0;
   }


   uint32_t installer::thread_proc_run(void * lpParam)
   {

      installer * pinstaller = (installer *) lpParam;

      uint32_t ui = pinstaller->run();

      //delete pinstaller;

      return ui;

   }

   void installer::set_progress(double dProgress)
   {
      if(dProgress < 0.0)
         dProgress = 0.0;
      else if(dProgress > 1.0)
         dProgress = 1.0;

      dProgress = m_dProgressStart + (m_dProgressEnd - m_dProgressStart) * dProgress;
      System.install().trace().trace_progress(dProgress);
   }



   ::count installer::download_file_list(::file::patha & stringa, string_to_intptr & mapLen, string_to_string & mapMd5, string_to_intptr & mapGzLen, string_to_intptr & mapFlag)
   {

      ::stringa straExpandFileSet;

      m_bShowPercentage = true;

      m_iGzLen2 = 0;

      m_dProgress = 0.0;

      m_iProgressTotalGzLen2 = 0;

      System.install().trace().rich_trace("***Downloading resource files.");

      {

         stringa.filter_out(
            [&](const string & strCurrent)
            {
               if(::str::ends_ci(strCurrent,".expand_fileset"))
               {
                  int iGzLen = mapGzLen[strCurrent];
                  m_iProgressTotalGzLen2 += iGzLen;
                  m_iTotalGzLen2 -= iGzLen;
                  return true;
               }
               else
               {
                  return false;
               }
            }, straExpandFileSet);

         //index iExpandFileSetStart = stringa.get_size();

         m_iGzLen2 = 0;

         for(auto strCurrent : straExpandFileSet)
         {

            ::file::path str = m_strInstall;

            str /= strCurrent;

            string str2 = dir::name(str);

            if(str2.substr(0,m_strInstall.length()) == m_strInstall)
            {

               str2 = str2.substr(21);

               str2.replace("\\","/");

            }

            str += ".bz";

            string str3  = str;

            string strMd5         = mapMd5[strCurrent];

            int iLen           = mapLen[strCurrent];

            int iGzLen         = mapGzLen[strCurrent];

            str += ".";

            str += strMd5;

            ::file::path strStageGz = ca2bz_get_dir(strCurrent) / ca2bz_get_file(strCurrent,strMd5);

            ::file::path strStageInplaceFile = ca2inplace_get_file(strCurrent);

            bool bDownload = true;

            System.install().trace().rich_trace(str::replace("\\","/",file_name_dup((str2 + str))));

            ::file::path strStageInplace = ca2bz_get_dir(strCurrent) / strStageInplaceFile;

            if(file_exists_dup(strStageInplace)
               && (iLen != -1) && file_length_dup(strStageInplace) == iLen
               && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace),strMd5) == 0)
            {

               bDownload = false;

            }

            if(bDownload && file_exists_dup(strStageGz))
            {

               dir::mk(dir::name(strStageInplace));

               bzuncompress(strStageInplace,strStageGz);

               if(file_exists_dup(strStageInplace)
                  && (iLen != -1) && file_length_dup(strStageInplace) == iLen
                  && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace),strMd5) == 0)
                  bDownload = false;

            }

            if(bDownload && download_file(strStageInplace,str3,false,false,iLen,strMd5,iGzLen,mapFlag[strCurrent]))
            {

               m_dProgress = m_dProgress2;

               string strRelative = dir::name(strCurrent) / file_title_dup(strCurrent);

               string strStageInplace2 = ca2inplace_get_dir(strRelative) / ca2inplace_get_file(strRelative);

               Application.file().ftd(strStageInplace2,strStageInplace);

            }

            string strExpand = strCurrent;

            strExpand += ".spa";

            strExpand = "app\\stage\\metastage\\" + strExpand;


            int64_t iPreviousTotalProgress = m_iProgressTotalGzLen2; // keep progress rate total calculator

            m_iProgressTotalGzLen2 = 0; // avoid progress rate change

            GetFileList(stringa,strExpand,mapLen,mapGzLen,mapMd5,mapFlag);

            m_iProgressTotalGzLen2 = iPreviousTotalProgress; // restore progress rate total calculator

            m_iGzLen2 += iGzLen;

            set_progress((double)m_iGzLen2 / (double)m_iProgressTotalGzLen2);

         }

         new_progress_end(0.84);

         System.install().trace().rich_trace("***Downloading files.");

         m_iGzLen2 = 0;

         m_iProgressTotalGzLen2 = m_iTotalGzLen2;

         for(int i = 0; i < stringa.get_count(); i++)
         {

            single_lock sl(&m_mutexOmp);

            string strFileName;

            string strCurrent  = stringa[i];

            strCurrent.trim();

            if(strCurrent.is_empty())
               continue;

            int iGzLen         = mapGzLen[strCurrent];

            ::file::path str = m_strInstall;

            str /= strCurrent;

            string str2 = dir::name(str);

            strFileName = file_name_dup(strCurrent);

            if(str2.substr(0,m_strInstall.length()) == m_strInstall)
            {

               str2 = str2.substr(21);

               str2.replace("\\","/");

            }

            str += ".bz";

            string str3  = str;

            string strMd5         = mapMd5[strCurrent];

            int iLen           = mapLen[strCurrent];

            str += ".";

            str += strMd5;

            ::file::path strStageGz = ca2bz_get_dir(strCurrent) / ca2bz_get_file(strCurrent,strMd5);

            ::file::path strStageInplaceFile = ca2inplace_get_file(strCurrent);

            bool bDownload = true;

            ::file::path strStageInplace = ca2inplace_get_dir(strCurrent) / ca2inplace_get_file(strCurrent);

            if(file_exists_dup(strStageInplace)
               && (iLen != -1) && file_length_dup(strStageInplace) == iLen
               && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace),strMd5) == 0)
            {

               bDownload = false;

            }

            if(bDownload && (::str::ends_ci(strFileName,".exe") || ::str::ends_ci(strFileName,".dll")))
            {

               string strCandidate = ::dir::app_install(System.install().get_platform()) / strFileName;

               if(file_exists_dup(strCandidate)
                  && iLen != -1 && file_length_dup(strCandidate) == iLen
                  && strMd5.has_char() && stricmp_dup(System.file().md5(strCandidate),strMd5) == 0)
               {

                  bDownload  = !::file_copy_dup(strStageInplace,strCandidate, false);

               }

            }

            if(bDownload && file_exists_dup(strStageGz))
            {

               dir::mk(dir::name(strStageInplace));

               bzuncompress(strStageInplace,strStageGz);

               if(file_exists_dup(strStageInplace)
                  && (iLen != -1) && file_length_dup(strStageInplace) == iLen
                  && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace),strMd5) == 0)
                  bDownload = false;

            }

            if(bDownload)
            {

               if(download_file(strStageInplace,str3,false,false,iLen,strMd5,iGzLen,mapFlag[stringa[i]]))
               {

                  m_dProgress = m_dProgress2;

               }

            }

            sl.lock();

            if(bDownload)
            {

               System.install().trace().rich_trace(str::replace(file_name_dup((str2 + str)),"\\","/"));

            }

            m_iaProgress.element_at_grow(0) = 0;

            m_iGzLen2 += iGzLen;

            set_progress((double)m_iGzLen2 / (double)m_iProgressTotalGzLen2);

            sl.unlock();

         }

         if(m_pwindow != NULL)
         {


         }

      }

      return stringa.get_count();

   }




   bool installer::get(const string& url_in, bool bExist, int64_t iLength, const char * pszMd5, int64_t iGzLen)
   {
      if(m_bOfflineInstall)
         return true;
      string url = url_in;
      string file;
      string file2;
      string dir;
      string dir2;
      dir = dir::element();
      if(dir.substr(dir.size() - 1, 1) != "\\")
      {
         dir += "\\";
      }
      dir2 = dir;
      dir += "time\\bz\\";
//      int32_t oldpos = -1;
      {
         index pos = url_in.find(m_strInstall);
         if(pos == 0)
         {
            url = url_in.substr(m_strInstall.length());
         }
         index oldpos = -1;
         pos = url.find("/");
         string lastfile;
         while (pos >=0)
         {
            file = url.substr(oldpos + 1, pos - oldpos -1);
            if(lastfile.size() > 0)
               dir +=  lastfile + "\\";
            lastfile = file;
            oldpos = pos;
            pos = url.find("/", oldpos + 1);
         }
         file = url.substr(oldpos + 1);
         if(lastfile.size() > 0)
            dir +=  lastfile + "\\";
      }
      if(stricmp_dup(file.substr(file.length() - 3, 3), ".bz") == 0)
      {
         file2 = file.substr(0, file.length() - 3);
      }
      else
      {
         file2 = file;
      }
      dir::mk(dir);

      if(bExist)
      {
         if(file_exists_dup(dir2 + file2))
         {
            if(iLength == -1 || iLength == file_length_dup((dir2 + file2)))
            {
               if(pszMd5 != NULL && strlen_dup(pszMd5) > 0 && stricmp_dup(System.file().md5((dir2 + file2)), pszMd5) == 0)
               {
                  return true;
               }
            }
         }
      }

      dir::mk(dir::name((dir + file)));

      m_dwCurFileLen = iGzLen;

      m_dwDownloadLen = 0;

      keep_true keepDownloadTrue(m_bMsDownload);

      string strUrl = url_in;

      property_set set;

      set["disable_ca2_sessid"] = true;

      set["raw_http"] = true;

      single_lock sl(&m_mutexOmp);

      sl.lock();
      sp(::sockets::http_session) & psession = m_httpsessionptra.element_at_grow(0);
      sl.unlock();

      System.install().trace().trace_add("\ndownloading " + strUrl + "\n");

      return Application.http().download(*m_psockethandler, psession,strUrl,(dir + file),set);

   }


   bool installer::download_file(const string& inplaceParam, const string& url_in, bool bExist, bool bCheck, int64_t iLength, const char * pszMd5, int64_t iGzLen, int_ptr & iFlag)
   {

      single_lock sl(&m_mutexOmp);

      if(m_bOfflineInstall)
         return true;

      string url = url_in;

      string file;

      string file2;

      string dir;

      string dir2;

      string dir3;

      dir = dir::element();

      if(dir.substr(dir.size() - 1, 1) != "\\")
      {

         dir += "\\";

      }

      dir3 = dir + "time\\patchwork\\";

      dir2 = dir;

      dir += "time\\bz\\";

//      int32_t oldpos = -1;

      {

         index pos = url_in.find(m_strInstall);

         if(pos == 0)
         {

            url = url_in.substr(m_strInstall.length());

         }

         index oldpos = -1;

         pos = url.find("/");

         string lastfile;

         while (pos >=0)
         {

            file = url.substr(oldpos + 1, pos - oldpos -1);

            if(lastfile.size() > 0)
               dir +=  lastfile + "\\";

            lastfile = file;

            oldpos = pos;

            pos = url.find("/", oldpos + 1);

         }

         file = url.substr(oldpos + 1);

         if(lastfile.size() > 0)
            dir +=  lastfile + "\\";

      }

      if(stricmp_dup(file.substr(file.length() - 3, 3), ".bz") == 0)
      {

         file2 = file.substr(0, file.length() - 3);

      }
      else
      {

         file2 = file;

      }

      dir::mk(dir);

      string inplace;

      if(inplaceParam.is_empty())
      {

         inplace = dir2 + file2;

      }
      else
      {
         inplace = inplaceParam;
      }

      // first check if the exiting (old file) is already up-to-date (the current one)
      if(bExist && bCheck)
      {
         if(file_exists_dup(inplace))
         {
            if(iLength != -1 && iLength == file_length_dup(inplace))
            {
               if(pszMd5 != NULL && strlen_dup(pszMd5) > 0 && stricmp_dup(System.file().md5(inplace), pszMd5) == 0)
               {

                  System.install().trace().trace_add(unitext(" up-to-date c"));

                  iFlag |= SPA_FILE_FLAG_VOID_COPY;

                  return true;

               }
            }
         }
      }

      bool bOk = false;

      int32_t iRetry = 0;

      int32_t iMaxRetry = 3;


      
      
//      char sz[1024];

//      int32_t iStatus;

//      if(file_exists_dup(inplace))
//      {

         // if file already exists, tries to patch it (bspatch)
//
//         // then first try to download and apply patch
//
//         string strOldMd5 = System.file().md5(inplace);
//
//         string strNewMd5 = pszMd5;
//
//         dir::mk(dir::name((dir + file)));
//
//         m_dwCurFileLen = iGzLen;
//
//         m_dwDownloadLen = 0;
//
//         keep_true keepDownloadTrue(m_bMsDownload);
//
//         uint64_t iBsLen = 0;
//
//         bool bPossible = false;
//
//         while(true)
//         {
//
//            string strUrl;
//
//            strUrl = "http://spa.api.server.ca2.cc/bspatch?file=";
//
//            strUrl += url_encode_dup(file2);
//
//            strUrl += "&old_hash=";
//
//            strUrl += strOldMd5;
//
//            strUrl += "&new_hash=";
//
//            strUrl += strNewMd5;
//
//            string strBsPatch = dir + file + "." + strOldMd5 + "." + strNewMd5 + ".bspatch";
//
//            property_set set;
//
//            set["int_scalar_source_listener"] = this;
//
//            set["disable_ca2_sessid"] = true;
//
//            set["raw_http"] = true;
//
//            sl.lock();
//            sp(::sockets::http_session) & psession = m_httpsessionptra.element_at_grow(0);
//            sl.unlock();
//
//            System.install().trace().trace_add("\ndownloading " + strUrl + "\n");
//
//            bOk = Application.http().download(*m_psockethandler, psession,strUrl,strBsPatch,set);
//
////            if(iStatus == 404)
//  //             break;
//
//            if (!bOk)
//               break;
//
//            if(!bPossible)
//            {
//
//               bPossible = true;
//
//               ///System.install().trace().trace_add(" patching may be possible");
//
//            }
//            if(bOk)
//            {
//               int32_t iResult = -1;
//               if(iLength != -1)
//               {
//                  dir::mk(dir::name(inplace));
////                  iResult = bspatch(inplace, (dir3 + file2), strBsPatch);
//                  if(iResult != 0)
//                  {
//                     //   System.install().trace().rich_trace("");
//                     // System.install().trace().rich_trace("Patch failed : files may be corrupted):");
//                     /*                  System.install().trace().rich_trace("old   file : ");
//                     System.install().trace().trace_add((dir2 + file2));
//                     System.install().trace().rich_trace("new   file :");
//                     System.install().trace().trace_add((dir3 + file2));
//                     System.install().trace().rich_trace("patch file :");
//                     System.install().trace().trace_add(strBsPatch);*/
//                     bOk = false;
//                     break;
//                  }
//               }
//            }
//            if(bOk)
//            {
//               if(iLength == -1)
//               {
//                  //  System.install().trace().rich_trace("Patch Length Check : There is no length information to check.");
//               }
//               else
//               {
//                  uint64_t iActualLength = file_length_dup((dir3 + file2));
//                  bOk = iLength == iActualLength;
//                  //System.install().trace().rich_trace("Patch Length Verification : ");
//                  /*sprintf(sz, "correct length : %d bytes", iLength);
//                  System.install().trace().rich_trace(sz);
//                  sprintf(sz, "actual  length : %d bytes", iActualLength);
//                  System.install().trace().rich_trace(sz);*/
//                  if(bOk)
//                  {
//                     //   System.install().trace().trace_add("Length match!");
//                  }
//                  else
//                  {
//                     // System.install().trace().trace_add(unitext("Length mismatch ¡X!"));
//                  }
//               }
//            }
//            if(bOk)
//            {
//               if(pszMd5 == NULL || strlen_dup(pszMd5) == 0)
//               {
//                  //System.install().trace().rich_trace("Patch MD5 Hash Verification : there is no MD5 Hash information to verify.");
//               }
//               else
//               {
//                  string strMd5;
//                  strMd5 = System.file().md5((dir3 + file2));
//                  bOk = stricmp_dup(strMd5, pszMd5) == 0;
//                  //System.install().trace().rich_trace("Patch MD5 Hash Verification");
//                  /*sprintf(sz, "correct MD5 Hash : %s", pszMd5);
//                  System.install().trace().rich_trace(sz);
//                  sprintf(sz, "actual  MD5 Hash : %str", strMd5);
//                  System.install().trace().rich_trace(sz);*/
//                  if(bOk)
//                  {
//                     // System.install().trace().trace_add("MD5 Hash match!");
//                  }
//                  else
//                  {
//                     //System.install().trace().trace_add(unitext("MD5 Hash mismatch ¡X!"));
//                  }
//               }
//            }
//            if(bOk)
//            {
//               if(!file_copy_dup(inplace, dir3 + file2, true))
//               {
//                  System.install().trace().rich_trace("Failed to copy patched file: Need Restart Because Of Reserved File");
//                  m_NeedRestartBecauseOfReservedFile = true;
//               }
//               iBsLen = file_length_dup(strBsPatch);
//               break;
//            }
//            iRetry++;
//            //sprintf(sz, "An error ocurred. Retrying... Attempt %d of %d.", iRetry, iMaxRetry);
//            //System.install().trace().rich_trace(sz);
//            //System.install().trace().rich_trace("Patch may fail due timeout if the patched file is large and it is the first");
//            //System.install().trace().rich_trace("time the patch is requested for the older-newer-pair patch combination.");
//            //System.install().trace().rich_trace("Other users later requesting the same older-newer-pair patch combination will");
//            //System.install().trace().rich_trace("benefit from the cached patch.");
//            Sleep((5000) * 5);
//            if(iRetry >= iMaxRetry)
//               break;
//         }
//         if(bOk)
//         {
//            if(iRetry > 0)
//            {
//               // sprintf(sz, unitext("Patch succeeded with %d retries %0.2fkbytes compressed ✓"), (iBsLen / 1000.0), iRetry);
//               //System.install().trace().trace_add(unitext(" c"));
//            }
//            else
//            {
//               // this System.install().rich_trace is added and signalized like that because it
//               // should be one of the most common cases and one of the most
//               // normal cases.
//               //sprintf(sz, unitext(" patched %0.2fkbytes compressed ✓"), (iBsLen / 1000.0));
//               //System.install().trace().trace_add(sz);
//               //System.install().trace().trace_add(unitext(" c"));
//            }
//         }
//         else
//         {
//            //System.install().trace().rich_trace(unitext("Patch failed ¡X!"));
//         }
//      }






      // then finally try to download the entire file
      sl.lock();
      sp(::sockets::http_session) & psession = m_httpsessionptra.element_at_grow(0);
      sl.unlock();
      if(!bOk)
      {
         dir::mk(dir::name((dir + file)));
         m_dwCurFileLen = iGzLen;
         m_dwDownloadLen = 0;
         keep_true keepDownloadTrue(m_bMsDownload);
         if(iRetry > 0)
         {
            //System.install().trace().rich_trace(("Going to download " + file_name_dup(file2) + " entirely"));
         }
         System.install().trace().rich_trace(file2);
         iRetry = 0;
         while(true)
         {

            property_set set;

            set["int_scalar_source_listener"] = this;

            set["disable_ca2_sessid"] = true;

            set["raw_http"] = true;

            System.install().trace().trace_add("\ndownloading " + (::file::path(url_in + "." + pszMd5)) + "\n");

            bOk = Application.http().download(*m_psockethandler, psession,::file::path(url_in + "." + pszMd5),::file::path(dir + file + "." + pszMd5),set);
            //bOk = Application.http().download((url_in + "." + pszMd5), (dir + file + "." + pszMd5), set);
//            if(iStatus == 404)
  //             break;
            if (!bOk)
               break;
            if(bOk)
            {
               if(iLength != -1)
               {
                  dir::mk(dir::name(inplace));
                  int32_t iResult = bzuncompress(inplace, (dir + file + "." + pszMd5));
                  if(iResult == -1)
                  {
                     m_NeedRestartBecauseOfReservedFile = true;
                  }
                  else if(iResult == -2)
                  {
                     m_NeedRestartFatalError = true;
                  }
               }
               bOk = iLength == -1 || iLength == file_length_dup(inplace);
               if(bOk)
               {
                  bOk = pszMd5 == NULL || strlen_dup(pszMd5) == 0 || stricmp_dup(System.file().md5(inplace), pszMd5) == 0;
                  if(bOk)
                  {
                     break;
                  }
               }
            }
            iRetry++;
            //sprintf(sz, "An error ocurred. Retrying... Attempt %d of %d.", iRetry, iMaxRetry);
            //System.install().trace().rich_trace(sz);
            if(iRetry >= iMaxRetry)
               break;
         }
         if(bOk)
         {
            //sprintf(sz, unitext("%0.2fkbytes compressed c"), (iGzLen / 1000.0));
            //System.install().trace().rich_trace(sz);
         }
         else
         {
            //System.install().trace().rich_trace((unitext("Failed to download file ") + file2 + unitext("¡X!")));
         }
      }
      if(!bOk)
      {
         //sprintf(sz, unitext("General failure to retrieve file \"%s\" ¡X!"), file2);
         //System.install().trace().rich_trace(sz);
      }
      return bOk;
   }


   ::file::path installer::ca2_get_dir(LPCSTR lpcszUrl)
   {
      string url_in(lpcszUrl);
      string dir;
      string url;
      string file;
      dir = dir::element();
      if(dir.substr(dir.size() - 1, 1) != "\\")
      {
         dir += "\\";
      }
      dir;
      string strFind;
      index pos = url_in.find(m_strInstall);
      if(pos == 0)
      {
         url = url_in.substr(m_strInstall.length());
      }
      else
      {
         url = url_in;
      }
      index oldpos = -1;
      pos = url.find("/");
      string lastfile;
      while (pos >=0)
      {
         file = url.substr(oldpos + 1, pos - oldpos -1);
         if(lastfile.size() > 0)
            dir +=  lastfile + "\\";
         lastfile = file;
         oldpos = pos;
         pos = url.find("/", oldpos + 1);
      }
      file = url.substr(oldpos + 1);
      if(lastfile.size() > 0)
         dir +=  lastfile + "\\";

      return dir;

   }


   ::file::path installer::ca2bz_get_dir(LPCSTR lpcszUrl)
   {

      ::file::path dir;

      if(m_bInternetInstall)
      {

         dir = dir::afterca2() / "time/bz";

      }
      else
      {

         dir = m_strInstallGz;

      }

      dir /= lpcszUrl;

      return dir.folder();

   }


   ::file::path installer::ca2unbz_get_dir(LPCSTR lpcszUrl)
   {
      string url_in(lpcszUrl);
      string dir;
      string url;
      string file;
      if(m_bInternetInstall)
      {
         dir = dir::afterca2() / "time/unbz";
      }
      else
      {
         dir = m_strInstallGz;
      }
      index pos = url_in.find(m_strInstall);
      if(pos == 0)
      {
         url = url_in.substr(m_strInstall.length());
      }
      else
      {
         url = url_in;
      }
      index oldpos = -1;
      pos = url.find("/");
      string lastfile;
      while (pos >=0)
      {
         file = url.substr(oldpos + 1, pos - oldpos -1);
         if(lastfile.size() > 0)
            dir +=  lastfile + "\\";
         lastfile = file;
         oldpos = pos;
         pos = url.find("/", oldpos + 1);
      }
      file = url.substr(oldpos + 1);
      if(lastfile.size() > 0)
         dir +=  lastfile + "\\";
      return dir;
   }


   ::file::path installer::ca2inplace_get_dir(LPCSTR lpcszUrl)
   {

      string url_in(lpcszUrl);
      string dir;
      string url;
      string file;
      if(m_bInternetInstall)
      {
         dir = dir::afterca2();
         if(!str_ends_ci_dup(dir, "\\") && !str_ends_ci_dup(dir, "/"))
         {
            dir += "\\";
         }
      }
      else
      {
         dir = m_strInstallGz;
      }
      index pos = url_in.find(m_strInstall);
      if(pos == 0)
      {
         url = url_in.substr(m_strInstall.length());
      }
      else
      {
         url = url_in;
      }
      index oldpos = -1;
      pos = url.find("/");
      string lastfile;
      while (pos >=0)
      {
         file = url.substr(oldpos + 1, pos - oldpos -1);
         if(lastfile.size() > 0)
            dir +=  lastfile + "\\";
         lastfile = file;
         oldpos = pos;
         pos = url.find("/", oldpos + 1);
      }
      file = url.substr(oldpos + 1);
      if(lastfile.size() > 0)
         dir +=  lastfile + "\\";
      return dir;
   }


   ::file::path installer::ca2_get_file(LPCSTR lpcszUrl)
   {
      string url_in(lpcszUrl);
      string dir;
      string url;
      string file;
      dir = dir::element();
      if(dir.substr(dir.size() - 1, 1) != "\\")
      {
         dir += "\\";
      }
      string strFind;
      index pos = url_in.find(m_strInstall);
      if(pos == 0)
      {
         url = url_in.substr(m_strInstall.length());
      }
      else
      {
         url = url_in;
      }
      index oldpos = -1;
      pos = url.find("/");
      string lastfile;
      while (pos >=0)
      {
         file = url.substr(oldpos + 1, pos - oldpos -1);
         if(lastfile.size() > 0)
            dir +=  lastfile + "\\";
         lastfile = file;
         oldpos = pos;
         pos = url.find("/", oldpos + 1);
      }
      file = url.substr(oldpos + 1);
      if(lastfile.size() > 0)
         dir +=  lastfile + "\\";
      if(file.substr(file.size() - 3, 3) == ".bz")
         return file.substr(0, file.size() - 3);
      else
         return file;
   }


   ::file::path installer::ca2bz_get_file(LPCSTR lpcszUrl, const char * pszMd5)
   {

      ::file::path file = ::file::path(lpcszUrl).name();

      if(pszMd5 != NULL)
      {

         if(file.substr(file.size() - 3,3) == ".bz")
         {

            return file + "." + pszMd5;

         }
         else
         {

            return file + ".bz." + pszMd5;

         }

      }
      else
      {
         if(file.substr(file.size() - 3,3) == ".bz")
         {

            return file;

         }
         else
         {

            return file + ".bz";

         }

      }

   }


   ::file::path installer::ca2unbz_get_file(LPCSTR lpcszUrl)
   {
      string url_in(lpcszUrl);
      string dir;
      string url;
      string file;
      dir = dir::afterca2() /  "time/unbz";
      string strFind;
      index pos = url_in.find(m_strInstall);
      if(pos == 0)
      {
         url = url_in.substr(m_strInstall.length());
      }
      else
      {
         url = url_in;
      }
      index oldpos = -1;
      pos = url.find("/");
      string lastfile;
      while (pos >=0)
      {
         file = url.substr(oldpos + 1, pos - oldpos -1);
         if(lastfile.size() > 0)
            dir +=  lastfile + "\\";
         lastfile = file;
         oldpos = pos;
         pos = url.find("/", oldpos + 1);
      }
      file = url.substr(oldpos + 1);
      if(lastfile.size() > 0)
         dir +=  lastfile + "\\";
      if(file.substr(file.size() - 3, 3) == ".bz")
         return file.substr(0, file.size() - 3);
      else
         return file;
   }


   ::file::path installer::ca2inplace_get_file(LPCSTR lpcszUrl)
   {
      string url_in(lpcszUrl);
      string url;
      string file;
      string strFind;
      index pos = url_in.find(m_strInstall);
      if(pos == 0)
      {
         url = url_in.substr(m_strInstall.length());
      }
      else
      {
         url = url_in;
      }
      index oldpos = -1;
      pos = url.find("/");
      string lastfile;
      while (pos >=0)
      {
         file = url.substr(oldpos + 1, pos - oldpos -1);
         lastfile = file;
         oldpos = pos;
         pos = url.find("/", oldpos + 1);
      }
      file = url.substr(oldpos + 1);
      if(file.substr(file.size() - 3, 3) == ".bz")
         return file.substr(0, file.size() - 3);
      else
         return file;
   }


   bool installer::ca2_fy_url(string & str,LPCSTR lpcszPath,bool bExist,int64_t iLength,const char * pszMd5,int64_t iGzLen,bool bIndex, bool * pbFileOk)
   {
      ::file::path strStage;
      ::file::path strStageGz;
      string strUrl;
      strUrl = lpcszPath;

      strStage = ca2_get_dir(strUrl) / ca2_get_file(strUrl);

      if(bExist)
      {

         if(file_exists_dup(strStage))
         {

            if(iLength == -1 || iLength == file_length_dup((strStage)))
            {

               if(pszMd5 != NULL && strlen_dup(pszMd5) > 0 && stricmp_dup(System.file().md5((strStage)),pszMd5) == 0)
               {

                  str = strStage;

                  if(pbFileOk != NULL)
                  {

                     *pbFileOk = true;

                  }

                  return true;

               }

            }

         }

      }

      if(pbFileOk != NULL)
      {

         *pbFileOk = false;

      }


      if(bIndex)
      {
         if (!get(m_strInstall / lpcszPath, bExist, iLength, pszMd5, iGzLen))
         {
            printf_dup("Failed: %s\n", (const char *) strUrl);
            return false;
         }
         if(m_bOfflineInstall)
         {
            strStageGz = strUrl;
            strStageGz = ca2bz_get_dir(strUrl) / ca2bz_get_file(strUrl, NULL);
         }
         else
         {
            strStageGz = ca2bz_get_dir(strUrl) / ca2bz_get_file(strUrl, NULL);
         }
      }
      else
      {
         if (!get(m_strInstall / lpcszPath + "." + pszMd5, bExist, iLength, pszMd5, iGzLen))
         {
            printf_dup("Failed: %s\n", (const char *) strUrl);
            return false;
         }
         if(m_bOfflineInstall)
         {
            strStageGz = strUrl;
            strStageGz = ca2bz_get_dir(strUrl) / ca2bz_get_file(strUrl, pszMd5);
         }
         else
         {
            strStageGz = ca2bz_get_dir(strUrl) / ca2bz_get_file(strUrl, pszMd5);
         }
      }
      dir::mk(dir::name(strStage));
      bzuncompress(strStage, strStageGz);
      str = strStage;
      return true;
   }



   int32_t installer::GetFileListEx(::file::patha & stringa, ::file::patha  & patha, string_to_intptr & mapLen, string_to_intptr & mapGzLen, string_to_string & mapMd5, string_to_intptr & mapFlag)
   {

      int32_t iCurrent;

      string strPlatform = System.install().get_platform();

      string strBinaryPlatform = time_binary_platform(strPlatform);

      string strVersion = m_strVersion;

      for(int32_t i = 0; i < patha.get_count(); i++)
      {

         string strPathParam = patha[i];

         if(strPathParam.begins_ci("stage\\basis\\"))
         {

            strPathParam = "time\\" + strBinaryPlatform + "\\" + strVersion + strPathParam.substr(11);

         }

         iCurrent = GetFileList(stringa, strPathParam, mapLen, mapGzLen, mapMd5, mapFlag);

         if(iCurrent == -2)
         {

            return -2;

         }
         else if(iCurrent == -1)
         {

            strPathParam.replace("/", "\\");

            if(stringa.add_unique(strPathParam) >= 0)
            {

               m_iTotalGzLen2 += mapGzLen[strPathParam];

            }

         }

      }

      return 1;

   }



   int32_t installer::GetFileList(::file::patha & stringa, LPCSTR lpcszPath, string_to_intptr & mapLen, string_to_intptr & mapGzLen, string_to_string & mapMd5, string_to_intptr & mapFlag)
   {
      string strPath(lpcszPath);
      strPath.replace("/", "\\");
      strPath = strPath;
      string strUrl(lpcszPath);
      if(strUrl.length() == 0)
         return -1;
      if(strUrl.ends_ci(".spa"))
      {
         if(m_strSpa.add_unique(strUrl) < 0)
         {
            return -1;
         }
      }
      else
      {
         return -1;
      }
      System.install().trace().rich_trace(str::replace("\\", "/", strUrl));
      char buf[2048];
      int32_t iCount = 0;
      int32_t iCurrent;
      strUrl += ".bz";
      string str;
      string strMd5 = mapMd5[strPath];
      strUrl.replace_ci("\\", "/");
      if(!ca2_fy_url(str, strUrl, true, -1, strMd5, -1))
         return -2;
      _FILE * f = fopen_dup(str, "rb");

      string strPlatform = System.install().get_platform();

      string strBinaryPlatform = time_binary_platform(strPlatform);

      string strVersion = m_strVersion;

      while(fgets_dup(buf, sizeof(buf), f))
      {
         buf[sizeof(buf) - 1] = '\0';
         while(buf[strlen_dup(buf) - 1] == '\r' || buf[strlen_dup(buf) - 1] == '\n')
         {
            buf[strlen_dup(buf) - 1] = '\0';
         }
         string str2;
         string strPathParam(buf);
         if(m_strVersion == "stage")
         {
//            strPathParam = ::str::replace_ci("msvcp120d.dll","msvcp120.dll",strPathParam);
            strPathParam = ::str::replace_ci("vcruntime140d.dll","vcruntime140.dll",strPathParam);
            strPathParam = ::str::replace_ci("vcomp140d.dll","vcomp140.dll",strPathParam);
         }
         
         if (::str::begins_eat(strPathParam, "[32]") && strBinaryPlatform != "Win32")
         {

            continue;

         }
         else if (::str::begins_eat(strPathParam, "[64]") && strBinaryPlatform != "x64")
         {

            continue;

         }

         if(strPathParam.begins_ci("stage\\basis\\"))
         {

            strPathParam = "time\\" + strBinaryPlatform + "\\" + strVersion + strPathParam.substr(11);

         }

         iCurrent = GetFileList(stringa, strPathParam, mapLen, mapGzLen, mapMd5, mapFlag);
         if(iCurrent == -2)
         {
            return -2;
         }
         else if(iCurrent == -1)
         {
            if(stringa.add_unique(strPathParam) >= 0)
            {
               m_iTotalGzLen2 += mapGzLen[strPathParam];
            }
         }
         else
         {
            iCount += iCurrent;
         }
      }
      fclose_dup(f);
      return 1;
   }

   int32_t installer::GetLocalFileList(::file::patha & stringa, LPCSTR lpcszUrl)
   {

      string strUrl(lpcszUrl);
      System.install().trace().rich_trace(strUrl);
      //MainWindowRedraw();
      if(strUrl.substr(strUrl.size() - 4, 4) != ".spa")
      {
         return -1;
      }
      char buf[2048];
      int32_t iCount = 0;
      int32_t iCurrent;
      strUrl += ".bz";
      string str;
      if(!ca2_fy_url(str, strUrl, false, -1, NULL, -1))
         return -2;
      _FILE * f = fopen_dup(str, "rb");
      char * pszFind1;
      char * pszFind2;
      while(fgets_dup(buf, sizeof(buf), f))
      {
         while(buf[strlen_dup(buf) - 1] == '\r' || buf[strlen_dup(buf) - 1] == '\n')
         {
            buf[strlen_dup(buf) - 1] = '\0';
         }
         pszFind1 = strstr_dup(buf, ",");
         pszFind2 = NULL;
         if(pszFind1 != NULL)
         {
            pszFind2 = strstr_dup(pszFind2 + 1, ",");
         }
         if(pszFind1 != NULL)
         {
            *pszFind1 = '\0';
            pszFind1++;
         }
         if(pszFind2 != NULL)
         {
            *pszFind2 = '\0';
            pszFind2++;
         }
         string strSpa;
         string str2;
         iCurrent = GetLocalFileList(stringa, buf);
         if(iCurrent == -1)
         {
            stringa.add_unique(buf);
         }
         else
         {
            iCount += iCurrent;
         }
      }
      fclose_dup(f);
      return iCount;
   }


   ::count installer::UncompressFileList(::file::patha & stringa, string_to_string & strmapMd5)
   {
      string strStage;
      string strStageGz;
      string str;
      double d = 0.0;
      m_dProgress = 0.0;
      for(int32_t i = 0; i < stringa.get_count(); i++)
      {
         //m_dProgress1 = d / ((double) stringa.size());
         //d += 1.0;
         //m_dProgress2 = d / ((double) stringa.size());
         str = stringa[i];
         str += ".bz";
         System.install().trace().rich_trace(str);
         strStageGz = m_strInstallGz / ca2bz_get_file(str, strmapMd5[stringa[i]]);
         strStage = ca2_get_dir(str) / ca2_get_file(str);
         dir::mk(dir::name(strStage));
         bzuncompress(strStage, strStageGz);
         d += 1.0;
         m_dProgress = d / ((double) stringa.get_count());
      }
      m_dProgress = 1.0;
      if(m_pwindow != NULL)
      {
//         throw todo(get_thread_app());
         //m_pwindow->redraw();
      }
      return stringa.get_count();
   }

   ::count installer::CopyFileList(::file::patha & stringa, string_to_intptr & mapFlag)
   {
      string strStage;
      string strStageUnbz;
      string str;
      double d = 0.0;
      m_dProgress = 0.0;
      for(int32_t i = 0; i < stringa.get_count(); i++)
      {

         str = stringa[i];

         int_ptr iFlag = mapFlag[str];
         if((iFlag & SPA_FILE_FLAG_VOID_COPY) != 0)
            continue;
         //m_dProgress1 = d / ((double) stringa.size());
         //d += 1.0;
         //m_dProgress2 = d / ((double) stringa.size());
         str += ".bz";
         System.install().trace().rich_trace(str);
         strStageUnbz = ca2unbz_get_dir(str) / ca2unbz_get_file(str);
         strStage = ca2_get_dir(str) / ca2_get_file(str);
         if(strStage.length() > strlen_dup(".expand_fileset") && stricmp_dup(strStage.substr(strStage.length() - strlen_dup(".expand_fileset"), strlen_dup(".expand_fileset")), ".expand_fileset") == 0)
         {
            //         file_ftd_dup(strStage.substr(0, strStage.length() - strlen_dup(".expand_fileset")), strStageUnbz);
         }
         else
         {
            dir::mk(dir::name(strStage));
            file_copy_dup(strStage, strStageUnbz, true);
         }
         d += 1.0;
         m_dProgress = d / ((double) stringa.get_count());
         set_progress((double) i / (double) stringa.get_count());
      }
      m_dProgress = 1.0;
      if(m_pwindow != NULL)
      {
         //throw todo(get_thread_app());
         //m_pwindow->redraw();
      }
      return stringa.get_count();
   }




   void installer::ParseSpaIndex(::xml::node & node)
   {
      if(node.get_name() == "spa" && node.get_children_count() > 0)
      {
         ::xml::node * lpnode = &node;
         for(int32_t ui = 0; ui < lpnode->get_children_count(); ui++)
         {
            if(lpnode->child_at(ui)->get_name() == "index")
            {
               if(lpnode->child_at(ui)->attr("start").is_set())
               {
                  if(string(lpnode->child_at(ui)->attr("start")).length() > 0)
                  {
                     m_iStart = 4;
                     m_strCommandLine = string(lpnode->child_at(ui)->attr("start"));
                     get_command_line_param(m_strApplicationId, m_strCommandLine,"app");
                  }
               }
               if(lpnode->child_at(ui)->attr("build").is_set())
               {
                  if(string(lpnode->child_at(ui)->attr("build")).length() > 0)
                  {
                     m_iStart = 4;
                     m_strBuildResource = string(lpnode->child_at(ui)->attr("build"));
                  }
               }
               if(lpnode->child_at(ui)->attr("type").is_set())
               {
#ifdef WINDOWSEX
                  if(string(lpnode->child_at(ui)->attr("type")) == "parse_file_name")
                  {
                     m_iStart = 4;
                     unichar buf[2048];
                     ::GetModuleFileNameW(NULL, buf, sizeof(buf));
                     string str = ::str::international::unicode_to_utf8(buf);
                     index iFind = str.reverse_find('\\');
                     if(iFind >= 0)
                     {
                        str = str.Mid(iFind + 1);
                     }
                     if(!stricmp_dup(str.substr(str.length()  - 4, 4), ".exe"))
                     {
                        str = str.substr(str.length() - 4);
                        if(str.length() > 19)
                        {
                           m_strApplicationId = str.substr(0, str.length() - 19);
                           m_strCommandLine = ": app=session session_start=" + m_strApplicationId + " app_type=" + m_strApplicationType + " install build_number=\"" + m_strBuild + "\"";
                           m_strBuildResource = str.substr(str.length() - 19);
                        }
                     }
                  }
                  else
#endif
                     if(string(lpnode->child_at(ui)->attr("type")) == "online_default")
                     {
                        m_bOfflineInstall = false;
                        m_strInstallGz = "http://ca2os.com/stage/";
                        m_strInstall = "http://ca2os.com/stage/";
                     }
                     else if(string(lpnode->child_at(ui)->attr("type")) == "offline")
                     {
                        m_bOfflineInstall = true;
                        m_strInstallGz = ::file::path(lpnode->child_at(ui)->attr("src").get_string()) / "stage.bz";
                        m_strInstall = ::file::path(lpnode->child_at(ui)->attr("src").get_string()) / "stage";
                     }
                     else if(string(lpnode->child_at(ui)->attr("type")) == "online")
                     {
                        m_bOfflineInstall = false;
                        m_bInstallSet = true;
                        m_strInstallGz = lpnode->child_at(ui)->attr("src");
                        m_strInstall = lpnode->child_at(ui)->attr("src");
                     }
               }
            }
         }
      }
   }

   string installer::load_string(const char * pszId, const char * pszDefault)
   {
      ::xml::node * lpnode = &m_xmldocStringTable;
      for(int32_t ui = 0; ui < lpnode->get_children_count(); ui++)
      {
         if(string(lpnode->child_at(ui)->attr("id")) == pszId)
         {
            return lpnode->child_at(ui)->get_value();
         }
      }
      return pszDefault;
   }

   void installer::ParseIndexFile(const char * psz, string_to_intptr & mapLen, string_to_string & mapMd5, string_to_intptr & mapGzLen, string_to_intptr & mapFlag)
   {
      _FILE * f = fopen_dup(psz, "rb");
      char * pszFind1;
      char * pszFind2;
      char * pszFind3;
      int32_t iBufSize = 16 * 1024;
      char * buf = (char *) memory_alloc(iBufSize);
      while(fgets_dup(buf, iBufSize, f))
      {
         while(buf[strlen_dup(buf) - 1] == '\r' || buf[strlen_dup(buf) - 1] == '\n')
         {
            buf[strlen_dup(buf) - 1] = '\0';
         }
         pszFind1 = strstr_dup(buf, ",");
         pszFind2 = NULL;
         pszFind3 = NULL;
         if(pszFind1 != NULL)
         {
            pszFind2 = strstr_dup(pszFind1 + 1, ",");
            if(pszFind2 != NULL)
            {
               pszFind3 = strstr_dup(pszFind2 + 1, ",");
            }
         }
         if(pszFind1 != NULL)
         {
            *pszFind1 = '\0';
            pszFind1++;
         }
         if(pszFind2 != NULL)
         {
            *pszFind2 = '\0';
            pszFind2++;
         }
         if(pszFind3 != NULL)
         {
            *pszFind3 = '\0';
            pszFind3++;
         }
         string strSpa;
         string str2;
         strSpa = m_strInstall;
         strSpa += buf;
         if(pszFind1 != NULL)
         {
            m_iProgressMode = 0;
            mapLen[(string)(const char *) buf] = atoi_dup(pszFind1);
         }
         else
            mapLen[(string)(const char *) buf] = -1;
         if(pszFind2 != NULL)
         {
            mapMd5[(string)(const char *) buf] = pszFind2;
         }
         else
            mapMd5[(string)(const char *) buf] = "";
         if(pszFind3 != NULL)
         {
            mapGzLen[(string)(const char *) buf] = atoi_dup(pszFind3);
         }
         else
            mapGzLen[(string)(const char *) buf] = -1;
      }
      fclose_dup(f);
      memory_free_dbg(buf, 0);
   }

   bool installer::spa_exec(const char * psz)
   {
      string strExec(psz);
      if(strExec.substr(0, 15) == "install_service")
      {
         string strStage;
         strStage = dir::element() / strExec.substr(16);

#ifdef METROWIN

         throw "todo";

#else

         ::aura::shell_launcher launcher1(m_pwindow == NULL ? NULL : m_pwindow->get_safe_handle(), "open", strStage, " : remove usehostlogin", dir::name(strStage), SW_SHOWNORMAL);

         launcher1.execute();

         ::aura::shell_launcher launcher2(m_pwindow == NULL ? NULL : m_pwindow->get_safe_handle(),"open",strStage," : install usehostlogin",dir::name(strStage),SW_SHOWNORMAL);

         launcher2.execute();

#endif

      }
      else if(strExec.substr(0, 10) == "install_ex")
      {
         string strStage;
         string str2 = strExec.substr(11);
         index iPos = str2.find(" ");
         string str3 = str2.substr(iPos + 1);
         strStage = dir::element() / str3;

#ifdef METROWIN

         throw "todo";

#else
         ::aura::shell_launcher launcher(m_pwindow == NULL ? NULL : m_pwindow->get_safe_handle(), "open", strStage, (" : " + str2.substr(0, iPos) + " usehostlogin"), dir::name(strStage), SW_SHOWNORMAL);

         launcher.execute();

#endif

      }
      return true;
   }


    void send_spaboot_install_post(int32_t a, int32_t b)
   {

#ifdef METROWIN

      throw "todo";

#else

      ::aura::ipc::tx txchannel(get_thread_app());

      if(!txchannel.open("core/spaboot_install_callback"))
         return;

#endif

#if defined(WINDOWSEX)

      ::PostMessage(txchannel.m_oswindow, WM_USER + 100, a, b);

#else

       throw "not implemented";

#endif

   }


   bool installer::ignit_phase2()
   {



      char szFormat[256];
      string strUrl;
      strUrl = m_strSpaIgnitionBaseUrl + "/query?node=install_application&id=";
      strUrl += m_strApplicationId;
      strUrl += "&key=post_install_count";

      string strCount = http_get(strUrl, false);

      int32_t iCount = atoi_dup(strCount);

      //set_progress(0.2);
      for(int32_t i = 0; i < iCount; i++)
      {
         strUrl = m_strSpaIgnitionBaseUrl + "/query?node=install_application&id=";
         strUrl += m_strApplicationId;
         strUrl += "&key=post_install";
         sprintf(szFormat, "[%d]", i);
         strUrl += szFormat;

         string strExec = http_get(strUrl, false);

         if(!spa_exec(strExec))
         {
            #ifdef WINDOWSEX
            ::simple_message_box(m_pwindow->get_safe_handle(),"Error","Error",MB_OK);
            #endif
         }
         set_progress(((double) i * (0.5 - 0.2) / (double) iCount) + 0.2);
      }
      //set_progress(0.5);
      //      uint32_t dwStartError;
      System.install().trace().rich_trace("starting app.install.exe...");


      string strCommandLine = m_strCommandLine;

      string strBuildNumberParam;

      if(!get_command_line_param(strBuildNumberParam,strCommandLine,"build_number") || strBuildNumberParam.is_empty())
      {

         strCommandLine += " build_number=\"" + m_strBuild + "\"";

      }

      int32_t i = run_ca2_application_installer(strCommandLine);

      /*if(m_bLaunchDesktopApplicationOnIgnitPhase2)
      {

         start_ca2_application();

      }*/


      /*if(m_strStart != "_set_windesk" && is_installed("application", "_set_windesk"))
      {
      //uint32_t dwStartError2;
      System.install().trace().rich_trace("starting windeskPackage...");
      int32_t i2 = run_ca2_application_installer("_set_windesk");
      System.install().trace().rich_trace("started windeskPackage");
      }*/
      System.install().trace().rich_trace(".");
      //set_progress(0.9);
      string strPlatform = System.install().get_platform();
      if(i == 0)
      {
         System.install().set_updated(m_strBuild);
//         System.install().installation_file_lock(false);
         //       // keeps g_bInstalling for a while
         //         Sleep((5000) * 5);
         //::PostMessage(g_oswindow, WM_CLOSE, 0, 0);

         if(m_straRestartProcess.get_count() > 0)
         {

            string str;

            str = "Now :\n\n";

            for(int32_t i = 0; i < m_straRestartProcess.get_count(); i++)
            {
               if(file_is_equal_path(file_get_mozilla_firefox_plugin_container_path(), m_straRestartProcess[i]))
               {
                  m_straRestartProcess.remove_at(i);
                  str += "You may reload the Firefox plugin or plugins that has/have been shutdown.\n\n";
               }
            }

            if(m_straRestartProcess.get_count() > 0)
            {
               str += "You may restart the applications listed below if they are not restarted automatically:\n\n";
               for(int32_t i = 0; i < m_straRestartProcess.get_count(); i++)
               {
                  str += "\t";
                  str += m_straRestartProcess[i];
                  if(i == m_straRestartProcess.get_count() - 1)
                  {
                     str += ".";
                  }
                  else if(i == m_straRestartProcess.get_count() - 2)
                  {
                     str += ", and;\n";
                  }
                  else
                  {
                     str += ";\n";
                  }

               }
            }

            ::simple_message_box(NULL,str,"You may restart the applications ...",MB_ICONINFORMATION | MB_OK);

         }

         System.install().trace().rich_trace(":::::Thank you");
         System.install().trace().rich_trace("***Thank you");
         System.install().trace().rich_trace("Thank you");
         System.install().trace().trace_progress(1.0);


         //send_spaboot_install_post(2, 0);

         return true;
      }
      else
      {
         //uint32_t dwError = dwStartError;
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("Failed to install");
         System.install().trace().rich_trace("Manually contact mailto:support.veriwell@ca2mail.com");
         System.install().trace().rich_trace("or Manually go to http://ca2os.com/");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("In ca2os.com you can try to");
         System.install().trace().rich_trace("download the most recent release version for installation");
         System.install().trace().rich_trace("(Automatic links not availabe by now)");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         System.install().trace().rich_trace("");
         return false;
      }
   }


   int32_t installer::run_file(const char * pszFile, int32_t nCmdShow)
   {

      m_strFile = pszFile;

      //	MESSAGE msg;
      //	HACCEL hAccelTable;



      ::xml::document node(get_app());

      // simple_message_box(NULL, "BegInstall", "Caption", MB_OK);
      if(m_iStart != 4)
      {
         string strCa2sp = file_as_string_dup(m_strFile);
         if(strCa2sp.length() == 0)
         {
#ifdef WINDOWSEX
            strCa2sp = read_resource_as_string_dup(NULL, 1984, "CA2SP");
#else
            throw "TODO";
#endif
         }
         if(strCa2sp.length() > 0)
         {
            node.load(strCa2sp);
            ParseSpaIndex(node);
         }
         else
         {
            m_strApplicationId = "session";
            m_strCommandLine = ": app=session session_start=session app_type=application install build_number=\"" + m_strBuild + "\"";
         }
      }



      /*
      char * pszStart = strstr_dup(lpCmdLine, "start=");
      char * pszEnd = NULL;
      if(pszStart != NULL)
      {
      pszStart += strlen_dup("start=");
      pszEnd = pszStart;
      while(isdigit(*pszEnd) || isalpha_dup(*pszEnd))
      {
      pszEnd++;
      }
      m_strStart = string(pszStart, pszEnd - pszStart);
      }
      else
      {
      m_strStart = "bergedge";
      }*/

      //   char * pszStart;
#ifdef WINDOWSEX
      char * pszEnd = NULL;
      char * lpCmdLine = ::GetCommandLine();
      char * pszLocale = strstr_dup((char *) lpCmdLine, "locale=");
      if(pszLocale != NULL)
      {
         pszLocale += strlen_dup("locale=");
         pszEnd = pszLocale;
         while(isdigit_dup(*pszEnd) || isalpha_dup(*pszEnd))
         {
            pszEnd++;
         }
         m_strLocale = string(pszLocale, pszEnd - pszLocale);
      }
      else
      {
         m_strLocale = "";
      }
#else
        throw "todo";
#endif

      return run_starter_start(nCmdShow);
   }


   int32_t installer::starter_start(const char * pszCommandLine)
   {

      m_strCommandLine           = pszCommandLine;

      m_strApplicationId         = get_command_line_param(pszCommandLine, "app", "session", "session_start").trimmed();

      get_command_line_param(m_strApplicationType, pszCommandLine,"app_type");

      if(m_strApplicationId.is_empty())
         return -1;

      if(m_strApplicationType.is_empty())
         m_strApplicationType    = "application";

      get_command_line_param(m_strInstallLocale, pszCommandLine,"locale");

      get_command_line_param(m_strInstallSchema,pszCommandLine,"schema");

      get_command_line_param(m_strVersion,pszCommandLine,"version");

      if(m_strVersion.is_empty())
         m_strVersion = "basis";

      m_strInstallSchema.trim();

      if(m_strInstallSchema.is_empty())
      {

         if(m_strInstallLocale.is_empty())
         {

            m_strInstallLocale      = "en";
            m_strInstallSchema      = "en";

         }
         else
         {

            m_strInstallSchema      = m_strInstallLocale;

         }

      }
      else
      {

         if(m_strInstallLocale.is_empty())
         {

            m_strInstallLocale      = m_strInstallSchema;

         }

      }

      m_bOfflineInstall          = false;

      m_strInstallGz             = "http://ca2os.com/stage/";

      m_strInstall               = "http://ca2os.com/stage/";

      if(m_strInstallLocale.CompareNoCase("sv") == 0 || ::str::begins_ci(m_strInstallLocale,"sv-") || m_strInstallLocale.CompareNoCase("se") == 0)
      {

         m_strInstallStatusTemplate = "Installera %APPNAME%";

      }
      else if(m_strInstallLocale.CompareNoCase("_std") == 0 || m_strInstallLocale.CompareNoCase("en") == 0 || ::str::begins_ci(m_strInstallLocale,"en-"))
      {

         m_strInstallStatusTemplate = "Installing %APPNAME%";

      }
      else if(m_strInstallLocale.CompareNoCase("pt") == 0 || ::str::begins_ci(m_strInstallLocale,"pt-"))
      {

         m_strInstallStatusTemplate = "Instalando %APPNAME%";

      }
      else if(m_strInstallLocale.CompareNoCase("fr") == 0 || ::str::begins_ci(m_strInstallLocale,"fr-"))
      {

         m_strInstallStatusTemplate = "Installation %APPNAME%";

      }
      else if(m_strInstallLocale.CompareNoCase("es") == 0 || ::str::begins_ci(m_strInstallLocale,"es-"))
      {

         m_strInstallStatusTemplate = unitext("Instalación de %APPNAME%");

      }
      else if(m_strInstallLocale.CompareNoCase("it") == 0 || ::str::begins_ci(m_strInstallLocale,"it-"))
      {

         m_strInstallStatusTemplate = "L'installazione di %APPNAME%";

      }
      else if(m_strInstallLocale.CompareNoCase("de") == 0 || ::str::begins_ci(m_strInstallLocale,"de-"))
      {

         m_strInstallStatusTemplate = unitext("Installieren %APPNAME%");

      }
      else if(m_strInstallLocale.CompareNoCase("ja") == 0 || ::str::begins_ci(m_strInstallLocale,"ja-") || m_strInstallLocale.CompareNoCase("jp") == 0)
      {

         m_strInstallStatusTemplate = unitext("%APPNAME%をインストールする");

      }
      else if(m_strInstallLocale.CompareNoCase("zh-cn") == 0 || ::str::begins_ci(m_strInstallLocale,"zh-") || m_strInstallLocale.CompareNoCase("zh") == 0)
      {

         m_strInstallStatusTemplate = unitext("安装%APPNAME%");

      }
      else if(m_strInstallLocale.CompareNoCase("zh-tw") == 0)
      {

         m_strInstallStatusTemplate = unitext("安裝%APPNAME%");

      }
      else
      {

         m_strInstallStatusTemplate = "Installing %APPNAME%";
         //m_strInstallStatusTemplate = Application.http().get_locale_schema("http://account.ca2.cc/defer_ls_get?sessid=noauth&id=spa::InstallStatusTemplate",m_strInstallLocale,m_strInstallSchema);

      }

      m_bForceUpdatedBuild       = true;

      m_bStarterStart            = true;

      return run_starter_start(m_bShow ? SW_SHOWNORMAL : SW_HIDE);

   }


   int32_t installer::application_name()
   {

      System.install().trace().rich_trace("***Getting application name");

      string strUrl;
      System.install().trace().rich_trace(("get application name from server http://spaignition.api.server.ca2.cc/ using application id \"" + m_strApplicationId + "\" "));
      strUrl = m_strSpaIgnitionBaseUrl + "/query?node=install_application&id=";
      strUrl += m_strApplicationId;
      strUrl += "&key=name";

      string strName;
      int32_t iRetry = 0;
      while(true)
      {
         strName = Application.http().get_locale_schema(strUrl, m_strInstallLocale, m_strInstallSchema);
         if(strName.length() > 0)
            break;
         else if(iRetry < 84)
            iRetry++;
         else
         {
            System.install().trace().rich_trace("main server seems to be unavailable. could not get application name");
/////////            exit(-1);
            return -1;
         }
         System.install().trace().trace_add(".");
         Sleep(184);
      }

      string strStatusTemplate;

      if(m_strInstallStatusTemplate.get_length() >= strlen_dup("%APPNAME%"))
      {
         strStatusTemplate = m_strInstallStatusTemplate;
      }
      else
      {
         strStatusTemplate = "Installing %APPNAME%";
      }

      m_strApplicationName = strName;

      strStatusTemplate.replace("%APPNAME%", strName);

      strName = strStatusTemplate;
      strName = strName + " " + m_strBuild;
      strName = strName + ::str::has_char(get_command_line_param2(m_strCommandLine,"locale")," \"","\"");
      strName = strName + ::str::has_char(get_command_line_param2(m_strCommandLine,"schema")," \"","\"");
      System.install().trace().rich_trace((":::::" + strName));

      m_strTitle = strName;

      return 0;
   }

   int32_t installer::appmatter_list()
   {

      string strUrl;

      System.install().trace().rich_trace(("get appmatter list for application with id : \"" + m_strApplicationId + "\" "));
      System.install().trace().rich_trace(("locale : \"" + m_strInstallLocale + "\" "));
      System.install().trace().rich_trace(("schema : \"" + m_strInstallSchema + "\" "));

      strUrl = m_strSpaIgnitionBaseUrl + "/appmatter_spa_list?app=";
      strUrl += m_strApplicationId;
      strUrl += "&locale=";
      strUrl += m_strInstallLocale;
      strUrl += "&schema=";
      strUrl += m_strInstallSchema;
      strUrl += "&version=";
      strUrl += m_strVersion;

      string str;
      int32_t iRetry = 0;
      while(true)
      {

         str = http_get(strUrl, false);

         if(str.length() > 0)
            break;
         else if(iRetry < 84)
            iRetry++;
         else
         {
            System.install().trace().rich_trace("main server seems to be unavailable. could not get appmatter list");
//            exit(-1);
            return -1;
         }
         System.install().trace().trace_add(".");
         Sleep(184);
      }

      m_strAppMatterList = str;

      System.install().trace().rich_trace("got appmatter list");

      return 0;

   }

   int32_t installer::ca2_build_version()
   {

      string strPlatform = System.install().get_platform();

      int32_t iRetry = 0;
RetryBuildNumber:
      if(m_strBuildResource.length() == 19) // heuristically valid
      {
         System.install().trace().rich_trace("***Internal build number");
         m_strBuild = m_strBuildResource;
      }
      else
      {
         System.install().trace().rich_trace("***Getting build number");
         if(iRetry > 10)
         {
            System.install().trace().rich_trace("could not get build number. exiting");
//            exit(-1);
            return -1;
         }
         iRetry++;
//         m_strBuild = Application.http().get(m_strSpaIgnitionBaseUrl + "/query?node=build", false, &::ms_get_callback, (void *) this);

         m_strBuild = http_get(m_strSpaIgnitionBaseUrl + "/query?node=build&sessid=noauth&version=" + m_strVersion + "&platform=" + strPlatform, false);

         m_strBuild.trim();

         if(m_strBuild.length() != 19)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }
      }
      System.install().trace().rich_trace(m_strBuild);
      return 0;
   }


   int32_t installer::ca2_build_version_etc(string & strSpaHost,int32_t &iHostRetry,stringa & straMd5, int_array & iaLen)
   {

      int32_t iRetry = 0;

      string strEtc;

      string strName;

      ::file::patha straTemplate;

      string strBuild;

      ::lemon::array::copy(straTemplate,::install_get_plugin_base_library_list(m_strPlatform, m_strVersion));

      string strUrl;

      ::file::memory_buffer file(get_app());

      string strAppMatterList;

      string strLen;

      file_size_t len;

      string strIndexBuild;
      ::file::path strIndexPath;
      string strIndexMd5;
      memory mem(get_app());

      string strBuildPath;

      strBuildPath = System.dir().element_commonappdata(::dir::element()) / "spa_build_" + System.install().get_platform() + ".txt";

      // using a guess for build number (the latest installed one)
      {

         strBuild = Application.file().as_string(strBuildPath);

         strIndexBuild = strBuild;

         strIndexBuild.replace(" ","_");

         strIndexBuild.replace(":","-");

         strIndexPath = "app/stage/metastage/index-" + strIndexBuild + ".spa.bz";

         strIndexPath = ca2_get_dir(strIndexPath) / ca2_get_file(strIndexPath);

         if(file_exists_dup(strIndexPath))
         {

            strIndexMd5 = System.file().md5(strIndexPath);

         }

      }

      string strStatus;

   RetryBuildNumber:

      if(m_strBuildResource.length() == 19) // heuristically valid
      {
         System.install().trace().rich_trace("***Internal build number");
         m_strBuild = m_strBuildResource;
      }
      else
      {
         System.install().trace().rich_trace("***Getting build number");
         if(iRetry > 10)
         {
            System.install().trace().rich_trace("could not get build number. exiting");
            //            exit(-1);
            return -1;
         }
         iRetry++;
         //         m_strBuild = Application.http().get(m_strSpaIgnitionBaseUrl + "/query?node=build", false, &::ms_get_callback, (void *) this);




         strUrl = m_strSpaIgnitionBaseUrl + "/query?node=build__host_and_application_name&sessid=noauth";
         strUrl += "&version=";
         strUrl += m_strVersion;
         strUrl += "&appid=";
         strUrl += m_strApplicationId;
         strUrl += "&stage=";
         strUrl += straTemplate.implode(",");
         strUrl += "&locale=";
         strUrl += m_strInstallLocale;
         strUrl += "&schema=";
         strUrl += m_strInstallSchema;
         strUrl += "&platform=";
         strUrl += System.install().get_platform();

         if(strIndexMd5.has_char())
         {

            strUrl += "&index_md5=";
            strUrl += strIndexMd5;

         }



         property_set set;

         set["disable_ca2_sessid"] = true;

         set["raw_http"] = true;

         file.Truncate(0);

         file.seek_to_begin();


         single_lock sl(&m_mutexOmp);

         sl.lock();
         sp(::sockets::http_session) & psession = m_httpsessionptra.element_at_grow(0);
         sl.unlock();

         System.install().trace().trace_add("\ndownloading " +  strUrl + "\n");

         if (!Application.http().download(*m_psockethandler, psession, strUrl, &file, set))
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         file.seek_to_begin();
//         strEtc = http_get(strUrl, true);

         if(file.get_size() < 19)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }


         strBuild.Empty();

         file.read_string(strBuild);

         if(strBuild.length() != 19)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         strName.Empty();

         file.read_string(strName);

         if(strName.length() <= 0)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         strSpaHost.Empty();

         file.read_string(strSpaHost);

         if(strSpaHost.length() <= 0)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         straMd5.set_size(straTemplate.get_count() + 1);
         iaLen.set_size(straTemplate.get_count() + 1);

         for(index i = 0; i < straMd5.get_size(); i++)
         {
            straMd5[i].Empty();
            string strMd5AndLen;
            file.read_string(strMd5AndLen);
            int iFind =strMd5AndLen.find('|');
            if(iFind < 0)
            {
               Sleep(184);
               goto RetryBuildNumber;
            }
            string strMd5 = strMd5AndLen.Left(iFind);
            iaLen[i] = atoi(strMd5AndLen.Mid(iFind + 1));
            straMd5[i] = strMd5;
            straMd5[i].trim();
            if(straMd5[i].length() != 32)
            {
               Sleep(184);
               goto RetryBuildNumber;
            }

         }

         file.read_string(strAppMatterList);

         strLen.Empty();

         file.read_string(strStatus);

         if(strStatus == "ok")
         {
            if(strIndexMd5.is_empty())
            {
               Sleep(184);
               goto RetryBuildNumber;
            }
         }
         else if(strStatus != "send")
         {
            Sleep(184);
            goto RetryBuildNumber;
         }
         else
         {

            file.read_string(strLen);

            if(strLen.length() <= 0)
            {
               Sleep(184);
               goto RetryBuildNumber;
            }

            len = atoi(strLen);

            strIndexBuild = strBuild;

            strIndexBuild.replace(" ","_");

            strIndexBuild.replace(":","-");

            strIndexPath = "app/stage/metastage/index-" + strIndexBuild + ".spa.bz";

            strIndexPath = ca2_get_dir(strIndexPath) / ca2_get_file(strIndexPath);

            {

               ::file::buffer_sp file2 = Application.file().get_file(strIndexPath,::file::mode_create | ::file::type_binary | ::file::mode_write | ::file::defer_create_directory);

               mem.allocate(1024 * 1024);

               file_size_t uiTotalRead = 0;

               memory_size_t uiRead;

               while((uiRead = file.read(mem, mem.get_size())))
               {
                  
                  file2->write(mem, uiRead);

                  uiTotalRead += uiRead;

               }

               if (uiTotalRead != len)
               {

                  Sleep(184);

                  goto RetryBuildNumber;

               }

            }

         }

      }

      m_strBuild = strBuild;

      System.install().trace().rich_trace(m_strBuild);

      string strStatusTemplate;

      if(m_strInstallStatusTemplate.get_length() >= strlen_dup("%APPNAME%"))
      {
         strStatusTemplate = m_strInstallStatusTemplate;
      }
      else
      {
         strStatusTemplate = "Installing %APPNAME%";
      }

      strStatusTemplate.replace("%APPNAME%",strName);

      strName = strStatusTemplate;
      strName = strName + " " + m_strBuild;
      strName = strName + ::str::has_char(get_command_line_param2(m_strCommandLine,"locale")," \"","\"");
      strName = strName + ::str::has_char(get_command_line_param2(m_strCommandLine,"schema")," \"","\"");
      System.install().trace().rich_trace((":::::" + strName));

      m_strTitle = strName;

      //strSpaHost = "server.ca2.cc";

      strAppMatterList.replace(",","\n");

      m_strAppMatterList = strAppMatterList;

      return 0;

   }


   int32_t installer::calc_host(string & strSpaHost, int32_t &iHostRetry)
   {
   RetryHost:

      System.install().trace().rich_trace("***Finding optimal server");

      if(iHostRetry > 84)
      {
//         exit(-1);
         return -1;
      }
      else if(iHostRetry > 0)
      {
         System.install().trace().rich_trace("Retrying to guess host...");
      }
      iHostRetry = 1;
      if(m_strLastHost.is_empty())
      {
         m_strLastHost = m_strCurrentHost;
      }
      else
      {
         m_strLastHost = m_strCurrentHost + ";" + m_strLastHost;
      }
      string strUrl;

      strUrl = m_strSpaIgnitionBaseUrl + "/query?node=spa_host&version=" + m_strVersion;

      if(!m_strLastHost.is_empty())
      {
         strUrl += "&last_host=" + m_strLastHost;
      }
      strUrl += "&sessid=noauth";
      System.install().trace().rich_trace("***Guessing fastest mirror");
      int32_t iGuessRetry = 0;
      System.install().trace().rich_trace(".");
      while(iGuessRetry < 30)
      {

         http_get(strUrl, false);

         if(strSpaHost.is_empty())
         {
            if(m_strLastHost.is_empty())
            {
               System.install().trace().trace_add(".");
               Sleep(184);
               iGuessRetry++;
            }
            else
            {

               strUrl = m_strSpaIgnitionBaseUrl + "/query?node=spa_host&version=" + m_strVersion;

            }

         }
         else
         {
            break;
         }
      }
      if(m_strCurrentHost == strSpaHost)
      {
         m_strLastHost.clear();
         m_strCurrentHost.clear();
         goto RetryHost;
      }
      return 0;
   }

   /*string installer::install(const char * psz)
   {
   return m_strInstall + psz;
   }*/



   int32_t installer::run_uninstall(const char * lpCmdLine, int32_t nCmdShow)
   {
      return run_install(lpCmdLine, nCmdShow);
   }

   int32_t installer::run_uninstall_run(const char * lpCmdLine, int32_t nCmdShow)
   {
      uint32_t dwStartError = 0;
      return ca2_app_install_run(lpCmdLine, dwStartError, true);
   }

   int32_t installer::run_install(const char * lpCmdLine, int32_t nCmdShow)
   {
      string str = lpCmdLine;
      string strFile = lpCmdLine;

      index i1 = str.find("\"");
      if(i1 >= 0)
      {
         index i2 =str.find("\"", i1 +1);
         strFile = str.substr(i1 + 1, i2 - i1 - 1);
      }
      else
      {
         strFile = str;
      }

      if(strFile.is_empty())
      {

         strFile = dir::module() / "spa.xml";

      }

      return run_file(strFile, nCmdShow);
   }



   int32_t installer::run_starter_start(int32_t nCmdShow)
   {

      string strName = "spa ignition";

      strcpy_dup(szTitle, strName);
      strcpy_dup(szWindowClassSpaAdmin, strName);

      if(nCmdShow != SW_HIDE)
      {
         //m_pwindow = new install::interaction_impl;
      }

#if defined(WINDOWSEX)

      //if(m_pwindow != NULL)
      //{

      //   m_pwindow->register_window_class(Application.m_hinstance);

      //}

#endif

      if (!init_instance(nCmdShow))
      {

         return FALSE;

      }

      return 0;

   }

   bool installer::init_instance(int32_t nCmdShow)
   {

      m_strInstallGz = dir::module() / "core/bz/stage";
      m_strInstall = dir::module() / "core/stage";

      m_strIndexGz = m_strInstallGz / ("app/stage/metastage/" + m_strApplicationId + ".spa.bz");
      m_strIndex = m_strInstallGz / ("app/stage/metastage/" + m_strApplicationId + ".spa");

      bool bOfflineInstall1 = dir::is(dir::module() / "core/bz");
      //bool bOfflineInstall2 = file_exists_dup(g_strIndexGz);
      //m_bOfflineInstall = bOfflineInstall1 && bOfflineInstall2;
      m_bOfflineInstall = bOfflineInstall1;

      // Default stage.bz folder not present, would default to internet install
      // since the spa.xml is not present and contains turning information.
      if(!m_bOfflineInstall && (m_strApplicationId.length() == 0 || (!m_bForceUpdatedBuild && m_strBuildResource.length() == 0)))
      {
         string str = file_as_string_dup(dir::module() / "spa.xml");
         ::xml::document node(get_app());
         node.load(str);
         ParseSpaIndex(node);
      }

#ifdef WINDOWSEX
      // Default stage.bz folder not present, would default to internet install
      // since the spa.xml is not present and contains turning information.
      if(!m_bOfflineInstall && !m_bInstallSet && (m_strApplicationId.length() == 0 || (!m_bForceUpdatedBuild && m_strBuildResource.length() == 0)))
      {
         string str = read_resource_as_string_dup(NULL, 1984, "CA2SP");
         ::xml::document node(get_app());
         node.load(str);
         ParseSpaIndex(node);
      }
#else
      throw "NEEDED?TODO?";
#endif


      if(m_strApplicationId == "***parse_file_name")
      {

      }



      m_strIndexGz = m_strInstallGz / ("app\\stage\\metastage\\" + m_strApplicationId + ".spa.bz");
      m_strIndex = m_strInstallGz / ("app\\stage\\metastage\\" + m_strApplicationId + ".spa");

      m_bInternetInstall = !m_bOfflineInstall;

      m_iStyle = 0;


      if(m_bSynch)
      {
         synch_starter_start();
      }
      else
      {
         start_starter_start();
      }

      return TRUE;
   }

   int32_t installer::spaadmin_main(const char * pszCommandLine)
   {

      ::srand(::get_tick_count());

      System.install().trace().ensure_trace_file();

      if(!m_machineevent.initialize())
      {
         return 5;
      }

      m_iStyle = 0;

      m_bShowPercentage = false;

      //UNREFERENCED_PARAMETER(lpCmdLine);
      m_iStart = 0;
      //m_iStyle = 0;

      string str = pszCommandLine;

      string strExe = file_name_dup(file_module_path_dup());

//      bool bSpa = false;

      if(str.find(" background ") >= 0
         || str.ends_ci(" background") || strExe.find(" in background ") >= 0
         || strExe.ends_ci(" in background"))
      {
         m_bShow = false;
      }

      m_bLaunchDesktopApplicationOnIgnitPhase2 =
         str.find(" enable_desktop_launch ") >= 0
         || str.ends_ci(" enable_desktop_launch")
         || strExe.find(" enable_desktop_launch ") >= 0
         || strExe.ends_ci(" enable_desktop_launch");

      if(str.find(" spa ") >= 0
         || str.ends_ci(" spa") || strExe.find(" in spa ") >= 0
         || strExe.ends_ci(" in spa"))
      {
         ::install::installer::do_spa();
         return 0;
      }

      m_bLoginStartup = false;
      index i = str.find("install_filter=");
      if(i >= 0)
      {
         index iStart = i + 15; // 15 = strlen_dup("install_filter=")
         i = str.find(" ", i);
         if(i >= 0)
         {
            m_strInstallFilter = str.substr(iStart, i - iStart);
         }
         else
         {
            m_strInstallFilter = str.substr(iStart);
         }
      }
      // simple_message_box(NULL, "xxx", "yyy", MB_OK);
      i = str.find("install=");
      if(i >= 0)
      {
         index iStart = i + 8; // 8 = strlen_dup("install=")
         i = str.find(" ", i);
         if(i >= 0)
         {
            m_strApplicationId = str.substr(iStart, i - iStart);
         }
         else
         {
            m_strApplicationId = str.substr(iStart);
         }
         m_strCommandLine = ": app=session session_start="  + m_strApplicationId + " app_type=" + m_strApplicationType + " install build_number=\"" + m_strBuild + "\"";
         m_iStart = 4;
         return run_install(pszCommandLine, m_nCmdShow);

      }
      if(strExe.begins_ci("Install "))
      {
         ::count iStart = strlen_dup("Install ");
         i = strExe.find(" ", iStart);
         string strIdStart;
         if(i >= 0)
         {
            strIdStart = strExe.substr(iStart, i - iStart);
         }
         else
         {
            strIdStart = strExe.substr(iStart);
         }
         m_iStart = 4;
         return starter_start(strIdStart);
      }
      string strFind("starter_start:");
      i = str.find(strFind);
      if(i >= 0)
      {
         index iStart = i + strFind.length(); // 8 = strlen_dup("install=")
         //         i = str.find(" ", iStart);
         //         string strCommandLine;
         //       if(i>= 0)
         //     {
         // strCommandLine = str.substr(iStart, i - iStart);
         //   }
         // else
         //{
         string strCommandLine;
         strCommandLine = str.substr(iStart);
         //       }
         m_iStart = 4;
         return starter_start(strCommandLine);
      }

#ifdef WINDOWSEX
      unichar * lpwstr = ::GetCommandLineW();
      int32_t numargs;
      int32_t numchars;
      wparse_cmdline(
         lpwstr,
         NULL,
         NULL,
         &numargs,
         &numchars);

      unichar * pszCmdLine = new WCHAR[numchars];
      unichar ** argv = new WCHAR*[numargs];

      wparse_cmdline(
         lpwstr,
         argv,
         pszCmdLine,
         &numargs,
         &numchars);

      if(numargs >= 2)
      {
         string str(::str::international::unicode_to_utf8(argv[1]));
         return run_file(str, m_nCmdShow);
      }


      index iFind;
      if((iFind = str.find("uninstall"))>= 0)
      {
         return run_uninstall_run(&str[iFind + 10], m_nCmdShow);
      }
      else if(str.find("uninstall")>= 0)
      {
         return run_uninstall(pszCommandLine, m_nCmdShow);
      }
      else
      {
         //m_strStart = "_set_windesk";  m_iStart = 4; return run_install(lpCmdLine, nCmdShow);
         return run_install(pszCommandLine, m_nCmdShow);
      }
#else

      throw "todo";

#endif
   }


   bool installer::is_application_opened(const char * psz)
   {

      /*   SECURITY_ATTRIBUTES MutexAttributes;
      memset_dup( &MutexAttributes, 0, sizeof(MutexAttributes) );
      MutexAttributes.nLength = sizeof( MutexAttributes );
      MutexAttributes.bInheritHandle = FALSE; // object uninheritable
      // declare and initialize a security descriptor
      SECURITY_DESCRIPTOR SD;
      bool bInitOk = InitializeSecurityDescriptor(
      &SD,
      SECURITY_DESCRIPTOR_REVISION );
      if ( bInitOk )
      {
      // give the security descriptor a Null Dacl
      // done using the  "TRUE, (PACL)NULL" here
      bool bSetOk = SetSecurityDescriptorDacl( &SD,
      TRUE,
      (PACL)NULL,
      FALSE );
      if ( bSetOk )
      {*/
      // Make the security attributes point
      // to the security descriptor
      //MutexAttributes.lpSecurityDescriptor = &SD;
      string strMutex;
      strMutex = "Global\\ca2_application_global_mutex:";
      strMutex += psz;
      //HANDLE hmutex = ::CreateMutex(&MutexAttributes, FALSE, strMutex);
      mutex mutex(get_thread_app(), false, strMutex);
      bool bOpened = ::GetLastError() == ERROR_ALREADY_EXISTS;
      if(bOpened)
      {
         string strMessage;
         strMessage = psz;
         strMessage += "app.exe is still opened!";
         System.install().trace().rich_trace(strMessage);
         System.install().trace().rich_trace("You may finalize the process by:");
         System.install().trace().rich_trace("   1. Pressing CTRL+ALT+DEL;");
         System.install().trace().rich_trace("   2. Opening Task Manager, and;");
         strMessage = "   4. Closing the process ";
         strMessage += psz;
         strMessage += "app.exe.";
         System.install().trace().rich_trace(strMessage);
         System.install().trace().rich_trace(".");
      }
      return bOpened;
      /*      }
      else
      {
      return false;
      }
      }
      else
      {
      return false;
      }
      */
   }

   bool installer::is_there_application_opened()
   {
      return
         is_application_opened("winactionarea")
         || is_application_opened("winservice_1")
         || is_application_opened("winutil")
         || is_application_opened("winshelllink")
         || is_application_opened("command")
         || is_application_opened("winservice_filesystemsize")
         || is_application_opened("filemanager");
   }

   void installer::machine_signalize_close_application()
   {
      synch_lock lockMachineEvent(&m_machineevent.m_mutex);
      machine_event_data data;
      m_machineevent.read(&data);
      data.m_fixed.m_bRequestCloseApplication = true;
      m_machineevent.write(&data);
   }

   bool installer::machine_unsignalize_close_application()
   {
      synch_lock lockMachineEvent(&m_machineevent.m_mutex);
      machine_event_data data;
      m_machineevent.read(&data);
      data.m_fixed.m_bRequestCloseApplication = false;
      bool bOk = m_machineevent.write(&data);
      return bOk;
   }

   bool installer::machine_check_close_application(bool bDefault)
   {
      synch_lock lockMachineEvent(&m_machineevent.m_mutex);
      machine_event_data data;
      if(!m_machineevent.read(&data))
         return false;
      return data.m_fixed.m_bRequestCloseApplication;
   }


   void installer::do_spa()
   {

      ::install::installer * pinstaller = new ::install::installer(get_thread_app());

      pinstaller->m_bStarterStart = false;

      ::create_thread(NULL, 0, ::install::installer::thread_proc_run, (LPVOID) pinstaller, 0, 0);

   }


   void installer::synch_starter_start()
   {

      m_bStarterStart = true;

      run();

   }


   void installer::start_starter_start()
   {

      m_bStarterStart = true;

      ::create_thread(NULL, 0, ::install::installer::thread_proc_run, (LPVOID) this, 0, 0);

   }



   void installer::add_spa_start(const char * pszId)
   {
      string strPath = dir::appdata(process_platform_dir_name2())/"spa_start.xml";
      string strContents = file_as_string_dup(strPath);
      ::xml::document node(get_app());
      node.load(strContents);
      node.set_name("spa");
      ::xml::node * lpnode = node.GetChildByAttr("start", "id", pszId);
      if(lpnode == NULL)
      {
         lpnode = node.add_child("start");
         lpnode->add_attr("id", pszId);
         file_put_contents_dup(strPath, node.get_xml());
      }
   }

   void installer::remove_spa_start(const char * pszId)
   {
      string strPath = dir::appdata(process_platform_dir_name2())/ "spa_start.xml";
      string strContents = file_as_string_dup(strPath);
      ::xml::document node(get_app());
      node.load(strContents);
      node.set_name("spa");
      ::xml::node * lpnode = node.GetChildByAttr("start", "id", pszId);
      if(lpnode != NULL)
      {
         node.remove_child(lpnode);
         file_put_contents_dup(strPath, node.get_xml());
      }
   }



   bool installer::m_reboot()
   {
#ifdef WINDOWSEX
      HANDLE hToken;
      TOKEN_PRIVILEGES tkp;
      if (!OpenProcessToken(GetCurrentProcess(),
         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
         return false;
      LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
      if (ExitWindowsEx(EWX_REBOOT, 0) == 0)
         return false;
      //reset the previlages
      tkp.Privileges[0].Attributes = 0;
      AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES) NULL, 0);
#else
      throw "TODO?";
#endif
      return true;
   }






   int32_t installer::run_ca2_application_installer(const char * pszCommandLine)
   {

      string strPath;

#if defined(METROWIN)

      throw "todo";

#else

      string param;

      param = "-install:";

      param += pszCommandLine;

#if defined(WINDOWS)

      strPath = System.install().app_install_get_intern_executable_path(m_strVersion, m_strBuild);

#elif defined(APPLEOS)

      char path[MAXPATHLEN];

      uint32_t path_len = MAXPATHLEN;

      _NSGetExecutablePath(path, &path_len);

      strPath - path;


#elif defined(ANDROID)

      char * psz = NULL;

#else

      strPath = br_find_exe("app.install");


#endif

      call_sync(strPath, param, 0, SW_HIDE, -1, 84, 0, 0);

#endif

      return 0;

   }

#ifdef WINDOWSEX

   void get_program_files_x86(wstring &wstr)
   {

      unichar * lpszModuleFolder = (unichar *)malloc(MAX_PATH * sizeof(unichar) * 8);

      unichar * lpszModuleFilePath = (unichar *)malloc(MAX_PATH * sizeof(unichar) * 8);

      wcscpy(lpszModuleFilePath,_wgetenv(L"PROGRAMFILES(X86)"));

      if(wcslen(lpszModuleFilePath) == 0)
      {

         SHGetSpecialFolderPathW(
            NULL,
            lpszModuleFilePath,
            CSIDL_PROGRAM_FILES,
            FALSE);

      }

      if(lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '\\'
         || lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] == '/')
      {
         lpszModuleFilePath[wcslen(lpszModuleFilePath) - 1] = '\0';
      }

      wcscpy(lpszModuleFolder,lpszModuleFilePath);

      wstr = lpszModuleFolder + wstr;

   }


#endif


   int32_t installer::start_ca2_application()
   {

   #ifdef WINDOWS

      wstring wstrApp;

      wstrApp = L"\\ca2\\";
#ifdef _M_X64
      wstrApp += L"stage\\x64\\";
#else
      wstrApp += L"stage\\x86\\";
#endif

      wstrApp += L"app.exe";

#ifndef METROWIN

      get_program_files_x86(wstrApp);

      STARTUPINFOW si;
      memset(&si,0,sizeof(si));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_SHOWNORMAL;
      PROCESS_INFORMATION pi;
      memset(&pi,0,sizeof(pi));

      // enable_desktop_launch is signaled here to ease the application to know that it is a desktop application, so when it reissues
      // a installation it can send app.install.exe a message with enable_desktop_launch set too.
      wstring wstrCmdLine = (L"\"" + wstrApp + L"\" : app=" + wstring(m_strApplicationId) + L" build_number=installed enable_desktop_launch").c_str();

      if(::CreateProcessW((unichar *)wstrApp.c_str(),(unichar *)wstrCmdLine.c_str(),
         NULL,NULL,FALSE,0,NULL,NULL,
         &si,&pi))
         return TRUE;

      return FALSE;

      #endif


#endif

      return 0;


   }
   typedef bool fn_defer_core_init();

   typedef bool fn_defer_core_term();



   int32_t installer::ca2_app_install_run(const char * pszCommandLine, uint32_t & dwStartError, bool bSynch)
   {

#if defined(METROWIN)

      throw "todo";

#else

      string strPlatform = System.install().get_platform();

#ifdef WINDOWS

      //::SetDllDirectory(dir::path(dir::element(), "stage\\" + strPlatform));

      ::SetDllDirectory(dir::stage(process_platform_dir_name2()));

#endif

      ::aura::library libraryCore(get_app());

      string strCore = "core";

      // load core library so that a core system is alloced
      libraryCore.open(dir::stage(process_platform_dir_name2()) / strCore);

      fn_defer_core_init * pfn_core_init = libraryCore.get< fn_defer_core_init *>("defer_" + strCore + "_init");

      fn_defer_core_term * pfn_core_term = libraryCore.get< fn_defer_core_term *>("defer_" + strCore + "_term");

      ::aura::library libraryOs(get_app());

      libraryOs.open(dir::stage(process_platform_dir_name2()) / "app_core");

      PFN_APP_CORE_MAIN pfn_app_core_main = (PFN_APP_CORE_MAIN)libraryOs.raw_get("app_core_main");

      string strFullCommandLine;

      strFullCommandLine = ::path::app(process_platform_dir_name2());

      strFullCommandLine = "\"" + strFullCommandLine + "\" ";

      strFullCommandLine = strFullCommandLine + pszCommandLine;

      strFullCommandLine += " install";

      if(!pfn_core_init())
         return -1;

      app_core appcore;

#ifdef WINDOWS

      pfn_app_core_main(::GetModuleHandleA(NULL),NULL,strFullCommandLine,SW_HIDE, appcore);

#else

      pfn_app_core_main(strFullCommandLine, SW_HIDE, appcore);

#endif

      pfn_core_term();

#endif

      return 0;

   }


   int32_t installer::install_synch(const char * pszCommandLine)
   {

      m_bStarterStart = true;

      m_bSynch = true;

      return spaadmin_main(pszCommandLine);

   }


   int32_t installer::install_asynch(const char * pszCommandLine)
   {


      m_bStarterStart = true;

      m_bSynch = false;

      return spaadmin_main(pszCommandLine);

   }


   void installer::on_http_complete(::sockets::http_socket * psocket, ::http::e_status estatus)
   {

      if (estatus != ::http::status_ok)
      {

         m_straHttpFailure.add(psocket->m_strUrl);

         System.install().trace().rich_trace("Failure downloading : " + psocket->m_strUrl);

      }

   }


   void installer::on_set_scalar(int_scalar_source * psource,e_scalar escalar,int64_t iValue,int iFlags)
   {

      if (escalar == scalar_download_size)
      {

         if(m_bProgressModeAppInstall)
         {

            int64_t iMax = 0;

            psource->get_scalar_maximum(escalar, iMax);

            if(iMax == 0)
            {

               iMax = iValue;

            }

            if(iMax == 0)
            {

               iValue = 0;

               iMax = 1;

            }

            synch_lock sl(&m_mutexOmp);

            int iTest = 0;

            m_daProgress.element_at_grow(iTest) = (double)iValue / (double)iMax;

            double dTotal = m_daProgress.get_total();

            dTotal += m_dAppInstallProgressBase;

            set_progress(dTotal / m_dAppInstallFileCount );

         }
         else
         {

            synch_lock sl(&m_mutexOmp);

            m_iaProgress.element_at_grow(0) = iValue;

            set_progress((double)(m_iGzLen2 + m_iaProgress.get_total()) / (double)m_iProgressTotalGzLen2);

         }

      }

   }

   /*
   void installer::ms_download_callback(int32_t i, uint_ptr dwLen)
   {
      bool bProgress = true;
      if (i == -1)
      {
         m_dwDownloadLen = 0;

         if (bProgress)
         {
            //System.install().trace().rich_trace("...");
         }

      }
      else if (i == 0)
      {
         m_dwDownloadCallbackLast100k = 0;
         m_iDownloadCallbackColumn = 0;
      }
      else if (i == 1)
      {
         m_dwDownloadLen = dwLen;
         /*if(bProgress)
         {
         for(int32_t i = 0; i < ((dwLen - m_dwDownloadCallbackLast100k) / (100 * 1024)); i++)
         {
         if(m_iDownloadCallbackColumn >= 84)
         {
         System.install().trace().rich_trace(".");
         m_iDownloadCallbackColumn = 1;
         }
         else
         {
         m_iDownloadCallbackColumn++;
         System.install().trace().trace_add(".");
         }
         }
         }
         m_dwDownloadCallbackLast100k += ((dwLen - m_dwDownloadCallbackLast100k) / (100 * 1024) )*(100 / 1024);*/
   /*      if (m_iProgressTotalGzLen > 0)
         {
            dlr(m_iGzLen + dwLen);
            set_progress((double)(m_iGzLen + dwLen) / (double)m_iProgressTotalGzLen);
         }
      }
      else if (i == 2)
      {
         while (bProgress && m_iDownloadCallbackColumn < 84)
         {
            //System.install().trace().trace_add(".");
            m_iDownloadCallbackColumn++;
         }

      }
   }
   */

//   void installer::ms_get_callback(int32_t i, uint_ptr dwLen)
//   {
//      //      bool bProgress = true;
//      if (i == -3)
//      {
//         //       uint32_t dwBufferLen = 1024 + 256;
//         //         simple_http_status * pstatus = (simple_http_status *) reinterpret_cast < void * > (dwLen);
//         //       System.install().trace().rich_trace("Application.http().get failed : status : " + itoa_dup(pstatus->m_dwStatusCode) + " - " + pstatus->m_pszStatus);
//      }
//      else if (i == -2)
//      {
//         System.install().trace().rich_trace("Application.http().get failed");
//#ifdef WINDOWS
//         uint32_t dw = ::GetLastError();
//         unichar lastErrorTxt[1024];
//         memset_dup(lastErrorTxt, 0, sizeof(lastErrorTxt));
//         FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, 0, lastErrorTxt, 1024, NULL);
//         System.install().trace().rich_trace().print_ln(string(lastErrorTxt));
//         System.install().trace().rich_trace().print_ln("Application.http().get failed : GetLastErrorCode : " + itoa_dup(dw));
//#endif
//      }
//      else
//      {
//         ms_download_callback(i, dwLen);
//      }
//   }
//
//

   string installer::http_get(const string & strUrl, bool bScalarListener)
   {

      ::property_set set(get_app());

      if(bScalarListener)
      {

         set["int_scalar_source_listener"] = this;

      }

      string str;

      set["get_response"].get_value() = &str;

      set["disable_ca2_sessid"] = true;

      set["raw_http"] = true;

      if(!System.http().request(*m_psockethandler, m_phttpsession,strUrl,set))
      {

         return "";

      }

      return str;

   }


} // namespace install








