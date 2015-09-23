#include "a_spalib.h"

string url_encode(const char * psz)
{
      string str;
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



SPALIB_API string a_spa::ms_get(const char * pszUrl, bool bCache)
{
   string strRet;
   string strUrl(pszUrl);
   string strHost;
   string strReq;
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

   string pwzHost = u16(strHost.c_str());

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
                printf("Out of memory\n");
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
            // Free the memory allocated to the buffer.
      }
      while (dwDownloaded>0);
   }
   delete [] pszOutBuffer;

   if (hRequest) InternetCloseHandle(hRequest);
   if (hConnect) InternetCloseHandle(hConnect);
   if (hSession) InternetCloseHandle(hSession);
   return strRet;
}

