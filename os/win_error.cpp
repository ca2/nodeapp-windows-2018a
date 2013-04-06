#include "framework.h"

string get_error_message(DWORD dwError)
{
   LPWSTR lpBuffer;
   FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM,
      ::null(),
      dwError,
      0,
      (LPWSTR) &lpBuffer,
      1,
      ::null());
   string str = ::ca::international::unicode_to_utf8(lpBuffer);
   LocalFree(lpBuffer);
   return str;
}
