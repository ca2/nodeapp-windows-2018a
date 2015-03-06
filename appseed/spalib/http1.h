#pragma once



std::string url_encode(const char * psz);

#include <WinINet.h>

SPALIB_API std::string ms_get(const char * pszUrl, bool bCache = false);
std::string ms_post(const char * pszUrl, const char * pszPost);
