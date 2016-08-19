#include "a_spalib.h"

//extern HINTERNET  g_hSession;
//extern HINTERNET  g_hConnect;
//HINTERNET  g_hPreviousRequest;
DWORD g_MsDownloadSize = 1024 * 128;
class download_buffer
{
public:

   char * m_p;

   download_buffer()
   {
      m_p = new char[g_MsDownloadSize];
   }
   ~download_buffer()
   {
      delete [] m_p;
   }

   operator char *()
   {
      return m_p;
   }
};


bool a_spa::ms_download(const char * pszUrl, const char * pszFile, bool bUrlEncode, int * piStatus)
{
   return ms_download_progress(pszUrl, pszFile, false, bUrlEncode, piStatus);
}


bool a_spa::ms_download_progress(const char * pszUrl,const char * pszFile,bool bProgress,bool bUrlEncode,int * piStatus)
{
   download_buffer buffer;
   if(piStatus != NULL)
      *piStatus = 0;
   string strUrl;

   if(file_exists_dup(pszFile) && !::DeleteFile(pszFile))
   {
      //trace("download failed: could not delete file prior to download.");
      return false;
   }

//   g_dwDownloadLen = 0;

   if(bProgress)
   {
      trace("...");
   }

   int iCol = 3;


   strUrl = pszUrl;
   if(bUrlEncode)
   {
      strUrl = url_encode(strUrl.c_str());
      strUrl = str_replace(strUrl.c_str(), "%5C", "\\");
      strUrl = str_replace(strUrl.c_str(), "\\", "/");
      strUrl = str_replace(strUrl.c_str(), "%3A", ":");
      strUrl = str_replace(strUrl.c_str(), "%2F", "/");
   }
   string strHost;
   string strReq;
   bool bSsl;
   if(strUrl.substr(0, 7) == "http://")
   {
      bSsl = false;
      size_t iPos = strUrl.find("/", 8);
      strHost = strUrl.substr(7, iPos - 7);
      strReq = strUrl.substr(iPos);
   }
   else if (strUrl.substr(0, 8) == "https://")
   {
      bSsl = true;
      size_t iPos = strUrl.find("/", 9);
      strHost = strUrl.substr(8, iPos - 8);
      strReq = strUrl.substr(iPos);
   }
   else
   {
      output_debug_string("ms_download_progress Neither http or https!!!\n\n");
      return false;
   }

   DWORD dwSize = 0;
   DWORD dwDownloaded = 0;
   LPSTR pszOutBuffer;
   BOOL  bResults = FALSE;

   string pwzHost = u16(strHost.c_str());

   DWORD dwLast100k = 0;

   HINTERNET hSession = NULL;

   int iRetry = 0;
Retry1:
   if(hSession == NULL)
   {
      hSession = InternetOpen(
         "ccvotagus_ca2_fontopus",  
         INTERNET_OPEN_TYPE_PRECONFIG,
         NULL, 
         NULL, 
         INTERNET_FLAG_EXISTING_CONNECT 
         | INTERNET_FLAG_KEEP_CONNECTION);
      HTTP_VERSION_INFO vi;
      vi.dwMajorVersion = 1;
      vi.dwMinorVersion = 1;
      InternetSetOption(
         hSession,
         INTERNET_OPTION_HTTP_VERSION,
         &vi,
         sizeof(vi));
   }

   HINTERNET hConnect = NULL;

   if(hSession != NULL)
   {
//      if(strHost != g_strHost || g_hConnect == NULL)
      {
         //if(hConnect != NULL)
         //{
         //   ::InternetCloseHandle(g_hConnect);
         //}
         try
         {
            hConnect = InternetConnect( hSession, strHost.c_str(),
               bSsl ? 443 : 80, NULL, NULL, INTERNET_SERVICE_HTTP, 
               INTERNET_FLAG_EXISTING_CONNECT 
               | INTERNET_FLAG_KEEP_CONNECTION, 
               1);
         }
         catch(...)
         {
            iRetry++;
            if(iRetry > 5)
            {
               return false;
            }
            //g_hSession = NULL;
            goto Retry1;
         }
         //g_strHost = strHost;
      }
   }

   DWORD dwFlags = 0;

   dwFlags |= INTERNET_FLAG_EXISTING_CONNECT;
   dwFlags |= INTERNET_FLAG_KEEP_CONNECTION;
   if (bSsl)
   {
      dwFlags |= INTERNET_FLAG_SECURE;
      dwFlags |= INTERNET_FLAG_IGNORE_CERT_CN_INVALID;
      dwFlags |= INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
   }

   HINTERNET hRequest = NULL;
   if (hConnect != NULL)
      hRequest = HttpOpenRequest( hConnect, "GET", strReq.c_str(),
      NULL, "ca2 ccvotagus spa", 
      NULL, 
      dwFlags,
      1);

   if (hRequest)
      bResults = HttpSendRequest(
         hRequest,
         NULL, 0,
         NULL, 0);

//   if(hRequest != g_hPreviousRequest)
  // {
    //  InternetCloseHandle(g_hPreviousRequest);
   //}


   DWORD dwStatusCode = 0;
   DWORD dwBufferLen = sizeof(dwStatusCode);
   DWORD dwIndex = 0;
   if(bResults)
      bResults = HttpQueryInfo(
      hRequest, 
      HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
      &dwStatusCode,
      &dwBufferLen,
      &dwIndex);
   if(piStatus != NULL)
      *piStatus = dwStatusCode;
   dwBufferLen = 1024;
   char * pszStatus = (char *) malloc(dwBufferLen);
   if(pszStatus != NULL)
   {
      memset(pszStatus, 0, dwBufferLen);
      dwIndex = 0;
      HttpQueryInfo(
         hRequest, 
         HTTP_QUERY_STATUS_TEXT,
         pszStatus,
         &dwBufferLen,
         &dwIndex);
   }

   if(bResults && dwStatusCode != HTTP_STATUS_OK)
   {
      if(piStatus == NULL)
      {
         /*dwBufferLen = 1024 + 256;
         char * pszMessage = (char *) malloc(dwBufferLen);
         if(pszMessage != NULL)
         {
            //sprintf_s(pszMessage, dwBufferLen, "download error : status %d - %s", dwStatusCode, pszStatus);
            //trace(pszMessage);
            free(pszMessage);
         }
         else
         {
            char sz[1024];
            sprintf_s(sz, sizeof(sz), "download error : status %d", dwStatusCode);
            trace(sz);
         }*/
      }
      bResults = FALSE;
   }
   if(pszStatus != NULL)
   {
      free(pszStatus);
   }

   if (bResults)
   {
      
      DWORD dwWritten;
      DWORD dwError;
      string strPath;
      strPath = pszFile;
      DWORD dwLen = 0;
      dir::mk(dir::name(strPath.c_str()).c_str());
      HANDLE hfile = ::CreateFile(strPath.c_str(),
           GENERIC_WRITE, 0, NULL,
           CREATE_ALWAYS, 0, NULL);
      if(hfile == INVALID_HANDLE_VALUE)
      {
         dwError = ::GetLastError();
         return false;
      }
      DWORD dwSize = g_MsDownloadSize;
      
      pszOutBuffer = buffer;
      if (!pszOutBuffer)
      {
         printf("Out of memory\n");
         return false;
      }
      while(true)
      {
         ZeroMemory(pszOutBuffer, dwSize);

         if (!InternetReadFile( hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
         {
         }
         else
         {
            ::WriteFile(hfile, pszOutBuffer, dwDownloaded, &dwWritten, NULL);
            dwLen += dwWritten;
         }
         if(dwDownloaded == 0)
            break;
      }
      ::CloseHandle(hfile);
   }
   DWORD dw = GetLastError();
   return bResults != FALSE;
}



//void dlr(DWORD dwDownload)
//{
//   if(!g_bShowPercentage)
//      return;
//   DWORD dw = ::GetTickCount();
//   DWORD dwDeltaTime = dw - g_dwDownloadTick;
//   if(dwDeltaTime < 184)
//      return;
//   g_dwDownloadTick = dw;
//   DWORD dwLen = dwDownload - g_dwDownload;
//   g_dwDownload = dwDownload;
//   if(g_daDownloadRate.size() < 100)
//   {
//      g_daDownloadRate.push_back(((double)dwLen / 1024.0) / ((double)(dwDeltaTime) / 1000.0));
//   }
//   else
//   {
//      if(g_iDownloadRate >= 100 || g_iDownloadRate < 0)
//         g_iDownloadRate = 0;
//      g_daDownloadRate[g_iDownloadRate] = ((double)dwLen / 1024.0) / ((double)(dwDeltaTime) / 1000.0);
//      g_iDownloadRate++;
//   }
//   if(g_daDownloadRate.average() == 0.0)
//   {
//      if(g_dDownloadRate != 0.0)
//      {
//         g_dDownloadRate = 0.0;
//         g_dwDownloadZeroRateTick = ::GetTickCount();
//         g_dwDownloadZeroRateRemain = g_dwDownloadRemain;
//      }
//      g_dwDownloadRemain = g_dwDownloadZeroRateRemain + ::GetTickCount() - g_dwDownloadZeroRateTick;
//   }
//   else
//   {
//      g_dDownloadRate = g_daDownloadRate.average();
//      g_dwDownloadRemain = (DWORD)(((g_iTotalGzLen - g_iGzLen) / 1024.0) / g_daDownloadRate.average());
//   }
//}
