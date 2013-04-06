#include "framework.h"


__STATIC inline bool IsDirSep(WCHAR ch)
{

   return (ch == '\\' || ch == '/');

}


namespace win
{


   file::file(sp(::ca::application) papp) :
      ca(papp)
   {

      m_hFile = (UINT) hFileNull;

      m_bCloseOnDelete = TRUE;

   }

   file::file(sp(::ca::application) papp, int32_t hFile) :
      ca(papp)
   {

      m_hFile = hFile;

      m_bCloseOnDelete = TRUE;

   }

   file::file(sp(::ca::application) papp, const char * lpszFileName, UINT nOpenFlags) :
      ca(papp)
   {

      ASSERT(__is_valid_string(lpszFileName));

      if(!open(lpszFileName, nOpenFlags))
         throw ::ca::file_exception(papp, ::ca::file_exception::none, -1, lpszFileName);

   }

   file::~file()
   {

      if (m_hFile != (UINT)hFileNull && m_bCloseOnDelete)
         close();

   }

   ::ca::file * file::Duplicate() const
   {
      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      file* pFile = new file(get_app(), hFileNull);
      HANDLE hFile;
      if (!::DuplicateHandle(::GetCurrentProcess(), (HANDLE)m_hFile,
         ::GetCurrentProcess(), &hFile, 0, FALSE, DUPLICATE_SAME_ACCESS))
      {
         delete pFile;
         //xxx      Ex1WinFileException::ThrowOsError(get_app(), (LONG)::GetLastError());
         throw 0;
      }
      pFile->m_hFile = (UINT)hFile;
      ASSERT(pFile->m_hFile != (UINT)hFileNull);
      pFile->m_bCloseOnDelete = m_bCloseOnDelete;
      return pFile;
   }

   bool file::open(const char * lpszFileName, UINT nOpenFlags)
   {

      if(lpszFileName == ::null() || *lpszFileName == '\0') 
      {
         TRACE("win::file::open file with empty name!!");
         return false;
      }

      if (m_hFile != (UINT)hFileNull)
         close();

      ASSERT_VALID(this);
      ASSERT(__is_valid_string(lpszFileName));
      ASSERT((nOpenFlags & type_text) == 0);   // text mode not supported

      // file objects are always binary and CreateFile does not need flag
      nOpenFlags &= ~(UINT)type_binary;


      if(nOpenFlags & ::ca::file::defer_create_directory)
      {
         System.dir_mk(System.dir_name(lpszFileName));
      }

      m_bCloseOnDelete = FALSE;
      m_hFile = (UINT)hFileNull;
      m_strFileName.Empty();

      m_strFileName     = lpszFileName;
      m_wstrFileName    = ::ca::international::utf8_to_unicode(m_strFileName);

      ASSERT(sizeof(HANDLE) == sizeof(uint_ptr));
      ASSERT(shareCompat == 0);

      // map read/write mode
      ASSERT((mode_read|mode_write|mode_read_write) == 3);
      DWORD dwAccess = 0;
      switch (nOpenFlags & 3)
      {
      case mode_read:
         dwAccess = GENERIC_READ;
         break;
      case mode_write:
         dwAccess = GENERIC_WRITE;
         break;
      case mode_read_write:
         dwAccess = GENERIC_READ|GENERIC_WRITE;
         break;
      default:
         dwAccess = GENERIC_READ;
         break;
      }

      // map share mode
      DWORD dwShareMode = 0;
      switch (nOpenFlags & 0x70)    // map compatibility mode to exclusive
      {
      default:
         ASSERT(FALSE);  // invalid share mode?
      case shareCompat:
      case shareExclusive:
         dwShareMode = 0;
         break;
      case shareDenyWrite:
         dwShareMode = FILE_SHARE_READ;
         break;
      case shareDenyRead:
         dwShareMode = FILE_SHARE_WRITE;
         break;
      case shareDenyNone:
         dwShareMode = FILE_SHARE_WRITE|FILE_SHARE_READ;
         break;
      }

      // Note: type_text and type_binary are used in derived classes only.

      // map modeNoInherit flag
      SECURITY_ATTRIBUTES sa;
      sa.nLength = sizeof(sa);
      sa.lpSecurityDescriptor = ::null();
      sa.bInheritHandle = (nOpenFlags & modeNoInherit) == 0;

      // map creation flags
      DWORD dwCreateFlag;
      if (nOpenFlags & mode_create)
      {
         if (nOpenFlags & modeNoTruncate)
            dwCreateFlag = OPEN_ALWAYS;
         else
            dwCreateFlag = CREATE_ALWAYS;
      }
      else
         dwCreateFlag = OPEN_EXISTING;

      // attempt file creation
      //HANDLE hFile = shell::CreateFile(::ca::international::utf8_to_unicode(m_strFileName), dwAccess, dwShareMode, &sa, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, ::null());
      HANDLE hFile = ::CreateFileW(m_wstrFileName, dwAccess, dwShareMode, &sa, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, ::null());
      if (hFile == INVALID_HANDLE_VALUE)
      {
         DWORD dwLastError = ::GetLastError();

         if(dwLastError != ERROR_FILE_NOT_FOUND && dwLastError != ERROR_PATH_NOT_FOUND)
         {
            /*         if (pException != ::null())
            {
            pException->create(allocer());
            ::ca::file_exception * pfe = dynamic_cast < ::ca::file_exception * > (pException->m_p);
            if(pfe != ::null())
            {
            pfe->m_lOsError = dwLastError;
            pfe->m_cause = file_exception::OsErrorToException(pfe->m_lOsError);
            pfe->m_strFileName = lpszFileName;
            }
            return FALSE;
            }
            else
            {*/


            vfxThrowFileException(get_app(), file_exception::OsErrorToException(dwLastError), dwLastError, lpszFileName);

            //}

         }

         try
         {
            ::win::file_system fs(get_app());
            fs.FullPath(m_wstrFileName, m_wstrFileName);
         }
         catch(...)
         {
            return FALSE;
         }

         m_strFileName = ::ca::international::unicode_to_utf8(m_wstrFileName);

         hFile = ::CreateFileW(m_wstrFileName, dwAccess, dwShareMode, &sa, dwCreateFlag, FILE_ATTRIBUTE_NORMAL, ::null());

         if (hFile == INVALID_HANDLE_VALUE)
         {
            /*if (pException != ::null())
            {
            pException->create(allocer());
            ::ca::file_exception * pfe = dynamic_cast < ::ca::file_exception * > (pException->m_p);
            if(pfe != ::null())
            {
            pfe->m_lOsError = ::GetLastError();
            pfe->m_cause = file_exception::OsErrorToException(pfe->m_lOsError);
            pfe->m_strFileName = lpszFileName;
            }
            return FALSE;
            }
            else
            {*/


            DWORD dwLastError = ::GetLastError();
            vfxThrowFileException(get_app(), file_exception::OsErrorToException(dwLastError), dwLastError, lpszFileName);


            //}

         }

      }

      m_hFile = (HFILE)hFile;

      m_bCloseOnDelete = TRUE;

      return TRUE;
   }

   ::primitive::memory_size file::read(void * lpBuf, ::primitive::memory_size nCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      if (nCount == 0)
         return 0;   // avoid Win32 "null-read"

      ASSERT(lpBuf != ::null());
      ASSERT(__is_valid_address(lpBuf, nCount));

      DWORD dwRead;
      if (!::ReadFile((HANDLE)m_hFile, lpBuf, (DWORD) nCount, &dwRead, ::null()))
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());

      return (UINT)dwRead;
   }

   void file::write(const void * lpBuf, ::primitive::memory_size nCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      if (nCount == 0)
         return;     // avoid Win32 "null-write" option

      ASSERT(lpBuf != ::null());
      ASSERT(__is_valid_address(lpBuf, nCount, FALSE));

      DWORD nWritten;
      if (!::WriteFile((HANDLE)m_hFile, lpBuf, (DWORD) nCount, &nWritten, ::null()))
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError(), m_strFileName);

      // Win32s will not return an error all the time (usually DISK_FULL)
      if (nWritten != nCount)
         vfxThrowFileException(get_app(), ::ca::file_exception::diskFull, -1, m_strFileName);
   }

   file_position file::seek(file_offset lOff, ::ca::e_seek nFrom)
   {

      if(m_hFile == (UINT)hFileNull)
         file_exception::ThrowOsError(get_app(), (LONG)0);

      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);
      ASSERT(nFrom == ::ca::seek_begin || nFrom == ::ca::seek_end || nFrom == ::ca::seek_current);
      ASSERT(::ca::seek_begin == FILE_BEGIN && ::ca::seek_end == FILE_END && ::ca::seek_current == FILE_CURRENT);

      LONG lLoOffset = lOff & 0xffffffff;
      LONG lHiOffset = (lOff >> 32) & 0xffffffff;

      file_position posNew = ::SetFilePointer((HANDLE)m_hFile, lLoOffset, &lHiOffset, (DWORD)nFrom);
      posNew |= ((file_position) lHiOffset) << 32;
      if(posNew  == (file_position)-1)
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());

      return posNew;
   }

   file_position file::get_position() const
   {
      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      LONG lLoOffset = 0;
      LONG lHiOffset = 0;

      file_position pos = ::SetFilePointer((HANDLE)m_hFile, lLoOffset, &lHiOffset, FILE_CURRENT);
      pos |= ((file_position)lHiOffset) << 32;
      if(pos  == (file_position)-1)
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());

      return pos;
   }

   void file::Flush()
   {
      ASSERT_VALID(this);

      if (m_hFile == (UINT)hFileNull)
         return;

      if (!::FlushFileBuffers((HANDLE)m_hFile))
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   void file::close()
   {

      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      bool bError = FALSE;
      DWORD dwLastError = 0;
      if (m_hFile != (UINT)hFileNull)
      {
         bError = !::CloseHandle((HANDLE)m_hFile);
         if(bError)
         {
            dwLastError = ::GetLastError();
         }
      }

      m_hFile = (UINT) hFileNull;
      m_bCloseOnDelete = FALSE;

      if (bError)
         file_exception::ThrowOsError(get_app(), dwLastError, m_strFileName);

   }

   void file::Abort()
   {
      ASSERT_VALID(this);
      if (m_hFile != (UINT)hFileNull)
      {
         // close but ignore errors
         ::CloseHandle((HANDLE)m_hFile);
         m_hFile = (UINT)hFileNull;
      }
      m_strFileName.Empty();
   }

   void file::LockRange(file_position dwPos, file_size dwCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      if (!::LockFile((HANDLE)m_hFile, LODWORD(dwPos), HIDWORD(dwPos), LODWORD(dwCount), HIDWORD(dwCount)))
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   void file::UnlockRange(file_position dwPos, file_size dwCount)
   {
      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      if (!::UnlockFile((HANDLE)m_hFile,  LODWORD(dwPos), HIDWORD(dwPos), LODWORD(dwCount), HIDWORD(dwCount)))
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   void file::set_length(file_size dwNewLen)
   {
      ASSERT_VALID(this);
      ASSERT(m_hFile != (UINT)hFileNull);

      seek((LONG)dwNewLen, (::ca::e_seek)::ca::seek_begin);

      if (!::SetEndOfFile((HANDLE)m_hFile))
         file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   file_size file::get_length() const
   {
      ASSERT_VALID(this);

      file_position dwLen, dwCur;

      // seek is a non const operation
      file* pFile = (file*)this;
      dwCur = pFile->seek(0L, ::ca::seek_current);
      dwLen = pFile->seek_to_end();
      VERIFY(dwCur == (uint64_t)pFile->seek((file_offset) dwCur, ::ca::seek_begin));

      return (file_size) dwLen;
   }

   // file does not support direct buffering (CMemFile does)
   uint64_t file::GetBufferPtr(UINT nCommand, uint64_t /*nCount*/,
      void ** /*ppBufStart*/, void ** /*ppBufMax*/)
   {
      ASSERT(nCommand == bufferCheck);
      UNUSED(nCommand);    // not used in retail build

      return 0;   // no support
   }

   /*
   void file::Rename(const char * lpszOldName, const char * lpszNewName)
   {
   if (!::MoveFile((LPTSTR)lpszOldName, (LPTSTR)lpszNewName))
   file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   void file::remove(const char * lpszFileName)
   {
   if (!::DeleteFile((LPTSTR)lpszFileName))
   file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());
   }
   */





   /////////////////////////////////////////////////////////////////////////////
   // file diagnostics


   void file::assert_valid() const
   {
      ::ca::object::assert_valid();
      // we permit the descriptor m_hFile to be any value for derived classes
   }

   void file::dump(dump_context & dumpcontext) const
   {
      ::ca::object::dump(dumpcontext);

      dumpcontext << "with handle " << (UINT)m_hFile;
      dumpcontext << " and name \"" << m_strFileName << "\"";
      dumpcontext << "\n";
   }




   /////////////////////////////////////////////////////////////////////////////
   // FileException helpers

#ifdef DEBUG
   static const char * rgszFileExceptionCause[] =
   {
      "none",
      "generic",
      "fileNotFound",
      "badPath",
      "tooManyOpenFiles",
      "accessDenied",
      "invalidFile",
      "removeCurrentDir",
      "directoryFull",
      "badSeek",
      "hardIO",
      "sharingViolation",
      "lockViolation",
      "diskFull",
      "endOfFile",
   };
   static const char szUnknown[] = "unknown";
#endif


   /*void CLASS_DECL_win vfxThrowFileException(int32_t cause, LONG lOsError,
   //   const char * lpszFileName /* == ::null() */
   /*{
   #ifdef DEBUG
   const char * lpsz;
   if (cause >= 0 && cause < _countof(rgszFileExceptionCause))
   lpsz = rgszFileExceptionCause[cause];
   else
   lpsz = szUnknown;
   TRACE3("file exception: %hs, file %W, App error information = %ld.\n",
   lpsz, (lpszFileName == ::null()) ? L"Unknown" : lpszFileName, lOsError);
   #endif
   THROW(new FileException(cause, lOsError, lpszFileName));
   }*/

   /* Error Codes */

#define EPERM           1
#define ENOENT          2
#define ESRCH           3
#define EINTR           4
#define EIO             5
#define ENXIO           6
#define E2BIG           7
#define ENOEXEC         8
#define EBADF           9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         36
#define ENAMETOOLONG    38
#define ENOLCK          39
#define ENOSYS          40
#define ENOTEMPTY       41
#define EILSEQ          42

   /*
   * Support EDEADLOCK for compatibiity with older MS-C versions.
   */
#define EDEADLOCK       EDEADLK


   /////////////////////////////////////////////////////////////////////////////
   // file name handlers

   string file::GetFileName() const
   {
      ASSERT_VALID(this);

      ::ca::file_status status;
      GetStatus(status);
      return System.file().name_(status.m_strFullName);
   }

   string file::GetFileTitle() const
   {
      ASSERT_VALID(this);

      ::ca::file_status status;
      GetStatus(status);
      return System.file().title_(status.m_strFullName);
   }

   string file::GetFilePath() const
   {
      ASSERT_VALID(this);

      ::ca::file_status status;
      GetStatus(status);
      return status.m_strFullName;
   }




   /////////////////////////////////////////////////////////////////////////////
   // FileException




   void file_exception::ThrowOsError(sp(::ca::application) papp, LONG lOsError, const char * lpszFileName /* = ::null() */)
   {
      if (lOsError != 0)
         vfxThrowFileException(papp, file_exception::OsErrorToException(lOsError), lOsError, lpszFileName);
   }

   void file_exception::ThrowErrno(sp(::ca::application) papp, int32_t nErrno, const char * lpszFileName /* = ::null() */)
   {
      if (nErrno != 0)
         vfxThrowFileException(papp, file_exception::ErrnoToException(nErrno), _doserrno, lpszFileName);
   }



   int32_t file_exception::OsErrorToException(LONG lOsErr)
   {
      // NT Error codes
      switch ((UINT)lOsErr)
      {
      case NO_ERROR:
         return ::ca::file_exception::none;
      case ERROR_FILE_NOT_FOUND:
         return ::ca::file_exception::fileNotFound;
      case ERROR_PATH_NOT_FOUND:
         return ::ca::file_exception::badPath;
      case ERROR_TOO_MANY_OPEN_FILES:
         return ::ca::file_exception::tooManyOpenFiles;
      case ERROR_ACCESS_DENIED:
         return ::ca::file_exception::accessDenied;
      case ERROR_INVALID_HANDLE:
         return ::ca::file_exception::fileNotFound;
      case ERROR_BAD_FORMAT:
         return ::ca::file_exception::invalidFile;
      case ERROR_INVALID_ACCESS:
         return ::ca::file_exception::accessDenied;
      case ERROR_INVALID_DRIVE:
         return ::ca::file_exception::badPath;
      case ERROR_CURRENT_DIRECTORY:
         return ::ca::file_exception::removeCurrentDir;
      case ERROR_NOT_SAME_DEVICE:
         return ::ca::file_exception::badPath;
      case ERROR_NO_MORE_FILES:
         return ::ca::file_exception::fileNotFound;
      case ERROR_WRITE_PROTECT:
         return ::ca::file_exception::accessDenied;
      case ERROR_BAD_UNIT:
         return ::ca::file_exception::hardIO;
      case ERROR_NOT_READY:
         return ::ca::file_exception::hardIO;
      case ERROR_BAD_COMMAND:
         return ::ca::file_exception::hardIO;
      case ERROR_CRC:
         return ::ca::file_exception::hardIO;
      case ERROR_BAD_LENGTH:
         return ::ca::file_exception::badSeek;
      case ERROR_SEEK:
         return ::ca::file_exception::badSeek;
      case ERROR_NOT_DOS_DISK:
         return ::ca::file_exception::invalidFile;
      case ERROR_SECTOR_NOT_FOUND:
         return ::ca::file_exception::badSeek;
      case ERROR_WRITE_FAULT:
         return ::ca::file_exception::accessDenied;
      case ERROR_READ_FAULT:
         return ::ca::file_exception::badSeek;
      case ERROR_SHARING_VIOLATION:
         return ::ca::file_exception::sharingViolation;
      case ERROR_LOCK_VIOLATION:
         return ::ca::file_exception::lockViolation;
      case ERROR_WRONG_DISK:
         return ::ca::file_exception::badPath;
      case ERROR_SHARING_BUFFER_EXCEEDED:
         return ::ca::file_exception::tooManyOpenFiles;
      case ERROR_HANDLE_EOF:
         return ::ca::file_exception::endOfFile;
      case ERROR_HANDLE_DISK_FULL:
         return ::ca::file_exception::diskFull;
      case ERROR_DUP_NAME:
         return ::ca::file_exception::badPath;
      case ERROR_BAD_NETPATH:
         return ::ca::file_exception::badPath;
      case ERROR_NETWORK_BUSY:
         return ::ca::file_exception::accessDenied;
      case ERROR_DEV_NOT_EXIST:
         return ::ca::file_exception::badPath;
      case ERROR_ADAP_HDW_ERR:
         return ::ca::file_exception::hardIO;
      case ERROR_BAD_NET_RESP:
         return ::ca::file_exception::accessDenied;
      case ERROR_UNEXP_NET_ERR:
         return ::ca::file_exception::hardIO;
      case ERROR_BAD_REM_ADAP:
         return ::ca::file_exception::invalidFile;
      case ERROR_NO_SPOOL_SPACE:
         return ::ca::file_exception::directoryFull;
      case ERROR_NETNAME_DELETED:
         return ::ca::file_exception::accessDenied;
      case ERROR_NETWORK_ACCESS_DENIED:
         return ::ca::file_exception::accessDenied;
      case ERROR_BAD_DEV_TYPE:
         return ::ca::file_exception::invalidFile;
      case ERROR_BAD_NET_NAME:
         return ::ca::file_exception::badPath;
      case ERROR_TOO_MANY_NAMES:
         return ::ca::file_exception::tooManyOpenFiles;
      case ERROR_SHARING_PAUSED:
         return ::ca::file_exception::badPath;
      case ERROR_REQ_NOT_ACCEP:
         return ::ca::file_exception::accessDenied;
      case ERROR_FILE_EXISTS:
         return ::ca::file_exception::accessDenied;
      case ERROR_CANNOT_MAKE:
         return ::ca::file_exception::accessDenied;
      case ERROR_ALREADY_ASSIGNED:
         return ::ca::file_exception::badPath;
      case ERROR_INVALID_PASSWORD:
         return ::ca::file_exception::accessDenied;
      case ERROR_NET_WRITE_FAULT:
         return ::ca::file_exception::hardIO;
      case ERROR_DISK_CHANGE:
         return ::ca::file_exception::fileNotFound;
      case ERROR_DRIVE_LOCKED:
         return ::ca::file_exception::lockViolation;
      case ERROR_BUFFER_OVERFLOW:
         return ::ca::file_exception::badPath;
      case ERROR_DISK_FULL:
         return ::ca::file_exception::diskFull;
      case ERROR_NO_MORE_SEARCH_HANDLES:
         return ::ca::file_exception::tooManyOpenFiles;
      case ERROR_INVALID_TARGET_HANDLE:
         return ::ca::file_exception::invalidFile;
      case ERROR_INVALID_CATEGORY:
         return ::ca::file_exception::hardIO;
      case ERROR_INVALID_NAME:
         return ::ca::file_exception::badPath;
      case ERROR_INVALID_LEVEL:
         return ::ca::file_exception::badPath;
      case ERROR_NO_VOLUME_LABEL:
         return ::ca::file_exception::badPath;
      case ERROR_NEGATIVE_SEEK:
         return ::ca::file_exception::badSeek;
      case ERROR_SEEK_ON_DEVICE:
         return ::ca::file_exception::badSeek;
      case ERROR_DIR_NOT_ROOT:
         return ::ca::file_exception::badPath;
      case ERROR_DIR_NOT_EMPTY:
         return ::ca::file_exception::removeCurrentDir;
      case ERROR_LABEL_TOO_LONG:
         return ::ca::file_exception::badPath;
      case ERROR_BAD_PATHNAME:
         return ::ca::file_exception::badPath;
      case ERROR_LOCK_FAILED:
         return ::ca::file_exception::lockViolation;
      case ERROR_BUSY:
         return ::ca::file_exception::accessDenied;
      case ERROR_INVALID_ORDINAL:
         return ::ca::file_exception::invalidFile;
      case ERROR_ALREADY_EXISTS:
         return ::ca::file_exception::accessDenied;
      case ERROR_INVALID_EXE_SIGNATURE:
         return ::ca::file_exception::invalidFile;
      case ERROR_BAD_EXE_FORMAT:
         return ::ca::file_exception::invalidFile;
      case ERROR_FILENAME_EXCED_RANGE:
         return ::ca::file_exception::badPath;
      case ERROR_META_EXPANSION_TOO_LONG:
         return ::ca::file_exception::badPath;
      case ERROR_DIRECTORY:
         return ::ca::file_exception::badPath;
      case ERROR_OPERATION_ABORTED:
         return ::ca::file_exception::hardIO;
      case ERROR_IO_INCOMPLETE:
         return ::ca::file_exception::hardIO;
      case ERROR_IO_PENDING:
         return ::ca::file_exception::hardIO;
      case ERROR_SWAPERROR:
         return ::ca::file_exception::accessDenied;
      default:
         return ::ca::file_exception::type_generic;
      }
   }


   // IMPLEMENT_DYNAMIC(WinFileException, base_exception)

   /////////////////////////////////////////////////////////////////////////////




   /////////////////////////////////////////////////////////////////////////////
   // file Status implementation

   bool file::GetStatus(::ca::file_status& rStatus) const
   {
      ASSERT_VALID(this);

      //memset(&rStatus, 0, sizeof(::ca::file_status));

      // copy file name from cached m_strFileName
      rStatus.m_strFullName = ::ca::international::unicode_to_utf8(m_wstrFileName);

      if (m_hFile != hFileNull)
      {
         // get time ::ca::seek_current file size
         FILETIME ftCreate, ftAccess, ftModify;
         if (!::GetFileTime((HANDLE)m_hFile, &ftCreate, &ftAccess, &ftModify))
            return FALSE;

         if ((rStatus.m_size = ::GetFileSize((HANDLE)m_hFile, ::null())) == (DWORD)-1L)
            return FALSE;

         if (m_strFileName.is_empty())
            rStatus.m_attribute = 0;
         else
         {
            DWORD dwAttribute = ::GetFileAttributesW(::ca::international::utf8_to_unicode(m_strFileName));

            // don't return an error for this because previous versions of ca API didn't
            if (dwAttribute == 0xFFFFFFFF)
               rStatus.m_attribute = 0;
            else
            {
               rStatus.m_attribute = (BYTE) dwAttribute & 0xff;
               //#ifdef DEBUG
               // ca API BUG: m_attribute is only a BYTE wide
               //             if (dwAttribute & ~0xFF)
               //                  TRACE0("Warning: file::GetStatus() returns m_attribute without high-order flags.\n");
               //#endif
            }
         }

         // convert times as appropriate
         rStatus.m_ctime = ::datetime::time(ftCreate);
         rStatus.m_atime = ::datetime::time(ftAccess);
         rStatus.m_mtime = ::datetime::time(ftModify);

         if (rStatus.m_ctime.get_time() == 0)
            rStatus.m_ctime = rStatus.m_mtime;

         if (rStatus.m_atime.get_time() == 0)
            rStatus.m_atime = rStatus.m_mtime;
      }
      return TRUE;
   }


   bool file::GetStatus(const char * lpszFileName, ::ca::file_status& rStatus)
   {
      // attempt to fully qualify path first
      wstring wstrFullName;
      wstring wstrFileName;
      wstrFileName = ::ca::international::utf8_to_unicode(lpszFileName);
      if (!vfxFullPath(wstrFullName, wstrFileName))
      {
         rStatus.m_strFullName.Empty();
         return FALSE;
      }
      ::ca::international::unicode_to_utf8(rStatus.m_strFullName, wstrFullName);

      WIN32_FIND_DATA findFileData;
      HANDLE hFind = FindFirstFile((LPTSTR)lpszFileName, &findFileData);
      if (hFind == INVALID_HANDLE_VALUE)
         return FALSE;
      VERIFY(FindClose(hFind));

      // strip attribute of NORMAL bit, our API doesn't have a "normal" bit.
      rStatus.m_attribute = (BYTE) (findFileData.dwFileAttributes & ~FILE_ATTRIBUTE_NORMAL);

      // get just the low DWORD of the file size
      ASSERT(findFileData.nFileSizeHigh == 0);
      rStatus.m_size = (LONG)findFileData.nFileSizeLow;

      // convert times as appropriate
      rStatus.m_ctime = ::datetime::time(findFileData.ftCreationTime);
      rStatus.m_atime = ::datetime::time(findFileData.ftLastAccessTime);
      rStatus.m_mtime = ::datetime::time(findFileData.ftLastWriteTime);

      if (rStatus.m_ctime.get_time() == 0)
         rStatus.m_ctime = rStatus.m_mtime;

      if (rStatus.m_atime.get_time() == 0)
         rStatus.m_atime = rStatus.m_mtime;

      return TRUE;
   }


   /*
   UINT CLASS_DECL_win vfxGetFileTitle(const wchar_t * lpszPathName, wchar_t * lpszTitle, UINT nMax)
   {
   ASSERT(lpszTitle == ::null() ||
   __is_valid_address(lpszTitle, _MAX_FNAME));
   ASSERT(__is_valid_string(lpszPathName));

   // use a temporary to avoid bugs in ::GetFileTitle when lpszTitle is ::null()
   WCHAR szTemp[_MAX_PATH];
   wchar_t * lpszTemp = lpszTitle;
   if (lpszTemp == ::null())
   {
   lpszTemp = szTemp;
   nMax = _countof(szTemp);
   }
   if (::GetFileTitleW(lpszPathName, lpszTemp, (WORD)nMax) != 0)
   {
   // when ::GetFileTitle fails, use cheap imitation
   return vfxGetFileName(lpszPathName, lpszTitle, nMax);
   }
   return lpszTitle == ::null() ? lstrlenW(lpszTemp)+1 : 0;
   }


   bool vfxComparePath(const wchar_t * lpszPath1, const wchar_t * lpszPath2)
   {
   // use case insensitive compare as a starter
   if (lstrcmpiW(lpszPath1, lpszPath2) != 0)
   return FALSE;

   // on non-DBCS systems, we are done
   if (!GetSystemMetrics(SM_DBCSENABLED))
   return TRUE;

   // on DBCS systems, the file name may not actually be the same
   // in particular, the file system is case sensitive with respect to
   // "full width" roman characters.
   // (ie. fullwidth-R is different from fullwidth-r).
   int32_t nLen = lstrlenW(lpszPath1);
   if (nLen != lstrlenW(lpszPath2))
   return FALSE;
   ASSERT(nLen < _MAX_PATH);

   // need to get both CT_CTYPE1 and CT_CTYPE3 for each filename
   LCID lcid = GetThreadLocale();
   WORD aCharType11[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE1, lpszPath1, -1, aCharType11));
   WORD aCharType13[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE3, lpszPath1, -1, aCharType13));
   WORD aCharType21[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE1, lpszPath2, -1, aCharType21));
   #ifdef DEBUG
   WORD aCharType23[_MAX_PATH];
   VERIFY(GetStringTypeExW(lcid, CT_CTYPE3, lpszPath2, -1, aCharType23));
   #endif

   // for every C3_FULLWIDTH character, make sure it has same C1 value
   int32_t i = 0;
   for (const wchar_t * lpsz = lpszPath1; *lpsz != 0; lpsz = _wcsinc(lpsz))
   {
   // check for C3_FULLWIDTH characters only
   if (aCharType13[i] & C3_FULLWIDTH)
   {
   #ifdef DEBUG
   ASSERT(aCharType23[i] & C3_FULLWIDTH); // should always match!
   #endif

   // if CT_CTYPE1 is different then file system considers these
   // file names different.
   if (aCharType11[i] != aCharType21[i])
   return FALSE;
   }
   ++i; // look at next character type
   }
   return TRUE; // otherwise file name is truly the same
   }
   */

   /*
   void file::SetStatus(const char * lpszFileName, const ::ca::file_status& status)
   {
   DWORD wAttr;
   FILETIME creationTime;
   FILETIME lastAccessTime;
   FILETIME lastWriteTime;
   LPFILETIME lpCreationTime = ::null();
   LPFILETIME lpLastAccessTime = ::null();
   LPFILETIME lpLastWriteTime = ::null();

   if ((wAttr = GetFileAttributes((LPTSTR)lpszFileName)) == (DWORD)-1L)
   file_exception::ThrowOsError(get_app(), (LONG)GetLastError());

   if ((DWORD)status.m_attribute != wAttr && (wAttr & readOnly))
   {
   // set file attribute, only if currently readonly.
   // This way we will be able to modify the time assuming the
   // caller changed the file from readonly.

   if (!SetFileAttributes((LPTSTR)lpszFileName, (DWORD)status.m_attribute))
   file_exception::ThrowOsError(get_app(), (LONG)GetLastError());
   }

   // last modification time
   if (status.m_mtime.get_time() != 0)
   {
   ::ca::TimeToFileTime(status.m_mtime, &lastWriteTime);
   lpLastWriteTime = &lastWriteTime;

   // last access time
   if (status.m_atime.get_time() != 0)
   {
   ::ca::TimeToFileTime(status.m_atime, &lastAccessTime);
   lpLastAccessTime = &lastAccessTime;
   }

   // create time
   if (status.m_ctime.get_time() != 0)
   {
   ::ca::TimeToFileTime(status.m_ctime, &creationTime);
   lpCreationTime = &creationTime;
   }

   HANDLE hFile = ::CreateFile(lpszFileName, GENERIC_READ|GENERIC_WRITE,
   FILE_SHARE_READ, ::null(), OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
   ::null());

   if (hFile == INVALID_HANDLE_VALUE)
   file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());

   if (!SetFileTime((HANDLE)hFile, lpCreationTime, lpLastAccessTime, lpLastWriteTime))
   file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());

   if (!::CloseHandle(hFile))
   file_exception::ThrowOsError(get_app(), (LONG)::GetLastError());
   }

   if ((DWORD)status.m_attribute != wAttr && !(wAttr & readOnly))
   {
   if (!SetFileAttributes((LPTSTR)lpszFileName, (DWORD)status.m_attribute))
   file_exception::ThrowOsError(get_app(), (LONG)GetLastError());
   }
   }
   */

   bool file::IsOpened()
   {
      return m_hFile != hFileNull;
   }


   void file::SetFilePath(const char * lpszNewName)
   {
      ASSERT_VALID(this);
      ASSERT(__is_valid_string(lpszNewName));
      m_strFileName = lpszNewName;
   }



   // ::ca::filesp
   file::operator HFILE() const
   {

      return m_hFile;

   }

   uint64_t file::ReadHuge(void * lpBuffer, uint64_t dwCount)
   {

      return (uint64_t) read(lpBuffer, (UINT)dwCount);

   }

   void file::WriteHuge(const void * lpBuffer, uint64_t dwCount)
   {

      write(lpBuffer, (UINT)dwCount);

   }



} // namespace win




#define _wcsdec(_cpc1, _cpc2) ((_cpc1)>=(_cpc2) ? ::null() : (_cpc2)-1)

#define _wcsinc(_pc)    ((_pc)+1)




// turn a file, relative path or other into an absolute path
bool CLASS_DECL_win vfxFullPath(wstring & wstrFullPath, const wstring & wstrPath)
   // lpszPathOut = buffer of _MAX_PATH
   // lpszFileIn = file, relative path or absolute path
   // (both in ANSI character set)
{

   strsize dwAllocLen = wstrPath.get_length() + _MAX_PATH;

   wstrFullPath.alloc(dwAllocLen);

   // first, fully qualify the path name
   wchar_t * lpszFilePart;

   strsize dwLen = GetFullPathNameW(wstrPath, (DWORD) dwAllocLen, wstrFullPath, &lpszFilePart);

   if(dwLen == 0)
   {
#ifdef DEBUG
      //      if (lpszFileIn[0] != '\0')
      //       TRACE1("Warning: could not parse the path '%s'.\n", lpszFileIn);
#endif
      wstrFullPath = wstrPath; // take it literally
      return FALSE;
   }
   else if(dwLen > dwAllocLen)
   {

      dwAllocLen = dwLen + _MAX_PATH;

      dwLen = GetFullPathNameW(wstrPath, (DWORD) dwAllocLen, wstrFullPath, &lpszFilePart);

      if(dwLen == 0 || dwLen > dwAllocLen)
      {
#ifdef DEBUG
         //      if (lpszFileIn[0] != '\0')
         //       TRACE1("Warning: could not parse the path '%s'.\n", lpszFileIn);
#endif
         wstrFullPath = wstrPath; // take it literally
         return FALSE;
      }

   }

   wstring wstrRoot;
   // determine the root name of the volume
   vfxGetRoot(wstrRoot, wstrFullPath);

   // get file system information for the volume
   DWORD dwFlags, dwDummy;
   if (!GetVolumeInformationW(wstrRoot, ::null(), 0, ::null(), &dwDummy, &dwFlags, ::null(), 0))
   {
      //      TRACE1("Warning: could not get volume information '%s'.\n", strRoot);
      return FALSE;   // preserving case may not be correct
   }

   // not all characters have complete uppercase/lowercase
   if (!(dwFlags & FS_CASE_IS_PRESERVED))
      CharUpperW(wstrFullPath);

   // assume non-UNICODE file systems, use OEM character set
   if (!(dwFlags & FS_UNICODE_STORED_ON_DISK))
   {
      WIN32_FIND_DATAW data;
      HANDLE h = FindFirstFileW(wstrPath, &data);
      if (h != INVALID_HANDLE_VALUE)
      {
         FindClose(h);
         int32_t iLenFileName = lstrlenW(data.cFileName);
         if(iLenFileName >=  MAX_PATH)
         {
            wstring wstrBackup = wstrFullPath;
            strsize iFilePart = lpszFilePart - wstrFullPath;
            wstrFullPath.alloc(iFilePart + iLenFileName + 32); // arrange more space with more 32 extra wchars
            lstrcpynW(wstrFullPath, wstrBackup, (int32_t) iFilePart);
            lpszFilePart = (wchar_t *) wstrFullPath + iFilePart;
         }
         lstrcpyW(lpszFilePart, data.cFileName);
         wstrFullPath.release_buffer();
      }
   }
   return TRUE;
}

/*void CLASS_DECL_win __get_root_path(const char * lpszPath, string & strRoot)
{
ASSERT(lpszPath != ::null());
// determine the root name of the volume
LPTSTR lpszRoot = strRoot.GetBuffer(_MAX_PATH);
memset(lpszRoot, 0, _MAX_PATH);
lstrcpyn(lpszRoot, lpszPath, _MAX_PATH);
LPTSTR lpsz;
for (lpsz = lpszRoot; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
{
// find first double slash and stop
if (IsDirSep(lpsz[0]) && IsDirSep(lpsz[1]))
break;
}
if (*lpsz != '\0')
{
// it is a UNC name, find second slash past '\\'
ASSERT(IsDirSep(lpsz[0]));
ASSERT(IsDirSep(lpsz[1]));
lpsz += 2;
while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
lpsz = _tcsinc(lpsz);
if (*lpsz != '\0')
lpsz = _tcsinc(lpsz);
while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
lpsz = _tcsinc(lpsz);
// terminate it just after the UNC root (ie. '\\server\share\')
if (*lpsz != '\0')
lpsz[1] = '\0';
}
else
{
// not a UNC, look for just the first slash
lpsz = lpszRoot;
while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
lpsz = _tcsinc(lpsz);
// terminate it just after root (ie. 'x:\')
if (*lpsz != '\0')
lpsz[1] = '\0';
}
strRoot.ReleaseBuffer();
}*/

/*bool CLASS_DECL_win ::ca::ComparePath(const char * lpszPath1, const char * lpszPath2)
{
// use case insensitive compare as a starter
if (lstrcmpi(lpszPath1, lpszPath2) != 0)
return FALSE;

// on non-DBCS systems, we are done
if (!GetSystemMetrics(SM_DBCSENABLED))
return TRUE;

// on DBCS systems, the file name may not actually be the same
// in particular, the file system is case sensitive with respect to
// "full width" roman characters.
// (ie. fullwidth-R is different from fullwidth-r).
int32_t nLen = lstrlen(lpszPath1);
if (nLen != lstrlen(lpszPath2))
return FALSE;
ASSERT(nLen < _MAX_PATH);

// need to get both CT_CTYPE1 and CT_CTYPE3 for each filename
LCID lcid = GetThreadLocale();
WORD aCharType11[_MAX_PATH];
VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath1, -1, aCharType11));
WORD aCharType13[_MAX_PATH];
VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath1, -1, aCharType13));
WORD aCharType21[_MAX_PATH];
VERIFY(GetStringTypeEx(lcid, CT_CTYPE1, lpszPath2, -1, aCharType21));
#ifdef DEBUG
WORD aCharType23[_MAX_PATH];
VERIFY(GetStringTypeEx(lcid, CT_CTYPE3, lpszPath2, -1, aCharType23));
#endif

// for every C3_FULLWIDTH character, make sure it has same C1 value
int32_t i = 0;
for (const char * lpsz = lpszPath1; *lpsz != 0; lpsz = _tcsinc(lpsz))
{
// check for C3_FULLWIDTH characters only
if (aCharType13[i] & C3_FULLWIDTH)
{
#ifdef DEBUG
ASSERT(aCharType23[i] & C3_FULLWIDTH); // should always match!
#endif

// if CT_CTYPE1 is different then file system considers these
// file names different.
if (aCharType11[i] != aCharType21[i])
return FALSE;
}
++i; // look at next character type
}
return TRUE; // otherwise file name is truly the same
}*/

/*UINT CLASS_DECL_win __get_file_title(const char * lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
ASSERT(lpszTitle == ::null() ||
__is_valid_address(lpszTitle, _MAX_FNAME));
ASSERT(__is_valid_string(lpszPathName));

// use a temporary to avoid bugs in ::GetFileTitle when lpszTitle is ::null()
char szTemp[_MAX_PATH];
LPTSTR lpszTemp = lpszTitle;
if (lpszTemp == ::null())
{
lpszTemp = szTemp;
nMax = _countof(szTemp);
}
if (::GetFileTitle(lpszPathName, lpszTemp, (WORD)nMax) != 0)
{
// when ::GetFileTitle fails, use cheap imitation
return ::ca::GetFileName(lpszPathName, lpszTitle, nMax);
}
return lpszTitle == ::null() ? lstrlen(lpszTemp)+1 : 0;
}*/

void CLASS_DECL_win vfxGetModuleShortFileName(HINSTANCE hInst, string& strShortName)
{
   WCHAR szLongPathName[_MAX_PATH];
   wstring wstrShortName;
   ::GetModuleFileNameW(hInst, szLongPathName, _MAX_PATH);
   if(::GetShortPathNameW(szLongPathName, wstrShortName.alloc(_MAX_PATH * 4), _MAX_PATH * 4) == 0)
   {
      // rare failure case (especially on not-so-modern file systems)
      ::ca::international::unicode_to_utf8(strShortName, szLongPathName);
   }
   else
   {
      wstrShortName.release_buffer();
      ::ca::international::unicode_to_utf8(strShortName, wstrShortName);
   }
}




string CLASS_DECL_win vfxStringFromCLSID(REFCLSID rclsid)
{
   WCHAR szCLSID[256];
   wsprintfW(szCLSID, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
      rclsid.Data1, rclsid.Data2, rclsid.Data3,
      rclsid.Data4[0], rclsid.Data4[1], rclsid.Data4[2], rclsid.Data4[3],
      rclsid.Data4[4], rclsid.Data4[5], rclsid.Data4[6], rclsid.Data4[7]);
   return szCLSID;
}

bool CLASS_DECL_win vfxGetInProcServer(const char * lpszCLSID, string & str)
{
   HKEY hKey = ::null();
   bool b = FALSE;
   if (RegOpenKey(HKEY_CLASSES_ROOT, "CLSID", &hKey) == ERROR_SUCCESS)
   {
      HKEY hKeyCLSID = ::null();
      if (RegOpenKey(hKey, lpszCLSID, &hKeyCLSID) == ERROR_SUCCESS)
      {
         HKEY hKeyInProc = ::null();
         if (RegOpenKey(hKeyCLSID, "InProcServer32", &hKeyInProc) ==
            ERROR_SUCCESS)
         {
            LPTSTR lpsz = str.GetBuffer(_MAX_PATH);
            DWORD dwSize = _MAX_PATH * sizeof(char);
            DWORD dwType;
            LONG lRes = ::RegQueryValueEx(hKeyInProc, "",
               ::null(), &dwType, (BYTE*)lpsz, &dwSize);
            str.ReleaseBuffer();
            b = (lRes == ERROR_SUCCESS);
            RegCloseKey(hKeyInProc);
         }
         RegCloseKey(hKeyCLSID);
      }
      RegCloseKey(hKey);
   }
   return b;
}
//#endif  //!___NO_OLE_SUPPORT


bool CLASS_DECL_win vfxResolveShortcut(string & strTarget, const char * pszSource, sp(::user::interaction) puiMessageParentOptional)
{

   sp(::user::interaction) pui = puiMessageParentOptional;

   wstring wstrFileOut;
   wstring wstrFileIn = ::ca::international::utf8_to_unicode(pszSource);

   DWORD dwVersion = GetVersion();

   // get the Windows version.

   DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
   DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

   // get the build number.

   DWORD dwBuild;

   if (dwVersion < 0x80000000)              // Windows NT
      dwBuild = (DWORD)(HIWORD(dwVersion));
   else if (dwWindowsMajorVersion < 4)      // Win32s
      dwBuild = (DWORD)(HIWORD(dwVersion) & ~0x8000);
   else                                     // Windows Me/98/95
      dwBuild =  0;

   bool bNativeUnicode;
   if (dwVersion < 0x80000000)              // Windows NT
      bNativeUnicode = TRUE;
   else if (dwWindowsMajorVersion < 4)      // Win32s
      bNativeUnicode = FALSE;
   else                                     // Windows Me/98/95
      bNativeUnicode = FALSE;


   //   __COM com;
   IShellLinkW* psl;
   wstrFileOut = L"";

   SHFILEINFOW info;
   if ((::win::shell::SHGetFileInfo(wstrFileIn, 0, &info, sizeof(info),
      SHGFI_ATTRIBUTES) == 0) || !(info.dwAttributes & SFGAO_LINK))
   {
      return FALSE;
   }

   HRESULT hr ; 
   if (FAILED(hr = CoCreateInstance(CLSID_ShellLink, ::null(), CLSCTX_INPROC_SERVER, IID_IShellLinkW,
      (LPVOID*)&psl)))
   {
      return FALSE;
   }

   IPersistFile *ppf;
   if (SUCCEEDED(psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf)))
   {
      if (SUCCEEDED(ppf->Load(wstrFileIn, STGM_READ)))
      {
         /* Resolve the link, this may post UI to find the link */
         if (SUCCEEDED(psl->Resolve(pui == ::null() ? ::null() : pui->get_handle(),
            SLR_ANY_MATCH | (pui == ::null() ? (SLR_NO_UI | (8400 << 16)) : 0))))
         {
            wstrFileOut.alloc(MAX_PATH);
            bool bOk;
            if(SUCCEEDED(psl->GetPath(wstrFileOut, MAX_PATH, ::null(), 0)))
            {
               bOk = true;
               wstrFileOut.release_buffer();
               strTarget = ::ca::international::unicode_to_utf8((LPCWSTR) wstrFileOut);
            }
            else
            {
               bOk = false;
            }
            ppf->Release();
            psl->Release();
            return bOk;
         }
      }
      ppf->Release();
   }
   psl->Release();
   return FALSE;
}

// turn a file, relative path or other into an absolute path
bool CLASS_DECL_win vfxFullPath(wchar_t * lpszPathOut, const wchar_t * lpszFileIn)
   // lpszPathOut = buffer of _MAX_PATH
   // lpszFileIn = file, relative path or absolute path
   // (both in ANSI character set)
{
   ASSERT(__is_valid_address(lpszPathOut, _MAX_PATH));

   // first, fully qualify the path name
   wchar_t * lpszFilePart;
   if (!GetFullPathNameW(lpszFileIn, _MAX_PATH, lpszPathOut, &lpszFilePart))
   {
#ifdef DEBUG
      //      if (lpszFileIn[0] != '\0')
      //       TRACE1("Warning: could not parse the path '%s'.\n", lpszFileIn);
#endif
      lstrcpynW(lpszPathOut, lpszFileIn, _MAX_PATH); // take it literally
      return FALSE;
   }

   string strRoot;
   // determine the root name of the volume
   vfxGetRoot(lpszPathOut, strRoot);

   // get file system information for the volume
   DWORD dwFlags, dwDummy;
   if (!GetVolumeInformationW(::ca::international::utf8_to_unicode(strRoot), ::null(), 0, ::null(), &dwDummy, &dwFlags, ::null(), 0))
   {
      //      TRACE1("Warning: could not get volume information '%s'.\n", strRoot);
      return FALSE;   // preserving case may not be correct
   }

   // not all characters have complete uppercase/lowercase
   if (!(dwFlags & FS_CASE_IS_PRESERVED))
      CharUpperW(lpszPathOut);

   // assume non-UNICODE file systems, use OEM character set
   if (!(dwFlags & FS_UNICODE_STORED_ON_DISK))
   {
      WIN32_FIND_DATAW data;
      HANDLE h = FindFirstFileW(lpszFileIn, &data);
      if (h != INVALID_HANDLE_VALUE)
      {
         FindClose(h);
         lstrcpyW(lpszFilePart, data.cFileName);
      }
   }
   return TRUE;
}




void CLASS_DECL_win vfxGetRoot(wstring & wstrRoot, const wstring & wstrPath)
{
   //   ASSERT(lpszPath != ::null());
   // determine the root name of the volume
   wstrRoot = wstrPath;
   wchar_t * lpszRoot = wstrRoot;
   wchar_t * lpsz;
   for (lpsz = lpszRoot; *lpsz != L'\0'; lpsz = _wcsinc(lpsz))
   {
      // find first double slash and stop
      if (IsDirSep(lpsz[0]) && IsDirSep(lpsz[1]))
         break;
   }
   if (*lpsz != '\0')
   {
      // it is a UNC name, find second slash past '\\'
      ASSERT(IsDirSep(lpsz[0]));
      ASSERT(IsDirSep(lpsz[1]));
      lpsz += 2;
      while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
         lpsz = _wcsinc(lpsz);
      if (*lpsz != '\0')
         lpsz = _wcsinc(lpsz);
      while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
         lpsz = _wcsinc(lpsz);
      // terminate it just after the UNC root (ie. '\\server\share\')
      if (*lpsz != '\0')
         lpsz[1] = '\0';
   }
   else
   {
      // not a UNC, look for just the first slash
      lpsz = lpszRoot;
      while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
         lpsz = _wcsinc(lpsz);
      // terminate it just after root (ie. 'x:\')
      if (*lpsz != '\0')
         lpsz[1] = '\0';
   }
   wstrRoot.release_buffer();
}


void CLASS_DECL_win vfxGetRoot(const wchar_t * lpszPath, string& strRoot)
{
   ASSERT(lpszPath != ::null());
   wstring wstrRoot;
   // determine the root name of the volume
   wchar_t * lpszRoot = wstrRoot.alloc(_MAX_PATH * 4);
   memset(lpszRoot, 0, _MAX_PATH * 4);
   lstrcpynW(lpszRoot, lpszPath, _MAX_PATH * 4);
   wchar_t * lpsz;
   for (lpsz = lpszRoot; *lpsz != '\0'; lpsz = _wcsinc(lpsz))
   {
      // find first double slash and stop
      if (IsDirSep(lpsz[0]) && IsDirSep(lpsz[1]))
         break;
   }
   if (*lpsz != '\0')
   {
      // it is a UNC name, find second slash past '\\'
      ASSERT(IsDirSep(lpsz[0]));
      ASSERT(IsDirSep(lpsz[1]));
      lpsz += 2;
      while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
         lpsz = _wcsinc(lpsz);
      if (*lpsz != '\0')
         lpsz = _wcsinc(lpsz);
      while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
         lpsz = _wcsinc(lpsz);
      // terminate it just after the UNC root (ie. '\\server\share\')
      if (*lpsz != '\0')
         lpsz[1] = '\0';
   }
   else
   {
      // not a UNC, look for just the first slash
      lpsz = lpszRoot;
      while (*lpsz != '\0' && (!IsDirSep(*lpsz)))
         lpsz = _wcsinc(lpsz);
      // terminate it just after root (ie. 'x:\')
      if (*lpsz != '\0')
         lpsz[1] = '\0';
   }
   ::ca::international::unicode_to_utf8(strRoot, wstrRoot);
}


/*bool CLASS_DECL_win vfxFullPath(char * lpszPathOut, const char * lpszFileIn)
// lpszPathOut = buffer of _MAX_PATH
// lpszFileIn = file, relative path or absolute path
// (both in ANSI character set)
{
ASSERT(__is_valid_address(lpszPathOut, _MAX_PATH));

// first, fully qualify the path name
wchar_t * lpszFilePart;
if (!shell::GetFullPathName(lpszFileIn, _MAX_PATH, lpszPathOut, &lpszFilePart))
{
#ifdef DEBUG
if (lpszFileIn[0] != '\0')
TRACE1("Warning: could not parse the path '%s'.\n", lpszFileIn);
#endif
lstrcpynW(lpszPathOut, lpszFileIn, _MAX_PATH); // take it literally
return FALSE;
}

string wstrRoot;
// determine the root name of the volume
vfxGetRoot(lpszPathOut, wstrRoot);

// get file system information for the volume
DWORD dwFlags, dwDummy;
if (!shell::GetVolumeInformation(wstrRoot, ::null(), 0, ::null(), &dwDummy, &dwFlags,
::null(), 0))
{
TRACE1("Warning: could not get volume information '%S'.\n",
(const char *)wstrRoot);
return FALSE;   // preserving case may not be correct
}

// not all characters have complete uppercase/lowercase
if (!(dwFlags & FS_CASE_IS_PRESERVED))
CharUpperW(lpszPathOut);

// assume non-UNICODE file systems, use OEM character set
if (!(dwFlags & FS_UNICODE_STORED_ON_DISK))
{
WIN32_FIND_DATAW data;
HANDLE h = shell::FindFirstFile(lpszFileIn, &data);
if (h != INVALID_HANDLE_VALUE)
{
FindClose(h);
wcscpy(lpszFilePart, data.cFileName);
}
}
return TRUE;
}*/




UINT CLASS_DECL_win vfxGetFileName(const wchar_t * lpszPathName, wchar_t * lpszTitle, UINT nMax)
{
   ASSERT(lpszTitle == ::null() ||
      __is_valid_address(lpszTitle, _MAX_FNAME));
   ASSERT(__is_valid_string(lpszPathName));

   // always capture the complete file name including extension (if present)
   wchar_t * lpszTemp = (wchar_t *)lpszPathName;
   for (const wchar_t * lpsz = lpszPathName; *lpsz != '\0'; lpsz = _wcsinc(lpsz))
   {
      // remember last directory/drive separator
      if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
         lpszTemp = (wchar_t *)_wcsinc(lpsz);
   }

   // lpszTitle can be ::null() which just returns the number of bytes
   if (lpszTitle == ::null())
      return lstrlenW(lpszTemp)+1;

   // otherwise copy it into the buffer provided
   lstrcpynW(lpszTitle, lpszTemp, nMax);
   return 0;
}






/////////////////////////////////////////////////////////////////////////////
// WinFileException helpers

void CLASS_DECL_win vfxThrowFileException(sp(::ca::application) papp, int32_t cause, LONG lOsError, const char * lpszFileName /* == ::null() */)
{
#ifdef DEBUG
   const char * lpsz;
   if (cause >= 0 && cause < _countof(::win::rgszFileExceptionCause))
      lpsz = ::win::rgszFileExceptionCause[cause];
   else
      lpsz = ::win::szUnknown;
   //   TRACE3("file exception: %hs, file %s, App error information = %ld.\n", lpsz, (lpszFileName == ::null()) ? "Unknown" : lpszFileName, lOsError);
#endif
   throw ::ca::file_exception(papp, cause, lOsError, lpszFileName);
}

namespace win
{

   int32_t file_exception::ErrnoToException(int32_t nErrno)
   {
      switch(nErrno)
      {
      case EPERM:
      case EACCES:
         return ::ca::file_exception::accessDenied;
      case EBADF:
         return ::ca::file_exception::invalidFile;
      case EDEADLOCK:
         return ::ca::file_exception::sharingViolation;
      case EMFILE:
         return ::ca::file_exception::tooManyOpenFiles;
      case ENOENT:
      case ENFILE:
         return ::ca::file_exception::fileNotFound;
      case ENOSPC:
         return ::ca::file_exception::diskFull;
      case EINVAL:
      case EIO:
         return ::ca::file_exception::hardIO;
      default:
         return ::ca::file_exception::type_generic;
      }

   }


} // namespace win






