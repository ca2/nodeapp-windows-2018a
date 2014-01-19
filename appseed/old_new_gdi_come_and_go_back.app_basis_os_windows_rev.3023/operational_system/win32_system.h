#pragma once


#define _BIND_TO_CURRENT_CRT_VERSION 1
#define _CRT_NOFORCE_MANIFEST
#define _STL_NOFORCE_MANIFEST


BEGIN_EXTERN_C


__declspec(selectany)  int _forceCRTManifest;


END_EXTERN_C


#define NOTHROW throw()
#define THROWS throw(...)


#define WINVER 0x0600
#define _WIN32_WINNT   0x0600


#define VC_EXTRALEAN


#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
