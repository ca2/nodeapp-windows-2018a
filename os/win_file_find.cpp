#include "framework.h"

namespace win
{

   file_find::file_find()
   {
      m_pFoundInfo = NULL;
      m_pNextInfo = NULL;
      m_hContext = NULL;
      m_chDirSeparator = '\\';
   }

   file_find::~file_find()
   {
      close();
   }

   void file_find::close()
   {
      if (m_pFoundInfo != NULL)
      {
         delete m_pFoundInfo;
         m_pFoundInfo = NULL;
      }

      if (m_pNextInfo != NULL)
      {
         delete m_pNextInfo;
         m_pNextInfo = NULL;
      }

      if (m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
      {
         CloseContext();
         m_hContext = NULL;
      }
   }

   void file_find::CloseContext()
   {
      ::FindClose(m_hContext);
      return;
   }

   bool file_find::FindFile(const char * pstrName /* = NULL */, DWORD dwUnused /* = 0 */)
   {
      UNUSED_ALWAYS(dwUnused);
      close();
      m_pNextInfo = new WIN32_FIND_DATAW;
      m_bGotLast = FALSE;

      if (pstrName == NULL)
         pstrName = "*.*";
      ca::international::MultiByteToUnicode(CP_UTF8, ((WIN32_FIND_DATAW*) m_pNextInfo)->cFileName, MAX_PATH, pstrName);

      wstring wstrName = ca::international::utf8_to_unicode(pstrName);

      m_hContext = ::win::shell::FindFirstFile(wstrName, (WIN32_FIND_DATAW*) m_pNextInfo);

      if (m_hContext == INVALID_HANDLE_VALUE)
      {
         DWORD dwTemp = ::GetLastError();
         close();
         ::SetLastError(dwTemp);
         return FALSE;
      }
      wstring wstrRoot;

      wchar_t * pstrRoot = wstrRoot.alloc(_MAX_PATH);
      const wchar_t * pstr = ::win::shell::_fullpath(pstrRoot, wstrName, _MAX_PATH);

      // passed name isn't a valid path but was found by the API
      ASSERT(pstr != NULL);
      if (pstr == NULL)
      {
         m_strRoot.Empty();
         close();
         ::SetLastError(ERROR_INVALID_NAME);
         return FALSE;
      }
      else
      {
         // find the last forward or backward whack
         wchar_t * pstrBack  = wcsrchr(pstrRoot, '\\');
         wchar_t * pstrFront = wcsrchr(pstrRoot, '/');

         if (pstrFront != NULL || pstrBack != NULL)
         {
            if (pstrFront == NULL)
               pstrFront = pstrRoot;
            if (pstrBack == NULL)
               pstrBack = pstrRoot;

            // from the start to the last whack is the root

            if (pstrFront >= pstrBack)
               *pstrFront = '\0';
            else
               *pstrBack = '\0';
         }
         wstrRoot.release_buffer();
      }
      m_strRoot = ca::international::unicode_to_utf8(wstrRoot);
      return TRUE;
   }

   bool file_find::MatchesMask(DWORD dwMask) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
         return (!!(((LPWIN32_FIND_DATAW) m_pFoundInfo)->dwFileAttributes & dwMask));
      else
         return FALSE;
   }

   bool file_find::GetLastAccessTime(FILETIME* pTimeStamp) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT(pTimeStamp != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = ((LPWIN32_FIND_DATAW) m_pFoundInfo)->ftLastAccessTime;
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetLastWriteTime(FILETIME* pTimeStamp) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT(pTimeStamp != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = ((LPWIN32_FIND_DATAW) m_pFoundInfo)->ftLastWriteTime;
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetCreationTime(FILETIME* pTimeStamp) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = ((LPWIN32_FIND_DATAW) m_pFoundInfo)->ftCreationTime;
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetLastAccessTime(::datetime::time& refTime) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
      {
         refTime = ::datetime::time(((LPWIN32_FIND_DATAW) m_pFoundInfo)->ftLastAccessTime);
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetLastWriteTime(::datetime::time& refTime) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
      {
         refTime = ::datetime::time(((LPWIN32_FIND_DATAW) m_pFoundInfo)->ftLastWriteTime);
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetCreationTime(::datetime::time& refTime) const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
      {
         refTime = ::datetime::time(((LPWIN32_FIND_DATAW) m_pFoundInfo)->ftCreationTime);
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::IsDots() const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      // return TRUE if the file name is "." or ".." and
      // the file is a directory

      bool bResult = FALSE;
      if (m_pFoundInfo != NULL && IsDirectory())
      {
         LPWIN32_FIND_DATAW pFindData = (LPWIN32_FIND_DATAW) m_pFoundInfo;
         if (pFindData->cFileName[0] == '.')
         {
            if (pFindData->cFileName[1] == '\0' ||
               (pFindData->cFileName[1] == '.' &&
               pFindData->cFileName[2] == '\0'))
            {
               bResult = TRUE;
            }
         }
      }

      return bResult;
   }

   bool file_find::FindNextFile()
   {
      ASSERT(m_hContext != NULL);

      if (m_hContext == NULL)
         return FALSE;
      if (m_pFoundInfo == NULL)
         m_pFoundInfo = new WIN32_FIND_DATAW;

      ASSERT_VALID(this);

      WIN32_FIND_DATAW * pTemp = m_pFoundInfo;
      m_pFoundInfo = m_pNextInfo;
      m_pNextInfo = pTemp;

      return ::win::shell::FindNextFile(m_hContext, (LPWIN32_FIND_DATAW) m_pNextInfo);
   }

   string file_find::GetFileURL() const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      string strResult(L"file://");
      strResult += GetFilePath();
      return strResult;
   }

   string file_find::GetRoot() const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);
      return m_strRoot;
   }

   string file_find::GetFilePath() const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      string strResult = GetRoot();
      if (strResult[strResult.get_length()-1] != '\\' &&
         strResult[strResult.get_length()-1] != '/')
         strResult += m_chDirSeparator;
      strResult += GetFileName();
      return strResult;
   }

   string file_find::GetFileTitle() const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      string strFullName = GetFileName();
      string strResult;

      _splitpath(strFullName, NULL, NULL, strResult.GetBuffer(_MAX_PATH), NULL);
      strResult.ReleaseBuffer();
      return strResult;
   }

   string file_find::GetFileName() const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      string ret;

      if (m_pFoundInfo != NULL)
      {
         ca::international::unicode_to_utf8(ret, ((LPWIN32_FIND_DATAW) m_pFoundInfo)->cFileName);
      }
      return ret;
   }

   int64_t file_find::get_length() const
   {
      ASSERT(m_hContext != NULL);
      ASSERT_VALID(this);

      if (m_pFoundInfo != NULL)
         return ((LPWIN32_FIND_DATAW) m_pFoundInfo)->nFileSizeLow +
         ((int64_t)(((LPWIN32_FIND_DATAW) m_pFoundInfo)->nFileSizeHigh) << 32);
      else
         return 0;
   }


   void file_find::dump(dump_context & dumpcontext) const
   {
      ::ca::object::dump(dumpcontext);
      dumpcontext << "\nm_hContext = " << (UINT) m_hContext;
   }

   void file_find::assert_valid() const
   {
      // if you trip the ASSERT in the else side, you've called
      // a get() function without having done at least one
      // FindNext() call

      if (m_hContext == NULL)
         ASSERT(m_pFoundInfo == NULL && m_pNextInfo == NULL);
      else
         ASSERT(m_pFoundInfo != NULL && m_pNextInfo != NULL);

   }


} // namespace win




