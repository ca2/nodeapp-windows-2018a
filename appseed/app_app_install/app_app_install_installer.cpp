#include "framework.h"
#include <stdio.h>
#include <Shlobj.h>


namespace app_app_install
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
      m_dAnime = 0.0;
      m_pwindow = NULL;
      m_dProgressStart = 0.0;
      m_dProgressEnd = 0.0;
      m_bInstallSet = false;
      m_dProgress = -1.0;
      m_dProgress1 = -1.0;
      m_dProgress2 = -1.0;
      m_oswindow = NULL;
      m_strPlat = process_platform_dir_name();
      m_strPlat2 = process_platform_dir_name2();

#if CA2_PLATFORM_VERSION == CA2_BASIS

      m_strVersion = "basis";

#else

      m_strVersion = "stage";

#endif

      m_pathBaseUrl = "http://server.ca2.cc/ccvotagus/" + m_strVersion + "/";

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


   int installer::install(string strCommand)
   {

      System.install().trace().ensure_trace_file();

      if (!m_machineevent.initialize())
      {

         return -5;

      }

      string strId;

      get_command_line_param(strId, strCommand, "app");

      if (strId.is_empty())
      {

         return -1;

      }

      string strLocale;

      get_command_line_param(strLocale, strCommand, "locale", "_std");

      string strSchema;

      get_command_line_param(strSchema, strCommand, "schema", strLocale);

      string strValue;


      if (System.install().is_installing_ca2())
      {

         return -1;

      }

      mutex mutexInstallingCa2(get_app(), false, "Global\\::ca::fontopus::ca2_spaboot_install_" + process_platform_dir_name2() + "::7807e510-5579-11dd-ae16-0800200c7784");

      if (!mutexInstallingCa2.lock())
      {

         return -1;

      }

      m_strCommand = strCommand;

      m_strApplicationId = strId;

      m_strLocale = strLocale;

      m_strSchema = strSchema;

      m_dProgressStart = 0.0;

      m_dProgressEnd = 0.0;

      System.install().trace().rich_trace("#----------");
      System.install().trace().rich_trace(":::::Thank you");
      System.install().trace().rich_trace("***Thank you");
      System.install().trace().rich_trace("Thank you");

      string_to_string mapMd5;

      string_to_intptr mapLen;

      new_progress_end(0.01);

      set_progress(0.001);

      m_bProgressModeAppInstall = true;

      string strVersion(m_strVersion);

      if (strVersion.is_empty())
      {

#if CA2_PLATFORM_VERSION == CA2_BASIS

         strVersion = "basis";

#else

         strVersion = "stage";

#endif

      }

      System.install().m_strVersion = strVersion;

      m_strSpaIgnitionBaseUrl = "http://spaignition.ca2.cc/api/spaignition";

      string strSpaHost;

      int32_t iHostRetry = 0;

      stringa straMd5;

      int_array iaLen;

      int32_t iRet = ca2_build_version_etc(strSpaHost, iHostRetry, straMd5, iaLen);

      if (iRet < 0)
      {

         return iRet;

      }

      stringa straMd5AppInstall;

      straMd5.slice(straMd5AppInstall, 0, straMd5.get_count() - 1);

      string strIndexMd5 = straMd5.last();

      set_progress(1.0);

      new_progress_end(0.02);

      System.install().app_install_get_extern_executable_path(m_strVersion, m_strBuild, &straMd5AppInstall, &iaLen, &mapMd5, &mapLen); // defer install install extern app_app_install.exe executable

      m_bProgressModeAppInstall = false;

      new_progress_end(0.1);

      m_dwInstallStartTime = ::get_tick_count();

   install_begin:;

   {

      m_strLastHost = "";

      m_strSpa.remove_all();

      m_iTotalGzLen2 = 0;

      m_iProgressTotalGzLen2 = 0;

      m_NeedRestartBecauseOfReservedFile = false;

      m_NeedRestartFatalError = false;

      int32_t iRet = 0;

      set_progress(0.2);

   retry_host:

      if (iHostRetry > 0)
      {

         iRet = calc_host(strSpaHost, iHostRetry);

         if (iRet < 0)
         {

            return iRet;

         }

      }

      m_strCurrentHost = strSpaHost;

      System.install().trace().rich_trace(("got server: " + strSpaHost));

      string strBuild(m_strBuild);

      strBuild.replace(" ", "_");

      strBuild.replace(":", "-");

      System.install().trace().rich_trace(strSpaHost);

      set_progress(0.3);

      string path;

      LPSTR lpsz = path.GetBufferSetLength(1024 * 4);

      ::GetModuleFileNameA(NULL, (char *)(const char *)path, 1024 * 4);

      path.ReleaseBuffer();

      string strFile;

      string strUrl;

      set_progress(0.4);

      ::file::patha straFileList;

      string_to_intptr mapGzLen;

      string_to_intptr mapFlag;

      if (iHostRetry > 0)
      {

         strUrl = "http://" + strSpaHost + "/ccvotagus/" + m_strVersion + "/app/stage/metastage/index-" + strBuild + ".md5";

         strIndexMd5 = http_get(strUrl, false);

      }

      ::xml::document nodeInstall(get_app());

      nodeInstall.load(file_as_string_dup(dir::appdata(process_platform_dir_name2()) / "install.xml"));

      ::xml::node * lpnodeVersion = nodeInstall.get_child(m_strVersion);

      System.install().trace().rich_trace("***Downloading file list.");

      string strIndexPath;

      bool bOkIndexPath = false;

      if (!ca2_fy_url(strIndexPath, ("app/stage/metastage/index-" + strBuild + ".spa.bz"), true, -1, strIndexMd5, -1, true, &bOkIndexPath))
      {

         System.install().trace().rich_trace("Failed to download file list!");

         System.install().trace().rich_trace("Going to retry host...");

         Sleep(500);

         goto retry_host;

      }

      if (iHostRetry > 0 || !bOkIndexPath)
      {

         if (strIndexMd5.length() != 32 || stricmp_dup(System.file().md5(strIndexPath), strIndexMd5) != 0)
         {

            System.install().trace().rich_trace("Invalid file list!");

            System.install().trace().rich_trace("Going to retry host...");

            Sleep(500);

            goto retry_host;

         }

      }

      set_progress(0.5);

      parse_index_file(strIndexPath, mapLen, mapMd5, mapGzLen, mapFlag);

      set_progress(0.6);

      if (iHostRetry > 0)
      {

         appmatter_list();

      }

      set_progress(0.7);

      if (!get_file_list(straFileList, ("app/stage/metastage/" + m_strApplicationId + ".spa"), mapLen, mapGzLen, mapMd5, mapFlag))
      {

         System.install().trace().rich_trace("Failed to download file list!");

         System.install().trace().rich_trace("Going to retry host...");

         Sleep(500);

         goto retry_host;

      }

      set_progress(0.8);

      for (int32_t i = 0; i < straFileList.get_count(); i++)
      {

         const char * psz = straFileList[i];

         output_debug_string(psz);

         output_debug_string("\r\n");

      }

      if (lpnodeVersion != NULL)
      {

         for (int32_t ui = 0; ui < lpnodeVersion->get_children_count(); ui++)
         {

            ::xml::node * lpnodeInstalled = lpnodeVersion->child_at(ui);

            if (!strcmp(lpnodeVersion->get_name(), "installed"))
            {

               for (int32_t ui = 0; ui < lpnodeInstalled->get_children_count(); ui++)
               {

                  ::xml::node * lpnodeType = lpnodeInstalled->child_at(ui);

                  for (int32_t ui = 0; ui < lpnodeType->get_children_count(); ui++)
                  {

                     string strId = lpnodeType->child_at(ui)->attr("id");

                     if (strcmp_dup(strId, m_strApplicationId) != 0)
                     {

                        get_file_list(straFileList, ("app/stage/metastage/" + strId + ".spa"), mapLen, mapGzLen, mapMd5, mapFlag);

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

         for (int32_t i = 0; i < patha.get_count(); i++)
         {

            System.install().trace().rich_trace(patha[i]);

         }

         get_file_list_ex(straFileList, patha, mapLen, mapGzLen, mapMd5, mapFlag);

      }

      for (int32_t i = 0; i < straFileList.get_count(); i++)
      {

         ::OutputDebugStringA(straFileList[i]);

         ::OutputDebugStringA("\r\n");

      }

      set_progress(0.9);

      m_straTerminateProcesses.remove_all();

      dword_array dwaPid;

      shared_library_process(dwaPid, m_straTerminateProcesses, dir::stage(process_platform_dir_name()) / "aura.dll");

      m_straRestartCommandLine.remove_all();

      m_straRestartProcess.remove_all();

      if (m_straTerminateProcesses.get_count() > 0)
      {

         string strCommand;

         bool bAsk;

         bool bRestart;

         for (int32_t i = 0; i < m_straTerminateProcesses.get_count(); i++)
         {

            if (i >= 1)
            {

               strCommand += ";";

            }

            bAsk = true;

            bRestart = true;

            string strPath = m_straTerminateProcesses.element_at(i);

            if (str_ends_ci_dup(strPath, "\\app_app_install.exe") && stristr_dup(strPath, "\\core\\") != NULL)
            {

               bAsk = false;

               bRestart = false;

            }
            else if (str_ends_ci_dup(strPath, ".exe") && stristr_dup(strPath, "\\app_app_install.exe") != NULL)
            {

               bAsk = false;

               bRestart = false;

            }
            else if (file_is_equal_path(strPath, file_get_mozilla_firefox_plugin_container_path()))
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

            if (!bAsk || ::simple_message_box(NULL, str, "need to terminate process to install", MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
            {

               HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_TERMINATE | PROCESS_VM_READ, FALSE, dwaPid[i]);

               if (hProcess == NULL)
               {

                  ::CloseHandle(hProcess);

                  str = "Failed to open process for termination - ";

                  str += m_straTerminateProcesses.element_at(i);

                  str += ".";

                  ::simple_message_box(NULL, "Failed to open process for termination", str, MB_OK);

                  return -1;

               }

               if (bRestart)
               {

                  m_straRestartCommandLine.add(get_command_line(hProcess));

                  m_straRestartProcess.add(m_straTerminateProcesses.element_at(i));

               }

               TerminateProcess(hProcess, -1);

               int32_t iRetry = 49;

               DWORD dwExitCode = (uint32_t)-1;

               while (::GetExitCodeProcess(hProcess, &dwExitCode) && dwExitCode == STILL_ACTIVE && iRetry > 0)
               {

                  Sleep(184);

                  iRetry--;

               }

               if (iRetry <= 0)
               {

                  ::CloseHandle(hProcess);

                  str = "Timeout while waiting for process - ";

                  str += m_straTerminateProcesses.element_at(i);

                  str += " - to exit.";

                  ::simple_message_box(NULL, "Failed to open process for termination", str, MB_OK);

                  return -1;

               }

               ::CloseHandle(hProcess);

            }
            else
            {

               return -1;

            }

         }

      }

      if (m_straRestartCommandLine.get_count() > 0)
      {

         oswindow oswindowSpaBoot = ::FindWindow(NULL, "::core::spaboot:callback_window");

         if (oswindowSpaBoot != NULL)
         {

            string str = m_straRestartCommandLine.encode_v16();

            COPYDATASTRUCT cds;

            memset_dup(&cds, 0, sizeof(cds));

            cds.dwData = 15111984;

            cds.cbData = (uint32_t)str.length();

            cds.lpData = (PVOID)(const char *)str;

            ::SendMessageA(oswindowSpaBoot, WM_COPYDATA, (WPARAM)0, (LPARAM)&cds);

         }

      }

      new_progress_end(0.49);

      if (file_exists_dup(::dir::system() / "config\\plugin\\do_not_download_file_list.txt"))
      {

         Sleep(15000);

      }
      else
      {

         download_file_list(straFileList, mapLen, mapMd5, mapGzLen, mapFlag);

      }

      System.install().set_ca2_updated(m_strBuild);

      new_progress_end(1.0);

      int iRetry = 0;

      while ((!machine_unsignalize_close_application() || machine_check_close_application(true)) && iRetry < 840)
      {

         if (iRetry == 0)
         {

            System.install().trace().rich_trace("Enabling applications to run.");

         }

         iRetry++;

         Sleep(184);

         if ((iRetry % 33) == 0)
         {

            System.install().trace().rich_trace(".");

         }
         else
         {

            System.install().trace().trace_add(".");

         }

      }

      System.install().trace().rich_trace("***Installing application.");

      if (m_NeedRestartBecauseOfReservedFile || m_NeedRestartFatalError)
      {

         add_spa_start(strCommand);

         if (::simple_message_box(NULL, "The computer need to be restarted!!\n\nDo you want to restart now?\n\nWe recommend you to close all other applications first and then agree with this question using the buttons below.", "spa - Restart Needed!!", MB_ICONEXCLAMATION | MB_YESNO)
            == IDYES)
         {

            reboot();

         }

      }
      //else
      //{

      //   //if (!launch_applications())
      //   //{

      //   //   System.install().trace().rich_trace("***Going to ebing!!");

      //   //   Sleep(2000);

      //   //   goto install_begin;

      //   //}

      //}

   }

   return 0;

   }


   void installer::set_progress(double dProgress)
   {

      if (dProgress < 0.0)
      {

         dProgress = 0.0;

      }
      else if (dProgress > 1.0)
      {

         dProgress = 1.0;

      }

      dProgress = m_dProgressStart + (m_dProgressEnd - m_dProgressStart) * dProgress;

      System.install().trace().trace_progress(dProgress);

   }


   ::count installer::download_file_list(::file::patha & stringa, string_to_intptr & mapLen, string_to_string & mapMd5, string_to_intptr & mapGzLen, string_to_intptr & mapFlag)
   {

      ::stringa straExpandFileSet;

      m_iGzLen2 = 0;

      m_dProgress = 0.0;

      m_iProgressTotalGzLen2 = 0;

      System.install().trace().rich_trace("***Downloading resource files.");

      {

         stringa.filter_out(
            [&](const string & strCurrent)
         {

            if (::str::ends_ci(strCurrent, ".expand_fileset"))
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

         m_iGzLen2 = 0;

         dir::afterca2() / "time/bz";

         for (auto strCurrent : straExpandFileSet)
         {

            ::file::path str = m_pathBaseUrl;

            str /= strCurrent;

            string str2 = dir::name(str);

            ::str::begins_eat_ci(str2, m_pathBaseUrl);

            str2.replace("\\", "/");

            str += ".bz";

            string str3 = str;

            string strMd5 = mapMd5[strCurrent];

            int iLen = mapLen[strCurrent];

            int iGzLen = mapGzLen[strCurrent];

            str += ".";

            str += strMd5;

            ::file::path strStageGz = ca2bz_get_dir(strCurrent) / ca2bz_get_file(strCurrent, strMd5);

            ::file::path strStageInplaceFile = ca2inplace_get_file(strCurrent);

            bool bDownload = true;

            System.install().trace().rich_trace(str::replace("\\", "/", file_name_dup((str2 + str))));

            ::file::path strStageInplace = ca2bz_get_dir(strCurrent) / strStageInplaceFile;

            if (file_exists_dup(strStageInplace)
               && (iLen != -1) && file_length_dup(strStageInplace) == iLen
               && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace), strMd5) == 0)
            {

               bDownload = false;

            }

            if (bDownload && file_exists_dup(strStageGz))
            {

               dir::mk(dir::name(strStageInplace));

               System.compress().unbz(get_app(), strStageInplace, strStageGz);

               if (file_exists_dup(strStageInplace)
                  && (iLen != -1) && file_length_dup(strStageInplace) == iLen
                  && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace), strMd5) == 0)
                  bDownload = false;

            }

            if (bDownload && download_file(strStageInplace, str3, false, false, iLen, strMd5, iGzLen, mapFlag[strCurrent]))
            {

               m_dProgress = m_dProgress2;

               string strRelative = dir::name(strCurrent) / file_title_dup(strCurrent);

               string strStageInplace2 = ca2inplace_get_dir(strRelative) / ca2inplace_get_file(strRelative);

               System.file().ftd(strStageInplace2, strStageInplace, get_app());

            }

            string strExpand = strCurrent;

            strExpand += ".spa";

            strExpand = "app\\stage\\metastage\\" + strExpand;

            int64_t iPreviousTotalProgress = m_iProgressTotalGzLen2; // keep progress rate total calculator

            m_iProgressTotalGzLen2 = 0; // avoid progress rate change

            get_file_list(stringa, strExpand, mapLen, mapGzLen, mapMd5, mapFlag);

            m_iProgressTotalGzLen2 = iPreviousTotalProgress; // restore progress rate total calculator

            m_iGzLen2 += iGzLen;

            set_progress((double)m_iGzLen2 / (double)m_iProgressTotalGzLen2);

         }

         new_progress_end(0.84);

         System.install().trace().rich_trace("***Downloading files.");

         m_iGzLen2 = 0;

         m_iProgressTotalGzLen2 = m_iTotalGzLen2;

         for (int i = 0; i < stringa.get_count(); i++)
         {

            single_lock sl(&m_mutexOmp);

            string strFileName;

            string strCurrent = stringa[i];

            strCurrent.trim();

            if (strCurrent.is_empty())
            {

               continue;

            }

            int iGzLen = mapGzLen[strCurrent];

            ::file::path str = m_pathBaseUrl;

            str /= strCurrent;

            string str2 = dir::name(str);

            strFileName = file_name_dup(strCurrent);

            ::str::begins_eat_ci(str2, m_pathBaseUrl);

            str += ".bz";

            string str3 = str;

            string strMd5 = mapMd5[strCurrent];

            int iLen = mapLen[strCurrent];

            str += ".";

            str += strMd5;

            ::file::path strStageGz = ca2bz_get_dir(strCurrent) / ca2bz_get_file(strCurrent, strMd5);

            ::file::path strStageInplaceFile = ca2inplace_get_file(strCurrent);

            bool bDownload = true;

            ::file::path strStageInplace = ca2inplace_get_dir(strCurrent) / ca2inplace_get_file(strCurrent);

            if (file_exists_dup(strStageInplace)
               && (iLen != -1) && file_length_dup(strStageInplace) == iLen
               && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace), strMd5) == 0)
            {

               bDownload = false;

            }

            if (bDownload && (::str::ends_ci(strFileName, ".exe") || ::str::ends_ci(strFileName, ".dll")))
            {

               string strCandidate = ::dir::app_install(System.install().get_platform()) / strFileName;

               if (file_exists_dup(strCandidate)
                  && iLen != -1 && file_length_dup(strCandidate) == iLen
                  && strMd5.has_char() && stricmp_dup(System.file().md5(strCandidate), strMd5) == 0)
               {

                  bDownload = !::file_copy_dup(strStageInplace, strCandidate, false);

               }

            }

            if (bDownload && file_exists_dup(strStageGz))
            {

               dir::mk(dir::name(strStageInplace));

               System.compress().unbz(get_app(), strStageInplace, strStageGz);

               if (file_exists_dup(strStageInplace)
                  && (iLen != -1) && file_length_dup(strStageInplace) == iLen
                  && strMd5.has_char() && stricmp_dup(System.file().md5(strStageInplace), strMd5) == 0)
                  bDownload = false;

            }

            if (bDownload)
            {

               if (download_file(strStageInplace, str3, false, false, iLen, strMd5, iGzLen, mapFlag[stringa[i]]))
               {

                  m_dProgress = m_dProgress2;

               }

            }

            sl.lock();

            if (bDownload)
            {

               System.install().trace().rich_trace(str::replace(file_name_dup((str2 + str)), "\\", "/"));

            }

            m_iaProgress.element_at_grow(0) = 0;

            m_iGzLen2 += iGzLen;

            set_progress((double)m_iGzLen2 / (double)m_iProgressTotalGzLen2);

            sl.unlock();

         }

      }

      return stringa.get_count();

   }


   bool installer::get(const string& url_in, bool bExist, int64_t iLength, const char * pszMd5, int64_t iGzLen)
   {

      string url = url_in;

      string file;

      string file2;

      string dir;

      string dir2;

      dir = dir::element();

      if (dir.substr(dir.size() - 1, 1) != "\\")
      {

         dir += "\\";

      }

      dir2 = dir;

      dir += "time\\bz\\";

      {

         index pos = url_in.find(m_pathBaseUrl);

         if (pos == 0)
         {

            url = url_in.substr(m_pathBaseUrl.length());

         }

         index oldpos = -1;

         pos = url.find("/");

         string lastfile;

         while (pos >= 0)
         {

            file = url.substr(oldpos + 1, pos - oldpos - 1);

            if (lastfile.size() > 0)
            {

               dir += lastfile + "\\";

            }

            lastfile = file;

            oldpos = pos;

            pos = url.find("/", oldpos + 1);

         }

         file = url.substr(oldpos + 1);

         if (lastfile.size() > 0)
         {

            dir += lastfile + "\\";

         }

      }

      if (stricmp_dup(file.substr(file.length() - 3, 3), ".bz") == 0)
      {

         file2 = file.substr(0, file.length() - 3);

      }
      else
      {

         file2 = file;

      }

      dir::mk(dir);

      if (bExist)
      {

         if (file_exists_dup(dir2 + file2))
         {

            if (iLength == -1 || iLength == file_length_dup((dir2 + file2)))
            {

               if (pszMd5 != NULL && strlen_dup(pszMd5) > 0 && stricmp_dup(System.file().md5((dir2 + file2)), pszMd5) == 0)
               {

                  return true;

               }

            }

         }

      }

      dir::mk(dir::name((dir + file)));

      m_dwCurFileLen = iGzLen;

      m_dwDownloadLen = 0;

      string strUrl = url_in;

      property_set set;

      set["disable_ca2_sessid"] = true;

      set["raw_http"] = true;

      single_lock sl(&m_mutexOmp);

      sl.lock();

      sp(::sockets::http_session) & psession = m_httpsessionptra.element_at_grow(0);

      sl.unlock();

      System.install().trace().trace_add("\ndownloading " + strUrl + "\n");

      return Application.http().download(*m_psockethandler, psession, strUrl, (dir + file), set);

   }


   bool installer::download_file(const string& inplaceParam, const string& url_in, bool bExist, bool bCheck, int64_t iLength, const char * pszMd5, int64_t iGzLen, int_ptr & iFlag)
   {

      single_lock sl(&m_mutexOmp);

      string url = url_in;

      string file;

      string file2;

      string dir;

      string dir2;

      string dir3;

      dir = dir::element();

      if (dir.substr(dir.size() - 1, 1) != "\\")
      {

         dir += "\\";

      }

      dir3 = dir + "time\\patchwork\\";

      dir2 = dir;

      dir += "time\\bz\\";

      {

         index pos = url_in.find(m_pathBaseUrl);

         if (pos == 0)
         {

            url = url_in.substr(m_pathBaseUrl.length());

         }

         index oldpos = -1;

         pos = url.find("/");

         string lastfile;

         while (pos >= 0)
         {

            file = url.substr(oldpos + 1, pos - oldpos - 1);

            if (lastfile.size() > 0)
            {

               dir += lastfile + "\\";

            }

            lastfile = file;

            oldpos = pos;

            pos = url.find("/", oldpos + 1);

         }

         file = url.substr(oldpos + 1);

         if (lastfile.size() > 0)
         {

            dir += lastfile + "\\";

         }

      }

      if (stricmp_dup(file.substr(file.length() - 3, 3), ".bz") == 0)
      {

         file2 = file.substr(0, file.length() - 3);

      }
      else
      {

         file2 = file;

      }

      dir::mk(dir);

      string inplace;

      if (inplaceParam.is_empty())
      {

         inplace = dir2 + file2;

      }
      else
      {

         inplace = inplaceParam;

      }

      // first check if the exiting (old file) is already up-to-date (the current one)
      if (bExist && bCheck)
      {

         if (file_exists_dup(inplace))
         {

            if (iLength != -1 && iLength == file_length_dup(inplace))
            {

               if (pszMd5 != NULL && strlen_dup(pszMd5) > 0 && stricmp_dup(System.file().md5(inplace), pszMd5) == 0)
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

      // then finally try to download the entire file

      sl.lock();

      sp(::sockets::http_session) & psession = m_httpsessionptra.element_at_grow(0);

      sl.unlock();

      if (!bOk)
      {

         dir::mk(dir::name((dir + file)));

         m_dwCurFileLen = iGzLen;

         m_dwDownloadLen = 0;

         System.install().trace().rich_trace(file2);

         int iRetry = 0;

         while (true)
         {

            property_set set;

            set["int_scalar_source_listener"] = this;

            set["disable_ca2_sessid"] = true;

            set["raw_http"] = true;

            System.install().trace().trace_add("\ndownloading " + (::file::path(url_in + "." + pszMd5)) + "\n");

            bOk = Application.http().download(*m_psockethandler, psession, ::file::path(url_in + "." + pszMd5), ::file::path(dir + file + "." + pszMd5), set);

            if (!bOk)
            {

               break;

            }

            if (bOk)
            {

               if (iLength != -1)
               {

                  dir::mk(dir::name(inplace));

                  int32_t iResult = System.compress().unbz(get_app(), inplace, (dir + file + "." + pszMd5));

                  if (iResult == -1)
                  {

                     m_NeedRestartBecauseOfReservedFile = true;

                  }
                  else if (iResult == -2)
                  {

                     m_NeedRestartFatalError = true;

                  }

               }

               bOk = iLength == -1 || iLength == file_length_dup(inplace);

               if (bOk)
               {

                  bOk = pszMd5 == NULL || strlen_dup(pszMd5) == 0 || stricmp_dup(System.file().md5(inplace), pszMd5) == 0;

                  if (bOk)
                  {

                     break;

                  }

               }

            }

            iRetry++;

            if (iRetry >= iMaxRetry)
            {

               break;

            }

         }

      }

      return bOk;

   }


   ::file::path installer::ca2_get_dir(LPCSTR lpcszUrl)
   {

      string url_in(lpcszUrl);

      ::file::path path;

      string url;

      string file;

      path = dir::element();

      ::str::begins_eat_ci(url_in, m_pathBaseUrl);

      path /= url_in;

      return path.folder();

   }


   ::file::path installer::ca2bz_get_dir(LPCSTR lpcszUrl)
   {

      return (::dir::afterca2() / "time/bz" / lpcszUrl).folder();

   }


   ::file::path installer::ca2unbz_get_dir(LPCSTR lpcszUrl)
   {

      string url_in(lpcszUrl);

      string dir;

      string url;

      string file;

      dir = dir::afterca2() / "time/unbz";

      ::str::begins_eat_ci(url_in, m_pathBaseUrl);

      return (dir / url_in).folder();

   }


   ::file::path installer::ca2inplace_get_dir(LPCSTR lpcszUrl)
   {

      string str(lpcszUrl);

      ::str::begins_eat_ci(str, m_pathBaseUrl);

      ::file::path path;

      path = ::dir::afterca2() / str;

      return path.folder();

   }


   ::file::path installer::ca2_get_file(LPCSTR lpcszUrl)
   {

      string str(lpcszUrl);

      ::str::begins_eat_ci(str, m_pathBaseUrl);

      ::file::path path(str);

      ::str::ends_eat_ci(path, ".bz");

      return path.name();

   }


   ::file::path installer::ca2bz_get_file(LPCSTR lpcszUrl, const char * pszMd5)
   {

      ::file::path file = ::file::path(lpcszUrl).name();

      if (pszMd5 != NULL)
      {

         if (::str::ends_ci(file, ".bz"))
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

         if (::str::ends_ci(file, ".bz"))
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

      string str(lpcszUrl);

      ::str::begins_eat_ci(str, m_pathBaseUrl);

      ::file::path path(str);

      ::str::ends_eat_ci(path, ".bz");

      return path.name();

   }


   ::file::path installer::ca2inplace_get_file(LPCSTR lpcszUrl)
   {

      string str(lpcszUrl);

      ::str::begins_eat_ci(str, m_pathBaseUrl);

      ::file::path path(str);

      ::str::ends_eat_ci(path, ".bz");

      return path.name();

   }


   bool installer::ca2_fy_url(string & str, LPCSTR lpcszPath, bool bExist, int64_t iLength, const char * pszMd5, int64_t iGzLen, bool bIndex, bool * pbFileOk)
   {

      ::file::path strStage;

      ::file::path strStageGz;

      string strUrl;

      strUrl = lpcszPath;

      strStage = ca2_get_dir(strUrl) / ca2_get_file(strUrl);

      if (bExist)
      {

         if (file_exists_dup(strStage))
         {

            if (iLength == -1 || iLength == file_length_dup((strStage)))
            {

               if (pszMd5 != NULL && strlen_dup(pszMd5) > 0 && stricmp_dup(System.file().md5((strStage)), pszMd5) == 0)
               {

                  str = strStage;

                  if (pbFileOk != NULL)
                  {

                     *pbFileOk = true;

                  }

                  return true;

               }

            }

         }

      }

      if (pbFileOk != NULL)
      {

         *pbFileOk = false;

      }

      if (bIndex)
      {

         if (!get(m_pathBaseUrl / lpcszPath, bExist, iLength, pszMd5, iGzLen))
         {

            printf_dup("Failed: %s\n", (const char *)strUrl);

            return false;

         }

         strStageGz = ca2bz_get_dir(strUrl) / ca2bz_get_file(strUrl, NULL);

      }
      else
      {

         if (!get(m_pathBaseUrl / lpcszPath + "." + pszMd5, bExist, iLength, pszMd5, iGzLen))
         {

            printf_dup("Failed: %s\n", (const char *)strUrl);

            return false;

         }

        strStageGz = ca2bz_get_dir(strUrl) / ca2bz_get_file(strUrl, pszMd5);

      }

      dir::mk(dir::name(strStage));

      if (Application.file().length(strStageGz) <= 0)
      {

         Application.file().touch(strStage);

      }
      else
      {

         System.compress().unbz(get_app(), strStage, strStageGz);

      }

      str = strStage;

      return true;

   }


   int32_t installer::get_file_list_ex(::file::patha & stringa, ::file::patha  & patha, string_to_intptr & mapLen, string_to_intptr & mapGzLen, string_to_string & mapMd5, string_to_intptr & mapFlag)
   {

      int32_t iCurrent;

      string strPlatform = System.install().get_platform();

      string strBinaryPlatform = time_binary_platform(strPlatform);

      string strVersion = m_strVersion;

      for (int32_t i = 0; i < patha.get_count(); i++)
      {

         string strPathParam = patha[i];

         if (strPathParam.begins_ci("stage\\basis\\"))
         {

            strPathParam = "time\\" + strBinaryPlatform + "\\" + strVersion + strPathParam.substr(11);

         }

         iCurrent = get_file_list(stringa, strPathParam, mapLen, mapGzLen, mapMd5, mapFlag);

         if (iCurrent == -2)
         {

            return -2;

         }
         else if (iCurrent == -1)
         {

            strPathParam.replace("/", "\\");

            if (stringa.add_unique(strPathParam) >= 0)
            {

               m_iTotalGzLen2 += mapGzLen[strPathParam];

            }

         }

      }

      return 1;

   }



   int32_t installer::get_file_list(::file::patha & stringa, LPCSTR lpcszPath, string_to_intptr & mapLen, string_to_intptr & mapGzLen, string_to_string & mapMd5, string_to_intptr & mapFlag)
   {

      string strPath(lpcszPath);

      strPath.replace("/", "\\");

      strPath = strPath;

      string strUrl(lpcszPath);

      if (strUrl.length() == 0)
      {

         return -1;

      }

      if (strUrl.ends_ci(".spa"))
      {

         if (m_strSpa.add_unique(strUrl) < 0)
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

      if (!ca2_fy_url(str, strUrl, true, -1, strMd5, -1))
      {

         return -2;

      }

      string strContents = Application.file().as_string(str);

      strContents.trim();

      if (strContents.is_empty())
      {

         return 1;

      }

      string strPlatform = System.install().get_platform();

      string strBinaryPlatform = time_binary_platform(strPlatform);

      string strVersion = m_strVersion;

      ::stringa straLines;

      straLines.add_lines(strContents);

      for(string & strLine : straLines)
      {

         string str2;

         string strPathParam(strLine);

         if (m_strVersion == "stage")
         {

            strPathParam = ::str::replace_ci("vcruntime140d.dll", "vcruntime140.dll", strPathParam);

            strPathParam = ::str::replace_ci("vcomp140d.dll", "vcomp140.dll", strPathParam);

         }

         if (::str::begins_eat(strPathParam, "[32]") && strBinaryPlatform != "Win32")
         {

            continue;

         }
         else if (::str::begins_eat(strPathParam, "[64]") && strBinaryPlatform != "x64")
         {

            continue;

         }

         if (strPathParam.begins_ci("stage\\basis\\"))
         {

            strPathParam = "time\\" + strBinaryPlatform + "\\" + strVersion + strPathParam.substr(11);

         }

         iCurrent = get_file_list(stringa, strPathParam, mapLen, mapGzLen, mapMd5, mapFlag);

         if (iCurrent == -2)
         {

            return -2;

         }
         else if (iCurrent == -1)
         {

            if (stringa.add_unique(strPathParam) >= 0)
            {

               m_iTotalGzLen2 += mapGzLen[strPathParam];

            }

         }
         else
         {

            iCount += iCurrent;

         }

      }

      return 1;

   }


   int32_t installer::get_local_file_list(::file::patha & stringa, LPCSTR lpcszUrl)
   {

      string strUrl(lpcszUrl);

      System.install().trace().rich_trace(strUrl);

      if (!strUrl.ends_ci(".spa"))
      {

         return -1;

      }

      int32_t iCount = 0;

      strUrl += ".bz";

      string strPath;

      if (!ca2_fy_url(strPath, strUrl, false, -1, NULL, -1))
      {

         return -2;

      }

      ::stringa straLines;

      file_load_stra(strPath, straLines, false);

      int32_t iCurrent;

      for (auto & strLine : straLines)
      {

         iCurrent = get_local_file_list(stringa, strLine);

         if (iCurrent == -1)
         {

            stringa.add_unique(strLine);

         }
         else
         {

            iCount += iCurrent;

         }

      }

      return iCount;

   }


   ::count installer::uncompress_file_list(::file::patha & stringa, string_to_string & strmapMd5)
   {

      string strStage;

      string strStageGz;

      string str;

      double d = 0.0;

      m_dProgress = 0.0;

      for (int32_t i = 0; i < stringa.get_count(); i++)
      {

         str = stringa[i];

         str += ".bz";

         System.install().trace().rich_trace(str);

         strStageGz = ca2bz_get_dir(str) / ca2bz_get_file(str, strmapMd5[stringa[i]]);

         strStage = ca2_get_dir(str) / ca2_get_file(str);

         dir::mk(dir::name(strStage));

         System.compress().unbz(get_app(), strStage, strStageGz);

         d += 1.0;

         m_dProgress = d / ((double)stringa.get_count());

      }

      m_dProgress = 1.0;

      return stringa.get_count();

   }


   ::count installer::copy_file_list(::file::patha & stringa, string_to_intptr & mapFlag)
   {

      string strStage;

      string strStageUnbz;

      string str;

      double d = 0.0;

      m_dProgress = 0.0;

      for (int32_t i = 0; i < stringa.get_count(); i++)
      {

         str = stringa[i];

         int_ptr iFlag = mapFlag[str];

         if ((iFlag & SPA_FILE_FLAG_VOID_COPY) != 0)
         {

            continue;

         }

         str += ".bz";

         System.install().trace().rich_trace(str);

         strStageUnbz = ca2unbz_get_dir(str) / ca2unbz_get_file(str);

         strStage = ca2_get_dir(str) / ca2_get_file(str);

         if (strStage.length() > strlen_dup(".expand_fileset") && stricmp_dup(strStage.substr(strStage.length() - strlen_dup(".expand_fileset"), strlen_dup(".expand_fileset")), ".expand_fileset") == 0)
         {

         }
         else
         {

            dir::mk(dir::name(strStage));

            file_copy_dup(strStage, strStageUnbz, true);

         }

         d += 1.0;

         m_dProgress = d / ((double)stringa.get_count());

         set_progress((double)i / (double)stringa.get_count());

      }

      m_dProgress = 1.0;

      return stringa.get_count();

   }




   string installer::load_string(const char * pszId, const char * pszDefault)
   {

      ::xml::node * lpnode = &m_xmldocStringTable;

      for (int32_t ui = 0; ui < lpnode->get_children_count(); ui++)
      {

         if (string(lpnode->child_at(ui)->attr("id")) == pszId)
         {

            return lpnode->child_at(ui)->get_value();

         }

      }

      return pszDefault;

   }


   void installer::parse_index_file(const char * psz, string_to_intptr & mapLen, string_to_string & mapMd5, string_to_intptr & mapGzLen, string_to_intptr & mapFlag)
   {

      ::file::file_sp file = Application.file().get_file(psz, ::file::mode_read | ::file::type_binary);

      memory mem;

      while (file->read_string(mem))
      {

         char * pszStart = (char *)mem.get_data();

         char * pszFind1 = next_separator_token(pszStart, ',');

         char * pszFind2 = next_separator_token(pszFind1, ',');

         char * pszFind3 = next_separator_token(pszFind2, ',');

         string strSpa(pszStart);

         if (strSpa.find_ci("user_service.spa") >= 0)
         {

            output_debug_string("strSpa.find_ci(\"user_service.spa\") >= 0");

         }

         if (pszFind1 != NULL)
         {
            m_iProgressMode = 0;
            mapLen[strSpa] = atoi_dup(pszFind1);
         }
         else
            mapLen[strSpa] = -1;
         if (pszFind2 != NULL)
         {
            mapMd5[strSpa] = pszFind2;
         }
         else
            mapMd5[strSpa] = "";
         if (pszFind3 != NULL)
         {
            mapGzLen[strSpa] = atoi_dup(pszFind3);
         }
         else
            mapGzLen[strSpa] = -1;
      }

   }


   bool installer::spa_exec(const char * psz)
   {
      string strExec(psz);
      if (strExec.substr(0, 15) == "install_service")
      {
         string strStage;
         strStage = dir::element() / strExec.substr(16);

#ifdef METROWIN

         throw "todo";

#else

         ::aura::shell_launcher launcher1(m_pwindow == NULL ? NULL : m_pwindow->get_safe_handle(), "open", strStage, " : remove usehostlogin", dir::name(strStage), SW_SHOWNORMAL);

         launcher1.execute();

         ::aura::shell_launcher launcher2(m_pwindow == NULL ? NULL : m_pwindow->get_safe_handle(), "open", strStage, " : install usehostlogin", dir::name(strStage), SW_SHOWNORMAL);

         launcher2.execute();

#endif

      }
      else if (strExec.substr(0, 10) == "install_ex")
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

      if (!txchannel.open("core/spaboot_install_callback"))
         return;

#endif

      ::PostMessage(txchannel.m_oswindow, WM_USER + 100, a, b);

   }


   //bool installer::launch_applications()
   //{

   //   System.install().trace().rich_trace("starting app_app_install.exe...");

   //   string strCommand = m_strCommand;

   //   int32_t i = run_ca2_application_installer(strCommand);

   //   System.install().trace().rich_trace(".");

   //   string strPlatform = System.install().get_platform();

   //   if (i == 0)
   //   {

   //      System.install().set_updated(m_strBuild);

   //      if (m_straRestartProcess.get_count() > 0)
   //      {

   //         string str;

   //         str = "Now :\n\n";

   //         for (int32_t i = 0; i < m_straRestartProcess.get_count(); i++)
   //         {
   //            if (file_is_equal_path(file_get_mozilla_firefox_plugin_container_path(), m_straRestartProcess[i]))
   //            {
   //               m_straRestartProcess.remove_at(i);
   //               str += "You may reload the Firefox plugin or plugins that has/have been shutdown.\n\n";
   //            }
   //         }

   //         if (m_straRestartProcess.get_count() > 0)
   //         {

   //            str += "You may restart the applications listed below if they are not restarted automatically:\n\n";

   //            for (int32_t i = 0; i < m_straRestartProcess.get_count(); i++)
   //            {

   //               str += "\t";

   //               str += m_straRestartProcess[i];

   //               if (i == m_straRestartProcess.get_count() - 1)
   //               {

   //                  str += ".";

   //               }
   //               else if (i == m_straRestartProcess.get_count() - 2)
   //               {

   //                  str += ", and;\n";

   //               }
   //               else
   //               {
   //                  str += ";\n";
   //               }

   //            }
   //         }

   //         ::simple_message_box(NULL, str, "You may restart the applications ...", MB_ICONINFORMATION | MB_OK);

   //      }

   //      System.install().trace().rich_trace(":::::Thank you");
   //      System.install().trace().rich_trace("***Thank you");
   //      System.install().trace().rich_trace("Thank you");
   //      System.install().trace().trace_progress(1.0);

   //      return true;

   //   }
   //   else
   //   {
   //      
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("Failed to install");
   //      System.install().trace().rich_trace("Manually contact mailto:support.veriwell@ca2mail.com");
   //      System.install().trace().rich_trace("or Manually go to http://ca2os.com/");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("In ca2os.com you can try to");
   //      System.install().trace().rich_trace("download the most recent release version for installation");
   //      System.install().trace().rich_trace("(Automatic links not availabe by now)");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      System.install().trace().rich_trace("");
   //      return false;
   //   }
   //}


   int32_t installer::appmatter_list()
   {

      string strUrl;

      System.install().trace().rich_trace(("get appmatter list for application with id : \"" + m_strApplicationId + "\" "));
      System.install().trace().rich_trace(("locale : \"" + m_strLocale + "\" "));
      System.install().trace().rich_trace(("schema : \"" + m_strSchema + "\" "));

      strUrl = m_strSpaIgnitionBaseUrl + "/appmatter_spa_list?app=";
      strUrl += m_strApplicationId;
      strUrl += "&locale=";
      strUrl += m_strLocale;
      strUrl += "&schema=";
      strUrl += m_strSchema;
      strUrl += "&version=";
      strUrl += m_strVersion;

      string str;
      int32_t iRetry = 0;
      while (true)
      {

         str = http_get(strUrl, false);

         if (str.length() > 0)
            break;
         else if (iRetry < 84)
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

      if (m_strBuildResource.length() == 19) // heuristically valid
      {
         
         System.install().trace().rich_trace("***Internal build number");

         m_strBuild = m_strBuildResource;

      }
      else
      {

         System.install().trace().rich_trace("***Getting build number");

         if (iRetry > 10)
         {

            System.install().trace().rich_trace("could not get build number. exiting");

            //            exit(-1);

            return -1;

         }

         iRetry++;

         m_strBuild = http_get(m_strSpaIgnitionBaseUrl + "/query?node=build&sessid=noauth&version=" + m_strVersion + "&platform=" + strPlatform, false);

         m_strBuild.trim();

         if (m_strBuild.length() != 19)
         {

            Sleep(184);

            goto RetryBuildNumber;

         }

      }

      System.install().trace().rich_trace(m_strBuild);

      return 0;

   }


   int32_t installer::ca2_build_version_etc(string & strSpaHost, int32_t &iHostRetry, stringa & straMd5, int_array & iaLen)
   {

      int32_t iRetry = 0;

      string strEtc;

      string strName;

      ::file::patha straTemplate;

      string strBuild;

      ::lemon::array::copy(straTemplate, ::install::get_app_app_install_module_list(m_strPlat2, m_strVersion));

      string strUrl;

      ::memory_file file(get_app());

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

         strIndexBuild.replace(" ", "_");

         strIndexBuild.replace(":", "-");

         strIndexPath = "app/stage/metastage/index-" + strIndexBuild + ".spa.bz";

         strIndexPath = ca2_get_dir(strIndexPath) / ca2_get_file(strIndexPath);

         if (file_exists_dup(strIndexPath))
         {

            strIndexMd5 = System.file().md5(strIndexPath);

         }

      }

      string strStatus;

   RetryBuildNumber:

      if (m_strBuildResource.length() == 19) // heuristically valid
      {

         System.install().trace().rich_trace("***Internal build number");

         m_strBuild = m_strBuildResource;

      }
      else
      {

         System.install().trace().rich_trace("***Getting build number");

         if (iRetry > 10)
         {

            System.install().trace().rich_trace("could not get build number. exiting");

            //            exit(-1);

            return -1;

         }

         iRetry++;

         strUrl = m_strSpaIgnitionBaseUrl + "/query?node=build__host_and_application_name&sessid=noauth";
         strUrl += "&version=";
         strUrl += m_strVersion;
         strUrl += "&appid=";
         strUrl += m_strApplicationId;
         strUrl += "&stage=";
         strUrl += straTemplate.implode(",");
         strUrl += "&locale=";
         strUrl += m_strLocale;
         strUrl += "&schema=";
         strUrl += m_strSchema;
         strUrl += "&platform=";
         strUrl += System.install().get_platform();

         if (strIndexMd5.has_char())
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

         System.install().trace().trace_add("\ndownloading " + strUrl + "\n");

         if (!Application.http().download(*m_psockethandler, psession, strUrl, &file, set))
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         string strDebug;

         strDebug = file.get_primitive_memory()->to_string();

         file.seek_to_begin();
         //         strEtc = http_get(strUrl, true);

         if (file.get_size() < 19)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }


         strBuild.Empty();

         file.read_string(strBuild);

         if (strBuild.length() != 19)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         strName.Empty();

         file.read_string(strName);

         if (strName.length() <= 0)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         strSpaHost.Empty();

         file.read_string(strSpaHost);

         if (strSpaHost.length() <= 0)
         {
            Sleep(184);
            goto RetryBuildNumber;
         }

         straMd5.set_size(straTemplate.get_count() + 1);
         iaLen.set_size(straTemplate.get_count() + 1);

         for (index i = 0; i < straMd5.get_size(); i++)
         {
            straMd5[i].Empty();
            string strMd5AndLen;
            file.read_string(strMd5AndLen);
            int iFind = strMd5AndLen.find('|');
            if (iFind < 0)
            {
               Sleep(184);
               goto RetryBuildNumber;
            }
            string strMd5 = strMd5AndLen.Left(iFind);
            iaLen[i] = atoi(strMd5AndLen.Mid(iFind + 1));
            straMd5[i] = strMd5;
            straMd5[i].trim();
            if (straMd5[i].length() != 32)
            {
               Sleep(184);
               goto RetryBuildNumber;
            }

         }

         file.read_string(strAppMatterList);

         strLen.Empty();

         file.read_string(strStatus);

         if (strStatus == "ok")
         {
            if (strIndexMd5.is_empty())
            {
               Sleep(184);
               goto RetryBuildNumber;
            }
         }
         else if (strStatus != "send")
         {
            Sleep(184);
            goto RetryBuildNumber;
         }
         else
         {

            file.read_string(strLen);

            if (strLen.length() <= 0)
            {
               Sleep(184);
               goto RetryBuildNumber;
            }

            len = atoi(strLen);

            strIndexBuild = strBuild;

            strIndexBuild.replace(" ", "_");

            strIndexBuild.replace(":", "-");

            strIndexPath = "app/stage/metastage/index-" + strIndexBuild + ".spa.bz";

            strIndexPath = ca2_get_dir(strIndexPath) / ca2_get_file(strIndexPath);

            {

               ::file::file_sp file2 = Application.file().get_file(strIndexPath, ::file::mode_create | ::file::type_binary | ::file::mode_write | ::file::defer_create_directory);

               mem.allocate(1024 * 1024);

               file_size_t uiTotalRead = 0;

               memory_size_t uiRead;

               while ((uiRead = file.read(mem, mem.get_size())))
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

      strName = "Installing " + strName;
      strName = strName + " " + m_strBuild;
      strName = strName + ::str::has_char(m_strLocale, " \"", "\"");
      strName = strName + ::str::has_char(m_strSchema, " \"", "\"");

      System.install().trace().rich_trace((":::::" + strName));

      m_strTitle = strName;

      strAppMatterList.replace(",", "\n");

      m_strAppMatterList = strAppMatterList;

      return 0;

   }


   int32_t installer::calc_host(string & strSpaHost, int32_t &iHostRetry)
   {
   retry_host:

      System.install().trace().rich_trace("***Finding optimal server");

      if (iHostRetry > 84)
      {
         //         exit(-1);
         return -1;
      }
      else if (iHostRetry > 0)
      {
         System.install().trace().rich_trace("Retrying to guess host...");
      }
      iHostRetry = 1;
      if (m_strLastHost.is_empty())
      {
         m_strLastHost = m_strCurrentHost;
      }
      else
      {
         m_strLastHost = m_strCurrentHost + ";" + m_strLastHost;
      }
      string strUrl;

      strUrl = m_strSpaIgnitionBaseUrl + "/query?node=spa_host&version=" + m_strVersion;

      if (!m_strLastHost.is_empty())
      {
         strUrl += "&last_host=" + m_strLastHost;
      }
      strUrl += "&sessid=noauth";
      System.install().trace().rich_trace("***Guessing fastest mirror");
      int32_t iGuessRetry = 0;
      System.install().trace().rich_trace(".");
      while (iGuessRetry < 30)
      {

         http_get(strUrl, false);

         if (strSpaHost.is_empty())
         {
            if (m_strLastHost.is_empty())
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
      if (m_strCurrentHost == strSpaHost)
      {
         m_strLastHost.clear();
         m_strCurrentHost.clear();
         goto retry_host;
      }
      return 0;
   }

   
   bool installer::is_application_opened(const char * psz)
   {

      string strMutex;
      strMutex = "Global\\ca2_application_global_mutex:";
      strMutex += psz;
      
      mutex mutex(get_thread_app(), false, strMutex);
      bool bOpened = ::GetLastError() == ERROR_ALREADY_EXISTS;
      if (bOpened)
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
      
      if (!m_machineevent.read(&data))
      {

         return false;

      }

      return data.m_fixed.m_bRequestCloseApplication;

   }





   void installer::add_spa_start(const char * pszId)
   {
      
      string strPath = dir::appdata(process_platform_dir_name2()) / "spa_start.xml";
      
      string strContents = file_as_string_dup(strPath);
      
      ::xml::document node(get_app());
      
      node.load(strContents);

      node.set_name("spa");

      ::xml::node * lpnode = node.GetChildByAttr("start", "id", pszId);

      if (lpnode == NULL)
      {
         
         lpnode = node.add_child("start");
         
         lpnode->add_attr("id", pszId);

         file_put_contents_dup(strPath, node.get_xml());

      }

   }


   void installer::remove_spa_start(const char * pszId)
   {
      
      string strPath = dir::appdata(process_platform_dir_name2()) / "spa_start.xml";
      
      string strContents = file_as_string_dup(strPath);

      ::xml::document node(get_app());

      node.load(strContents);

      node.set_name("spa");

      ::xml::node * lpnode = node.GetChildByAttr("start", "id", pszId);

      if (lpnode != NULL)
      {

         node.remove_child(lpnode);

         file_put_contents_dup(strPath, node.get_xml());

      }

   }


   bool installer::reboot()
   {

      HANDLE hToken;

      TOKEN_PRIVILEGES tkp;

      if (!OpenProcessToken(GetCurrentProcess(),
         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
         return false;

      LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

      tkp.PrivilegeCount = 1;

      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

      AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

      if (ExitWindowsEx(EWX_REBOOT, 0) == 0)
         return false;
      
      tkp.Privileges[0].Attributes = 0;

      AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

      return true;

   }


   int32_t installer::run_ca2_application_installer(const char * pszCommandLine)
   {

      string strPath;

      string param;

      param = "-install:";

      param += pszCommandLine;

      strPath = System.install().app_install_get_intern_executable_path(m_strVersion, m_strBuild);

      call_sync(strPath, param, 0, SW_HIDE, -1, 84, 0, 0);

      return 0;

   }


   //bool installer::launch_application()
   //{

   //   ::launch_application(get_app(), m_strApplicationId, "enable_desktop_launch", OSBIT);

   //   return true;

   //}


   int32_t installer::app_install_synch(const char * pszCommandLine, uint32_t & dwStartError, bool bSynch)
   {

      SHELLEXECUTEINFOW sei = {};

      wstring wstrFile = u16(dir::stage(process_platform_dir_name()) / "app");

      wstring wstrParams = u16(string(pszCommandLine) + " install");

      {

         sei.cbSize = sizeof(SHELLEXECUTEINFOW);
         sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
         sei.lpVerb = L"RunAs";
         sei.lpFile = wstrFile.c_str();
         sei.lpParameters = wstrParams.c_str();
         ::ShellExecuteExW(&sei);

      }

      DWORD dwGetLastError = GetLastError();

      DWORD dwExitCode = 0;

      for (int i = 0; i < 10 * 1000; i++)
      {

         if (::GetExitCodeProcess(sei.hProcess, &dwExitCode))
         {

            if (dwExitCode != STILL_ACTIVE)
            {

               break;

            }

         }
         else
         {

            Sleep(100);

            break;

         }

         Sleep(100);

      }

      ::CloseHandle(sei.hProcess);

      return 0;

   }


   void installer::on_http_complete(::sockets::http_socket * psocket, ::http::e_status estatus)
   {

      if (estatus != ::http::status_ok)
      {

         m_straHttpFailure.add(psocket->m_strUrl);

         System.install().trace().rich_trace("Failure downloading : " + psocket->m_strUrl);

      }

   }


   void installer::on_set_scalar(int_scalar_source * psource, e_scalar escalar, int64_t iValue, int iFlags)
   {

      if (escalar == scalar_download_size)
      {

         if (m_bProgressModeAppInstall)
         {

            int64_t iMax = 0;

            psource->get_scalar_maximum(escalar, iMax);

            if (iMax == 0)
            {

               iMax = iValue;

            }

            if (iMax == 0)
            {

               iValue = 0;

               iMax = 1;

            }

            synch_lock sl(&m_mutexOmp);

            int iTest = 0;

            m_daProgress.element_at_grow(iTest) = (double)iValue / (double)iMax;

            double dTotal = m_daProgress.get_total();

            dTotal += m_dAppInstallProgressBase;

            set_progress(dTotal / m_dAppInstallFileCount);

         }
         else
         {

            synch_lock sl(&m_mutexOmp);

            m_iaProgress.element_at_grow(0) = iValue;

            set_progress((double)(m_iGzLen2 + m_iaProgress.get_total()) / (double)m_iProgressTotalGzLen2);

         }

      }

   }


   string installer::http_get(const string & strUrl, bool bScalarListener)
   {

      ::property_set set(get_app());

      if (bScalarListener)
      {

         set["int_scalar_source_listener"] = this;

      }

      string str;

      set["get_response"].get_value() = &str;

      set["disable_ca2_sessid"] = true;

      set["raw_http"] = true;

      if (!System.http().request(*m_psockethandler, m_phttpsession, strUrl, set))
      {

         return "";

      }

      return str;

   }


} // namespace app_app_install








