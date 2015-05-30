#include "StdAfx.h"

//void trace(const char * psz);

//HINTERNET  g_hSession = NULL;
//HINTERNET  g_hConnect = NULL;
//DWORD g_dwDownloadLen;


std::string url_encode(const char * psz)
{
      std::string str;
      char sz[256];
      while(*psz != '\0')
      {
         unsigned char uch = *psz;
         if(isdigit(uch)
         || isalpha(uch)
         || uch == '.'
         || uch == '-'
         || uch== '_')
         {
            str += uch;
         }
         else if(uch == ' ')
         {
            str += "+";
         }
         else
         {
            sprintf(sz, "%%%02X", uch);
            str += sz;
         }
         psz++;
      }
      return str;
   }



SPALIB_API std::string ms_get(const char * pszUrl, bool bCache)
{
   std::string strRet;
   std::string strUrl(pszUrl);
   std::string strHost;
   std::string strReq;
   if(strUrl.substr(0, 7) == "http://")
   {
      size_t iPos = strUrl.find("/", 8);
      strHost = strUrl.substr(7, iPos - 7);
      strReq = strUrl.substr(iPos);
   }
   DWORD dwSize = 0;
   DWORD dwDownloaded = 0;
   LPSTR pszOutBuffer;
   BOOL  bResults = FALSE;
   HINTERNET  hSession = NULL, 
              hConnect = NULL,
              hRequest = NULL;

   wstring pwzHost = u16(strHost.c_str());

   /*WCHAR * pwzAutoUrl = NULL;
   if(WinHttpDetectAutoProxyConfigUrl(
      WINHTTP_AUTO_DETECT_TYPE_DHCP|
      WINHTTP_AUTO_DETECT_TYPE_DNS_A, pwzAutoUrl))
   {
      WinHttpGetProxyForUrl
      GlobalFree((HGLOBAL) pwzAutoUrl);
   }*/



    // Use WinHttpOpen to obtain a session handle.
    hSession = 
       InternetOpen(
         "ccvotagus_ca2_fontopus",  
         INTERNET_OPEN_TYPE_PRECONFIG,
         NULL, 
         NULL,
         0);

   if(hSession)
      hConnect = 
         InternetConnect( 
            hSession,
            strHost.c_str(),
            80, 
            NULL,
            NULL,
            INTERNET_SERVICE_HTTP,
            0,
            1);

   DWORD dwFlags = 0;

   if(!bCache)
   {
      dwFlags |= INTERNET_FLAG_NO_CACHE_WRITE;
      dwFlags |= INTERNET_FLAG_PRAGMA_NOCACHE;
      dwFlags |= INTERNET_FLAG_RELOAD;
   }
    
   if(hConnect)
      hRequest = 
         HttpOpenRequest(
            hConnect, 
            "GET", 
            strReq.c_str(),
            NULL, 
            "ca2 ccvotagus spa", 
            NULL, // accept types
            dwFlags,
            1); // dwContext

   if(hRequest)
      bResults = 
         HttpSendRequest(
            hRequest,
            NULL, 0,
            NULL, 0);

   if(!bResults)
   {
      DWORD dw = ::GetLastError();
	   char lastErrorTxt[1024];
      memset(lastErrorTxt, 0, sizeof(lastErrorTxt));
	   FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,dw,0,lastErrorTxt,1024,NULL);
      trace("ms_get failed");
      trace(lastErrorTxt);
      sprintf_s(lastErrorTxt, sizeof(lastErrorTxt), "ms_get failed : GetLastErrorCode : %d ", dw);
      trace(lastErrorTxt);
   }

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
      dwBufferLen = 1024 + 256;
      char * pszMessage = (char *) malloc(dwBufferLen);
      if(pszMessage != NULL)
      {
         sprintf_s(pszMessage, dwBufferLen, "ms_get failed : status : %d - %s", dwStatusCode, pszStatus);
         trace(pszMessage);
         free(pszMessage);
      }
      else
      {
         char sz[1024];
         sprintf_s(sz, sizeof(sz), "ms_get failed : status : %d", dwStatusCode);
         trace(sz);
      }
      bResults = FALSE;
   }
   if(pszStatus != NULL)
   {
      free(pszStatus);
   }

   DWORD dwLen = 0;
   DWORD dwLast100k = 0;
   dwSize = 1024 * 64;
   // Allocate space for the buffer.
   pszOutBuffer = new char[dwSize+1];
    if (bResults)
    {
        do 
        {
            if (!pszOutBuffer)
            {
                printf("Out of primitive::memory\n");
                dwSize=0;
            }
            else
            {
                // Read the Data.
                ZeroMemory(pszOutBuffer, dwSize+1);

                if (!InternetReadFile( hRequest, (LPVOID)pszOutBuffer, 
                                      dwSize, &dwDownloaded))
                    printf( "Error %u in WinHttpReadData.\n", 
                            GetLastError());
                else
                {
                   dwLen += dwSize;
//                   g_dwDownloadLen += dwSize;
            for(int i = 0; i < ((dwLen - dwLast100k) / (100 * 1024)); i++)
            {
//               trace_add(".");
            }
            dwLast100k += ((dwLen - dwLast100k) / (100 * 1024) )*(100 / 1024);
                   strRet += pszOutBuffer;
                 //   printf("%s", pszOutBuffer);
                }
            }
            // Free the primitive::memory allocated to the buffer.
      }
      while (dwDownloaded>0);
   }
   delete [] pszOutBuffer;

   if (hRequest) InternetCloseHandle(hRequest);
   if (hConnect) InternetCloseHandle(hConnect);
   if (hSession) InternetCloseHandle(hSession);
   return strRet;
}

/*
std::string ms_post(const char * pszUrl, const char * pszPost)
{
   std::string strUrl(pszUrl);
   std::string strHost;
   std::string strReq;
   int iPort;
   if(strUrl.substr(0, 7) == "http://")
   {
      int iPos = strUrl.find("/", 8);
      strHost = strUrl.substr(7, iPos - 7);
      strReq = strUrl.substr(iPos);
      iPort = 80;
   }
   else if(strUrl.substr(0, 8) == "https://")
   {
      int iPos = strUrl.find("/", 9);
      strHost = strUrl.substr(8, iPos - 8);
      strReq = strUrl.substr(iPos);
      iPort = 443;
   }
   DWORD dwSize = 0;
   DWORD dwDownloaded = 0;
   LPSTR pszOutBuffer;
   BOOL  bResults = FALSE;
   HINTERNET  hSession = NULL, 
              hConnect = NULL,
              hRequest = NULL;

   WCHAR * pwzHost = u16(strHost.c_str());

   /*WCHAR * pwzAutoUrl = NULL;
   if(WinHttpDetectAutoProxyConfigUrl(
      WINHTTP_AUTO_DETECT_TYPE_DHCP|
      WINHTTP_AUTO_DETECT_TYPE_DNS_A, pwzAutoUrl))
   {
      WinHttpGetProxyForUrl
      GlobalFree((HGLOBAL) pwzAutoUrl);
   }*/



    // Use WinHttpOpen to obtain a session handle.
  /*  hSession = WinHttpOpen( L"ccvotagus_ca2_fontopus",  
                            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                            WINHTTP_NO_PROXY_NAME, 
                            WINHTTP_NO_PROXY_BYPASS, 0);

    WCHAR * pwzReq = u16(strReq.c_str());
    WCHAR * pwzUrl = u16(strUrl.c_str());
    config_session_proxy(hSession, pwzUrl);





    // Specify an HTTP server.
    if (hSession)
        hConnect = WinHttpConnect( hSession, pwzHost,
                                   iPort, 0);

    
    // Create an HTTP request handle.
    if (hConnect)
        hRequest = WinHttpOpenRequest( hConnect, L"POST", 
                                       pwzReq,
                                       NULL, WINHTTP_NO_REFERER, 
                                       WINHTTP_DEFAULT_ACCEPT_TYPES, 
                                       iPort == 443 ? WINHTTP_FLAG_SECURE : 0);
//                                       WINHTTP_FLAG_SECURE);

// Add a request header.
    WCHAR * hd1 = u16(("Host: " + strHost).c_str());
   if( hRequest )
      bResults = WinHttpAddRequestHeaders( hRequest, 
                 hd1,
                                         -1,
                                         WINHTTP_ADDREQ_FLAG_ADD );
   if( hRequest )
      bResults = WinHttpAddRequestHeaders( hRequest, 
                 L"Content-Type: application/x-www-form-urlencoded",
                                         -1,
                                         WINHTTP_ADDREQ_FLAG_ADD );

    // Send a request.
    if (hRequest)
        bResults = WinHttpSendRequest( hRequest,
                                       WINHTTP_NO_ADDITIONAL_HEADERS,
                                       0, WINHTTP_NO_REQUEST_DATA, 0, 
                                       strlen(pszPost), 0);

    DWORD dwUploaded;
    if (bResults)
      if (! (bResults = WinHttpWriteData( hRequest, (LPVOID)pszPost, 
                          strlen(pszPost), &dwUploaded)))
        printf( "Error %u in WinHttpReadData.\n", 
                GetLastError());
 
    // End the request.
    if (bResults)
        bResults = WinHttpReceiveResponse( hRequest, NULL);

    std::string strResult;

    // Keep checking for data until there is nothing left.
    if (bResults)
    {
        do 
        {

            // Check for available data.
            dwSize = 0;
            if (!WinHttpQueryDataAvailable( hRequest, &dwSize))
                printf( "Error %u in WinHttpQueryDataAvailable.\n",
                        GetLastError());

            // Allocate space for the buffer.
            pszOutBuffer = new char[dwSize+1];
            if (!pszOutBuffer)
            {
                printf("Out of primitive::memory\n");
                dwSize=0;
            }
            else
            {
                // Read the Data.
                ZeroMemory(pszOutBuffer, dwSize+1);

                if (!WinHttpReadData( hRequest, (LPVOID)pszOutBuffer, 
                                      dwSize, &dwDownloaded))
                    printf( "Error %u in WinHttpReadData.\n", 
                            GetLastError());
                else
                {
                   g_dwDownloadLen += dwSize;
                   strResult += pszOutBuffer;
                }
            
                // Free the primitive::memory allocated to the buffer.
                delete [] pszOutBuffer;
            }

        } while (dwSize>0);
    }

   DWORD dw = GetLastError();
    // Report any errors.  
    if (!bResults)
        printf("Error %d has occurred.\n",dw);

    delete [] hd1;
    delete [] pwzUrl;
    delete [] pwzReq;
    delete [] pwzHost;

    // Close any open handles.
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    return strResult;
}*/

/*
void config_session_proxy(HINTERNET hSession, WCHAR * pwzUrl)
{
   WINHTTP_AUTOPROXY_OPTIONS apop;

   char szPac[4096];
   memset(szPac, 0, sizeof(szPac));
   DWORD lcbPac;
   HKEY hkey;
   memset(&apop, 0, sizeof(apop));
   bool bGot = false;
      lcbPac = sizeof(szPac);
   if(RegOpenKey(HKEY_CURRENT_USER,
        "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Connections",
       &hkey) == ERROR_SUCCESS)
   {
      LONG l;
      DWORD dw;
      if((l = RegQueryValueEx(hkey, "DefaultConnectionSettings", NULL, NULL, (LPBYTE) &szPac, &lcbPac)) == ERROR_SUCCESS
         && (szPac[8] & 8))
      {
         apop.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP | WINHTTP_AUTO_DETECT_TYPE_DNS_A;
         apop.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT;
         apop.fAutoLogonIfChallenged = TRUE;
         apop.lpszAutoConfigUrl = NULL;
         bGot = true;
      }
      else
      {         
         FormatMessage(0, 0, l, 0, szPac, sizeof(szPac), NULL);
      }
   }
lcbPac = sizeof(szPac);
   if(!bGot)
   {
      if(RegOpenKey(HKEY_CURRENT_USER,
          "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",
          &hkey) == ERROR_SUCCESS)
      {
         LONG l;
         if((l = RegQueryValueEx(hkey, "AutoConfigURL", NULL, NULL, (LPBYTE) szPac, &lcbPac)) == ERROR_SUCCESS)
         {
            
            apop.dwAutoDetectFlags = 0;
            apop.dwFlags = WINHTTP_AUTOPROXY_CONFIG_URL;
            apop.lpszAutoConfigUrl = u16(szPac);
            apop.fAutoLogonIfChallenged = TRUE;
            bGot = true;
          }
          else
          {         
             FormatMessage(0, 0, l, 0, szPac, sizeof(szPac), NULL);
          }
       }
   }
   if(!bGot)
   {
      return;
   }

   WINHTTP_PROXY_INFO pinfo;


   if(WinHttpGetProxyForUrl(
      hSession,
      pwzUrl,
      &apop,
      &pinfo))
   {
      WinHttpSetOption(
         hSession,
         WINHTTP_OPTION_PROXY,
         &pinfo,
         sizeof(pinfo));

   }


}*/
