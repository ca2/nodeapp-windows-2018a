#include "framework.h"


namespace app_app
{


   tool::tool(::aura::application * papp) :
      ::object(papp),
      ::sockets::socket_handler(papp),
      ::netserver::socket_handler(papp)
   {

   }


   tool::~tool()
   {


   }


   void tool::app_install_call_sync(const char * pszPlatform, const char * szParameters, const char * pszBuild)
   {

      bool bLaunch;

      if (_stricmp(szParameters, "exit") == 0 || _stricmp(szParameters, "quit") == 0)
      {

         bLaunch = false;

      }
      else
      {

         bLaunch = true;

      }

      app_install_send_short_message(pszPlatform, szParameters, bLaunch, pszBuild);

   }


   bool tool::app_install_send_short_message(const char * pszPlatform, const char * pszParams, bool bLaunch, const char * pszBuild)
   {

#ifdef METROWIN

      throw "todo";

#else

      ::aura::ipc::tx txchannel(::get_thread_app());

      ::install::launcher launcher(pszPlatform, "", "");

      string strChannel;

      // "core/spaboot_install"

      strChannel = "::ca2::fontopus::ca2_spaboot_install_" + string(pszPlatform) + "::7807e510-5579-11dd-ae16-0800200c7784";

      if (!txchannel.open(strChannel, bLaunch ? &launcher : NULL))
         return false;

      txchannel.send(pszParams, INFINITE);

#endif

      return true;

   }



   void tool::start_app_install_in_context(string strPlatform, bool bAlreadyElevated)
   {

      ::install::launcher launcher(strPlatform, "", "");

      launcher.start_in_context(bAlreadyElevated);

   }


   void tool::http_get_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform)
   {

      if (straTemplate.get_size() <= 0)
      {

         return;

      }

      string strFormatBuild(pszFormatBuild);

      //string strUrl;

      //strUrl = "https://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + m_strVersion + "&stage=";
      //strUrl += url().url_encode(string(pszTemplate));
      //strUrl += "&build=";
      //strUrl += url().url_encode(string(pszFormatBuild));
      //strUrl += "&platform=";
      //strUrl += url().url_encode(strPlatform);

      //return http_get(strUrl);

      string strUrl;

      strUrl = "https://" + System.get_system_configuration() + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + System.get_system_configuration() + "&stage=";
      strUrl += System.url().url_encode(straTemplate.implode(","));
      strUrl += "&build=";
      strUrl += System.url().url_encode(strFormatBuild);
      strUrl += "&platform=";
      strUrl += System.url().url_encode(strPlatform);

      string strMd5List = Application.http_get(strUrl);

      stringa straMd5;

      straMd5.add_tokens(strMd5List.c_str(), ",");

      if (straMd5.get_size() != straTemplate.get_size())
      {

         return;

      }

      for (index i = 0; i < straTemplate.get_size(); i++)
      {

         md5_item & item = m_mapMd5[straTemplate[i]][strFormatBuild][strPlatform];

         // if (item.m_strMd5.is_empty() || (get_tick_count() - item.m_dwStart) > 10000)
         //{

         item.m_strMd5 = straMd5[i];

         item.m_dwStart = get_tick_count();

      }

   }


   stringa tool::get_reference_md5(stringa straTemplate, const char * pszFormatBuild, string strPlatform)
   {

      string strFormatBuild(pszFormatBuild);

      stringa straRequest;

      for (index i = 0; i < straTemplate.get_size(); i++)
      {

         md5_item & item = m_mapMd5[straTemplate[i]][strFormatBuild][strPlatform];

         if (item.m_strMd5.is_empty() || (get_tick_count() - item.m_dwStart) > 30000)
         {

            straRequest.add(straTemplate[i]);

         }

      }

      if (straRequest.has_elements())
      {

         http_get_md5(straRequest, strFormatBuild, strPlatform);

      }

      stringa straMd5;

      for (index i = 0; i < straTemplate.get_size(); i++)
      {

         md5_item & item = m_mapMd5[straTemplate[i]][strFormatBuild][strPlatform];

         straMd5.add(item.m_strMd5);

      }

      return straMd5;

   }


   bool tool::is_file_ok(const char * path1, const char * pszTemplate, const char * pszFormatBuild, string strPlatform)
   {

      if (!file_exists_dup(path1))
      {

         return false;

      }

      stringa straReferenceMd5 = get_reference_md5({ pszTemplate }, pszFormatBuild, strPlatform);

      if (straReferenceMd5.get_size() <= 0)
      {

         return false;

      }

      if (straReferenceMd5[0].compare_ci(file_md5_dup(path1)) != 0)
      {

         return false;

      }

      return true;

   }


   bool tool::is_file_ok(const stringa & straPath, const stringa & straTemplate, stringa & straMd5, const string & strFormatBuild, int iMd5Retry, string strPlatform)
   {

      bool bOk = true;

      if (straPath.size() != straTemplate.size())
      {

         return false;

      }

      if (bOk)
      {

         for (int i = 0; i < straPath.size(); i++)
         {

            string strPath = straPath[i];

            if (!file_exists_dup(strPath))
            {

               bOk = false;

               break;

            }

         }

      }

      if (iMd5Retry > 0 || straMd5.size() != straPath.size())
      {

         /*string strUrl;

         strUrl = "https://" + m_strVersion + "-server.ca2.cc/api/spaignition/md5a?authnone&version=" + m_strVersion + "&stage=";
         strUrl += url().url_encode(straTemplate.implode(","));
         strUrl += "&build=";
         strUrl += url().url_encode(strFormatBuild);
         strUrl += "&platform=";
         strUrl += url().url_encode(strPlatform);*/

         straMd5 = get_reference_md5(straTemplate, strFormatBuild, strPlatform);

         //string strMd5List = ms_get(strUrl.c_str());

         //straMd5.add_tokens(strMd5List.c_str(), ",");

         if (straMd5.size() != straPath.size())
         {

            OutputDebugString("Error: Fetched " + str::from(straMd5.size()));
            OutputDebugString(" from " + str::from(straPath.size()));
            OutputDebugString("\r\n");

            int iMax = MAX(straMd5.size(), straPath.size());
            for (index i = 0; i < iMax; i++)
            {

               OutputDebugString("File: ");
               if (i < straPath.size())
               {
                  OutputDebugString(straPath[i]);
               }
               else
               {
                  OutputDebugString("(missing)");
               }
               OutputDebugString("\r\n");
               OutputDebugString("MD5: ");
               if (i < straPath.size())
               {
                  OutputDebugString(straMd5[i]);
               }
               else
               {
                  OutputDebugString("(missing)");
               }
               OutputDebugString("\r\n");

            }

            return false;

         }

         if (!bOk)
         {

            return false;

         }

      }

      for (int i = 0; i < straMd5.size(); i++)
      {

         string strPath = straPath[i];

         string strFileMd5 = file_md5_dup(strPath);

         string strMd5 = straMd5[i];

         if (strFileMd5.compare_ci(strMd5) != 0)
         {

            OutputDebugString("MD5 failed: " + strPath + " file MD5=" + strFileMd5 + " ref MD5=" + strMd5);
            OutputDebugString("\r\n");

            return false;

         }

      }

      return true;

   }


   void tool::trace(const string & str)
   {

      synch_lock sl(m_pmutex);

      trace_file(get_app(), m_strTraceLabel).print(str);

   }


   void tool::trace(double dRate)
   {

      synch_lock sl(m_pmutex);

      trace_file(get_app(), m_strTraceLabel).print(dRate);

   }


   bool tool::get_admin()
   {

      return System.get_admin();

   }



   void tool::run_vcredist(string strPlatform)
   {

      SHELLEXECUTEINFOW sei = {};

      string str = ::path::vcredist(strPlatform);

      if (!::file_exists_dup(str))
      {

         return;

      }

      wstring wstr(str);

      sei.cbSize = sizeof(SHELLEXECUTEINFOW);
      sei.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
      // sei.lpVerb = L"RunAs";
      sei.lpFile = wstr.c_str();
      sei.lpParameters = L"/install /passive /norestart /q";
      ::ShellExecuteExW(&sei);
      DWORD dwGetLastError = GetLastError();

      DWORD dwExitCode = 0;

      for (int i = 0; i < (19840 + 19770); i++)
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

            Sleep(5000);

            break;

         }

         Sleep(200);

      }

      ::CloseHandle(sei.hProcess);

   }






} // namespace app_app




