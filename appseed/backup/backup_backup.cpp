﻿#include "framework.h"


namespace backup
{


   backup::backup(sp(::ca::application) papp) :
      ca(papp),
      thread(papp),
      ::simple_thread(papp),
      production(papp)
   {
      m_bClean       = false;
      m_bBuild       = true;
      m_bFinished    = true;
      m_eversion     = version_stage;
   }

   backup::~backup()
   {
   }

   string backup::get_new_repos_local_path(const char * psz)
   {
      string strNewRepos;
      strNewRepos.Format("V:\\ca\\bk\\%s\\repos\\%s", m_strTag, psz);
      return strNewRepos;
   }

   string backup::get_new_db_local_path(const char * psz)
   {
      string strNewRepos;
      strNewRepos.Format("V:\\ca\\bk\\%s\\db\\%s", m_strTag, psz);
      return strNewRepos;
   }

   int32_t backup::run()
   {
      if(m_iStep == 1)
      {
         string strStartTime;
         m_timeStart.Format(strStartTime, "%Y-%m-%d %H-%M-%S");
         add_status("Backup starting at " + strStartTime);
         add_status(unitext("Thank you!!"));
         m_dwStartTick = ::GetTickCount();
         m_timeStart.Format(m_strTag, "%Y-%m-%d_%H-%M-%S");

         keeper < bool > keepFinishedFalse(&m_bFinished, false, true, true);
         string str;

         string strFile = System.dir().ca2("basis/ca/app/dbbk.bat");
         if(!Application.file().exists(strFile))
         {
            string str;
            str.Format("***File %s does not exist. (mysqldump -uroot -ppassword --opt --all-databases > %%1)", strFile);
            add_status(str);
            return 0;
         }

         if(!hotcopy_repos("ca2os"))
            return 0;
         if(!hotcopy_repos("app"))
            return 0;
         if(!hotcopy_repos("net"))
            return 0;
         if(!hotcopy_repos("hi5"))
            return 0;
         if(!hotcopy_repos("hi5-net"))
            return 0;
         if(!hotcopy_repos("public_reading"))
            return 0;
         if(!hotcopy_repos("main"))
            return 0;
         if(!all_db_dump())
            return 0;

         if(!compress_repos("ca2os"))
            return 0;
         if(!compress_repos("app"))
            return 0;
         if(!compress_repos("net"))
            return 0;
         if(!compress_repos("hi5"))
            return 0;
         if(!compress_repos("hi5-net"))
            return 0;
         if(!compress_repos("public_reading"))
            return 0;
         if(!compress_repos("main"))
            return 0;
         if(!db_copy())
            return 0;


         throw not_implemented(get_app());

         /*      string strServer;
         strServer = "loft3099.myftpbackup.com";
         add_status("ftp-putting in backup server " + strServer);
         {
         CInternetSession session;
         CFtpConnection ftpconn(
         &session,
         strServer,
         "loft3099",
         "gustavo514Lund");
         ftp_put_dir(ftpconn, "C:\\", "ca/bk/"  + m_strTag, 0);
         }
         strServer = "netnode.ca2.cc";
         add_status("ftp-putting in backup server " + strServer);
         {
         CInternetSession session;
         CFtpConnection ftpconn(
         &session,
         strServer,
         "ccauthbk",
         "gustavo514Lund");
         ftp_put_dir(ftpconn, "C:\\", "ca/bk/"  + m_strTag, 0);
         }*/


         m_bFinished = true;
         m_timeEnd = ::datetime::time::get_current_time();
         m_dwEndTick = ::GetTickCount();
         uint32_t dwMin = (m_dwEndTick - m_dwStartTick) / 1000 / 60;
         uint32_t dwSeg = ((m_dwEndTick - m_dwStartTick) / 1000) % 60;
         string strTime;

         m_timeStart.Format(strStartTime, "%Y-%m-%d %H-%M-%S");
         string strEndTime;
         m_timeEnd.Format(strEndTime, "%Y-%m-%d %H-%M-%S");
         add_status("Backup started at " + strStartTime);
         add_status("Backup ending at " + strEndTime);
         strTime.Format("Backup took: %dm %ds", dwMin, dwSeg);
         add_status(strTime);
         add_status(unitext("Finished!! Thank you!!"));
         add_status("");
         add_status("");
         add_status("");
         add_status("");
         add_status("");
      }


      return 0;
   }

   bool  backup::hotcopy_repos(const char * psz)
   {
      string strStatus;
      strStatus.Format("hotcopy repository: %s ...", psz);
      add_status(strStatus);

      string str;
      string strBase = m_strBase;
      STARTUPINFO si;
      PROCESS_INFORMATION pi; 
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_HIDE; 
      string strNewRepos = get_new_repos_local_path(psz);
      Application.dir().mk(System.dir().name(strNewRepos));
      str.Format("svnadmin hotcopy C:\\repos\\%s %s", psz, strNewRepos);

      if(!::CreateProcess(NULL, (LPTSTR) (const char *) str, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
      {
         strStatus.Format("     Error: Check svn installation!!");
         add_status(strStatus);
         return false;
      }

      DWORD dwExitCode;
      int32_t i = 1;
      while(true)
      {
         if(!GetExitCodeProcess(pi.hProcess, &dwExitCode))
            break;
         if(dwExitCode != STILL_ACTIVE)
            break;
         Sleep(2300);
         str.Format("%d hotcopy repository: %s ...", i, psz);
         add_status(str);
         i++;
      }
      return true;
   }

   bool  backup::all_db_dump()
   {
      string strStatus;
      strStatus.Format("dumping all databases");
      add_status(strStatus);

      string str;
      string strBase = m_strBase;
      STARTUPINFO si;
      PROCESS_INFORMATION pi; 
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_HIDE; 
      string strdump = get_new_db_local_path("all.sql");
      Application.dir().mk(System.dir().name(strdump));

      str.Format("%s \"%s\"",
         System.dir().ca2("basis/ca/app/dbbk.bat"),
         strdump);

      if(!::CreateProcess(NULL, (LPTSTR) (const char *) str,
         NULL, NULL, FALSE, 0, NULL,
         "C:\\", &si, &pi))
      {
         strStatus.Format("     Error: Check svn installation!!");
         add_status(strStatus);
         return false;
      }

      DWORD dwExitCode;
      int32_t i = 1;
      while(true)
      {
         if(!GetExitCodeProcess(pi.hProcess, &dwExitCode))
            break;
         if(dwExitCode != STILL_ACTIVE)
            break;
         Sleep(2300);
         str.Format("%d dumping all databases ...", i);
         add_status(str);
         i++;
      }
      return true;
   }

   bool  backup::compress_repos(const char * psz)
   {
      string strStatus;
      strStatus.Format("compressing repository: %s ...", psz);
      add_status(strStatus);

      string str;
      string strBase = m_strBase;
      STARTUPINFO si;
      PROCESS_INFORMATION pi; 
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_HIDE; 
      string strNewRepos = get_new_repos_local_path(psz);
      string strTar;
      strTar.Format("C:\\ca\\bk\\%s\\repos\\%s.tar", m_strTag, psz);
      Application.dir().mk(System.dir().name(strTar));
      str.Format("7za.exe a -r -ttar \"%s\" \"%s\"", strTar, strNewRepos);

      if(!::CreateProcess(NULL, (LPTSTR) (const char *) str, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
      {
         strStatus.Format("     Error: Check 7-zip installation! Cannot 7za.exe command line utility!");
         add_status(strStatus);
         return false;
      }

      DWORD dwExitCode;
      int32_t i = 1;
      while(true)
      {
         if(!GetExitCodeProcess(pi.hProcess, &dwExitCode))
            break;
         if(dwExitCode != STILL_ACTIVE)
            break;
         Sleep(2300);
         str.Format("%d compressing repository: %s ...", i, psz);
         add_status(str);
         i++;
      }
      return true;
   }



   bool  backup::db_copy()
   {

      string strStatus;
      strStatus.Format("duplicating db backup ...");
      add_status(strStatus);

      string strSrc = get_new_db_local_path("all.sql");
      string strDst;
      strDst.Format("C:\\ca\\bk\\%s\\db\\all.sql", m_strTag);
      Application.file().copy(strDst, strSrc);

      return true;

   }


} // namespace backup




