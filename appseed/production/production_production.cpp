﻿#include "framework.h"
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/x509.h>


namespace production
{


   production::production(sp(base_application) papp) :
      element(papp),
      thread(papp),
      simple_thread(papp),
      m_mutexStatus(papp),
      m_mutexCompress(papp),
      m_evFinish(papp)
   {

      m_bEndProduction  = false;
      m_bClean          = false;
      m_bBuild          = true;
      m_bFinished       = true;
      m_eversion        = version_stage;
      m_bReleased       = true;
      m_iRelease        = 0;

      {
         stringa & stra = m_straStageDirAcceptedFileExtensions;
         stra.add("exe");
         stra.add("dll");
         stra.add("manifest");
      }
      m_bLoop           = false;
      m_bEndStatus      = false;


   }

   production::~production()
   {
   }

   void production::start_loop(e_version eversion, int32_t iLoopCount)
   {
      m_bLoop = true;
      if(m_iRelease > 0)
      {
         add_status("There are pending releases!!");
         return;
      }
      m_iLoop = -1;
      m_iLoopCount = iLoopCount;
      m_iRelease = 0;
      m_bEndProduction = false;
      m_timeStart = ::datetime::time::get_current_time();
      m_eversion = eversion;


      m_iStep = 1;
      m_iGlobalRetry = 0;

      if (m_eversion == version_basis)
      {
         m_strTwitterConsumerKey = "mKYvWA6cZkUEUwjoygUuVw";
         m_strTwitterConsumerSecret = "JwtNLBLyXlPvGLqKA4c8w4XH0PPLmkoVzm0TOocvSyY";
      }
      else
      {
         m_strTwitterConsumerKey = "AHsGuuGPEqDheXbx93JQ";
         m_strTwitterConsumerSecret = "6b8zc7HxM0ImjYgq0kOM1AzrHSIxTCxVOWsM7gRN90";
      }



      twitter_auth();


      begin();



   }

   void production::start_production(e_version eversion)
   {

      m_bLoop = false;

      if(m_iRelease > 0)
      {
         add_status("There are pending releases!!");
         return;
      }

      m_iRelease = 0;
      m_bEndProduction = false;
      m_timeStart = ::datetime::time::get_current_time();
      m_eversion = eversion;


      m_iStep = 1;
      m_iGlobalRetry = 0;

      twitter_auth();

      begin();


   }

   void production::step()
   {
      m_iStep++;
      begin();
   }

   void production::defer_quit()
   {
      if(!m_bReleased)
         return;
      if(m_iLoopCount > 0)
      {
         if(m_iLoop > m_iLoopCount)
         {
            System.os().post_to_all_threads(WM_QUIT, 0, 0);
         }
      }

   }




   int32_t production::run()
   {

      string str;

      property_set set;

      int32_t iProduce;

      string strBackPostColor   = "background-color: #CCCCC2;";

      while(true)
      {

         try
         {
            iProduce = produce();
         }
         catch(...)
         {
            iProduce = -1;
         }

         if(iProduce >= 0)
            break;

         m_iStep = 1;


         //m_timeStart = ::datetime::time::get_current_time();

         m_iGlobalRetry++;

         if(m_iGlobalRetry > 8)
         {

            add_status("Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" this command!");

            if(m_eversion == version_basis)
            {
               set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #882266; display: block; margin-bottom: 1.5em;\">Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" " + m_strVersion + " build command!</span>";
            }
            else
            {
               set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" " + m_strVersion + " build command!</span>";
            }

            Application.http().get("http://api.ca2.cc/status/insert", str, set);


            string strTwit = "Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" " + m_strVersion + " build command!";

            twitter_twit(strTwit);

            return -1;

         }


         add_status("General build failure. Restarting");

         if(m_iGlobalRetry == 1)
         {
            m_strTry = "1st";
         }
         else if(m_iGlobalRetry == 2)
         {
            m_strTry = "2nd";
         }
         else if(m_iGlobalRetry == 3)
         {
            m_strTry = "3rd";
         }
         else
         {
            m_strTry = ::str::from(m_iGlobalRetry) + "th";
         }

         if(m_eversion == version_basis)
         {
            set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #222218; display: block; margin-bottom: 1.5em;\">General build failure. Starting " + m_strTry + " retry of new <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> release.<br />";
         }
         else
         {
            set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #222218; display: block; margin-bottom: 1.5em;\">General build failure. Starting " + m_strTry + " retry  of new <a href=\"http://ca2.cc/\">stage</a> release.<br />"; 
         }

         if(m_straStatus.get_count() > 1)
         {

            set["post"]["new_status"] = set["post"]["new_status"] + "Last Stati:<br />";
            for(::count i = min(5, m_straStatus.get_count() - 1); i >= 1 ; i--)
            {
               set["post"]["new_status"] = set["post"]["new_status"] + System.html()->entities(m_straStatus.element_at(i));
               set["post"]["new_status"] = set["post"]["new_status"] + "<br />";
            }
            set["post"]["new_status"] = set["post"]["new_status"] + "<br />";
            set["post"]["new_status"] = set["post"]["new_status"] + "</span>";

         }


         Application.http().get("http://api.ca2.cc/status/insert", str, set);

         string strTwit = "General failure of build " + version_to_international_datetime(m_strBuild) + ". Starting " + m_strTry + " retry of build " + m_strVersion + ". More details at http://status.ca2.cc/" + System.url().url_encode(m_strStatusEmail);

         twitter_twit(strTwit);

      }

      /*if(System.directrix()->m_varTopicQuery.has_property("quit_on_finish"))
      {
         
         System.os().post_to_all_threads(WM_QUIT, 0, 0);

      }*/

      OnUpdateRelease();

      return iProduce;


   }


   int32_t production::produce()
   {

      int32_t iRetry = 0;

restart:

      if (m_iStep == 1)
      {

         if (m_eversion == version_basis)
         {
            m_strStdPostColor = "color: #882277;";
            m_strBackPostColor = "background-color: #CFC2CF;";
            m_strEmpPostColor = "color: #660060;";
            m_strVersion = "basis";
            m_strStatusEmail = "ca2dev@ca2.cc";
            m_strDownloadSite = "eu-download.ca2.cc";
         }
         else
         {
            m_strStdPostColor = "color: #448855;";
            m_strBackPostColor = "background-color: #A0CCAA;";
            m_strEmpPostColor = "color: #007700;";
            m_strVersion = "stage";
            m_strStatusEmail = "production@ca2.cc";
            m_strDownloadSite = "download.ca2.cc";
         }


      bool bMediumSizeStatusText = false;

         if (bMediumSizeStatusText)
         {

            property_set set;

            Application.http().get("http://api.ca2.cc/status/insert");
         string strEndTime;
         m_timeEnd.FormatGmt(strEndTime, "%Y-%m-%d %H-%M-%S");
         m_strBuild = "\"This a long build version string text description that makes really trigger a size that is will end up making this status rquired and needed to be recorded and written to another set of records or table...\"";
         if (m_eversion == version_basis)
         {
            set["post"]["new_status"] = "<div style=\"display: block; background-color: #FFE0FF; \"><h2 style=\"margin-bottom:0px; color: #FF55CC;\">Medium Size Status Text" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #882255; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://laboratory.ca2.cc/\" class=\"fluidbasis\" >laboratory.ca2.cc</a> or <a href=\"http://warehouse.ca2.cc/\" class=\"fluidbasis\" >warehouse.ca2.cc</a> for simple gateway implementations.</span></div>";
         }
         else
         {
            set["post"]["new_status"] = "<div style=\"display: block; background-color: #E0FFCC; \"><h2 style=\"margin-bottom:0px; color: #55CCAA;\">Medium Size Status Text" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://ca2.cc/\">stage</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://desktop.ca2.cc/\">desktop.ca2.cc</a> or <a href=\"http://store.ca2.cc/\">store.ca2.cc</a> for simple gateway implementations.</span></div";
         }
         string str;
         Application.http().get("http://api.ca2.cc/status/insert", str, set);

         ExitProcess(true);


      }
         //m_strBase = Application.command()->m_varTopicQuery["base_dir"];
         m_strBase = Application.file().as_string("C:\\ca2\\config\\nodeapp-windows\\production\\base_dir.txt").trimmed();


         {

            string strContentsSrc = System.dir().path(m_strBase, "include", "product.version.config.h");
            
            string strContentsSet;
            
            string strContentsGet = Application.file().as_string(strContentsSrc);

            if(m_eversion == version_basis)
            {

               strContentsSet = "#pragma once\n\n\n#define CA2_PLATFORM_VERSION CA2_BASIS\n\n\n\n";
         
            }
            else
            {

               strContentsSet = "#pragma once\n\n\n#define CA2_PLATFORM_VERSION CA2_STAGE\n\n\n\n";

            }

            if(strContentsSet != strContentsGet)
            {

               Application.file().put_contents(strContentsSrc, strContentsSet);

            }

         }


         m_strSignTool = System.dir().element("nodeapp/thirdparty/binary/signtool.exe");
         m_strSpc = "X:\\sensitive\\sensitive\\certificate\\ca2.p12";
         m_strSignPass = Application.file().as_string("X:\\sensitive\\sensitive\\certificate\\2011-05-ca2.pass");

         m_iLoop++;
         defer_quit();

         

         m_straRoot.remove_all();

         Application.dir().ls_dir(m_strBase, NULL, &m_straRoot);

         m_straRoot.filter_begins_ci("app-");

         m_straRoot.insert_at(0, "app");

         m_straRoot.add("nodeapp");

         //   goto skipCompress;
         //goto skipBuild;

         bool bSkip1 = false;

         if(!bSkip1 && m_iGlobalRetry <= 0)
         {

            for(int32_t i = 0; i < m_straRoot.get_size(); i++)
            {

               if(!sync_source(m_straRoot[i], NULL))
                  return 1;

            }

         }


         string strRevision;

         string strTime;
         ::datetime::time time;
         time = m_timeStart;
         time.FormatGmt(strTime, "%Y-%m-%d %H-%M-%S");
         string strVerWin;
         time.FormatGmt(strVerWin, "%Y,%m%d,%H%M,%S");
         string strSvnVersionCmd;
         strSvnVersionCmd.Format("svnversion %s", System.dir().path(m_strBase, "app"));
         m_strBuild = strTime;
         m_strFormatBuild = strTime;
         m_strFormatBuild.replace(" ", "_");
         strRevision = System.process().get_output(strSvnVersionCmd);
         string strSVN = "SVN" + strRevision;
         strSVN.trim();
         if(m_iGlobalRetry <= 0 && m_strSubversionRevision == strSVN)
         {
            iRetry++;
            if(iRetry > 3)
               return 3;
            goto restart;
         }

         string strSVNKey;
         string strAddRevision;

         strSVNKey = "app:" + strSVN;

         if (!bSkip1)
         {

            for (int32_t i = 1; i < m_straRoot.get_size(); i++)
            {

               strSvnVersionCmd.Format("svnversion %s", System.dir().path(m_strBase, m_straRoot[i]));
               strAddRevision = System.process().get_output(strSvnVersionCmd);
               strAddRevision.trim();
               strSVNKey += ", " + m_straRoot[i] + ":SVN" + strAddRevision;

            }

         }

         m_bReleased = false;
         m_iLoop = -1;
         m_timeStart.FormatGmt(m_strStartTime, "%Y-%m-%d %H-%M-%S");
         add_status("Build starting at " + version_to_international_datetime(m_strStartTime) + " - build version!");

         {
            // good pratice to initialize authentication of ca2status.com with account.ca2.cc auth information
            string str;
            
            
            property_set set;

            Application.http().get("http://api.ca2.cc/status/insert");

            if(m_eversion == version_basis)
            {
               set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #552250;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #882266; display: block; margin-bottom: 1.5em;\">Starting production of new <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> release.</span>";
            }
            else
            {
               set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #22552F;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">Starting production of new <a href=\"http://ca2.cc/\">stage</a> release.</span>";
            }

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

            string strTwit;

            if(m_iGlobalRetry <= 0)
            {
               strTwit = "ca2twit-lib : new " + m_strVersion + " build starting " + version_to_international_datetime(m_strBuild) + ". More details at http://status.ca2.cc/" + System.url().url_encode(m_strStatusEmail);
            }
            else
            {
               strTwit = "ca2twit-lib : " + m_strTry + " automatic retry " + m_strVersion + " build starting " + version_to_international_datetime(m_strBuild) + ". More details at http://status.ca2.cc/" + System.url().url_encode(m_strStatusEmail);
            }

            twitter_twit(strTwit);

         }

         add_status(unitext("Thank you!!"));
         m_dwStartTick = ::GetTickCount();

         m_bEndStatus = false;

         keeper < bool > keepFinishedFalse(&m_bFinished, false, true, true);
         string str;
         m_iBaseLen = m_strBase.get_length();
         if(m_strBase.Right(1) != "/" && m_strBase.Right(1) != "\\")
            m_iBaseLen++;

         
         
         string strStatus;
         m_strTag = strTime + " " + strSVNKey;
         m_strTagPath = System.dir().path("C:\\ca2\\build\\stage", m_strFormatBuild +".txt");

         string strBuildH;
         strBuildH.Format("-c1-production -c2-producer -t12n-producing -mmmi- %s", m_strTag);
         strBuildH += " - ";
         strBuildH += Application.file().as_string(System.dir().path(m_strBase, "app/stage", "build_machine_pp_comment.txt"));
         strBuildH += "#define THIS_PRODUCT_VERSION \"" + m_strTag + "\\0\"\r\n#define THIS_FILE_VERSION \""+ m_strTag +"\\0\"\r\n";
         strBuildH += "#define __THIS_PRODUCT_VERSION " + strVerWin + "\r\n#define __THIS_FILE_VERSION "+ strVerWin +"\r\n";
         strBuildH += "\r\n";
         strBuildH += "\r\n";
         strBuildH += "\r\n";
         strBuildH += "\r\n";
         strBuildH += "\r\n";




         m_strVrel = "C:\\ca2\\vrel\\stage\\" + m_strFormatBuild;

         m_strCCAuth = "C:\\home\\ca2os\\ca2_spa\\stage\\" + m_strFormatBuild;
         m_strCCVrel = "C:\\home\\ca2os\\ca2_spa\\stage";
         m_strCCVrelNew = "C:\\home\\ca2os\\ca2_spa\\ccvrelnew\\stage\\" + m_strFormatBuild;

         uint32_t dwExitCode;

         int32_t i;
         if(m_bClean)
         {
            add_status("Cleaning ca2 fontopus ca2os ...");
            {
               string str;
               
               
               property_set set;

               set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Cleaning...</span></div>";

               Application.http().get("http://api.ca2.cc/status/insert", str, set);

            }
            ::core::process process;
            string strPath;
            strPath = System.dir().element("nodeapp\\stage\\script\\stage_clean.bat");
            if(!process.create_child_process(strPath, false))
            {
               uint32_t dw = GetLastError();
               string str;
               str.Format("Error creating clean process: %d", dw);
               add_status(str);
               return 0;   
            }
            i = 1;
            while(!process.has_exited(&dwExitCode))
            {
               Sleep(5000);
               str.Format("%d Cleaning ca2 fontopus ca2os ...", i);
               add_status(str);
               i++;
            }
         }

         //Application.dir().mk(System.dir().path(m_strBase, "time"));
         Application.file().put_contents(System.dir().path(m_strBase, "app\\build.txt"), m_strBuild);
         Application.file().put_contents_utf8(System.dir().path(m_strBase, "app\\this_version_info.h"), strBuildH);
         Application.file().put_contents_utf8(System.dir().path(m_strBase, "app\\this_version_info.txt"), strBuildH); //  + "\r\n\r\n" + strDedicaverse + "\r\n\r\n" + strCgclcst);

         if(!commit_for_new_build_and_new_release())
            return 2;


         m_strSubversionRevision = "SVN" + ::str::from(atoi(strRevision) + 1);

         if(m_bBuild)
         {
            build("app-veriwell");
         }

         {
            string str;
            
            
            property_set set;

            set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Cleaning...</span></div>";

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

         }

         add_status("Cleaning site...");
         string strPath = System.dir().element("time\\stage\\app\\matter\\job.bat");


         //System.http().ms_download("http://api.ca2.cc/spaignition/clean", 
         //   System.dir().element("time\\spaignition_update.txt"), NULL, post, headers, ::ca2::app(get_app()).user()->get_user());
         add_status("Cleaning ca2os folder...");
         ::core::process process;
         Application.file().put_contents(strPath, "rmdir /s /q C:\\ca2\\vrel\\stage");
         if(!process.create_child_process(strPath, false))
         {
            uint32_t dw = GetLastError();
            string str;
            str.Format("Error creating process: %d", dw);
            add_status(str);
            return 0;   
         }
         i = 1;
         while(!process.has_exited(&dwExitCode))
         {
            Sleep(500);
            str.Format("%d Cleaning ca2os folder ...", i);
            add_status(str);
            i++;
         }

         m_straFiles.remove_all();


         get_file_list(m_strBase, "stage/x86", m_strRemote, m_straFiles, m_straTitle, m_straRelative);
         get_file_list(m_strBase, "stage/x64", m_strRemote, m_straFiles, m_straTitle, m_straRelative);
         get_file_list(m_strBase, "app/stage/metastage", m_strRemote, m_straFiles, m_straTitle, m_straRelative);


         generate_appmatter_spa();


         ::count iCount = m_straFiles.get_size();
         for(int32_t i = 0; i < iCount;)
         {
            if(m_straFiles[i].find("\\.svn\\") >= 0 || (m_straFiles[i].get_length() < 5 || m_straFiles[i].Right(5) == "\\.svn"))
            {
               m_straFiles.remove_at(i);
            }
            else
            {
               i++;
            }
         }
         TRACE("\n");
         for(int32_t i = 0; i < m_straFiles.get_size(); i++)
         {
            const char * lpcsz = m_straFiles[i];
            TRACE("file(%05d)=%s\n", i, lpcsz);
         }
         m_pview->post_message(WM_USER, 2);
         {
            string str;
            
            
            property_set set;

            set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Copying...</span></div>";

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

         }
      }
      else if(m_iStep == 2)
      {
         string m_strStartTime;
         m_timeStart.FormatGmt(m_strStartTime, "%Y-%m-%d %H-%M-%S");
         {
            string str;
            
            
            property_set set;

            set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" +version_to_international_datetime( ::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Compressing...</span></div>";

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

         }
         compress();
         {
            string str;
            
            
            property_set set;

            set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" +version_to_international_datetime( ::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Resources...</span></div>";

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

         }

         release_production();

         if(!release_npca2("x86"))
         {
            return 1;
         }
         if(!release_npca2("x64"))
         {
            return 1;
         }
         if(!release_iexca2("x86"))
         {
            return 1;
         }
         if(!release_iexca2("x64"))
         {
            return 1;
         }
         if(!release_crxca2("x86"))
         {
            return 1;
         }
         /*if(!release_crxca2("x64"))
         {
         return 1;
         }*/
         Application.dir().mk("C:\\home\\ca2os\\ca2_spa\\stage\\app\\");
         Application.file().put_contents("C:\\home\\ca2os\\ca2_spa\\stage\\app\\build.txt", m_strBuild);
         Application.file().put_contents(m_strCCVrelNew + "\\app\\build.txt", m_strBuild);
         Application.dir().mk(System.dir().name(m_strTagPath));
         Application.file().put_contents(m_strTagPath, m_strTag);

         {
            string str;
            
            
            property_set set;

            set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Storing Symbols...</span></div>";

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

         }


         
         {

            add_status("Storing Symbols x86...");

            uint32_t dwExitCode;

            string strPath = System.dir().element("time\\stage\\app\\matter\\store_symbols_job_x86.bat");

            ::core::process process;
            string strCommand = "\"C:\\Program Files (x86)\\Windows Kits\\8.0\\Debuggers\\x86\\symstore.exe\"  add /r  -:REL /f \\\\sewindows\\stage\\" + m_strFormatBuild + "\\stage\\x86\\ /s \\\\sewindows\\SymbolServer\\ /t \"ca2\" /v \"" + m_strFormatBuild + "\"";
            Application.file().put_contents(strPath, strCommand);
            if(!process.create_child_process(strPath, false))
            {
               uint32_t dw = GetLastError();
               string str;
               str.Format("Error creating process: %d", dw);
               add_status(str);
               return 0;   
            }

            int32_t i = 1;

            string str;

            while(!process.has_exited(&dwExitCode))
            {
               Sleep(584);
               str.Format("%d Storing Symbols x86 ...", i);
               add_status(str);
               i++;
            }

         }





         {

            add_status("Storing Symbols x64...");

            uint32_t dwExitCode;

            string strPath = System.dir().element("time\\stage\\app\\matter\\store_symbols_job_x64.bat");

            ::core::process process;
            string strCommand = "\"C:\\Program Files (x86)\\Windows Kits\\8.0\\Debuggers\\x64\\symstore.exe\"  add /r  -:REL /f \\\\sewindows\\stage\\" + m_strFormatBuild + "\\stage\\x64\\ /s \\\\sewindows\\SymbolServer\\ /t \"ca2\" /v \"" + m_strFormatBuild + "\"";
            Application.file().put_contents(strPath, strCommand);
            if(!process.create_child_process(strPath, false))
            {
               uint32_t dw = GetLastError();
               string str;
               str.Format("Error creating process: %d", dw);
               add_status(str);
               return 0;   
            }

            int32_t i = 1;

            string str;

            while(!process.has_exited(&dwExitCode))
            {
               Sleep(584);
               str.Format("%d Storing Symbols x86 ...", i);
               add_status(str);
               i++;
            }

         }






         {
            string str;
            
            
            property_set set;

            set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Packaging...</span></div>";

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

         }



         add_status("dtf - fileset - file from directory app");
         System.file().dtf(m_strCCVrelNew + "\\ca2_spa_app.fileset", m_strCCVrelNew + "\\app", get_app());
         add_status("dtf - fileset - file from directory stage");
         System.file().dtf(m_strCCVrelNew + "\\ca2_spa_stage.fileset", m_strCCVrelNew + "\\stage", get_app());
         add_status("bz - bzip - compressing app");
         System.compress().bz(get_app(), m_strCCVrelNew + "\\ca2_spa_app.fileset.bz", m_strCCVrelNew + "\\ca2_spa_app.fileset");
         add_status("bz - bzip - compressing stage");
         System.compress().bz(get_app(), m_strCCVrelNew + "\\ca2_spa_stage.fileset.bz", m_strCCVrelNew + "\\ca2_spa_stage.fileset");

         class release * prelease = NULL;

         /*      add_status("_001cgcl - releasing at United States, GoDaddy, cgcl...");
         class release * prelease = new class release(this);
         prelease->m_strRelease = "http://production.server1serves.ca2os.com/release_ca2_ccauth_spa?authnone=1&version_shift="
         + m_strVersionShift + "&format_build=" + m_strFormatBuild;
         prelease->begin();


         add_status("_002cst - releasing at United States, GoDaddy, cst...");
         prelease = new class release(this);
         prelease->m_strRelease = "http://production.server2serves.ca2os.com/release_ca2_ccauth_spa?authnone=1&version_shift="
         + m_strVersionShift + "&format_build=" + m_strFormatBuild;
         prelease->begin();*/


         {
            string str;
            
            
            property_set set;

            set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Releasing...</span></div>";

            Application.http().get("http://api.ca2.cc/status/insert", str, set);

         }

         //Application.http().get("http://account.ca2.cc/update_plugins?authnone");


         m_bEndProduction = true;


         add_status("ca2.se - freigeben auf Deutschland, Hessen, Frankfurt, ServerLoft...");
         prelease = new class release(this);
         prelease->m_strRelease = "http://production.server4serves.ca2os.com/release_ca2_ccauth_spa?secure=0&authnone=1&format_build=" + m_strFormatBuild;
         prelease->begin();

         add_status(unitext("ca2.cl - lançando no Brasil, Rio Grande do Sul, Porto Alegre, RedeHost..."));
         //add_status("ca2.cl - lancando no Brasil, Rio Grande do Sul, Porto Alegre, RedeHost...");
         prelease = new class release(this);
         prelease->m_strRelease = "http://production.server5serves.ca2os.com/release_ca2_ccauth_spa?secure=0&authnone=1&format_build=" + m_strFormatBuild;
         prelease->begin();

         /*
         add_status("releasing in server1serves.ca2os.com - United States...");
         {
         CInternetSession session;
         CFtpConnection ftpconn(
         &session,
         "server1serves.ca2os.com",
         "ca2os",
         "ccauth514Lund");
         ftp_put_dir(ftpconn, "C:\\home\\ca2os", "ca2_spa/" + m_strVersionShiftFwd + "stage", 0);
         ftp_put_dir(ftpconn, "C:\\home\\ca2os", "ca2_spa/" + m_strVersionShiftFwd + "app", 0);
         }
         */

         /*
         add_status("releasing in server2serves.ca2os.com - United States...");
         {
         CInternetSession session;
         CFtpConnection ftpconn(
         &session,
         "server2serves.ca2os.com",
         "ca2os",
         "ccauth514Lund");
         ftp_put_dir(ftpconn, "C:\\home\\ca2os", "ca2_spa/" + m_strVersionShiftFwd + "stage", 0);
         ftp_put_dir(ftpconn, "C:\\home\\ca2os", "ca2_spa/" + m_strVersionShiftFwd + "app", 0);
         }
         */

         /*
         add_status("releasing in server3serves.ca2os.com - United States...");
         {
         CInternetSession session;
         CFtpConnection ftpconn(
         &session,
         "server3serves.ca2os.com",
         "ca2os",
         "ccauth514Lund");
         ftp_put_dir(ftpconn, "C:\\home\\ca2os", "ca2_spa/" + m_strVersionShiftFwd + "stage", 0);
         ftp_put_dir(ftpconn, "C:\\home\\ca2os", "ca2_spa/" + m_strVersionShiftFwd + "app", 0);
         }
         */

         /*System.http().ms_get("http://veriwell.us/release_ca2_ccauth_spa?authnone=1");
         try
         {
         CFtpConnection ftpconn(
         &session,
         "veriwell.de",
         "ca2os",
         "ccauth514Lund");
         add_status("send app to Germany...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_app.fileset.gz", "ca2_spa_app.fileset.gz");
         add_status("send stage to Germany...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_stage.fileset.gz", "ca2_spa_stage.fileset.gz");


         }
         catch(...)
         {
         }
         add_status("releasing in Germany...");
         System.http().ms_get("http://veriwell.de/release_ca2_ccauth_spa?authnone=1");
         try
         {
         CFtpConnection ftpconn(
         &session,
         "veriwell.co.uk",
         "ca2os",
         "ccauth514Lund");
         add_status("send app to United Kingdom...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_app.fileset.gz", "ca2_spa_app.fileset.gz");
         add_status("send stage to United Kingdom...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_stage.fileset.gz", "ca2_spa_stage.fileset.gz");


         }
         catch(...)
         {
         }
         add_status("releasing in United Kingdom...");
         System.http().ms_get("http://veriwell.co.uk/release_ca2_ccauth_spa?authnone=1");
         try
         {
         CFtpConnection ftpconn(
         &session,
         "veriwell.jp",
         "ca2os",
         "ccauth514Lund");
         add_status("send app to Japan...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_app.fileset.gz", "ca2_spa_app.fileset.gz");
         add_status("send stage to Japan...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_stage.fileset.gz", "ca2_spa_stage.fileset.gz");
         }
         catch(...)
         {
         }
         add_status("releasing in Japan...");
         System.http().ms_get("http://xn--gckc2msch3hc.jp/release_ca2_ccauth_spa?authnone=1");
         try
         {
         CFtpConnection ftpconn(
         &session,
         "veriwell.com.br",
         "ca2os",
         "ccauth514Lund");
         add_status("send app to Brazil...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_app.fileset.gz", "ca2_spa_app.fileset.gz");
         add_status("send stage to Brazil...");
         ftpconn.PutFile("C:\\home\\ca2os\\ca2_spa_stage.fileset.gz", "ca2_spa_stage.fileset.gz");
         }
         catch(...)
         {
         }
         add_status("releasing in Brazil...");
         System.http().ms_get("http://veriwell.com.br/release_ca2_ccauth_spa?authnone=1");*/
         m_bFinished = true;
         m_timeEnd = ::datetime::time::get_current_time();
         m_dwEndTick = ::GetTickCount();
         uint32_t dwMin = (m_dwEndTick - m_dwStartTick) / 1000 / 60;
         uint32_t dwSeg = ((m_dwEndTick - m_dwStartTick) / 1000) % 60;
         string strTime;

         string strEndTime;
         m_timeEnd.FormatGmt(strEndTime, "%Y-%m-%d %H-%M-%S");
         add_status("Build started at " + m_strStartTime);
         add_status("Build ending at " + strEndTime);
         strTime.Format("Build took: %dm %ds", dwMin, dwSeg);
         add_status(strTime);
         m_strBuildTook = strTime;
         add_status(unitext("Finished!! Thank you!!"));
         add_status("");
         add_status("");
         add_status("");
         add_status("");
         add_status("");
         m_evFinish.SetEvent();
         if(m_bLoop)
         {
            m_iStep = 1;
            goto restart;
         }
      }

      return 0;


   }




   void production::compress(const char * lpcszRelative)
   {
      string strStatus;
      strStatus.Format("compressing %s", System.file().name_(lpcszRelative));
      add_status(strStatus);
      string strSrcFile = System.dir().path(m_strVrel, lpcszRelative);
      if(::str::ends_ci(lpcszRelative, ".dll")
         || ::str::ends_ci(lpcszRelative, ".exe")
         || ::str::ends_ci(lpcszRelative, ".ocx")
         || ::str::ends_ci(lpcszRelative, ".cab"))
      {

         string strStatus;
         strStatus.Format("signing %s", System.file().name_(lpcszRelative));
         add_status(strStatus);

         string strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strSrcFile + "\"";
         System.process().synch(strCmd);

         add_status("Signing code ...");

      }
      else if(::str::ends_ci(lpcszRelative, ".sys"))
      {

         string strStatus;
         strStatus.Format("signing driver %s", System.file().name_(lpcszRelative));
         add_status(strStatus);

         string strCmd = "\"" + m_strSignTool + "\" sign /v /ac \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strSrcFile + "\"";
         System.process().synch(strCmd);

         add_status("Signing driver code ...");

      }
      int32_t iRetry = 0;
retry2:
      try
      {
         System.compress().bz(get_app(), System.dir().path(m_strCCAuth, lpcszRelative) + ".bz", strSrcFile);
      }
      catch(...)
      {
         if(iRetry < 5)
         {
            iRetry++;
            Sleep(1984 + 1977);
            goto retry2;
         }
         else
         {
            add_status("build failure compressing " + string(lpcszRelative));
         }
      }
      strStatus.Format("%s compressed", System.file().name_(lpcszRelative));
      add_status(strStatus);
   }


   void production::compress()
   {
      string strStatus;
      strStatus = "Compressing";
      add_status(strStatus);

      string strVrel;
      strVrel = "C:\\ca2\\vrel\\stage\\" + m_strFormatBuild;

      string strCCAuth;
      strCCAuth = "C:\\home\\ca2os\\ca2_spa\\stage\\" + m_strFormatBuild;

      string strRelative;
      string strBz;
      string strUn;
      int32_t i = 0;
      single_lock sl(&m_mutexCompress, TRUE);
      for(;i < m_straFiles.get_size();i++)
      {
         string & strFile = m_straFiles[i];
         if(System.file().extension(strFile) == "zip")
         {
         }
         else if(Application.dir().is(strFile))
         {
            continue;
         }
         else if(System.file().extension(strFile) == "pdb")
         {
            continue;
         }
         else if(System.file().extension(strFile) == "ilk")
         {
            continue;
         }
         //strStatus.Format("compressing %s", strFile);
         //add_status(strStatus);
         m_straCompress.add(strFile.Mid(m_iBaseLen));
      }
      sl.unlock();

      uint32_t uiProcessorCount = get_current_process_affinity_order();

      array < compress_thread * > threada;
      if(uiProcessorCount == 0)
      {
         while(compress_next())
         {
         }
      }
      else
      {
         smart_pointer_array < manual_reset_event > eventa(get_app());
         eventa.set_size_create(uiProcessorCount);
         sync_object_ptra syncobjectptra;
         for(uint32_t ui = 0; ui < uiProcessorCount; ui++)
         {
            compress_thread * pthread = new compress_thread(this, eventa(ui));
            threada.add(pthread);
            pthread->m_dwThreadAffinityMask = 1 << ui;
            pthread->m_bAutoDelete = FALSE;
            pthread->m_p->m_bAutoDelete = FALSE;
            pthread->begin();
            syncobjectptra.add(dynamic_cast < waitable * > (eventa(ui).m_p));
         }
         multi_lock ml(syncobjectptra);
         ml.lock();
         add_status("finished multi-threaded compression task");
         Sleep(584);
      }
      /*while(threada.get_size() > 0)
      {
      try
      {
      delete threada.last_element();
      }
      catch(...)
      {
      }
      threada.remove_last();
      }*/
   }

   bool production::compress_next()
   {
      single_lock sl(&m_mutexCompress, TRUE);
      if(m_straCompress.get_size() <= 0)
         return false;
      string strNext = m_straCompress[0];
      m_straCompress.remove_at(0);
      sl.unlock();
      compress(strNext);
      return true;
   }


   production::compress_thread::compress_thread(production * pproduction, manual_reset_event * peventFinished) :
      element(pproduction->get_app()),
      thread(pproduction->get_app()),
      m_pevFinished(peventFinished)
   {
      m_pevFinished->ResetEvent();
      m_pproduction = pproduction;
   }

   int32_t production::compress_thread::run()
   {

      SetThreadAffinityMask(::GetCurrentThread(), m_dwThreadAffinityMask);

      set_thread_priority(::core::scheduling_priority_highest);

      while(m_pproduction->compress_next())
      {
      }

      m_pevFinished->SetEvent();

      return 0;

   }

   /*void production::compress()
   {
   string strStatus;
   strStatus = "Compressing";
   add_status(strStatus);

   int32_t i = 0;
   while(i < m_straCC.get_size())
   {
   string strUrl;
   string strVar;
   strVar.Empty();
   m_straCC.implode(strVar, ",", i, min(8, m_straCC.get_size() - i));
   strUrl = "http://api.ca2.cc/spaignition/compress?file=";
   strUrl += strVar;

   strStatus.Format("compress step: %d", i);
   add_status(strStatus);

   

   
   System.http().ms_download(strUrl, 
   System.dir().element("time\\spaignition_update.txt"), NULL, post, headers, System.user()->get_user());
   i += 8;
   }
   }*/


   bool  production::sync_source(const char * psz, const char * pszRevision)
   {
      string strStatus;
      strStatus.Format("Updating source: %s ...", psz);
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
      if(pszRevision != NULL && pszRevision[0] != '\0')
      {
         str.Format("svn update --revision %s %s", pszRevision, System.dir().path(strBase, psz));
      }
      else
      {
         str.Format("svn update %s", System.dir().path(strBase, psz));
      }
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
         str.Format("%d Updating source: %s ...", i, psz);
         add_status(str);
         i++;
      }
      return true;
   }

   bool production::commit_for_new_build_and_new_release()
   {
      string strStatus;
      strStatus = unitext("Commit for new Build and new Release!!");
      add_status(strStatus);


      for(int32_t i = 0; i < m_straRoot.get_size(); i++)
      {

         if(!commit_source(m_straRoot[i]))
            return false;

      }

      return true;
   }

   bool production::commit_source(const char * psz)
   {
      string strStatus;
      strStatus = unitext("Commit ") + psz;
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
      str.Format("svn commit --force-log --encoding utf-8 --file %s %s", 
         System.dir().path(m_strBase, "app\\this_version_info.txt"),
         System.dir().path(strBase, psz));
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
         str.Format("%d: Commit for new Build and new Release : %s ...", i, psz);
         add_status(str);
         i++;
      }
      return true;
   }

   bool production::get_file_list(const char * pszBase, const char * pszDir, string & strRemote, stringa & stra, stringa & straTitle, stringa & straRelative, bool bFileSet)
   {
      string strBase(pszBase);
      string strRelease;
      string strDirParam(pszDir);
      string strLocal(strDirParam);
      strLocal.replace("/", "\\");
      if(strLocal.Right(1) != "\\") strLocal += "\\";
      strRemote = strDirParam;
      strRemote.replace("\\", "/");
      if(strRemote.Right(1) != "/") strRemote += "/";
      if(strRemote.Left(1) != "/") strRemote = "/" + strRemote;
      strRelease = System.dir().path(strBase, strLocal);
      if(bFileSet)
      {
         string strFile;
         string strTitle;
         string strRelative;
         ::str::ends_eat(strRelease, "\\");
         ::str::ends_eat(strRelease, "/");
         ::str::ends_eat(strLocal, "\\");
         ::str::ends_eat(strLocal, "/");
         strFile = strRelease + ".expand_fileset";
         strTitle = System.file().name_(strRelease) + ".expand_fileset";
         strRelative = strLocal + ".expand_fileset";
         strRelative.replace("/", "\\");

         stringa stra1;
         stringa stra2;
         Application.dir().rls(strRelease, &stra1, NULL, &stra2);
         for(int32_t i = 0; i < stra1.get_size();)
         {
            if(stra1[i].find("\\.svn\\") >= 0 || (stra1[i].get_length() < 5 || stra1[i].Right(5) == "\\.svn"))
            {
               stra1.remove_at(i);
               stra2.remove_at(i);
            }
            else
            {
               i++;
            }
         }
         System.file().dtf(strFile, stra1, stra2, get_app());
         stra.add(strFile);
         straTitle.add(strTitle);
         straRelative.add(strRelative);
      }
      else
      {
         Application.dir().rls(strRelease, &stra, &straTitle, &straRelative);
      }
      strRemote = strRemote + "/stage/" + m_strFormatBuild + "/";
      return true;
   }



   void production::release_production()
   {
      m_strStatus = "generating index file and releasing...";
      add_status(m_strStatus);


      strsize iBaseLen = m_strBase.get_length();
      if(m_strBase.Right(1) != "/" && m_strBase.Right(1) != "\\")
         iBaseLen++;

      stringa straStageDir;

      straStageDir.add(System.dir().path(m_strBase, "stage/x86"));
      straStageDir.add(System.dir().path(m_strBase, "stage/x64"));

      string strRelative;
      string strBz;
      string strUn;
      string strRelease;
      string strReleaseNew;
      var varUnSize;
      string strMd5;
      var varBzSize;
      string strContents;
      int32_t i = 0;
      for(;i < m_straFiles.get_size();i++)
      {
         string & strFile = m_straFiles[i];
         if(::str::ends_ci(strFile, ".zip"))
         {
         }
         else if(Application.dir().is(strFile))
         {
            continue;
         }
         if(straStageDir.str_find_first_begins_ci(strFile) >= 0
            && !m_straStageDirAcceptedFileExtensions.contains(
            System.file().extension(strFile)))
         {
            continue;
         }
         //strStatus.Format("compressing %s", strFile);
         //add_status(strStatus);
         strRelative = strFile.Mid(iBaseLen);
         strBz = System.dir().path(m_strCCAuth, strRelative) + ".bz";
         strUn = System.dir().path(m_strVrel, strRelative);
         strMd5 = System.file().md5(strUn);
         varUnSize = System.file().length(strUn);
         varBzSize = System.file().length(strBz);
         strRelease = System.dir().path(m_strCCVrel, strRelative);
         strRelease += ".bz.";
         strRelease += strMd5;
         strReleaseNew = System.dir().path(m_strCCVrelNew, strRelative);
         strReleaseNew += ".bz.";
         strReleaseNew += strMd5;
         strContents += strRelative;
         strContents += ",";
         strContents += varUnSize.get_string();
         strContents += ",";
         strContents += strMd5;
         strContents += ",";
         strContents += varBzSize.get_string();
         strContents += "\n";
         if(!Application.file().exists(strRelease))
         {
            Application.file().copy(strRelease, strBz, false);
         }
         Application.file().copy(strReleaseNew, strBz, false);
      }

      strRelative = "app\\stage\\metastage\\index-" + m_strFormatBuild + ".spa";
      string strIndex = System.dir().path(m_strVrel, strRelative);
      Application.file().put_contents(
         strIndex,
         strContents);

      m_strIndexMd5 = System.file().md5(strIndex);

      strBz = System.dir().path(m_strCCAuth, strRelative) + ".bz"; 
      ::DeleteFileW(::str::international::utf8_to_unicode(strBz));
      compress(strRelative);

      string strRelativeMd5 = "app\\stage\\metastage\\index-" + m_strFormatBuild + ".md5";
      strMd5 =  System.dir().path(m_strVrel, strRelativeMd5);
      Application.file().put_contents(strMd5 , m_strIndexMd5);

      //string strStage = System.dir().path("C:\\home\\ca2os\\ca2_spa\\" + m_strVersionShift, strRelative) + ".bz"; 
      //::DeleteFileW(::str::international::utf8_to_unicode(
      // strStage));
      //System.file().copy(strStage, strBz);
      strRelease = System.dir().path(m_strCCVrel, strRelative) + ".bz";
      //::DeleteFileW(::str::international::utf8_to_unicode(
      // strRelease));
      Application.file().copy(strRelease, strBz);
      strRelease = System.dir().path(m_strCCVrel, strRelativeMd5);
      Application.file().copy(strRelease, strMd5);
      strReleaseNew = System.dir().path(m_strCCVrelNew, strRelative) + ".bz";
      //::DeleteFileW(::str::international::utf8_to_unicode(
      // strRelease));
      Application.file().copy(strReleaseNew, strBz);
      strReleaseNew = System.dir().path(m_strCCVrelNew, strRelativeMd5);
      Application.file().copy(strReleaseNew, strMd5);
   }

   void production::generate_appmatter_spa()
   {

      for(int32_t i = 0; i < m_straRoot.get_size(); i++)
      {
         generate_appmatter_spa(m_straRoot[i]);
      }

   }

   void production::generate_appmatter_spa(const char * pszRoot)
   {

      stringa straRelative;

      string strBase = System.dir().path(m_strBase, pszRoot, "appmatter");

      Application.dir().ls_dir(strBase, NULL, &straRelative);

      for(int32_t i = 0; i < straRelative.get_count(); i++)
      {
         if(::str::begins(straRelative[i], "_"))
         {
            generate_appmatter_spa_folder(pszRoot, straRelative[i]);
         }
         else
         {
            generate_appmatter_spa_locale(pszRoot, straRelative[i]);
         }
      }

   }

   void production::generate_appmatter_spa_folder(const char * pszRoot, const char * pszRelative)
   {

      stringa straRelative;

      string strBase = System.dir().path(m_strBase, pszRoot, "appmatter");

      strBase = System.dir().path(strBase, pszRelative);

      Application.dir().ls_dir(strBase, NULL, &straRelative);

      for(int32_t i = 0; i < straRelative.get_count(); i++)
      {
         if(::str::begins(straRelative[i], "_") && straRelative[i] != "_std")
         {
            generate_appmatter_spa_folder(pszRoot, System.dir().path(pszRelative, straRelative[i]));
         }
         else
         {
            generate_appmatter_spa_locale(pszRoot, System.dir().path(pszRelative, straRelative[i]));
         }
      }

   }

   void production::generate_appmatter_spa_locale(const char * pszRoot, const char * pszRelative)
   {
      stringa straRelative;

      string strBase = System.dir().path(m_strBase, pszRoot, "appmatter");

      strBase = System.dir().path(strBase, pszRelative);

      Application.dir().ls_dir(strBase, NULL, &straRelative);

      for(int32_t i = 0; i < straRelative.get_count(); i++)
      {

         generate_appmatter_spa_style(pszRoot, System.dir().path(pszRelative, straRelative[i]));

      }

   }

   void production::generate_appmatter_spa_style(const char * pszRoot, const char * pszRelative)
   {

      stringa straRelative;

      string strBase = System.dir().path(m_strBase, pszRoot, "appmatter");

      strBase = System.dir().path(strBase, pszRelative);

      Application.dir().ls_dir(strBase, NULL, &straRelative);

      for(int32_t i = 0; i < straRelative.get_count(); i++)
      {

         generate_appmatter_spa(pszRoot, System.dir().path(pszRelative, straRelative[i]));

      }

   }


   void production::generate_appmatter_spa(const char * pszRoot, const char * pszRelative)
   {

      string strStatus;

      strStatus = "updating appmatter spa spfile : " + string(pszRoot) + " : " + string(pszRelative);

      add_status(strStatus);

      stringa straFiles;

      stringa straRelative;

      stringa stra1;

      stringa stra2;

      Application.dir().rls(System.dir().path(m_strBase, System.dir().path(pszRoot, "appmatter", pszRelative)), &straFiles, NULL, &straRelative);

      strsize iBaseLen = m_strBase.get_length();

      if(m_strBase.Right(1) != "/" && m_strBase.Right(1) != "\\")
         iBaseLen++;

      string strRelative;
      string strFile;
      string strBz;
      string strUn;
      var varUnSize;
      string strMd5;
      var varBzSize;
      string strContents;
      int32_t i = 0;
      for(;i < straFiles.get_size();i++)
      {
         string & strFile = straFiles[i];
         if(System.file().extension(strFile).CompareNoCase("zip") == 0)
         {
         }
         else if(Application.dir().is(strFile))
         {
            continue;
         }
         else if(System.file().extension(strFile) == "pdb")
         {
            continue;
         }
         else if(System.file().extension(strFile) == "ilk")
         {
            continue;
         }
         else if(System.file().extension(strFile) == "spa")
         {
            continue;
         }
         else if(System.file().extension(strFile) == "expand_fileset")
         {
            continue;
         }
         //strStatus.Format("compressing %s", strFile);
         //add_status(strStatus);
         strRelative = strFile.Mid(iBaseLen);
         strContents += strRelative;
         strContents += "\n";

         m_straFiles.add(strFile);
         m_straTitle.add(System.file().title_(strFile));
         m_straRelative.add(strRelative);

         stra1.add(strFile);
         stra2.add(straRelative[i]);

      }

      strRelative = string(pszRoot) +  + "\\appmatter\\" + string(pszRelative);

      strFile = System.dir().path(m_strBase, "app\\stage\\metastage", strRelative + ".expand_fileset.spa");

      m_straFiles.add(strFile);

      m_straTitle.add(System.file().title_(strFile));

      m_straRelative.add("app\\stage\\metastagez\\" + strRelative + ".expand_fileset.spa");

      Application.file().put_contents(strFile, strContents);


      strFile = System.dir().path(m_strBase, strRelative + ".expand_fileset");


      System.file().dtf(strFile, stra1, stra2, get_app());
      m_straFiles.add(strFile);
      m_straTitle.add(System.file().title_(strFile));
      m_straRelative.add(strRelative + ".expand_fileset.spa");


      //get_file_list(m_strBase, strRelative, m_strRemote, m_straFiles, m_straTitle, m_straRelative);

      //get_file_list(m_strBase, strRelative, m_strRemote, m_straFiles, m_straTitle, m_straRelative, true);

   }

   /*#!/usr/bin/env python
   import os, sys, re, hashlib, zipfile, base64, M2Crypto

   def signDir(source_dir, key_file, output_file):
   source_dir = os.path.abspath(source_dir)

   # Build file list
   filelist = []
   for dirpath, dirs, files in os.walk(source_dir):
   for file in files:
   abspath = os.path.join(dirpath, file)
   relpath = os.path.relpath(abspath, source_dir).replace('\\', '/')
   handle = open(abspath, 'rb')
   filelist.append((abspath, relpath, handle.read()))
   handle.close()

   # Generate manifest.mf and zigbert.sf data
   manifest_sections = []
   signature_sections = []
   def digest(data):
   md5 = hashlib.md5()
   md5.update(data)
   sha1 = hashlib.sha1()
   sha1.update(data)
   return 'Digest-Algorithms: MD5 SHA1\nMD5-Digest: %s\nSHA1-Digest: %s\n' % \
   (base64.b64encode(md5.digest()), base64.b64encode(sha1.digest()))
   def departament(manifest, signature):
   manifest_sections.append(manifest)
   signature_sections.append(signature + digest(manifest))
   departament('Manifest-Version: 1.0\n', 'Signature-Version: 1.0\n')
   for filepath, relpath, data in filelist:
   departament('Name: %s\n%s' % (relpath, digest(data)), 'Name: %s\n' % relpath)
   manifest = '\n'.join(manifest_sections)
   signature = '\n'.join(signature_sections)

   # Generate zigbert.rsa (detached zigbert.sf signature)
   handle = open(key_file, 'rb')
   key_data = handle.read()
   handle.close()
   certstack = M2Crypto.X509.X509_Stack()
   first = True
   certificates = re.finditer(r'-----BEGIN CERTIFICATE-----.*?-----END CERTIFICATE-----', key_data, re.S)
   # Ignore first certificate, we will sign with this one. Rest of them needs to
   # be added to the stack manually however.
   certificates.next()
   for match in certificates:
   certstack.push(M2Crypto.X509.load_cert_string(match.group(0)))

   mime = M2Crypto.SMIME.SMIME()
   mime.load_key(key_file)
   mime.set_x509_stack(certstack)
   pkcs7 = mime.sign(M2Crypto.BIO.MemoryBuffer(signature),
   M2Crypto.SMIME.PKCS7_DETACHED | M2Crypto.SMIME.PKCS7_BINARY)
   pkcs7_buffer = M2Crypto.BIO.MemoryBuffer()
   pkcs7.write_der(pkcs7_buffer)

   # Write everything into a ZIP file, with zigbert.rsa as first file
   zip = zipfile.ZipFile(output_file, 'w', zipfile.ZIP_DEFLATED)
   zip.writestr('META-INF/zigbert.rsa', pkcs7_buffer.read())
   zip.writestr('META-INF/zigbert.sf', signature)
   zip.writestr('META-INF/manifest.mf', manifest)
   for filepath, relpath, data in filelist:
   zip.writestr(relpath, data)

   if __name__ == '__main__':
   if len(sys.argv) < 4:
   print 'Usage: %s source_dir key_file output_file' % sys.argv[0]
   sys.exit(2)
   signDir(sys.argv[1], sys.argv[2], sys.argv[3])*/











   /*  def digest(data):
   md5 = hashlib.md5()
   md5.update(data)
   sha1 = hashlib.sha1()
   sha1.update(data)
   return 'Digest-Algorithms: MD5 SHA1\nMD5-Digest: %s\nSHA1-Digest: %s\n' % \
   (base64.b64encode(md5.digest()), base64.b64encode(sha1.digest()))
   */
   string production::xpi_digest(primitive::memory & mem)
   {
      primitive::memory memMd5;
      primitive::memory memSha1;
      ::hex_to_memory(memMd5, System.crypto().md5(mem));
      ::hex_to_memory(memSha1, System.crypto().sha1(mem));
      return string("Digest-Algorithms: MD5 SHA1\n") +
         "MD5-Digest: " + System.base64().encode(memMd5) +  "\n" +
         "SHA1-Digest: " + System.base64().encode(memSha1) + "\n";

   }


   void production::xpi_section(const char * pszManifest, const char * pszSignature)
   {
      m_straManifest.add(pszManifest);
      primitive::memory memManifest(pszManifest);
      m_straSignature.add(string(pszSignature) + xpi_digest(memManifest));
   }

   void production::add_path(const char * pszDir, const char * pszRelative)
   {
      m_straRelative.add(pszRelative);
      m_straPath.add(System.dir().path(pszDir, pszRelative));
   }

   void production::xpi_sign_dir(const char * pszDir)
   {

      m_straRelative.remove_all();
      m_straPath.remove_all();

      add_path(pszDir, "META-INF\\zigbert.rsa");
      add_path(pszDir, "install.rdf");
      add_path(pszDir, "chrome.manifest");
      add_path(pszDir, "plugins\\app-install.exe");
      add_path(pszDir, "plugins\\npca2.dll");
      add_path(pszDir, "plugins\\base.dll");
      add_path(pszDir, "plugins\\os.dll");
      add_path(pszDir, "plugins\\msvcp120d.dll");
      add_path(pszDir, "plugins\\msvcr120d.dll");
      add_path(pszDir, "plugins\\draw2d_gdiplus.dll");
      add_path(pszDir, "skin\\classic\\ca2-5c-32.png");
      add_path(pszDir, "META-INF\\manifest.mf");
      add_path(pszDir, "META-INF\\zigbert.sf");

      string strSignerPath = "X:\\sensitive\\sensitive\\certificate\\npca2signer.pem";
      string strKeyPath = "X:\\sensitive\\sensitive\\certificate\\npca2key.pem";
      string strOthersPath = "X:\\sensitive\\sensitive\\certificate\\npca2others.pem";


      // Generate manifest.mf and zigbert.sf data

      m_straManifest.remove_all();
      m_straSignature.remove_all();

      string strComment = "Created-By: ca2 production "+ m_strBuild + "\nComments: PLEASE DO NOT EDIT THIS FILE. YOU WILL BREAK IT.\n";


      xpi_section("Manifest-Version: 1.0\n" + strComment, "Signature-Version: 1.0\n" + strComment);

      primitive::memory mem;

      for(int32_t i = 0; i < m_straPath.get_count(); i++)
      {
         string strRelative = m_straRelative[i];
         if(::str::begins_ci(strRelative, "META-INF\\"))
            continue;
         strRelative.replace("\\", "/");
         mem.allocate(0);
         Application.file().as_memory(m_straPath[i], mem);
         xpi_section("Name: " + strRelative + "\n" + xpi_digest(mem), "Name: " + strRelative + "\n");
      }


      string strManifest = m_straManifest.implode("\n");
      string strSignature = m_straSignature.implode("\n");

      Application.file().put_contents(System.dir().path(pszDir, "META-INF/manifest.mf"), strManifest);
      Application.file().put_contents(System.dir().path(pszDir, "META-INF/zigbert.sf"), strSignature);


      X509 * signer = NULL;
      {
         string strSigner = Application.file().as_string(strSignerPath);
         BIO * pbio = BIO_new_mem_buf((void *) (LPCTSTR) strSigner, (int32_t) strSigner.get_length());
         //signer = PEM_read_bio_X509_AUX(pbio, NULL, 0, NULL);
         signer = PEM_read_bio_X509(pbio, NULL, 0, NULL);
         BIO_free(pbio);
      }

      EVP_PKEY * pkey;
      {
         string strKey = Application.file().as_string(strKeyPath);
         BIO * pbio = BIO_new_mem_buf((void *) (LPCTSTR) strKey, (int32_t) strKey.get_length());
         pkey = PEM_read_bio_PrivateKey(pbio, NULL, NULL, NULL);
         BIO_free(pbio);
      }


      stack_st_X509 * pstack509 = NULL;
      {
         string strOthers = Application.file().as_string(strOthersPath);
         array < X509 * > xptra;
         strsize iStart = 0;
         strsize iFind;
         string strEnd = "-----END CERTIFICATE-----";
         string strCertificate;
         strsize iEndLen = strEnd.get_length();
         ::count iCount = 0;
         while((iFind = strOthers.find("-----BEGIN CERTIFICATE-----", iStart)) >= 0)
         {
            strsize iEnd = strOthers.find(strEnd, iFind);
            if(iEnd < 0)
               break;
            strCertificate = strOthers.Mid(iFind, iEnd + iEndLen - iFind);
            X509 * x;
            BIO * pbio = BIO_new(BIO_s_mem());
            BIO_puts(pbio, strCertificate);
            //x = PEM_read_bio_X509_AUX(pbio, NULL, 0, NULL);
            x = PEM_read_bio_X509(pbio, NULL, 0, NULL);
            BIO_free(pbio);
            if(x == NULL)
            {
               return;
            }
            xptra.add(x);
            iCount++;
            iStart = iEnd + iEndLen;
         }
         pstack509 = sk_X509_new_null();

         for(int32_t i = 0; i < xptra.get_count(); i++)
         {
            sk_X509_push(pstack509, xptra[i]);
         }
      }

      BIO * input = BIO_new_mem_buf((void *) (LPCTSTR) strSignature, (int32_t) strSignature.get_length());

      PKCS7 * pkcs7 = PKCS7_sign(signer, pkey, pstack509, input, PKCS7_BINARY | PKCS7_DETACHED);

      BIO_free(input);
      sk_X509_free(pstack509);
      EVP_PKEY_free(pkey);
      X509_free(signer);

      BIO * output = BIO_new(BIO_s_mem());

      i2d_PKCS7_bio(output, pkcs7);

      char * pchData = NULL;
      long count = BIO_get_mem_data(output, &pchData);

      Application.file().put_contents(System.dir().path(pszDir, "META-INF/zigbert.rsa"), pchData, count);

      BIO_free(output);
      PKCS7_free(pkcs7);

   }



   bool production::release_npca2(const char * pszPlatform)
   {

      string strPlatform(pszPlatform);

      string strDir;
      strDir = System.dir().path(m_strBase, "time/npca2/" + strPlatform);


      string strNpca2Version;

      strNpca2Version.Format(
         "%d.%d%02d.%d%02d.%d",
         atoi(m_strFormatBuild.Mid(0, 4)),
         atoi(m_strFormatBuild.Mid(5, 2)),
         atoi(m_strFormatBuild.Mid(8, 2)),
         atoi(m_strFormatBuild.Mid(11, 2)),
         atoi(m_strFormatBuild.Mid(14, 2)),
         atoi(m_strFormatBuild.Mid(17, 2))
         );

      string strVersionUrl;
      if(m_eversion == version_basis)
      {
         strVersionUrl = "/basis";
      }

      string strChromeManifest = Application.file().as_string(System.dir().path(m_strBase, "nodeapp/stage/matter/npca2/chrome.manifest"));
      strChromeManifest.replace("%BUILD%", strNpca2Version);
      strChromeManifest.replace("%PLATFORM%", strPlatform);
      strChromeManifest.replace("%DOWNLOADSITE%", m_strDownloadSite);
      strChromeManifest.replace("%VERSION%", strVersionUrl);
      Application.file().put_contents(System.dir().path(strDir, "npca2", "chrome.manifest"), strChromeManifest);

      string strIcon;
      if(m_eversion == version_basis)
      {
         strIcon = Application.dir().matter("fluidbasis-5c-32.png");
      }
      else
      {
         strIcon = Application.dir().matter("ca2-5c-32.png");
      }
      Application.file().copy(System.dir().path(strDir, "npca2/skin/classic", "ca2-5c-32.png"), strIcon);

      string strInstall = Application.file().as_string(System.dir().path(m_strBase, "nodeapp/stage/matter/npca2/install.rdf"));
      strInstall.replace("%BUILD%", strNpca2Version);
      strInstall.replace("%PLATFORM%", strPlatform);
      strInstall.replace("%DOWNLOADSITE%", m_strDownloadSite);
      strInstall.replace("%VERSION%", strVersionUrl);
      Application.file().put_contents(System.dir().path(strDir, "npca2", "install.rdf"), strInstall);


      string strWindows = Application.file().as_string(System.dir().path(m_strBase, "nodeapp/stage/matter/npca2/windows.rdf"));
      strWindows.replace("%BUILD%", strNpca2Version);
      strWindows.replace("%PLATFORM%", strPlatform);
      strWindows.replace("%DOWNLOADSITE%", m_strDownloadSite);
      strWindows.replace("%VERSION%", strVersionUrl);
      Application.file().put_contents(System.dir().path(strDir, "windows.rdf"), strWindows);


      add_status("Signing npca2.dll for Firefox ...");
      string strFile = System.dir().path(strDir, "npca2/plugins", "npca2.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/npca2.dll"));
      string strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);



      add_status("Signing app-install.exe for Firefox ...");
      strFile = System.dir().path(strDir, "npca2/plugins", "app-install.exe");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/app-install.exe"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing base.dll for Firefox ...");
      strFile = System.dir().path(strDir, "npca2/plugins", "base.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/base.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing os.dll for Firefox ...");
      strFile = System.dir().path(strDir, "npca2/plugins", "os.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/os.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing msvcr120d.dll for Firefox ...");
      strFile = System.dir().path(strDir, "npca2/plugins", "msvcr120d.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/msvcr120d.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing msvcp120d.dll for Firefox ...");
      strFile = System.dir().path(strDir, "npca2/plugins", "msvcp120d.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/msvcp120d.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      strFile = System.dir().path(strDir, "npca2/plugins", "draw2d_gdiplus.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/draw2d_gdiplus.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing code for Firefox ...");

      System.file().del(System.dir().path(strDir, "npca2.xpi"));

      create_xpi(pszPlatform, false);
      string strVersion;
      if(m_eversion == version_basis)
      {
         strVersion = "\\basis";
      }


      Application.file().copy("C:\\netnodenet\\net\\netseed\\front\\cc\\ca2\\_std\\download\\xpi\\"+strPlatform+strVersion+"\\npca2.xpi", System.dir().path(strDir, "npca2.xpi"));
      Application.file().copy("C:\\netnodenet\\net\\netseed\\front\\cc\\ca2\\_std\\download\\rdf\\"+strPlatform+strVersion+"\\windows.rdf", System.dir().path(strDir, "windows.rdf"));

      return true;
   }

   bool production::create_xpi(const char * pszPlatform, bool bSigned)
   {

      string strPlatform(pszPlatform);

      string strDir;
      strDir = System.dir().path(m_strBase, "time/npca2/" + strPlatform);

      Application.dir().rm(System.dir().path(strDir, "npca2/META-INF"));


      if(bSigned)
      {
         return create_signed_xpi(pszPlatform);
      }
      else
      {
         return create_unsigned_xpi(pszPlatform);
      }

   }

   bool production::create_signed_xpi(const char * pszPlatform)
   {

      string strPlatform(pszPlatform);

      string strDir;
      strDir = System.dir().path(m_strBase, "time/npca2/" + strPlatform);

      add_status("Signing extension ...");

      xpi_sign_dir(System.dir().path(strDir, "npca2/"));

      string str;

      string strXpi = System.dir().path(strDir, "npca2.xpi") ;

      string strPath;

      ::core::process process;

      uint32_t dwExitCode;

      for(int32_t i = 0; i < m_straRelative.get_count(); i++)
      {
         strPath = "zip \"" + strXpi + "\" \"" + m_straRelative[i] + "\"";
         if(!process.create_child_process(strPath, false,  System.dir().path(strDir, "npca2/")))
         {
            uint32_t dw = GetLastError();
            string str;
            str.Format("Error compressing npca2: %d is zip command line utility installed?", dw);
            add_status(str);
            return 0;   
         }
         while(!process.has_exited(&dwExitCode))
         {
            Sleep(284);
            str.Format("%d Compressing npca2 ...", i);
            add_status(str);
         }
      }
      return true;
   }

   bool production::create_unsigned_xpi(const char * pszPlatform)
   {

      string strPlatform(pszPlatform);

      string strDir;
      strDir = System.dir().path(m_strBase, "time/npca2/" + strPlatform);

      add_status("Creating uint32_t extension ...");
      string str;
      uint32_t dwExitCode;
      string strXpi = System.dir().path(strDir, "npca2.xpi") ;
      ::core::process process;
      string strPath = "zip -r -D \""+strXpi+"\" * ";
      if(!process.create_child_process(strPath, false,  System.dir().path(strDir, "npca2/")))
      {
         uint32_t dw = GetLastError();
         string str;
         str.Format("Error compressing npca2: %d is zip command line utilty installed?", dw);
         add_status(str);
         return 0;   
      }
      int32_t i = 1;
      while(!process.has_exited(&dwExitCode))
      {
         Sleep(284);
         str.Format("%d Compressing npca2 ...", i);
         add_status(str);
         i++;
      }

      return true;

   }

   bool production::release_iexca2(const char * pszPlatform)
   {

      string strStatus;
      strStatus.Format("releasing iexca2 " + string(pszPlatform));
      add_status(strStatus);

      string strPlatform(pszPlatform);


      Application.dir().mk(System.dir().path(m_strBase, "time\\iexca2\\"+strPlatform));

      uint32_t dwExitCode;
      string str;
      ::core::process process;
      string strPath;
      strPath = System.dir().path(m_strBase, "nodeapp\\stage\\script\\makecab" + string(pszPlatform) + ".bat");
      if(!process.create_child_process(strPath, false, System.dir().name(strPath)))
      {
         uint32_t dw = GetLastError();
         string str;
         str.Format("Error creating iexca2.cab: %d", dw);
         add_status(str);
         return 0;   
      }
      int32_t i;
      i = 1;
      while(!process.has_exited(&dwExitCode))
      {
         Sleep(5000);
         str.Format("%d Creating iexca2.cab  " + string(pszPlatform) + "...", i);
         add_status(str);
         i++;
      }

      string strVersion;
      if(m_eversion == version_basis)
      {
         strVersion = "\\basis";
      }

      Application.file().copy("C:\\netnodenet\\net\\netseed\\front\\cc\\ca2\\_std\\download\\cab\\" +  strPlatform + strVersion + "\\iexca2.cab", System.dir().path(m_strBase, "time\\iexca2\\"+strPlatform+"\\iexca2.cab"));

      return true;

   }

   bool production::release_crxca2(const char * pszPlatform)
   {

      string strPlatform(pszPlatform);

      string strDir;
      strDir = System.dir().path(m_strBase, "time/crxca2/" + strPlatform + "/crxca2");

      string strCrxca2Version;

      strCrxca2Version.Format(
         "%d.%d%02d.%d%02d.%d",
         atoi(m_strFormatBuild.Mid(0, 4)),
         atoi(m_strFormatBuild.Mid(5, 2)),
         atoi(m_strFormatBuild.Mid(8, 2)),
         atoi(m_strFormatBuild.Mid(11, 2)),
         atoi(m_strFormatBuild.Mid(14, 2)),
         atoi(m_strFormatBuild.Mid(17, 2))
         );

      string strManifestJson = Application.file().as_string(System.dir().path(m_strBase, "nodeapp/stage/matter/crxca2/manifest.json"));
      strManifestJson.replace("%BUILD%", strCrxca2Version);
      strManifestJson.replace("%PLATFORM%", strPlatform);
      strManifestJson.replace("%DOWNLOADSITE%", m_strDownloadSite);
      Application.file().put_contents(System.dir().path(strDir,  "manifest.json"), strManifestJson);

      string strIcon;
      if(m_eversion == version_basis)
      {
         strIcon = Application.dir().matter("fluidbasis-5c-32.png");
      }
      else
      {
         strIcon = Application.dir().matter("ca2-5c-32.png");
      }
      Application.file().copy(System.dir().path(strDir, "ca2-5c-32.png"), strIcon);

      add_status("Signing npca2.dll for Chrome ...");
      string strFile = System.dir().path(strDir, "npca2.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/npca2.dll"));
      string strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing app-install.exe for Chrome ...");
      strFile = System.dir().path(strDir, "app-install.exe");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/app-install.exe"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing base.dll for Chrome ...");
      strFile = System.dir().path(strDir, "base.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/base.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing os.dll for Chrome ...");
      strFile = System.dir().path(strDir, "os.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/os.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing msvcp120d.dll for Chrome ...");
      strFile = System.dir().path(strDir, "msvcp120d.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/msvcp120d.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing msvcr120d.dll for Chrome ...");
      strFile = System.dir().path(strDir, "msvcr120d.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/msvcr120d.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Signing draw2d_gdiplus.dll for Chrome ...");
      strFile = System.dir().path(strDir, "draw2d_gdiplus.dll");
      Application.file().copy(strFile, System.dir().path(m_strVrel, "stage/" + strPlatform + "/draw2d_gdiplus.dll"));
      strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      System.process().synch(strCmd);

      add_status("Creating crxca2.crx for Chrome ...");

      WCHAR * pwsz;

      if(FAILED(SHGetKnownFolderPath(FOLDERID_ProgramFilesX86, 0, NULL, &pwsz)))
      {
   
         add_status("Chrome not found...");

         return false;

      }

      strCmd = "\"";

      strCmd += pwsz;

      CoTaskMemFree(pwsz);

      strCmd += "\\Google\\Chrome\\Application\\chrome.exe\" --no-message-box --pack-extension=\"" +strDir + "\" --pack-extension-key=\"X:\\sensitive\\sensitive\\certificate\\npca2pk.pem\"";

      System.process().synch(strCmd);

      string strVersion;

      if(m_eversion == version_basis)
      {
         strVersion = "\\basis";
      }


      Application.file().copy("C:\\netnodenet\\net\\netseed\\front\\cc\\ca2\\_std\\download\\crx\\"+strPlatform+strVersion+"\\crxca2.crx", System.dir().path(System.dir().name(strDir), "crxca2.crx"));

      return true;
   }


   void production::add_status(const char * psz)
   {
      single_lock sl(&m_mutexStatus, TRUE);
      m_straStatus.add(psz);
      TRACE0(psz);
      m_pview->send_message(WM_USER, 1);
   }

   void production::change_status(const char * psz)
   {
      single_lock sl(&m_mutexStatus, TRUE);
      if(m_straStatus.get_count() == 0)
      {
         m_straStatus.add(psz);
      }
      else
      {
         m_straStatus.last_element() = psz;
      }
      m_pview->send_message(WM_USER, 1);
   }



   production::release::release(production * pproduction) : 
      ::element(pproduction->get_app()),
      thread(pproduction->get_app())
   {
      m_pproduction = pproduction;
      m_pproduction->m_iRelease++;
      m_pproduction->OnUpdateRelease();
   }

   bool production::release::initialize_instance()
   {
      return true;
   }
   int32_t production::release::run()
   {
      string str;
      property_set set(get_app());
      set["disable_ca2_sessid"] = true;
      Application.http().get(m_strRelease, str, set);
      m_pproduction->m_iRelease--;
      m_pproduction->OnUpdateRelease();
      return 0;
   }


   void production::OnUpdateRelease()
   {
      if(m_iRelease == 0 && m_bEndProduction && !m_bEndStatus)
      {
         m_bEndStatus = true;
         m_evFinish.wait();
         m_evFinish.ResetEvent();
         add_status("All releases have been commanded and production has deemed ended!!.!.!!.");
         
         
         property_set set;
         string strEndTime;
         m_timeEnd.FormatGmt(strEndTime, "%Y-%m-%d %H-%M-%S");
         if(m_eversion == version_basis)
         {
            set["post"]["new_status"] = "<div style=\"display: block; background-color: #FFE0FF; \"><h2 style=\"margin-bottom:0px; color: #FF55CC;\">" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #882255; display: block; margin-bottom: 1.5em;\">"+m_strBuildTook+" and finished at "+ strEndTime + "<br>New release of <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://laboratory.ca2.cc/\" class=\"fluidbasis\" >laboratory.ca2.cc</a> or <a href=\"http://warehouse.ca2.cc/\" class=\"fluidbasis\" >warehouse.ca2.cc</a> for simple gateway implementations.</span></div>";
         }
         else
         {
            set["post"]["new_status"] = "<div style=\"display: block; background-color: #E0FFCC; \"><h2 style=\"margin-bottom:0px; color: #55CCAA;\">" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">"+m_strBuildTook+" and finished at "+ strEndTime + "<br>New release of <a href=\"http://ca2.cc/\">stage</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://desktop.ca2.cc/\">desktop.ca2.cc</a> or <a href=\"http://store.ca2.cc/\">store.ca2.cc</a> for simple gateway implementations.</span></div";
         }
         string str;
         Application.http().get("http://api.ca2.cc/status/insert", str, set);

         string strTwit;

         strTwit = "New build " + m_strBuild;

         if(m_iGlobalRetry > 0)
         {

            if(m_iGlobalRetry == 1)
            {
               strTwit += " with 1 retry";
            }
            else
            {
               strTwit += " with " + ::str::from(m_iGlobalRetry) + " retries";
            }
         }

         strTwit += " : http://status.ca2.cc/" + System.url().url_encode(m_strStatusEmail);

         twitter_twit(strTwit);

         m_bReleased = true;

         defer_quit();

      }
      else if(m_iRelease > 0)
      {
         string strStatus;
         strStatus.Format("There are %d releases in command list!!", m_iRelease);
         add_status(strStatus);
      }
   }


   bool production::twitter_auth()
   {


      ::hi5::twit twitterObj(get_app());


      string tmpStr( "" );
      string replyMsg( "" );


      /* OAuth flow begins */
      /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      string strPathKey = Application.dir().userappdata("twitterClient_token_key"+::str::from(m_eversion)+".txt");
      string strPathSecret = Application.dir().userappdata("twitterClient_token_secret"+::str::from(m_eversion)+".txt");
      /* Step 1: Check if we alredy have OAuth access token from a previous run */
      //    char szKey[1024];
      string myOAuthAccessTokenKey = Application.file().as_string(strPathKey);
      string myOAuthAccessTokenSecret = Application.file().as_string(strPathSecret);

      if( myOAuthAccessTokenKey.has_char() && myOAuthAccessTokenSecret.has_char() )
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey( myOAuthAccessTokenKey );
         twitterObj.get_oauth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
      }
      else
      {
         /* Step 2: Get request token key and secret */
         twitterObj.oAuthRequestToken( tmpStr );

         /* Step 3: Ask user to visit web link and get PIN */
         string szOAuthVerifierPin;

         ::hi5::twitter::authorization authapp(get_app(), tmpStr, "twitter\\authorization.xhtml", true);
         szOAuthVerifierPin = authapp.get_pin();

         tmpStr = szOAuthVerifierPin;
         twitterObj.get_oauth().setOAuthPin( tmpStr );

         /* Step 4: Exchange request token with access token */
         twitterObj.oAuthAccessToken();

         /* Step 5: Now, save this access token key and secret for future use without PIN */
         twitterObj.get_oauth().getOAuthTokenKey( myOAuthAccessTokenKey );
         twitterObj.get_oauth().getOAuthTokenSecret( myOAuthAccessTokenSecret );

         /* Step 6: Save these keys in a file or wherever */

         Application.file().put_contents(strPathKey, myOAuthAccessTokenKey);
         Application.file().put_contents(strPathSecret, myOAuthAccessTokenSecret);

      }

      return true;
   }



   string production::twitter_twit(const char * pszMessage)
   {

      int32_t iRetry = 0;

Retry2:

      ::hi5::twit twitterObj(get_app());
      string tmpStr( "" );
      string replyMsg( "" );

      /* OAuth flow begins */
      /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      string strPathKey = Application.dir().userappdata("twitterClient_token_key" + ::str::from(m_eversion) + ".txt");
      string strPathSecret = Application.dir().userappdata("twitterClient_token_secret"+::str::from(m_eversion)+".txt");
      /* Step 1: Check if we alredy have OAuth access token from a previous run */
      //    char szKey[1024];
      string myOAuthAccessTokenKey = Application.file().as_string(strPathKey);
      string myOAuthAccessTokenSecret = Application.file().as_string(strPathSecret);

      if( myOAuthAccessTokenKey.has_char() && myOAuthAccessTokenSecret.has_char() )
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey( myOAuthAccessTokenKey );
         twitterObj.get_oauth().setOAuthTokenSecret( myOAuthAccessTokenSecret );
      }
      else
      {
         return "failed";
      }

      /* OAuth flow ends */

      // /* set twitter username and password */
      //twitterObj.setTwitterUsername( userName );
      //twitterObj.setTwitterPassword( passWord );

      /* Post a new status message */
      tmpStr = pszMessage;
      replyMsg = "";
      if( twitterObj.statusUpdate( tmpStr ) )
      {
         //replyMsg=twitterObj.get_response(  );

         //property_set set(get_app());

         //set.parse_json(replyMsg);

         //set[""]
         /*xml::document document(get_app());
         if(document.load(replyMsg))
         {
            if(document.get_root() != NULL)
            {
               if(document.get_root()->child_at(0) != NULL && document.get_root()->child_at(0)->get_name() == "error")
               {
                  if(document.get_root()->child_at(0)->attr("code") != 34
                  && document.get_root()->child_at(0)->get_value().CompareNoCase("Status is a duplicate.") != 0)
                  {

                     goto retry1;

                  }
               }
            }
         }*/
         //printf( "\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str() );
      }
      else
      {
         goto retry1;

         //printf( "\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str() );
      }
      return replyMsg;
retry1:
      if(iRetry >= 3)
      {
         return replyMsg ="failed";
      }
      System.file().del(strPathKey);
      System.file().del(strPathSecret);
      twitter_auth();
      iRetry++;
      goto Retry2;

   }

   /* Destroy a status message */
   /*memset( statusMsg, 0, 1024 );
   printf( "\nEnter status message id to delete: " );
   gets( statusMsg );
   tmpStr = statusMsg;
   replyMsg = "";
   if( twitterObj.statusDestroyById( tmpStr ) )
   {
   twitterObj.getLastWebResponse( replyMsg );
   printf( "\ntwitterClient:: twitCurl::statusDestroyById web response:\n%s\n", replyMsg.c_str() );
   }
   else
   {
   twitterObj.getLastCurlError( replyMsg );
   printf( "\ntwitterClient:: twitCurl::statusDestroyById error:\n%s\n", replyMsg.c_str() );
   }

   /* Get user timeline */
   /*replyMsg = "";
   printf( "\nGetting user timeline\n" );
   if( twitterObj.timelineUserGet() )
   {
   twitterObj.getLastWebResponse( replyMsg );
   printf( "\ntwitterClient:: twitCurl::timelineUserGet web response:\n%s\n", replyMsg.c_str() );
   }
   else
   {
   twitterObj.getLastCurlError( replyMsg );
   printf( "\ntwitterClient:: twitCurl::timelineUserGet error:\n%s\n", replyMsg.c_str() );
   }

   /* Get public timeline */
   /*replyMsg = "";
   printf( "\nGetting public timeline\n" );
   if( twitterObj.timelinePublicGet() )
   {
   twitterObj.getLastWebResponse( replyMsg );
   printf( "\ntwitterClient:: twitCurl::timelinePublicGet web response:\n%s\n", replyMsg.c_str() );
   }
   else
   {
   twitterObj.getLastCurlError( replyMsg );
   printf( "\ntwitterClient:: twitCurl::timelinePublicGet error:\n%s\n", replyMsg.c_str() );
   }

   /* Get friend ids */
   /*replyMsg = "";
   printf( "\nGetting friend ids\n" );
   tmpStr = "techcrunch";
   if( twitterObj.friendsIdsGet( tmpStr, false ) )
   {
   twitterObj.getLastWebResponse( replyMsg );
   printf( "\ntwitterClient:: twitCurl::friendsIdsGet web response:\n%s\n", replyMsg.c_str() );
   }
   else
   {
   twitterObj.getLastCurlError( replyMsg );
   printf( "\ntwitterClient:: twitCurl::friendsIdsGet error:\n%s\n", replyMsg.c_str() );
   }

   /* Get trends */
   /*if( twitterObj.trendsDailyGet() )
   {
   twitterObj.getLastWebResponse( replyMsg );
   printf( "\ntwitterClient:: twitCurl::trendsDailyGet web response:\n%s\n", replyMsg.c_str() );
   }
   else
   {
   twitterObj.getLastCurlError( replyMsg );
   printf( "\ntwitterClient:: twitCurl::trendsDailyGet error:\n%s\n", replyMsg.c_str() );
   }*/

   //return 0;



   void production::build(const char * psz)
   {

      string strApp(psz);

      add_status("Building ca2 fontopus ca2os " + strApp + "...");
      {
         string str;
         
         
         property_set set;

         set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor +" display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt( "%Y-%m-%d %H-%M-%S")) +  " Building " + strApp + "...</span></div>";

         Application.http().get("http://api.ca2.cc/status/insert", str, set);

      }
      ::core::process process;
      string strPath;
      if(Application.m_eversion == version_basis)
      {
         strPath = System.dir().element(strApp + "\\stage\\script\\stage_build.bat");
      }
      else
      {
         strPath = System.dir().element(strApp + "\\stage\\script\\basis_build.bat");
      }
      if(!process.create_child_process(strPath, false))
      {
         uint32_t dw = GetLastError();
         string str;
         str.Format("Error creating build process: %d for build of " + strApp, dw);
         add_status(str);
         return;   
      }
      int32_t i = 1;
      uint32_t dwExitCode;
      string str;
      while(!process.has_exited(&dwExitCode))
      {
         Sleep(5000);
         str.Format("%d Building ca2 fontopus ca2os " + strApp + "...", i);
         add_status(str);
         i++;
      }

   }

   string production::version_to_international_datetime(const char * psz)
   {
      string str(psz);
      str.replace("-", ":", 11);
      return str;
   }


} // namespace production



