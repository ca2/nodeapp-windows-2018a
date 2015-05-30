#pragma once

extern DWORD g_MsDownloadSize;
//extern char * g_MsDownloadBuffer;


bool ms_download(const char * pszUrl, const char * pszFile, bool bUrlEncode = true, int * piStatus = NULL);
bool ms_download_progress(const char * pszUrl, const char * pszFile, bool bProgress, bool bUrlEncode = true, int * piStatus = NULL);
void dlr(DWORD dwDownload);
