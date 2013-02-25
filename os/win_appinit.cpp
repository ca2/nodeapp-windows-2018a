// This is ca API library.
// 
// 
//
// 
// 
// 
// 
// 

#include "framework.h"
#include "sal.h"



/////////////////////////////////////////////////////////////////////////////

/*bool CLASS_DECL_win ::ca::WinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int32_t nCmdShow)
{
}*/


/////////////////////////////////////////////////////////////////////////////
// ::ca::filesp implementation helpers

//#ifdef ::ca::GetFileName
//#undef ::ca::GetFileName
//#endif


/*UINT CLASS_DECL_win ::ca::GetFileName(const char * lpszPathName, __out_ecount_opt(nMax) LPTSTR lpszTitle, UINT nMax)
{
   ASSERT(lpszTitle == NULL ||
      __is_valid_address(lpszTitle, nMax));
   ASSERT(__is_valid_string(lpszPathName));

   ENSURE_ARG(lpszPathName != NULL);

   // always capture the complete file name including extension (if present)
   LPTSTR lpszTemp = ::PathFindFileName(lpszPathName);

   // lpszTitle can be NULL which just returns the number of bytes
   if (lpszTitle == NULL)
      return lstrlen(lpszTemp)+1;

   // otherwise copy it into the buffer provided
   ::ca::strncpy_s(lpszTitle, nMax, lpszTemp, _TRUNCATE);
   return 0;
}


*/