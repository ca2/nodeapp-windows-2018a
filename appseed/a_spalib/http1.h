#pragma once



string url_encode(const char * psz);

#include <WinINet.h>

SPALIB_API string ms_get(const char * pszUrl, bool bCache = false);

