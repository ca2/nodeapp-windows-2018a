#include "framework.h"
#include "core/user/hi5/hi5.h"
#include <mmsystem.h>


namespace production
{


   production::production(::aura::application * papp) :
      ::object(papp),
      thread(papp),
      simple_thread(papp),
      m_mutexStatus(papp),
      m_mutexCompress(papp),
      m_evFinish(papp),
      m_mutexRelease(papp)
   {

      m_bEndProduction = false;
      m_bClean = false;
      m_bBuild = true;
      m_bFinished = true;
      m_eversion = version_stage;
      m_bReleased = false;
      m_iRelease = 0;

      {
         stringa & stra = m_straStageDirAcceptedFileExtensions;
         stra.add("exe");
         stra.add("dll");
         stra.add("manifest");
      }
      m_bLoop = false;
      m_bEndStatus = false;


   }

   production::~production()
   {
   }

   void production::start_loop(e_version eversion, int32_t iLoopCount)
   {
      m_bLoop = true;
      if (m_iRelease > 0)
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
      m_iGlobalRetry = -1;

      if (m_eversion == version_basis)
      {
         m_strTwitterConsumerKey = "WMQkLWgdSpgYxn03Dyw";
         m_strTwitterConsumerSecret = "KeyhRHv2hlLmvwRyOPEkF4juF1Xy97uYFrS2EzdLEg";
      }
      else
      {
         m_strTwitterConsumerKey = "duJpErhbQC3T7hocP4imW7tNU";
         m_strTwitterConsumerSecret = "p3J42fpPNySwRuupwsa3Ra6xZjolQPWx27BPhiAad8mn7mLZEO";
      }



      twitter_auth();

      //facebook_auth();


      begin();



   }

   void production::start_production(e_version eversion)
   {

      m_bLoop = false;


      if (m_iRelease > 0)
      {
         add_status("There are pending releases!!");
         return;
      }

      m_iRelease = 0;
      m_bEndProduction = false;
      m_timeStart = ::datetime::time::get_current_time();
      m_eversion = eversion;


      m_iStep = 1;
      m_iGlobalRetry = -1;

      twitter_auth();

      //facebook_auth();

      begin();


   }

   void production::step()
   {
      m_iStep++;
      begin();
   }

   void production::defer_quit()
   {

      if (!m_bReleased)
         return;

      Application.post_to_all_threads(WM_QUIT, 0, 0);

   }




   void production::run()
   {

      //PlaySoundW(L"C:\\bergedge\\hi5\\audio\\production_ready.wav", NULL, SND_FILENAME);

      string str;

      property_set set;

      int32_t iProduce;

      string strBackPostColor = "background-color: #CCCCC2;";


      {

         ::file::path pathMirror = dir::system() / "config/production/mirror.txt";

         ::file::path pathMirrorStatus = dir::system() / "config/production/mirror_status.txt";

         if (!file_exists_dup(pathMirror)
               || !file_exists_dup(pathMirrorStatus))
         {

            MessageBox(NULL, "both " + pathMirror + " and " + pathMirrorStatus + " files must exist and maybe empty...", "The h***!!", MB_ICONEXCLAMATION);
            m_error.m_iaErrorCode2.add(-1);
            return ;

         }

         m_straMirror.add_lines(::file_as_string_dup(pathMirror));
         m_straMirrorStatus.add_lines(::file_as_string_dup(pathMirrorStatus));

         m_straMirror.remove_empty();
         m_straMirrorStatus.remove_empty();

         m_straMirror.set_size(MIN(m_straMirror.get_size(), m_straMirrorStatus.get_size()));
         m_straMirrorStatus.set_size(m_straMirror.get_size());


      }



      while (true)
      {

         try
         {
            iProduce = produce();
         }
         catch (...)
         {
            iProduce = -1;
         }

         if (iProduce >= 0)
            break;

         m_iStep = 1;


         //m_timeStart = ::datetime::time::get_current_time();


         if (m_iGlobalRetry > 8)
         {

            add_status("Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" this command!");

            //if (m_eversion == version_basis)
            //{
            //   set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #882266; display: block; margin-bottom: 1.5em;\">Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" " + m_strConfiguration + " build command!</span>";
            //}
            //else
            //{
            //   set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" " + m_strConfiguration + " build command!</span>";
            //}

            //Application.http().get("http://api.ca2.cc/status/insert", str, set);


            //string strTwit =  version_to_international_datetime(m_strStartTime) + " UTC Retried " + ::str::from(m_iGlobalRetry) + " times - \"giving up\" " + m_strConfiguration + " build command!";

            //twitter_twit(strTwit);

            m_error.m_iaErrorCode2.add(-1);
            return;

         }


         add_status("General build failure. Restarting");

         ///*   if (m_iGlobalRetry == 1)
         //   {
         //      m_strTry = "1st";
         //   }
         //   else if (m_iGlobalRetry == 2)
         //   {
         //      m_strTry = "2nd";
         //   }
         //   else if (m_iGlobalRetry == 3)
         //   {
         //      m_strTry = "3rd";
         //   }
         //   else
         //   {
         //      m_strTry = ::str::from(m_iGlobalRetry) + "th";
         //   }

         //   if (m_eversion == version_basis)
         //   {
         //      set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #222218; display: block; margin-bottom: 1.5em;\">General build failure. Starting " + m_strTry + " retry of new <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> release.<br />";
         //   }
         //   else
         //   {
         //      set["post"]["new_status"] = "<div style=\"display: block; " + strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #555550;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #222218; display: block; margin-bottom: 1.5em;\">General build failure. Starting " + m_strTry + " retry  of new <a href=\"http://ca2.cc/\">stage</a> release.<br />";
         //   }

         //   if (m_straStatus.get_count() > 1)
         //   {
         //      single_lock sl(&m_mutexStatus,TRUE);
         //      set["post"]["new_status"] = set["post"]["new_status"] + "Last Stati:<br />";
         //      for (::count i = MIN(5, m_straStatus.get_count() - 1); i >= 1; i--)
         //      {
         //         set["post"]["new_status"] = set["post"]["new_status"] + System.m_phtml->entities(m_straStatus.element_at(i));
         //         set["post"]["new_status"] = set["post"]["new_status"] + "<br />";
         //      }
         //      set["post"]["new_status"] = set["post"]["new_status"] + "<br />";
         //      set["post"]["new_status"] = set["post"]["new_status"] + "</span>";

         //   }


         //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

         //   ::datetime::time timeNow = ::datetime::time::get_current_time();

         //   string strTwit = "General failure of build " + version_to_international_datetime(m_strBuild) + ". Starting " + m_strTry + " retry of build " + m_strConfiguration + " - " + System.datetime().international().get_gmt_date_time(timeNow) + ". More details at http://status.ca2.cc/" + m_strStatusEmail;

         //   twitter_twit(strTwit);*/

      }

      /*if(System.directrix()->m_varTopicQuery.has_property("quit_on_finish"))
      {

      System.os().post_to_all_threads(WM_QUIT, 0, 0);

      }*/

      OnUpdateRelease();

      if (iProduce != 0)
      {

         m_error.m_iaErrorCode2.add(iProduce);

      }


   }


   int32_t production::produce()
   {

      string str;


      int32_t iRetry = 0;

restart:

      if (m_iStep == 1)
      {
         m_strStatusEmail = ApplicationUser.m_strLogin;

         m_strDownloadSite = "server.ca2.cc";

         if (m_eversion == version_basis)
         {
            m_strStdPostColor = "color: #882277;";
            m_strBackPostColor = "background-color: #CFC2CF;";
            m_strEmpPostColor = "color: #660060;";
            m_strConfiguration = "basis";
         }
         else
         {
            m_strStdPostColor = "color: #448855;";
            m_strBackPostColor = "background-color: #A0CCAA;";
            m_strEmpPostColor = "color: #007700;";
            m_strConfiguration = "stage";
         }

         bool bMediumSizeStatusText = false;

         if (bMediumSizeStatusText)
         {

            //{

            //   property_set set(get_app());

            //   Application.http().get("http://api.ca2.cc/status/insert", set);

            //}

            string strEndTime;

            m_timeEnd.FormatGmt(strEndTime, "%Y-%m-%d %H-%M-%S");

            m_strBuild = "\"This a long build version string text description that makes really trigger a size that is will end up making this status rquired and needed to be recorded and written to another set of records or table...\"";

            //string str;

            //{

            //   property_set set(get_app());

            //   if (m_eversion == version_basis)
            //   {
            //      set["post"]["new_status"] = "<div style=\"display: block; background-color: #FFE0FF; \"><h2 style=\"margin-bottom:0px; color: #FF55CC;\">Medium Size Status Text" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #882255; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://laboratory.ca2.cc/\" class=\"fluidbasis\" >laboratory.ca2.cc</a> or <a href=\"http://warehouse.ca2.cc/\" class=\"fluidbasis\" >warehouse.ca2.cc</a> for simple gateway implementations.</span></div>";
            //   }
            //   else
            //   {
            //      set["post"]["new_status"] = "<div style=\"display: block; background-color: #E0FFCC; \"><h2 style=\"margin-bottom:0px; color: #55CCAA;\">Medium Size Status Text" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://ca2.cc/\">stage</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://desktop.ca2.cc/\">desktop.ca2.cc</a> or <a href=\"http://store.ca2.cc/\">store.ca2.cc</a> for simple gateway implementations.</span></div";
            //   }

            //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

            //}

            ExitProcess(true);


         }
         //m_strBase = Application.command()->m_varTopicQuery["base_dir"];
         m_strBase = Application.file().as_string("C:\\ca2\\config\\nodeapp-windows\\production\\base_dir.txt").trimmed();


         /*

         {

            string strContentsSrc = m_strBase /  "include", "product.version.config.h");

            string strContentsSet;

            string strContentsGet = Application.file().as_string(strContentsSrc);

            if (m_eversion == version_basis)
            {

              strContentsSet = "#pragma once\n\n\n#define CA2_PLATFORM_VERSION CA2_BASIS\n\n\n\n";

            }
            else
            {

              strContentsSet = "#pragma once\n\n\n#define CA2_PLATFORM_VERSION CA2_STAGE\n\n\n\n";

            }

            if (strContentsSet != strContentsGet)
            {

              Application.file().put_contents(strContentsSrc, strContentsSet);

            }

         }

         */


         m_strSignTool = "C:\\bergedge\\hi5\\program\\bin\\signtool.exe";
         m_strSpc = "C:\\sensitive\\sensitive\\certificate\\ca2.p12";
         m_strSignPass = Application.file().as_string("C:\\sensitive\\sensitive\\certificate\\2011-05-ca2.pass");

         m_iLoop++;
         defer_quit();

         {

            ::file::listing listing(get_app());

            listing.ls_dir(m_strBase);

            listing.to_name();

            listing.filter_begins_ci("app-");

            ::lemon::array::copy(m_straRoot, listing);

         }

         //         m_straRoot.filter([](const ::file::path & p) {return ::str::begins_ci(p.name(),"app-"); });

         m_straRoot.insert_at(0, "app");

         m_straRoot.add("design");

         m_straRoot.add("game");

         m_straRoot.add("nodeapp");

         //   goto skipCompress;
         //goto skipBuild;

         //if(false)
         {

            bool bSkip1 = false;

            if (!bSkip1 && m_iGlobalRetry <= 0)
            {

               for (int32_t i = 0; i < m_straRoot.get_size(); i++)
               {

                  if (!sync_source(m_straRoot[i], NULL))
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
            strSvnVersionCmd.Format("svnversion %s", m_strBase / "app");
            m_strBuild = strTime;
            m_strFormatBuild = strTime;
            m_strFormatBuild.replace(" ", "_");

            {

               string strStatus;
               strStatus.Format("Getting Revision: %s ...", "app");
               add_status(strStatus);

            }

            strRevision = System.process().get_output(strSvnVersionCmd);
            strRevision.trim();

            {

               string strStatus;
               strStatus.Format("Revision of %s is %s", "app", strRevision);
               add_status(strStatus);

            }
            if (str::from(atoi(strRevision)) != strRevision)
            {
               // good pratice to initialize authentication of ca2status.com with account.ca2.cc auth information
               //string str;

               //{

               //   property_set set(get_app());

               //   Application.http().get("http://api.ca2.cc/status/insert",set);

               //}

               //{

               //   property_set set(get_app());

               //   if(m_eversion == version_basis)
               //   {
               //      set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #552250;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #882266; display: block; margin-bottom: 1.5em;\">Check app working copy.</span>";
               //   }
               //   else
               //   {
               //      set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #22552F;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">Check app working copy.</span>";
               //   }

               //   Application.http().get("http://api.ca2.cc/status/insert",str,set);

               //}

               return 4;
            }


            string strSVN = "SVN" + strRevision;
            strSVN.trim();

            if (m_iGlobalRetry <= 0 && m_strSubversionRevision == strSVN)
            {

               iRetry++;

               if (iRetry > 3)
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

                  strSvnVersionCmd.Format("svnversion %s", m_strBase / m_straRoot[i]);
                  {

                     string strStatus;
                     strStatus.Format("Getting Revision: %s ...", m_straRoot[i]);
                     add_status(strStatus);

                  }
                  strAddRevision = System.process().get_output(strSvnVersionCmd);
                  strAddRevision.trim();
                  {

                     string strStatus;
                     strStatus.Format("Revision of %s is %s", m_straRoot[i], strAddRevision);
                     add_status(strStatus);

                  }

                  strSVNKey += ", " + m_straRoot[i] + ":SVN" + strAddRevision;

               }

            }

            m_iGlobalRetry++;

            m_bReleased = false;
            m_iLoop = -1;
            m_timeStart.FormatGmt(m_strStartTime, "%Y-%m-%d %H-%M-%S");
            add_status("Build starting at " + version_to_international_datetime(m_strStartTime) + " - build version!");

            //{
            //   // good pratice to initialize authentication of ca2status.com with account.ca2.cc auth information

            //   {

            //      property_set set(get_app());

            //      Application.http().get("http://api.ca2.cc/status/insert",set);

            //   }

            //{

            //   property_set set(get_app());

            //   if(m_eversion == version_basis)
            //   {
            //      set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #552250;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #882266; display: block; margin-bottom: 1.5em;\">Starting production of new <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> release.</span>";
            //   }
            //   else
            //   {
            //      set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h3 style=\"margin-bottom:0px; color: #22552F;\">" + version_to_international_datetime(m_strStartTime) + "</h3><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">Starting production of new <a href=\"http://ca2.cc/\">stage</a> release.</span>";
            //   }

            //   Application.http().get("http://api.ca2.cc/status/insert",str,set);

            //}

            //{

            //   property_set set(get_app());

            //   string str;

            m_timeEnd = m_timeStart;
            string strEndTime;

            m_timeEnd.FormatGmt(strEndTime, "%Y-%m-%d %H-%M-%S");

            //if (m_eversion == version_basis)
            //{
            //   set["post"]["new_status"] = "<div style=\"display: block; background-color: #FFE0FF; \"><h2 style=\"margin-bottom:0px; color: #FF55CC;\">" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #882255; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://laboratory.ca2.cc/\" class=\"fluidbasis\" >laboratory.ca2.cc</a> or <a href=\"http://warehouse.ca2.cc/\" class=\"fluidbasis\" >warehouse.ca2.cc</a> for simple gateway implementations.</span></div>";
            //}
            //else
            //{
            //   set["post"]["new_status"] = "<div style=\"display: block; background-color: #E0FFCC; \"><h2 style=\"margin-bottom:0px; color: #55CCAA;\">" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://ca2.cc/\">stage</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://desktop.ca2.cc/\">desktop.ca2.cc</a> or <a href=\"http://store.ca2.cc/\">store.ca2.cc</a> for simple gateway implementations.</span></div";
            //}

            //Application.http().get("http://api.ca2.cc/status/insert", str, set);

            //}


            /*          string strTwit;

                   if(m_iGlobalRetry <= 0)
                   {
                      strTwit = "ca2twit-lib : new " + m_strConfiguration + " build starting " + version_to_international_datetime(m_strBuild) + " UTC. More details at http://status.ca2.cc/" + m_strStatusEmail;
                   }
                   else
                   {
                      strTwit = "ca2twit-lib : " + m_strTry + " automatic retry " + m_strConfiguration + " build starting " + version_to_international_datetime(m_strBuild) + " UTC. More details at http://status.ca2.cc/" + m_strStatusEmail;
                   }

                   twitter_twit(strTwit);*/

            //}

            add_status(unitext("Thank you!!"));
            m_dwStartTick = ::GetTickCount();

            m_bEndStatus = false;

            keep < bool > keepFinishedFalse(&m_bFinished, false, true, true);
            string str;
            m_iBaseLen = m_strBase.get_length();
            if (m_strBase.Right(1) != "/" && m_strBase.Right(1) != "\\")
               m_iBaseLen++;



            string strStatus;
            m_strTag = strTime + " " + strSVNKey;
            m_strTagPath = ::file::path("C:\\ca2\\build") / m_strConfiguration, m_strFormatBuild + ".txt";

            string strBuildH;
            strBuildH.Format("-c1-production -c2-producer -t12n-producing -mmmi- %s", m_strTag);
            strBuildH += " - ";
            strBuildH += Application.file().as_string(m_strBase / "app/stage" / "build_machine_pp_comment.txt");
            strBuildH += "#define THIS_PRODUCT_VERSION \"" + m_strTag + "\\0\"\r\n#define THIS_FILE_VERSION \"" + m_strTag + "\\0\"\r\n";
            strBuildH += "#define __THIS_PRODUCT_VERSION " + strVerWin + "\r\n#define __THIS_FILE_VERSION " + strVerWin + "\r\n";
            strBuildH += "\r\n";
            strBuildH += "\r\n";
            strBuildH += "\r\n";
            strBuildH += "\r\n";
            strBuildH += "\r\n";

            m_strVrel = "C:\\ca2\\vrel\\" + m_strConfiguration + "\\" + m_strFormatBuild;

            m_strCCAuth = "C:\\home\\ccvotagus\\ca2_spa\\" + m_strConfiguration + "\\" + m_strFormatBuild;
            m_strCCVrel = "C:\\home\\ccvotagus\\ca2_spa\\" + m_strConfiguration + "";
            m_strCCVrelNew = "C:\\home\\ccvotagus\\ca2_spa\\ccvrelnew\\" + m_strConfiguration + "\\" + m_strFormatBuild;

            int32_t i;
            if (m_bClean)
            {
               add_status("Cleaning ca2 fontopus ccvotagus ...");
               //{
               //   string str;


               //   property_set set;

               //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Cleaning...</span></div>";

               //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

               //}
               ::process::process_sp process(allocer());
               string strPath;
               strPath = System.dir().install() / "nodeapp\\stage\\script\\stage_clean.bat";
               if (!process->create_child_process(strPath, false))
               {
                  uint32_t dw = GetLastError();
                  string str;
                  str.Format("Error creating clean process: %d", dw);
                  add_status(str);
                  return 0;
               }
               i = 1;
               while (!process->has_exited())
               {
                  Sleep(5000);
                  str.Format("%d Cleaning ca2 fontopus ccvotagus ...", i);
                  add_status(str);
                  i++;
               }
            }

            //Application.dir().mk(m_strBase /  "time"));
            Application.file().put_contents(m_strBase / "app\\build.txt", m_strBuild);
            Application.file().put_contents_utf8(m_strBase / "app\\this_version_info.h", strBuildH);
            Application.file().put_contents_utf8(m_strBase / "app\\this_version_info.txt", strBuildH);

            update_rc_file_version(m_strBase / "app\\appseed\\base\\base.rc");
            update_rc_file_version(m_strBase / "app-core\\appseed\\iexca2\\iexca2.rc");
            update_rc_file_version(m_strBase / "nodeapp\\appseed\\app_app_admin\\app_app_admin.rc");
            update_rc_file_version(m_strBase / "nodeapp\\appseed\\draw2d_gdiplus\\draw2d_gdiplus.rc");

            if (!commit_for_new_build_and_new_release())
               return 2;


            m_strSubversionRevision = "SVN" + str::from(atoi(strRevision) + 1);

            //if (m_bBuild)
            {
               //build()
               build("nodeapp");
            }

            //{
            //   string str;


            //   property_set set;

            //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Cleaning...</span></div>";

            //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

            //}

            add_status("Cleaning site...");
            ::file::path strPath = System.dir().install() / "time\\stage\\app\\matter\\job.bat";


            //System.http().ms_download("http://api.ca2.cc/spaignition/clean",
            //   System.dir().install() / "time\\spaignition_update.txt"), NULL, post, headers, ::ca2::app(get_app()).user()->get_user());
            /*add_status("Cleaning ccvotagus folder...");
            ::process::process_sp process(allocer());
            Application.file().put_contents(strPath, "rmdir /s /q C:\\ca2\\vrel\\" + m_strConfiguration);
            if (!process->create_child_process(strPath, false))
            {
               uint32_t dw = GetLastError();
               string str;
               str.Format("Error creating process: %d", dw);
               add_status(str);
               return 0;
            }
            i = 1;
            while (!process->has_exited(&dwExitCode))
            {
               Sleep(500);
               str.Format("%d Cleaning ccvotagus folder ...", i);
               add_status(str);
               i++;
            }*/

         }




         m_straFiles.remove_all();


         get_file_list(m_strBase, "time/Win32/" + m_strConfiguration, m_straFiles);
         get_file_list(m_strBase, "time/x64/" + m_strConfiguration, m_straFiles);
         get_file_list(m_strBase, "app/stage/metastage", m_straFiles);


         generate_appmatter_spa();


         ::count iCount = m_straFiles.get_size();
         for (int32_t i = 0; i < iCount;)
         {
            if (m_straFiles[i].find("\\.svn\\") >= 0 || (m_straFiles[i].get_length() < 5 || m_straFiles[i].Right(5) == "\\.svn"))
            {
               m_straFiles.remove_at(i);
            }
            else
            {
               i++;
            }
         }
         TRACE("\n");
         string strStatus;

         ::file::path pathTarget;

         ::count cDirMkErrorCount = 0;
         ::count cFileCopyErrorCount = 0;

         int iBaseLen = m_strBase.length();

         if (m_strBase.ends_ci("\\") || m_strBase.ends_ci("/"))
         {

            iBaseLen++;

         }

         ::file::path pathLastFolder;

         ::file::path pathFolder;

         for (int32_t i = 0; i < m_straFiles.get_size(); i++)
         {
            //const char * lpcsz = m_straFiles[i];

            strStatus.Empty();

            pathTarget = m_strVrel / m_straFiles[i].Mid(iBaseLen);

            pathFolder = pathTarget.folder();

            if (pathFolder != pathLastFolder)
            {

               if (!::dir::is(pathTarget.folder()))
               {

                  if (!::dir::mk(pathTarget.folder()))
                  {

                     strStatus += "<1>";

                     cDirMkErrorCount++;

                  }
                  else
                  {

                     pathLastFolder = pathFolder;

                  }

               }
               else
               {

                  pathLastFolder = pathFolder;

               }

            }

            if (!::file_copy_dup(pathTarget, m_straFiles[i], true))
            {
               strStatus += "<2>";
               cFileCopyErrorCount++;

            }

            if (strStatus.is_empty())
            {

               strStatus = "ok";
            }

            output_debug_string(strStatus + " " + m_straFiles[i] + "\n");
         }

         if (cDirMkErrorCount == 0 && cFileCopyErrorCount == 0)
         {

            output_debug_string("100% Successfull file copy batch to \"" + m_strVrel + "\"\n");

         }
         else
         {

            output_debug_string(::str::from(cDirMkErrorCount) + " errors attempting to create directory and " + ::str::from(cFileCopyErrorCount) + " errors attempting to do file copy batch at \"" + m_strVrel + "\"\n");

         }

         m_iStep = 2;

         //m_pview->post_message(WM_USER, 2);
         //{
         //   string str;


         //   property_set set;

         //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Copying...</span></div>";

         //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

         //}
         //}
         //else if (m_iStep == 2)
         //{
         string m_strStartTime;
         m_timeStart.FormatGmt(m_strStartTime, "%Y-%m-%d %H-%M-%S");
         //{
         //   string str;


         //   property_set set;

         //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Compressing...</span></div>";

         //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

         //}
         compress();
         //{
         //   string str;


         //   property_set set;

         //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Resources...</span></div>";

         //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

         //}

         release_production();

         if (!release_npca2("x86"))
         {
            return 1;
         }
         if (!release_npca2("x64"))
         {
            return 1;
         }
         if (!release_iexca2("x86"))
         {
            return 1;
         }
         if (!release_iexca2("x64"))
         {
            return 1;
         }
         //if (!release_crxca2("x86"))
         //{
         // return 1;
         //}
         /*if(!release_crxca2("x64"))
         {
         return 1;
         }*/
         Application.dir().mk("C:\\home\\ccvotagus\\ca2_spa\\" + m_strConfiguration + "\\app\\");
         Application.file().put_contents("C:\\home\\ccvotagus\\ca2_spa\\" + m_strConfiguration + "\\app\\build.txt", m_strBuild);
         Application.file().put_contents(m_strCCVrelNew + "\\app\\build.txt", m_strBuild);
         Application.dir().mk(m_strTagPath.folder());
         Application.file().put_contents(m_strTagPath, m_strTag);

         //commit_source("C:\\netnodenet\\net");

         //{
         //   string str;


         //   property_set set;

         //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Storing Symbols...</span></div>";

         //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

         //}



         {

            add_status("Storing Symbols x86...");

            ::file::path strPath = System.dir().install() / "time\\stage\\app\\matter\\store_symbols_job_x86.bat";

            ::process::process_sp process(allocer());
            string strCommand = "\"C:\\Program Files (x86)\\Windows Kits\\8.0\\Debuggers\\x86\\symstore.exe\"  add /r  -:REL /f \\\\sewindows\\stage\\" + m_strFormatBuild + "\\stage\\x86\\ /s \\\\sewindows\\SymbolServer\\ /t \"ca2\" /v \"" + m_strFormatBuild + "\"";
            Application.file().put_contents(strPath, strCommand);
            if (!process->create_child_process(strPath, false))
            {
               uint32_t dw = GetLastError();
               string str;
               str.Format("Error creating process: %d", dw);
               add_status(str);
               return 0;
            }

            int32_t i = 1;

            string str;

            while (!process->has_exited())
            {
               Sleep(500);
               str.Format("%d Storing Symbols x86 ...", i);
               add_status(str);
               i++;
            }

         }





         {

            add_status("Storing Symbols x64...");

            ::file::path strPath = System.dir().install() / "time\\stage\\app\\matter\\store_symbols_job_x64.bat";

            ::process::process_sp process(allocer());
            string strCommand = "\"C:\\Program Files (x86)\\Windows Kits\\8.0\\Debuggers\\x64\\symstore.exe\"  add /r  -:REL /f \\\\sewindows\\stage\\" + m_strFormatBuild + "\\stage\\x64\\ /s \\\\sewindows\\SymbolServer\\ /t \"ca2\" /v \"" + m_strFormatBuild + "\"";
            Application.file().put_contents(strPath, strCommand);
            if (!process->create_child_process(strPath, false))
            {
               uint32_t dw = GetLastError();
               string str;
               str.Format("Error creating process: %d", dw);
               add_status(str);
               return 0;
            }

            int32_t i = 1;

            string str;

            while (!process->has_exited())
            {
               Sleep(500);
               str.Format("%d Storing Symbols x86 ...", i);
               add_status(str);
               i++;
            }

         }






         //{

         //   string str;


         //   property_set set;

         //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Packaging...</span></div>";

         //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

         //}



         add_status("");
         add_status("");
         add_status("");
         add_status("");
         add_status("");

         if (m_straMirror.has_elements())
         {

            add_status("");

            ::file::listing straRoot;

            straRoot = m_straRoot;

            straRoot.add("time");

            add_status("***Preparing to release to mirrors in two phases with " + ::str::from(straRoot.get_count()) + " steps each...");

            add_status("1st phase : archiving files and directories into one archive...");

            for (index i = 0; i < straRoot.get_count(); i++)
            {

               string strRoot = straRoot[i];

               string strSpa = "ca2_spa_" + ::str::replace("-", "_", strRoot);

               add_status(::str::from(i + 1) + ". dtf - fileset - file from directory " + strRoot);

               System.file().dtf(m_strCCVrelNew + "\\" + strSpa + ".fileset", m_strCCVrelNew + "\\" + strRoot, get_app());

            }


            add_status("");
            add_status("");

            add_status("2st phase : compressing archives...");

            for (index i = 0; i < straRoot.get_count(); i++)
            {

               string strRoot = straRoot[i];

               string strSpa = "ca2_spa_" + ::str::replace("-", "_", strRoot);

               add_status(::str::from(i + 1) + ". bz - bzip - compressing " + strRoot);

               System.compress().bz(get_app(), m_strCCVrelNew + "\\" + strSpa + ".fileset.bz", m_strCCVrelNew + "\\" + strSpa + ".fileset");

            }


         }


         m_bEndProduction = true;

         class release * prelease = NULL;

         stringa straStatus;
         stringa straServer;




         //         178.33.157.33 fr - api.ca2.cc
         //          178.33.157.34 de - api.ca2.cc
         //        158.69.159.130 west - api.ca2.cc
         //      158.69.159.129 east - api.ca2.cc


         straStatus.add(unitext("100 releasing at North America netnode, Beauharnois, Canada"));
         straServer.add("ca2.cc");

         straStatus.add(m_straMirrorStatus);
         straServer.add(m_straMirror);
         /*
         straStatus.add(unitext("330 releasing at netnode : France (Gravelines, France)"));
         //straServer.add("fr-api.ca2.cc");
         straServer.add("a.ca2.fr");

         straStatus.add(unitext("230 releasing at netnode : Deutschland (Gravelines, France)"));
         //straServer.add("de-api.ca2.cc");
         straServer.add("a.ca2.com.de");

         if(m_strConfiguration == "basis")
         {

            straStatus.swap(0,1);
            straServer.swap(0,1);

         }


         straStatus.add(unitext("500 releasing at netnode : Sverige/East US (Beauharnois, Canada)"));
         //straServer.add("east-api.ca2.cc");
         straServer.add("blue.ca2.cc");

         straStatus.add(unitext("800 releasing at netnode : India/West US (Beauharnois, Canada)"));
         //straServer.add("west-api.ca2.cc");
         straServer.add("gold.ca2.cc");
         */


         //straStatus.add("230 releasing at netnode : west us");
         //straServer.add("west-api.ca2.cc");


         //straStatus.add("010 releasing at netnode : east us");
         //straServer.add("east-api.ca2.cc");


         //straStatus.add(unitext("025 releasing at netnode : América Latina (São Paulo, Brazil)"));
         //straServer.add("la-api.ca2.cc");


         //straStatus.add(unitext("033 releasing at netnode : Sverige/East US (Beauharnois, Canada)"));
         //straServer.add("east-api.ca2.cc");


         //straStatus.add(unitext("023 releasing at netnode : India/West US (Beauharnois, Canada)"));
         //straServer.add("west-api.ca2.cc");


         //straStatus.add(unitext("022 releasing at netnode : License/US (Beauharnois, Canada)"));
         //straServer.add("license-api.ca2.cc");


         //straStatus.add("077 releasing at netnode : hong kong");
         //straServer.add("hk-api.ca2.cc");


         //straStatus.add(unitext("084 releasing at netnode : Asia (日本、東京)"));
         //straServer.add("asia-api.ca2.cc");


         //straStatus.add(unitext("077 releasing at netnode : Oceania/Australia/New Zealand (Sydney)"));
         //straServer.add("cyan-api.ca2.cc");


         string strObject1 = "/api/production/main/release_ca2?authnone=1&configuration=" + m_strConfiguration + "&build=" + m_strFormatBuild;

         string strObject = "/api/production/mirror/release_ca2?authnone=1&configuration=" + m_strConfiguration + "&build=" + m_strFormatBuild;

         {

            for (index i = 0; i < straStatus.get_count(); i++)
            {

               string strStatus = straStatus[i];

               string strServer = straServer[i];

               add_status(strStatus);

               if (i == 0)
               {

                  class release * prelease = new class release(this, "https://" + strServer + strObject1, strServer);

                  prelease->raw_run();

               }
               else
               {

                  class release * prelease = new class release(this, "https://" + strServer + strObject, strServer);

                  prelease->begin();

               }

               //{

               //   string str;

               //   property_set set;

               //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " " + strStatus + "</span></div>";

               //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

               //}

               Sleep(2000);

            }

         }


         //{
         //   string str;


         //   property_set set;

         //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Releasing...</span></div>";

         //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

         //}

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

         add_status("Launching post build tasks :");
         add_status("Making available debugging symbols");
         {

            string strCmdLine = "\"C:\\bergedge\\hi5\\program\\hstart.exe\" \"C:\\bergedge\\lemon\\windows\\scripts\\production\\" + m_strConfiguration + "\\index.bat\"";
            string strDir = "C:\\bergedge\\lemon\\windows\\scripts\\production\\" + m_strConfiguration + "\\";
            System.process().launch(strCmdLine, SW_SHOWNORMAL, strDir);

         }


         add_status("");
         add_status("");
         add_status("");
         add_status("");
         add_status("");

         PlaySoundW(L"C:\\bergedge\\hi5\\audio\\production_ready.wav", NULL, SND_FILENAME);


         Sleep(5000);

         m_evFinish.SetEvent();

      }

      return 0;


   }




   void production::compress(const ::file::path & lpcszRelative)
   {
      string strStatus;
      strStatus.Format("compressing %s", lpcszRelative.name());
      add_status(strStatus);
      string strSrcFile = m_strVrel / lpcszRelative;
      if (::str::ends_ci(lpcszRelative, ".dll")
            || ::str::ends_ci(lpcszRelative, ".exe")
            || ::str::ends_ci(lpcszRelative, ".ocx")
            || ::str::ends_ci(lpcszRelative, ".cab"))
      {

         string strStatus;
         strStatus.Format("signing %s", lpcszRelative.name());
         add_status(strStatus);

         string strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strSrcFile + "\"";
         System.process().synch(strCmd);

         add_status("Signing code ...");

      }
      else if (::str::ends_ci(lpcszRelative, ".sys"))
      {

         string strStatus;
         strStatus.Format("signing driver %s", lpcszRelative.name());
         add_status(strStatus);

         string strCmd = "\"" + m_strSignTool + "\" sign /v /ac \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strSrcFile + "\"";
         System.process().synch(strCmd);

         add_status("Signing driver code ...");

      }
      int32_t iRetry = 0;
retry2:
      try
      {
         System.compress().bz(get_app(), m_strCCAuth / lpcszRelative + ".bz", strSrcFile);
      }
      catch (...)
      {
         if (iRetry < 5)
         {
            iRetry++;
            Sleep(5000);
            goto retry2;
         }
         else
         {
            add_status("build failure compressing " + string(lpcszRelative));
         }
      }

      strStatus.Format("%s compressed", lpcszRelative.name());

      add_status(strStatus);

   }


   void production::compress()
   {
      string strStatus;
      strStatus = "Compressing";
      add_status(strStatus);

      string strVrel;
      strVrel = m_strVrel;

      string strCCAuth;
      strCCAuth = m_strCCAuth;

      string strRelative;
      string strBz;
      string strUn;
      int32_t i = 0;
      single_lock sl(&m_mutexCompress, TRUE);
      for (; i < m_straFiles.get_size(); i++)
      {
         ::file::path & strFile = m_straFiles[i];
         if (strFile.extension() == "zip")
         {
         }
         else if (Application.dir().is(strFile))
         {
            continue;
         }
         else if (strFile.ext() == "pdb")
         {
            continue;
         }
         else if (strFile.ext() == "ilk")
         {
            continue;
         }
         //strStatus.Format("compressing %s", strFile);
         //add_status(strStatus);
         m_straCompress.add(strFile.Mid(m_iBaseLen));
      }
      sl.unlock();

      uint32_t uiProcessorCount = get_current_process_affinity_order();
      //uiProcessorCount = 0;
      array < compress_thread * > threada;
      if (uiProcessorCount == 0)
      {
         while (compress_next())
         {
         }
      }
      else
      {
         smart_pointer_array < manual_reset_event > eventa(get_app());
         eventa.set_size_create(uiProcessorCount);
         sync_object_ptra ptra;
         for (uint32_t ui = 0; ui < uiProcessorCount; ui++)
         {
            compress_thread * pthread = new compress_thread(this, eventa[ui]);
            threada.add(pthread);
            pthread->m_dwThreadAffinityMask = 1 << ui;
            //pthread->m_bAutoDelete = false;
            pthread->begin();
            ptra.add(eventa[ui]);
         }
         multi_lock ml(ptra);
         ml.lock();
         add_status("finished multi-threaded compression task");
         Sleep(500);
      }
      /*while(threada.get_size() > 0)
      {
      try
      {
      delete threada.last();
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
      if (m_straCompress.get_size() <= 0)
         return false;
      string strNext = m_straCompress[0];
      m_straCompress.remove_at(0);
      sl.unlock();
      compress(strNext);
      return true;
   }


   production::compress_thread::compress_thread(production * pproduction, manual_reset_event * peventFinished) :
      ::object(pproduction->get_app()),
      thread(pproduction->get_app()),
      m_pevFinished(peventFinished)
   {

      m_pevFinished->ResetEvent();

      m_pproduction = pproduction;

   }


   void production::compress_thread::run()
   {

      SetThreadAffinityMask(::GetCurrentThread(), m_dwThreadAffinityMask);

      set_thread_priority(::multithreading::priority_highest);

      while (m_pproduction->compress_next())
      {
      }

      m_pevFinished->SetEvent();

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
   m_straCC.implode(strVar, ",", i, MIN(8, m_straCC.get_size() - i));
   strUrl = "http://api.ca2.cc/spaignition/compress?file=";
   strUrl += strVar;

   strStatus.Format("compress step: %d", i);
   add_status(strStatus);




   System.http().ms_download(strUrl,
   System.dir().install() / "time\\spaignition_update.txt"), NULL, post, headers, Session.user()->get_user());
   i += 8;
   }
   }*/


   bool  production::sync_source(const char * psz, const char * pszRevision)
   {
      string strStatus;
      strStatus.Format("Updating source: %s ...", psz);
      add_status(strStatus);

      string str;
      ::file::path strBase = m_strBase;
      STARTUPINFO si;
      PROCESS_INFORMATION pi;
      memset(&si, 0, sizeof(si));
      memset(&pi, 0, sizeof(pi));
      si.cb = sizeof(si);
      si.dwFlags = STARTF_USESHOWWINDOW;
      si.wShowWindow = SW_HIDE;
      if (pszRevision != NULL && pszRevision[0] != '\0')
      {
         str.Format("svn update --revision %s %s", pszRevision, strBase / psz);
      }
      else
      {
         str.Format("svn update %s", strBase / psz);
      }
      if (!::CreateProcess(NULL, (LPTSTR)(const char *)str, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
      {
         strStatus.Format("     Error: Check svn installation!!");
         add_status(strStatus);
         return false;
      }

      DWORD dwExitCode;
      int32_t i = 1;
      while (true)
      {
         if (!GetExitCodeProcess(pi.hProcess, &dwExitCode))
            break;
         if (dwExitCode != STILL_ACTIVE)
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


      for (int32_t i = 0; i < m_straRoot.get_size(); i++)
      {

         if (!commit_source(m_straRoot[i]))
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
      if (string(psz).find(":\\") > 0)
      {
         str.Format("svn commit --force-log --encoding utf-8 --file %s %s", m_strBase / "app\\this_version_info.txt", psz);
      }
      else
      {
         str.Format("svn commit --force-log --encoding utf-8 --file %s %s", m_strBase / "app\\this_version_info.txt", strBase / psz);
      }
      if (!::CreateProcess(NULL, (LPTSTR)(const char *)str, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
      {
         strStatus.Format("     Error: Check svn installation!!");
         add_status(strStatus);
         return false;
      }

      DWORD dwExitCode;
      int32_t i = 1;
      while (true)
      {
         if (!GetExitCodeProcess(pi.hProcess, &dwExitCode))
            break;
         if (dwExitCode != STILL_ACTIVE)
            break;
         Sleep(2300);
         str.Format("%d: Commit for new Build and new Release : %s ...", i, psz);
         add_status(str);
         i++;
      }
      return true;
   }

   bool production::get_file_list(const char * pszBase, const char * pszDir, ::file::listing & stra, bool bFileSet)
   {
      ::file::path strBase(pszBase);
      ::file::path strRelease;
      string strDirParam(pszDir);
      string strLocal(strDirParam);
      strLocal.replace("/", "\\");
      if (strLocal.Right(1) != "\\") strLocal += "\\";
      strRelease = strBase / strLocal;
      if (bFileSet)
      {
         ::file::path strFile;
         string strTitle;
         string strRelative;
         ::str::ends_eat(strRelease, "\\");
         ::str::ends_eat(strRelease, "/");
         ::str::ends_eat(strLocal, "\\");
         ::str::ends_eat(strLocal, "/");
         strFile = strRelease + ".expand_fileset";
         strTitle = strRelease.sname() + ".expand_fileset";
         strRelative = strLocal + ".expand_fileset";
         strRelative.replace("/", "\\");

         ::file::listing stra1(get_app());

         stra1.rls_file(strRelease);

         for (int32_t i = 0; i < stra1.get_size();)
         {
            if (stra1[i].find("\\.svn\\") >= 0 || (stra1[i].get_length() < 5 || stra1[i].Right(5) == "\\.svn"))
            {
               stra1.remove_at(i);
            }
            else
            {
               i++;
            }
         }
         System.file().dtf(strFile, stra1, get_app());
         stra.add(strFile);
      }
      else
      {

         stra.m_pprovider = get_app();

         stra.rls_file(strRelease);

      }
      return true;
   }



   void production::release_production()
   {
      m_strStatus = "generating index file and releasing...";
      add_status(m_strStatus);


      strsize iBaseLen = m_strBase.get_length();
      if (m_strBase.Right(1) != "/" && m_strBase.Right(1) != "\\")
         iBaseLen++;

      stringa straStageDir;

      straStageDir.add(m_strBase / "time/Win32" / +m_strConfiguration);
      straStageDir.add(m_strBase / "time/x64" / +m_strConfiguration);

      string strStatus;

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
      for (; i < m_straFiles.get_size(); i++)
      {
         ::file::path & strFile = m_straFiles[i];
         if (::str::ends_ci(strFile, ".zip"))
         {
         }
         else if (Application.dir().is(strFile))
         {
            continue;
         }
         if (straStageDir.str_find_first_begins_ci(strFile) >= 0
               && !m_straStageDirAcceptedFileExtensions.contains(
               strFile.final_extension()))
         {
            continue;
         }
         //strStatus.Format("compressing %s", strFile);
         //add_status(strStatus);
         strRelative = strFile.Mid(iBaseLen);

         strBz = m_strCCAuth / strRelative + ".bz";
         strUn = m_strVrel / strRelative;

         strMd5 = Application.file().md5(strUn);

         varUnSize = Application.file().length(strUn);
         varBzSize = Application.file().length(strBz);

         strRelease = m_strCCVrel / strRelative;
         strRelease += ".bz.";
         strRelease += strMd5;

         strReleaseNew = m_strCCVrelNew / strRelative;
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

         strStatus.Empty();

         if (!Application.file().exists(strRelease))
         {

            if (Application.file().copy(strRelease, strBz, false).failed())
            {

               strStatus += "<1>";

            }

         }

         if (Application.file().copy(strReleaseNew, strBz, false).failed())
         {

            strStatus += "<2>";

         }

         if (strStatus.is_empty())
         {

            strStatus = "ok";

         }

         TRACE(strStatus + " " + strRelative);

      }

      strRelative = "app\\stage\\metastage\\index-" + m_strFormatBuild + ".spa";
      string strIndex = m_strVrel / strRelative;
      Application.file().put_contents(strIndex, strContents);

      m_strIndexMd5 = Application.file().md5(strIndex);

      strBz = m_strCCAuth / strRelative + ".bz";
      ::DeleteFileW(::str::international::utf8_to_unicode(strBz));
      compress(strRelative);

      string strRelativeMd5 = "app\\stage\\metastage\\index-" + m_strFormatBuild + ".md5";
      strMd5 = m_strVrel / strRelativeMd5;
      Application.file().put_contents(strMd5, m_strIndexMd5);

      //string strStage = System.dir().path("C:\\home\\ccvotagus\\ca2_spa\\" + m_strVersionShift, strRelative) + ".bz";
      //::DeleteFileW(::str::international::utf8_to_unicode(
      // strStage));
      //System.file().copy(strStage, strBz);
      strRelease = m_strCCVrel / strRelative + ".bz";
      //::DeleteFileW(::str::international::utf8_to_unicode(
      // strRelease));
      Application.file().copy(strRelease, strBz);
      strRelease = m_strCCVrel / strRelativeMd5;
      Application.file().copy(strRelease, strMd5);
      strReleaseNew = m_strCCVrelNew / strRelative + ".bz";
      //::DeleteFileW(::str::international::utf8_to_unicode(
      // strRelease));
      Application.file().copy(strReleaseNew, strBz);
      strReleaseNew = m_strCCVrelNew / strRelativeMd5;
      Application.file().copy(strReleaseNew, strMd5);
   }

   void production::generate_appmatter_spa()
   {

      for (int32_t i = 0; i < m_straRoot.get_size(); i++)
      {
         generate_appmatter_spa(m_straRoot[i]);
      }

   }

   void production::generate_appmatter_spa(const ::file::path & pszRoot)
   {

      ::file::listing listing(get_app());

      string strBase = m_strBase / pszRoot / "appmatter";

      listing.ls_dir(strBase);

      for (int32_t i = 0; i < listing.get_count(); i++)
      {
         if (::str::begins(listing[i].name(), "_"))
         {
            generate_appmatter_spa_folder(pszRoot, listing[i].name());
         }
         else
         {
            generate_appmatter_spa_locale(pszRoot, listing[i].name());
         }
      }

   }

   void production::generate_appmatter_spa_folder(const ::file::path & pszRoot, const ::file::path & pszRelative)
   {

      ::file::listing listing(get_app());

      ::file::path strBase = m_strBase / pszRoot / "appmatter";

      strBase = strBase / pszRelative;

      listing.ls_dir(strBase);

      for (int32_t i = 0; i < listing.get_count(); i++)
      {
         if (::str::begins(listing[i].relative(), "_") && listing[i].relative() != "_std")
         {
            generate_appmatter_spa_folder(pszRoot, pszRelative / listing[i].relative());
         }
         else
         {
            generate_appmatter_spa_locale(pszRoot, pszRelative / listing[i].relative());
         }
      }

   }

   void production::generate_appmatter_spa_locale(const ::file::path & pszRoot, const ::file::path & pszRelative)
   {

      ::file::listing listing(get_app());

      ::file::path strBase = m_strBase / pszRoot / "appmatter";

      strBase = strBase / pszRelative;

      listing.ls_dir(strBase);

      for (int32_t i = 0; i < listing.get_count(); i++)
      {

         generate_appmatter_spa_style(pszRoot, pszRelative / listing[i].name());

      }

   }

   void production::generate_appmatter_spa_style(const ::file::path & pszRoot, const ::file::path & pszRelative)
   {

      ::file::listing listing(get_app());

      ::file::path strBase = m_strBase / pszRoot / "appmatter";

      strBase /= pszRelative;

      listing.ls_dir(strBase);

      for (int32_t i = 0; i < listing.get_count(); i++)
      {

         generate_appmatter_spa(pszRoot, pszRelative / listing[i].name());

      }

   }


   void production::generate_appmatter_spa(const ::file::path & pszRoot, const ::file::path & pszRelative)
   {

      string strStatus;

      strStatus = "updating appmatter spa spfile : " + string(pszRoot) + " : " + string(pszRelative);

      add_status(strStatus);

      ::file::listing straFiles(get_app());

      ::file::listing stra1;

      straFiles.rls(m_strBase / pszRoot / "appmatter" / pszRelative);

      strsize iBaseLen = m_strBase.get_length();

      if (m_strBase.Right(1) != "/" && m_strBase.Right(1) != "\\")
         iBaseLen++;

      string strRelative;
      ::file::path strFile;
      string strBz;
      string strUn;
      var varUnSize;
      string strMd5;
      var varBzSize;
      string strContents;
      int32_t i = 0;
      for (; i < straFiles.get_size(); i++)
      {
         ::file::path & strFile = straFiles[i];
         if (strFile.ext().compare_ci("zip") == 0)
         {
         }
         else if (Application.dir().is(strFile))
         {
            continue;
         }
         else if (strFile.ext() == "pdb")
         {
            continue;
         }
         else if (strFile.ext() == "ilk")
         {
            continue;
         }
         else if (strFile.ext() == "spa")
         {
            continue;
         }
         else if (strFile.ext() == "expand_fileset")
         {
            continue;
         }
         //strStatus.Format("compressing %s", strFile);
         //add_status(strStatus);
         strRelative = strFile.Mid(iBaseLen);
         strContents += strRelative;
         strContents += "\n";

         m_straFiles.add(strFile);
         //         m_straTitle.add(System.file().title_(strFile));
         //       m_straRelative.add(strRelative);

         stra1.add(strFile);
         //     stra2.add(straRelative[i]);

      }

      strRelative = string(pszRoot) + "\\appmatter\\" + string(pszRelative);

      strFile = m_strBase / "app\\stage\\metastage" / strRelative + ".expand_fileset.spa";

      m_straFiles.add(strFile);

      //      m_straTitle.add(System.file().title_(strFile));

      //m_straRelative.add("app\\stage\\metastagez\\" + strRelative + ".expand_fileset.spa");

      Application.file().put_contents(strFile, strContents);


      strFile = m_strBase / strRelative + ".expand_fileset";


      System.file().dtf(strFile, stra1, get_app());
      m_straFiles.add(strFile);
      //m_straTitle.add(System.file().title_(strFile));
      //m_straRelative.add(strRelative + ".expand_fileset.spa");


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
   def department(manifest, signature):
   manifest_sections.append(manifest)
   signature_sections.append(signature + digest(manifest))
   department('Manifest-Version: 1.0\n', 'Signature-Version: 1.0\n')
   for filepath, relpath, data in filelist:
   department('Name: %s\n%s' % (relpath, digest(data)), 'Name: %s\n' % relpath)
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
   string production::xpi_digest(memory & mem)
   {
      memory memMd5;
      memory memSha1;
      memMd5.from_hex(System.crypto().md5(mem));
      memSha1.from_hex(System.crypto().sha1(mem));
      return string("Digest-Algorithms: MD5 SHA1\n") +
             "MD5-Digest: " + System.base64().encode(memMd5) + "\n" +
             "SHA1-Digest: " + System.base64().encode(memSha1) + "\n";

   }


   void production::xpi_section(const char * pszManifest, const char * pszSignature)
   {
      m_straManifest.add(pszManifest);
      memory memManifest(pszManifest);
      m_straSignature.add(string(pszSignature) + xpi_digest(memManifest));
   }

   void production::add_path(const ::file::path & pszDir, const ::file::path & pszRelative)
   {
      m_straPath.add(pszDir / pszRelative);
      m_straPath.last().m_iRelative = strlen(pszDir) + 1;
   }

   void production::xpi_sign_dir(const char * pszPlatform, const ::file::path & pszDir)
   {

      m_straPath.remove_all();


      add_path(pszDir, "META-INF\\zigbert.rsa");
      add_path(pszDir, "install.rdf");
      add_path(pszDir, "chrome.manifest");
      add_path(pszDir, "plugins\\npca2.dll");

      ::file::listing straBase;

      //::lemon::array::copy(straBase, ::install::get_app_app_install_module_list(pszPlatform, m_strConfiguration));
      //::lemon::array::copy(straBase, ::install::get_app_app_install_module_list());

      //for (index i = 0; i < straBase.get_count(); i++)
      //{

      //   add_path(pszDir, "plugins\\" + straBase[i]);

      //}

      //add_path(pszDir,"plugins\\aura.dll");
      //add_path(pszDir,"plugins\\aurasqlite.dll");
      //add_path(pszDir,"plugins\\axis.dll");
      //add_path(pszDir,"plugins\\axisbsdiff.dll");
      //add_path(pszDir,"plugins\\axisbzip2.dll");
      //add_path(pszDir,"plugins\\axisfreeimage.dll");
      //add_path(pszDir,"plugins\\axisfreetype.dll");
      //add_path(pszDir,"plugins\\axisidn.dll");
      //add_path(pszDir,"plugins\\axismysql.dll");
      //add_path(pszDir,"plugins\\axisopenssl.dll");
      //add_path(pszDir,"plugins\\axiszlib.dll");
      //add_path(pszDir, "plugins\\base.dll");
      //add_path(pszDir, "plugins\\msvcp120d.dll");
      //add_path(pszDir, "plugins\\msvcr120d.dll");
      //add_path(pszDir, "plugins\\draw2d_gdiplus.dll");
      string strIconUrl;
      if (m_eversion == version_basis)
      {
         add_path(pszDir, "skin\\classic\\fluidbasis-5c-32.png");
      }
      else
      {
         add_path(pszDir, "skin\\classic\\ca2-5c-32.png");
      }
      add_path(pszDir, "META-INF\\manifest.mf");
      add_path(pszDir, "META-INF\\zigbert.sf");

      string strSignerPath = "C:\\sensitive\\sensitive\\certificate\\npca2signer.pem";
      string strKeyPath = "C:\\sensitive\\sensitive\\certificate\\npca2key.pem";
      string strOthersPath = "C:\\sensitive\\sensitive\\certificate\\npca2others.pem";


      // Generate manifest.mf and zigbert.sf data

      m_straManifest.remove_all();
      m_straSignature.remove_all();

      string strComment = "Created-By: ca2 production " + m_strBuild + "\nComments: PLEASE DO NOT EDIT THIS FILE. YOU WILL BREAK IT.\n";


      xpi_section("Manifest-Version: 1.0\n" + strComment, "Signature-Version: 1.0\n" + strComment);

      memory mem;

      for (int32_t i = 0; i < m_straPath.get_count(); i++)
      {
         ::file::path strRelative = m_straPath[i].relative();
         if (::str::begins_ci(strRelative, "META-INF\\"))
            continue;
         strRelative.replace("\\", "/");
         mem.allocate(0);
         Application.file().as_memory(m_straPath[i], mem);
         xpi_section("Name: " + strRelative + "\n" + xpi_digest(mem), "Name: " + strRelative + "\n");
      }


      string strManifest = m_straManifest.implode("\n");
      string strSignature = m_straSignature.implode("\n");

      Application.file().put_contents(pszDir / "META-INF/manifest.mf", strManifest);
      Application.file().put_contents(pszDir / "META-INF/zigbert.sf", strSignature);

      System.crypto().np_make_zigbert_rsa(pszDir, strSignerPath, strKeyPath, strOthersPath, strSignature);

   }



   bool production::release_npca2(const char * pszPlatform)
   {

      string strPlatform(pszPlatform);

      string strDir;
      strDir = m_strBase / "time/npca2" / strPlatform;


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
      string strIconUrl;
      if (m_eversion == version_basis)
      {
         strVersionUrl = "basis";
         strIconUrl = "chrome://npca2@ca2.cc/skin/fluidbasis-5c-32.png";
      }
      else
      {
         strVersionUrl = "stage";
         strIconUrl = "chrome://npca2@ca2.cc/skin/ca2-5c-32.png";
      }

      string strChromeManifest = Application.file().as_string(m_strBase / "nodeapp/stage/matter/npca2/chrome.manifest");
      strChromeManifest.replace("%BUILD%", strNpca2Version);
      strChromeManifest.replace("%PLATFORM%", "/" + m_strFormatBuild + "/stage/" + strPlatform);
      strChromeManifest.replace("%DOWNLOADSITE%", m_strDownloadSite);
      strChromeManifest.replace("%VERSION%", strVersionUrl);
      strChromeManifest.replace("%ICONURL%", strIconUrl);

      Application.file().put_contents(strDir / "npca2" / "chrome.manifest", strChromeManifest);

      string strIcon;
      string strIconName;
      if (m_eversion == version_basis)
      {
         strIcon = Application.dir().matter("fluidbasis-5c-32.png");
         strIconName = "fluidbasis-5c-32.png";


      }
      else
      {
         strIcon = Application.dir().matter("ca2-5c-32.png");
         strIconName = "ca2-5c-32.png";
      }
      Application.file().copy(strDir / "npca2/skin/classic" / strIconName, strIcon);

      string strInstall = Application.file().as_string(m_strBase / "nodeapp/stage/matter/npca2/install.rdf");
      strInstall.replace("%BUILD%", strNpca2Version);
      strInstall.replace("%PLATFORM%", "/plugin/" + strPlatform);
      strInstall.replace("%DOWNLOADSITE%", "anycast.ca2.cc/ccvotagus");
      strInstall.replace("%VERSION%", strVersionUrl);
      strInstall.replace("%ICONURL%", strIconUrl);

      Application.file().put_contents(strDir / "npca2/install.rdf", strInstall);


      string strWindows = Application.file().as_string(m_strBase / "nodeapp/stage/matter/npca2/npca2_windows.rdf");
      strWindows.replace("%BUILD%", strNpca2Version);
      strWindows.replace("%PLATFORM%", "/" + m_strFormatBuild + "/stage/" + strPlatform);
      strWindows.replace("%DOWNLOADSITE%", m_strDownloadSite + "/ccvotagus");
      strWindows.replace("%VERSION%", strVersionUrl);
      strWindows.replace("%ICONURL%", strIconUrl);
      Application.file().put_contents(strDir / "npca2_windows.rdf", strWindows);


      //add_status("Signing npca2.dll for Firefox ...");
      //string strFile = strDir /  "npca2/plugins", "npca2.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/npca2.dll"));
      //string strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);



      //add_status("Signing app_app_admin.exe for Firefox ...");
      //strFile = strDir /  "npca2/plugins", "app_app_admin.exe");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/app_app_admin.exe"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      ::file::listing straBase;

      //::lemon::array::copy(straBase,::install::get_app_app_install_module_list(strPlatform, m_strConfiguration));
//      ::lemon::array::copy(straBase, ::install::get_app_app_install_module_list());

      straBase.add("npca2.dll");

      string strFile;

      string strCmd;

      for (index i = 0; i < straBase.get_count(); i++)
      {

         ::file::path strLibrary = straBase[i];

         add_status("Signing " + strLibrary + " for Firefox ...");

         strFile = strDir / "npca2/plugins" / strLibrary;

         Application.file().copy(strFile, m_strVrel / "time" / stage_platform(strPlatform) / m_strConfiguration / strLibrary);

         strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";

         System.process().synch(strCmd);

      }


      //add_status("Signing base.dll for Firefox ...");
      //strFile = strDir /  "npca2/plugins", "base.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/base.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      /*
          add_status("Signing os.dll for Firefox ...");
          strFile = strDir /  "npca2/plugins", "os.dll");
          Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/os.dll"));
          strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
          System.process().synch(strCmd);
          */

      //add_status("Signing msvcr120d.dll for Firefox ...");
      //strFile = strDir /  "npca2/plugins", "msvcr120d.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/msvcr120d.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      //add_status("Signing msvcp120d.dll for Firefox ...");
      //strFile = strDir /  "npca2/plugins", "msvcp120d.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/msvcp120d.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      //strFile = strDir /  "npca2/plugins", "draw2d_gdiplus.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/draw2d_gdiplus.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      add_status("Signing code for Firefox ...");

      Application.file().del(strDir / "npca2.xpi");

      create_xpi(pszPlatform, false);

      Application.file().copy(m_strVrel / "time" / stage_platform(strPlatform) / m_strConfiguration / "npca2.xpi", strDir / "npca2.xpi");
      Application.file().copy(m_strCCVrel / "plugin" / strPlatform / "npca2_windows.rdf", strDir / "npca2_windows.rdf");

      return true;
   }

   bool production::create_xpi(const char * pszPlatform, bool bSigned)
   {

      string strPlatform(pszPlatform);

      string strDir;

      strDir = m_strBase / "time/npca2/" + strPlatform;

      Application.dir().rm(strDir / "npca2/META-INF");


      if (bSigned)
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

      strDir = m_strBase / "time/npca2" / strPlatform;

      add_status("Signing extension ...");

      xpi_sign_dir(pszPlatform, strDir / "npca2");

      string str;

      string strXpi = strDir / "npca2.xpi";

      string strPath;

      ::process::process_sp process(allocer());

      for (int32_t i = 0; i < m_straPath.get_count(); i++)
      {
         strPath = "zip -9 \"" + strXpi + "\" \"" + m_straPath[i].relative() + "\"";
         if (!process->create_child_process(strPath, false, strDir / "npca2"))
         {
            uint32_t dw = GetLastError();
            string str;
            str.Format("Error compressing npca2: %d is zip command line utility installed?", dw);
            add_status(str);
            return 0;
         }
         while (!process->has_exited())
         {
            Sleep(300);
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
      strDir = m_strBase / "time/npca2" / strPlatform;

      add_status("Creating uint32_t extension ...");
      string str;
      string strXpi = strDir / "npca2.xpi";
      ::process::process_sp process(allocer());
      string strPath = "zip -9 -r -D \"" + strXpi + "\" * ";
      if (!process->create_child_process(strPath, false, strDir / "npca2"))
      {
         uint32_t dw = GetLastError();
         string str;
         str.Format("Error compressing npca2: %d is zip command line utilty installed?", dw);
         add_status(str);
         return 0;
      }
      int32_t i = 1;
      while (!process->has_exited())
      {
         Sleep(300);
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


      Application.dir().mk(m_strBase / "time\\iexca2" / strPlatform);

      string strNpca2Version;

      strNpca2Version.Format(
      "%d,%d%02d,%d%02d,%d",
      atoi(m_strFormatBuild.Mid(0, 4)),
      atoi(m_strFormatBuild.Mid(5, 2)),
      atoi(m_strFormatBuild.Mid(8, 2)),
      atoi(m_strFormatBuild.Mid(11, 2)),
      atoi(m_strFormatBuild.Mid(14, 2)),
      atoi(m_strFormatBuild.Mid(17, 2))
      );

      string strChromeManifest = Application.file().as_string(m_strBase / "nodeapp/stage/script/iexca2.inf");
      strChromeManifest.replace("%VERSION%", strNpca2Version);
      //      strChromeManifest.replace("%PLATFORM%", "/" + m_strFormatBuild + "/stage/" + strPlatform);
      //    strChromeManifest.replace("%DOWNLOADSITE%", m_strDownloadSite);
      //      strChromeManifest.replace("%VERSION%", strVersionUrl);
      Application.file().put_contents(m_strBase / "time\\iexca2" / strPlatform / "iexca2.inf", strChromeManifest);


      string str;
      ::process::process_sp process(allocer());
      ::file::path strPath;
      strPath = m_strBase / "nodeapp\\stage\\script\\makecab" + string(pszPlatform) + "_" + m_strConfiguration + ".bat";
      if (!process->create_child_process(strPath, false, strPath.folder()))
      {
         uint32_t dw = GetLastError();
         string str;
         str.Format("Error creating iexca2.cab: %d", dw);
         add_status(str);
         return 0;
      }
      int32_t i;
      i = 1;
      while (!process->has_exited())
      {
         Sleep(5000);
         str.Format("%d Creating iexca2.cab  " + string(pszPlatform) + "...", i);
         add_status(str);
         i++;
      }

      Application.file().copy(m_strVrel / "time" / stage_platform(strPlatform) / m_strConfiguration / "iexca2.cab", m_strBase / "time\\iexca2" / strPlatform / "iexca2.cab");

      return true;

   }

   bool production::release_crxca2(const char * pszPlatform)
   {

      string strPlatform(pszPlatform);

      ::file::path strDir;

      strDir = m_strBase / "time/crxca2" / strPlatform / "crxca2";

      string strCrxca2Version;

      int iHour = atoi(m_strFormatBuild.Mid(11, 2));

      if (iHour == 0)
         iHour = 24;

      int iSecond = atoi(m_strFormatBuild.Mid(17, 2));

      if (iSecond == 0)
         iSecond = 60;

      strCrxca2Version.Format(
      "%d.%d%02d.%d%02d.%d",
      atoi(m_strFormatBuild.Mid(0, 4)),
      atoi(m_strFormatBuild.Mid(5, 2)),
      atoi(m_strFormatBuild.Mid(8, 2)),
      iHour,
      atoi(m_strFormatBuild.Mid(14, 2)),
      atoi(m_strFormatBuild.Mid(17, 2)),
      iSecond);

      string strIconUrl;
      if (m_eversion == version_basis)
      {
         strIconUrl = "fluidbasis-5c-32.png";
      }
      else
      {
         strIconUrl = "ca2-5c-32.png";
      }


      string strManifestJson = Application.file().as_string(m_strBase / "nodeapp/stage/matter/crxca2/manifest.json");
      strManifestJson.replace("%BUILD%", strCrxca2Version);
      strManifestJson.replace("%PLATFORM%", strPlatform);
      strManifestJson.replace("%DOWNLOADSITE%", m_strDownloadSite);
      strManifestJson.replace("%ICONURL%", strIconUrl);
      Application.file().put_contents(strDir / "manifest.json", strManifestJson);

      string strIcon;
      string strIconName;
      if (m_eversion == version_basis)
      {
         strIcon = Application.dir().matter("fluidbasis-5c-32.png");
         strIconName = "fluidbasis-5c-32.png";


      }
      else
      {
         strIcon = Application.dir().matter("ca2-5c-32.png");
         strIconName = "ca2-5c-32.png";
      }
      Application.file().copy(strDir / strIconName, strIcon);


      ::file::listing straBase;

      //::lemon::array::copy(straBase,::install::get_app_app_install_module_list(strPlatform, m_strConfiguration));
      //::lemon::array::copy(straBase, ::install::get_app_app_install_module_list());

      //straBase.add("npca2.dll");

      string strFile;

      string strCmd;

      for (index i = 0; i < straBase.get_count(); i++)
      {

         ::file::path strLibrary = straBase[i];

         add_status("Signing " + strLibrary + " for Chrome ...");

         strFile = strDir / "npca2/plugins" / strLibrary;

         Application.file().copy(strFile, m_strVrel / "time" / stage_platform(strPlatform) / m_strConfiguration / strLibrary);

         strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";

         System.process().synch(strCmd);

      }

      //add_status("Signing npca2.dll for Chrome ...");
      //string strFile = strDir /  "npca2.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/npca2.dll"));
      //string strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      //add_status("Signing app_app_admin.exe for Chrome ...");
      //strFile = strDir /  "app_app_admin.exe");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/app_app_admin.exe"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      //add_status("Signing base.dll for Chrome ...");
      //strFile = strDir /  "base.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/base.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      /*
          add_status("Signing os.dll for Chrome ...");
          strFile = strDir /  "os.dll");
          Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/os.dll"));
          strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
          System.process().synch(strCmd);
          */

      //add_status("Signing msvcp120d.dll for Chrome ...");
      //strFile = strDir /  "msvcp120d.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/msvcp120d.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      //add_status("Signing msvcr120d.dll for Chrome ...");
      //strFile = strDir /  "msvcr120d.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/msvcr120d.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      //add_status("Signing draw2d_gdiplus.dll for Chrome ...");
      //strFile = strDir /  "draw2d_gdiplus.dll");
      //Application.file().copy(strFile, m_strVrel / "stage/" + strPlatform + "/draw2d_gdiplus.dll"));
      //strCmd = "\"" + m_strSignTool + "\" sign /f \"" + m_strSpc + "\" /p " + m_strSignPass + " \"" + strFile + "\"";
      //System.process().synch(strCmd);

      add_status("Creating crxca2.crx for Chrome ...");

      WCHAR * pwsz;

      if (FAILED(SHGetKnownFolderPath(FOLDERID_ProgramFilesX86, 0, NULL, &pwsz)))
      {

         add_status("Chrome not found...");

         return false;

      }

      strCmd = "\"";

      strCmd += pwsz;

      CoTaskMemFree(pwsz);

      strCmd += "\\Google\\Chrome\\Application\\chrome.exe\" --no-message-box --pack-extension=\"" + strDir + "\" --pack-extension-key=\"C:\\sensitive\\sensitive\\certificate\\npca2pk.pem\"";

      System.process().synch(strCmd);

      string strVersion;

      if (m_eversion == version_basis)
      {
         strVersion = "\\basis";
      }


      Application.file().copy(m_strVrel / "time" / stage_platform(strPlatform) / m_strConfiguration / "crxca2.crx", strDir.folder() / "crxca2.crx");

      return true;
   }


   void production::add_status(const char * psz)
   {
      {
         single_lock sl(&m_mutexStatus, TRUE);
         m_straStatus.add(psz);
      }
      TRACE("%s", psz);
      m_pview->post_message(WM_USER, 1);
   }

   void production::change_status(const char * psz)
   {
      {
         single_lock sl(&m_mutexStatus, TRUE);
         if (m_straStatus.get_count() == 0)
         {
            m_straStatus.add(psz);
         }
         else
         {
            m_straStatus.last() = psz;
         }
      }
      m_pview->post_message(WM_USER, 1);
   }



   production::release::release(production * pproduction, const char * pszRelease, const char * pszServer) :
      ::object(pproduction->get_app()),
      thread(pproduction->get_app()),
      m_strRelease(pszRelease),
      m_strServer(pszServer)
   {

      m_pproduction = pproduction;

      {

         synch_lock sl(&m_pproduction->m_mutexRelease);

         m_pproduction->m_iRelease++;

         m_pproduction->m_straRelease.add(m_strServer);

      }

      m_pproduction->OnUpdateRelease();

   }

   bool production::release::init_thread()
   {

      return true;

   }


   int32_t production::release::raw_run()
   {

      string str;

      property_set set(get_app());

      set["disable_ca2_sessid"] = true;

      Application.http().get(m_strRelease, str, set);

      synch_lock sl(&m_pproduction->m_mutexRelease);

      m_pproduction->m_iRelease--;

      m_pproduction->m_straRelease.remove(m_strServer);

      return 0;

   }



   void production::release::run()
   {

      int iResult = raw_run();

      m_pproduction->OnUpdateRelease();

      if (iResult != 0)
      {

         m_error.m_iaErrorCode2.add(iResult);

      }

   }


   void production::OnUpdateRelease()
   {
      if (m_iRelease == 0 && m_bEndProduction && !m_bEndStatus)
      {
         m_bEndStatus = true;
         m_evFinish.wait();
         m_evFinish.ResetEvent();
         add_status("All releases have been commanded and production has deemed ended!!.!.!!.");


         property_set set;

         string strEndTime;

         m_timeEnd.FormatGmt(strEndTime, "%Y-%m-%d %H-%M-%S");

         if (m_eversion == version_basis)
         {
            set["post"]["new_status"] = "<div style=\"display: block; background-color: #FFE0FF; \"><h2 style=\"margin-bottom:0px; color: #FF55CC;\">" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #882255; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://code.ca2.cc/\" class=\"fluidbasis\" >basis</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://laboratory.ca2.cc/\" class=\"fluidbasis\" >laboratory.ca2.cc</a> or <a href=\"http://warehouse.ca2.cc/\" class=\"fluidbasis\" >warehouse.ca2.cc</a> for simple gateway implementations.</span></div>";
         }
         else
         {
            set["post"]["new_status"] = "<div style=\"display: block; background-color: #E0FFCC; \"><h2 style=\"margin-bottom:0px; color: #55CCAA;\">" + version_to_international_datetime(m_strBuild) + "</h2><span style=\"color: #228855; display: block; margin-bottom: 1.5em;\">" + m_strBuildTook + " and finished at " + strEndTime + "<br>New release of <a href=\"http://ca2.cc/\">stage</a> applications labeled " + m_strBuild + " is ready for download through compatible gateways.<br>Check <a href=\"http://desktop.ca2.cc/\">desktop.ca2.cc</a> or <a href=\"http://store.ca2.cc/\">store.ca2.cc</a> for simple gateway implementations.</span></div";
         }

         string str;

         Application.http().get("http://api.ca2.cc/status/insert", str, set);

         string strTwit;

         strTwit = "New build " + m_strBuild;

         if (m_iGlobalRetry > 0)
         {

            if (m_iGlobalRetry == 1)
            {
               strTwit += " with 1 retry";
            }
            else
            {
               strTwit += " with " + ::str::from(m_iGlobalRetry) + " retries";
            }
         }

         if (Application.m_eversion == version_basis)
         {

            strTwit += " : http://basis.ca2.cc/";

         }
         else
         {

            strTwit += " : http://stage.ca2.cc/";

         }

         twitter_twit(strTwit);

         m_bReleased = true;

         Sleep(2000 + 1977);

         defer_quit();

      }
      else if (m_iRelease > 0)
      {
         synch_lock sl(&m_mutexRelease);
         string strStatus;
         strStatus.Format("There are %d releases in command list!! (%s)", m_iRelease, m_straRelease.implode(";"));
         add_status(strStatus);

      }
   }


   bool production::twitter_auth()
   {


      ::hi5::twit twitterObj(get_app());


      string tmpStr("");
      string replyMsg("");


      /* OAuth flow begins */
      /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      string strPathKey = System.dir().appdata() / "twitterClient_token_key" + ::str::from((int)m_eversion) + ".txt";
      string strPathSecret = System.dir().appdata() / "twitterClient_token_secret" + ::str::from((int)m_eversion) + ".txt";
      /* Step 1: Check if we alredy have OAuth access token from a previous run */
      //    char szKey[1024];
      string myOAuthAccessTokenKey = Application.file().as_string(strPathKey);
      string myOAuthAccessTokenSecret = Application.file().as_string(strPathSecret);

      if (myOAuthAccessTokenKey.has_char() && myOAuthAccessTokenSecret.has_char())
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey(myOAuthAccessTokenKey);
         twitterObj.get_oauth().setOAuthTokenSecret(myOAuthAccessTokenSecret);
      }
      else
      {
         /* Step 2: Get request token key and secret */
         twitterObj.oAuthRequestToken(tmpStr);

         /* Step 3: Ask user to visit web link and get PIN */
         string szOAuthVerifierPin;

         ::hi5::twitter::authorization authapp(get_app(), tmpStr, "twitter\\authorization.xhtml", true);
         szOAuthVerifierPin = authapp.get_pin();

         tmpStr = szOAuthVerifierPin;
         twitterObj.get_oauth().setOAuthPin(tmpStr);

         /* Step 4: Exchange request token with access token */
         twitterObj.oAuthAccessToken();

         /* Step 5: Now, save this access token key and secret for future use without PIN */
         twitterObj.get_oauth().getOAuthTokenKey(myOAuthAccessTokenKey);
         twitterObj.get_oauth().getOAuthTokenSecret(myOAuthAccessTokenSecret);

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
      string tmpStr("");
      string replyMsg("");

      /* OAuth flow begins */
      /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      string strPathKey = System.dir().appdata() / "twitterClient_token_key" + ::str::from((int)m_eversion) + ".txt";
      string strPathSecret = System.dir().appdata() / "twitterClient_token_secret" + ::str::from((int)m_eversion) + ".txt";
      /* Step 1: Check if we alredy have OAuth access token from a previous run */
      //    char szKey[1024];
      string myOAuthAccessTokenKey = Application.file().as_string(strPathKey);
      string myOAuthAccessTokenSecret = Application.file().as_string(strPathSecret);

      if (myOAuthAccessTokenKey.has_char() && myOAuthAccessTokenSecret.has_char())
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey(myOAuthAccessTokenKey);
         twitterObj.get_oauth().setOAuthTokenSecret(myOAuthAccessTokenSecret);
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
      if (twitterObj.statusUpdate(tmpStr))
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
         && document.get_root()->child_at(0)->get_value().compare_ci("Status is a duplicate.") != 0)
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
      if (iRetry >= 3)
      {
         return replyMsg = "failed";
      }
      Application.file().del(strPathKey);
      Application.file().del(strPathSecret);
      twitter_auth();
      iRetry++;
      goto Retry2;

   }

   string production::facebook_status(const char * pszMessage)
   {

      int32_t iRetry = 0;

Retry2:

      ::hi5::twit twitterObj(get_app());
      string tmpStr("");
      string replyMsg("");

      /* OAuth flow begins */
      /* Step 0: set OAuth related params. These are got by registering your app at twitter.com */
      twitterObj.get_oauth().setConsumerKey(m_strTwitterConsumerKey);
      twitterObj.get_oauth().setConsumerSecret(m_strTwitterConsumerSecret);

      string strPathKey = System.dir().appdata() / "facebookClient_token_key" + ::str::from_int(m_eversion) + ".txt";
      string strPathSecret = System.dir().appdata() / "facebookClient_token_secret" + ::str::from_int(m_eversion) + ".txt";
      /* Step 1: Check if we alredy have OAuth access token from a previous run */
      //    char szKey[1024];
      string myOAuthAccessTokenKey = Application.file().as_string(strPathKey);
      string myOAuthAccessTokenSecret = Application.file().as_string(strPathSecret);

      if (myOAuthAccessTokenKey.has_char() && myOAuthAccessTokenSecret.has_char())
      {
         /* If we already have these keys, then no need to go through auth again */
         twitterObj.get_oauth().setOAuthTokenKey(myOAuthAccessTokenKey);
         twitterObj.get_oauth().setOAuthTokenSecret(myOAuthAccessTokenSecret);
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
      if (twitterObj.statusUpdate(tmpStr))
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
         && document.get_root()->child_at(0)->get_value().compare_ci("Status is a duplicate.") != 0)
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
      if (iRetry >= 3)
      {
         return replyMsg = "failed";
      }
      Application.file().del(strPathKey);
      Application.file().del(strPathSecret);
      facebook_auth();
      iRetry++;
      goto Retry2;

   }

   bool production::facebook_auth()
   {
      return true;
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

      // Build before production!!
      //
      // When SSD (solid state drive) is back to build machine or some very faster Drive, you may reintroducde build inline
      //

      return;

      string strApp(psz);

      add_status("Building ca2 fontopus ccvotagus " + strApp + "...");
      //{
      //   string str;


      //   property_set set;

      //   set["post"]["new_status"] = "<div style=\"display: block; " + m_strBackPostColor + "\"><h5 style=\"margin-bottom:0px; " + m_strEmpPostColor + "\">" + version_to_international_datetime(m_strStartTime) + "</h5><span style=\"" + m_strStdPostColor + m_strBackPostColor + " display: block; margin-bottom: 0.95em;\">" + version_to_international_datetime(::datetime::time::get_current_time().FormatGmt("%Y-%m-%d %H-%M-%S")) + " Building " + strApp + "...</span></div>";

      //   Application.http().get("http://api.ca2.cc/status/insert", str, set);

      //}
      ::process::process_sp process(allocer());
      string strPath;
      if (Application.m_eversion == version_basis)
      {
         strPath = m_strBase / strApp / "stage\\script\\basis_build.bat";
      }
      else
      {
         strPath = m_strBase / strApp / "stage\\script\\stage_build.bat";
      }
      if (!process->create_child_process(strPath, true))
      {
         uint32_t dw = GetLastError();
         string str;
         str.Format("Error creating build process: %d for build of " + strApp, dw);
         add_status(str);
         return;
      }
      int32_t i = 1;
      string str;
      string strAccumul;
      while (!process->has_exited())
      {
         Sleep(100);
         //str.Format("%d Building ca2 fontopus ccvotagus " + strApp + "...", i);
         while (true)
         {
            str = process->read();
            strAccumul += str;
            if (!str.has_char())
               break;
         }
         index iFind;
         while ((iFind = strAccumul.find("\r\n")) >= 0)
         {
            add_status(strAccumul.Left(iFind));
            strAccumul = strAccumul.Mid(iFind + 2);
         }

         i++;
      }
      while (true)
      {
         str = process->read();
         strAccumul += str;
         if (!str.has_char())
            break;
      }
      index iFind;
      while ((iFind = strAccumul.find("\r\n")) >= 0)
      {
         add_status(strAccumul.Left(iFind));
         strAccumul = strAccumul.Mid(iFind + 2);
      }
      add_status(strAccumul);

   }

   string production::version_to_international_datetime(const char * psz)
   {
      string str(psz);
      str.replace("-", ":", 11);
      return str;
   }


   void production::update_rc_file_version(const char * pszUrl)
   {

      string str = Application.file().as_string(pszUrl);

      index iFind1 = str.find_tail("FILEVERSION ");
      if (iFind1 > 0)
      {

         index iFind2 = str.find("\r\n", iFind1);

         string strVersion1;

         strVersion1.Format(
         "%d,%d%02d,%d%02d,%d",
         atoi(m_strFormatBuild.Mid(0, 4)),
         atoi(m_strFormatBuild.Mid(5, 2)),
         atoi(m_strFormatBuild.Mid(8, 2)),
         atoi(m_strFormatBuild.Mid(11, 2)),
         atoi(m_strFormatBuild.Mid(14, 2)),
         atoi(m_strFormatBuild.Mid(17, 2))
         );

         str = str.Left(iFind1) + strVersion1 + str.Mid(iFind2);

      }

      iFind1 = str.find_tail("VALUE \"FileVersion\", \"");
      if (iFind1 > 0)
      {

         index iFind2 = str.find("\"", iFind1);

         string strVersion2;

         strVersion2.Format(
         "%d.%d%02d.%d%02d.%d",
         atoi(m_strFormatBuild.Mid(0, 4)),
         atoi(m_strFormatBuild.Mid(5, 2)),
         atoi(m_strFormatBuild.Mid(8, 2)),
         atoi(m_strFormatBuild.Mid(11, 2)),
         atoi(m_strFormatBuild.Mid(14, 2)),
         atoi(m_strFormatBuild.Mid(17, 2))
         );

         str = str.Left(iFind1) + strVersion2 + str.Mid(iFind2);

      }

      Application.file().put_contents(pszUrl, str);


   }


   string production::stage_platform(string strPlatform)
   {

      if (strPlatform.compare_ci("x86") == 0
            || strPlatform.compare_ci("Win32") == 0)
      {

         return "Win32";

      }

      if (strPlatform.compare_ci("x64") == 0
            || strPlatform.compare_ci("amd64") == 0)
      {

         return "x64";

      }

      return strPlatform;

   }

} // namespace production



