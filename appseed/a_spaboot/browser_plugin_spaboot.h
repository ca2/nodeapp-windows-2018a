#pragma once

#include "../spalib/spalib.h"
#include "ca2/compress/bsdiff/bzlib.h"

bool file_exists_dup(const char * path1);
int spaboot_start(const char * pszVersion, const char * pszId);
int APIENTRY ca2_cube_install(const char * pszId);
bool read_resource_as_file_dup(const char * pszFile, HINSTANCE hinst, UINT nID, LPCTSTR lpcszType);
bool defer_check_ms();