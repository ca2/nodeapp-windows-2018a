#include "framework.h"


namespace app_app
{


   install_item::install_item(bootstrap * pbootstrap, string strFile, string strPlatform, install * pinstall) :
      object(pbootstrap->get_app()),
      m_pbootstrap(pbootstrap),
      m_strFile(strFile),
      m_strPlatform(strPlatform),
      m_pinstall(pinstall)
   {

      m_eitemtype = item_type_spa;

      __begin_thread(get_app(), &install_item::proc, this, 50, 0, 0, NULL, &m_dwThreadId);

   }


   install_item::install_item(bootstrap * pbootstrap, string strUrlPrefix, string strPath, string strFile, install * pinstall, string strMd5, string strPlatform) :
      object(pbootstrap->get_app()),
      m_pbootstrap(pbootstrap),
      m_strUrlPrefix(strUrlPrefix),
      m_strPath(strPath),
      m_strFile(strFile),
      m_pinstall(pinstall),
      m_strMd5(strMd5),
      m_strPlatform(strPlatform)
   {

      m_eitemtype = item_type_set;

      __begin_thread(get_app(), &install_item::proc, this, 50, 0, 0, NULL, &m_dwThreadId);

   }


   UINT c_cdecl install_item::proc(LPVOID lp)
   {

      install_item * pitem = (install_item *)lp;

      try
      {

         pitem->run();

      }
      catch (...)
      {

         InterlockedDecrement(&pitem->m_pinstall->m_lProcessing);

      }

      delete pitem;

      return 0;

   }


   void install_item::progress(double dRate)
   {

      if (System.get_admin())
      {

         if (m_eitemtype == item_type_spa)
         {

            System.trace(0.05 + ((((double)m_pinstall->m_lTotal - (double)(m_pinstall->m_lProcessing)) * (0.25 - 0.05)) / ((double)m_pinstall->m_lTotal)));

         }
         else if (m_eitemtype == item_type_set)
         {

            System.trace(0.3 + ((((double)m_pinstall->m_lTotal - (double)(m_pinstall->m_lProcessing)) * (0.84 - 0.3)) / ((double)m_pinstall->m_lTotal)));

         }

      }

   }
   void install_item::op_spa()
   {

      string strPlatform = m_strPlatform;

      if (m_strFile == "spaadmin")
      {

         if (m_pbootstrap->check_app_app_admin_bin(strPlatform))
         {

            OutputDebugString("op_spa spaadmin Success\r\n");
            InterlockedIncrement(&m_pinstall->m_lOk);

         }
         else
         {

            OutputDebugString("op_spa spaadmin Failed\r\n");
            InterlockedIncrement(&m_pinstall->m_lBad);

         }

      }
      else if (m_strFile == "spa")
      {

         if (m_pbootstrap->check_app_app_bin(strPlatform))
         {

            OutputDebugString("op_spa spa Success\r\n");
            InterlockedIncrement(&m_pinstall->m_lOk);

         }
         else
         {
            OutputDebugString("op_spa spa Failed\r\n");
            InterlockedIncrement(&m_pinstall->m_lBad);

         }

      }
      else if (m_strFile == "vcredist")
      {

         if (m_pbootstrap->check_vcredist(strPlatform))
         {

            OutputDebugString("op_spa vcredist Success\r\n");
            InterlockedIncrement(&m_pinstall->m_lOk);

         }
         else
         {
            OutputDebugString("op_spa vcredist Failed\r\n");
            InterlockedIncrement(&m_pinstall->m_lBad);

         }

      }
      else if (m_strFile == "install_bin_set")
      {

         if (m_pbootstrap->check_install_bin_set(strPlatform))
         {

            OutputDebugString("op_spa install_bin_set Success\r\n");
            InterlockedIncrement(&m_pinstall->m_lOk);

         }
         else
         {
            OutputDebugString("op_spa install_bin_set Failed\r\n");
            InterlockedIncrement(&m_pinstall->m_lBad);

         }

      }



   }


   void install_item::op_set()
   {

      string strUrlPrefix = m_strUrlPrefix;

      string strPath = m_strPath;

      string strFile = m_strFile;

      string strMd5 = m_strMd5;

      string strPlatform = m_strPlatform;

      string strDownload = dir::name(strPath) / strFile;

      if (!file_exists_dup(strDownload) || _stricmp(file_md5_dup(strDownload).c_str(), strMd5) != 0)
      {

         if (strFile.compare_ci("app_app_install.exe") == 0)
         {

            System.os().terminate_processes_by_title(strFile);

         }

         string strUrl;

         int iRetry;

         bool bFileNice;

         iRetry = 0;

         strUrl = strUrlPrefix + strFile + ".bz";

         bFileNice = false;

         while (iRetry < 8 && !bFileNice)
         {

            if (Application.http_download(strUrl, strDownload + ".bz"))
            {

               System.compress().unbz(get_app(), strDownload, strDownload + ".bz");

               if (file_exists_dup(strDownload) && _stricmp(file_md5_dup(strDownload), strMd5) == 0)
               {

                  bFileNice = true;

               }


            }

            iRetry++;

         }

         if (!bFileNice)
         {

            // failed by too much retry in any number of the files already downloaded :
            // so, return failure (no eligible app.install.exe file).
            //return "";

         }

      }

   }

   void install_item::run()
   {

      if (m_eitemtype == item_type_spa)
      {

         op_spa();

      }
      else if (m_eitemtype == item_type_set)
      {

         op_set();

      }


      InterlockedDecrement(&m_pinstall->m_lProcessing);

      progress();

   }





} // namespace app_app



