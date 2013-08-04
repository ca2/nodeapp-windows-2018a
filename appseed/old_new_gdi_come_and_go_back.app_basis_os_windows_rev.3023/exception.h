#pragma once


namespace _template
{


   #ifndef _ATL_NO_EXCEPTIONS


   // Throw a atl_exception corresponding to the result of ::GetLastError
   ATL_NOINLINE DECLSPEC_NO_RETURN inline void WINAPI AtlThrowLastWin32()
   {
      DWORD dwError = ::GetLastError();
      AtlThrow( HRESULT_FROM_WIN32( dwError ) );
   }

   #else  // no exception handling

   // Throw a atl_exception corresponding to the result of ::GetLastError
   ATL_NOINLINE inline void WINAPI AtlThrowLastWin32()
   {
      DWORD dwError = ::GetLastError();
      AtlThrow( HRESULT_FROM_WIN32( dwError ) );
   }

   #endif  // no exception handling

}  // namespace _template
