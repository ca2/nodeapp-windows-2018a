#pragma once


class FileException;
struct FileStatus;


void CLASS_DECL_win vfxGetRoot(const wchar_t * lpszPath, string& wstrRoot);
void CLASS_DECL_win vfxGetRoot(wstring & wstrRoot, const wstring & wstrPath);


/////////////////////////////////////////////////////////////////////////////
// File - raw unbuffered disk file I/O


namespace win
{


   class CLASS_DECL_win file :
      virtual public ex1::file
   {
   public:


      enum Attribute
      {
         normal =    0x00,
         readOnly =  0x01,
         hidden =    0x02,
         system =    0x04,
         volume =    0x08,
         directory = 0x10,
         archive =   0x20
      };

      enum
      {
      
         hFileNull = -1
      
      };

      enum BufferCommand
      { 
         
         bufferRead, 
         bufferWrite, 
         bufferCommit, 
         bufferCheck 
      
      };

      zip::Util *    m_pziputil;
      bool           m_bCloseOnDelete;
      string         m_strFileName;
      wstring        m_wstrFileName;
      UINT           m_hFile;


      file(::ca::application * papp);
      file(::ca::application * papp, int32_t hFile);
      file(::ca::application * papp, const char * lpszFileName, UINT nOpenFlags);
      virtual ~file();


      operator HFILE() const;

      virtual file_position get_position() const;
      bool GetStatus(ex1::file_status & rStatus) const;
      virtual string GetFileName() const;
      virtual string GetFileTitle() const;
      virtual string GetFilePath() const;
      virtual void SetFilePath(const char * lpszNewName);

      virtual bool open(const char * lpszFileName, UINT nOpenFlags);

      virtual bool PASCAL GetStatus(const char * lpszFileName, ::ex1::file_status& rStatus);

      uint64_t ReadHuge(void * lpBuffer, uint64_t dwCount);
      void WriteHuge(const void * lpBuffer, uint64_t dwCount);

      virtual ex1::file * Duplicate() const;

      virtual file_position seek(file_offset lOff, ::ex1::e_seek nFrom);
      virtual void set_length(file_size dwNewLen);
      virtual file_size get_length() const;

      virtual ::primitive::memory_size read(void * lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);

      virtual void LockRange(file_position dwPos, file_size dwCount);
      virtual void UnlockRange(file_position dwPos, file_size dwCount);

      virtual void Abort();
      virtual void Flush();
      virtual void close();

      virtual bool IsOpened();
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
      
      virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0, void ** ppBufStart = NULL, void ** ppBufMax = NULL);


   };


   namespace file_exception
   {


      int32_t PASCAL OsErrorToException(LONG lOsError);
      int32_t PASCAL ErrnoToException(int32_t nErrno);
      void PASCAL ThrowOsError(::ca::application * papp, LONG lOsError, const char * lpszFileName = NULL);
      void PASCAL ThrowErrno(::ca::application * papp, int32_t nErrno, const char * lpszFileName = NULL);


   }  // namespace file_exception


} // namepsace win