#pragma once


namespace ca2
{


   #ifndef _NO_EXCEPTIONS


   // Throw a atl_exception corresponding to the result of ::GetLastError
   NOINLINE DECLSPEC_NO_RETURN inline void WINAPI gen_ThrowLastWin32(sp(::application) papp)
   {
      DWORD dwError = ::GetLastError();
      throw hresult_exception(papp, HRESULT_FROM_WIN32( dwError ) );
   }

   #else  // no exception handling

   // Throw a atl_exception corresponding to the result of ::GetLastError
   NOINLINE inline void WINAPI gen_ThrowLastWin32(sp(::application) papp)
   {
      DWORD dwError = ::GetLastError();
      throw hresult_exception(papp, HRESULT_FROM_WIN32( dwError ) );
   }

   #endif  // no exception handling

}  // namespace ca2
