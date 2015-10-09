#pragma once


class FileException;
struct FileStatus;

void CLASS_DECL_VMSWIN vfxGetRoot(const wchar_t * lpszPath, string& wstrRoot);

/////////////////////////////////////////////////////////////////////////////
// File - raw unbuffered disk file I/O

class CLASS_DECL_VMSWIN WinFile :
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

   enum { hFileNull = -1 };

   zip::Util *    m_pziputil;
   BOOL           m_bCloseOnDelete;
   string         m_strFileName;
   UINT           m_hFile;


   WinFile(::ca::application * papp);
   WinFile(::ca::application * papp, int hFile);
   WinFile(::ca::application * papp, const char * lpszFileName, UINT nOpenFlags);
   virtual ~WinFile();


   operator HFILE() const;

   virtual file_position get_position() const;
   BOOL GetStatus(ex1::file_status & rStatus) const;
   virtual string GetFileName() const;
   virtual string GetFileTitle() const;
   virtual string GetFilePath() const;
   virtual void SetFilePath(const char * lpszNewName);

   virtual BOOL open(const char * lpszFileName, UINT nOpenFlags, ex1::file_exception_sp * pError = NULL);

   virtual BOOL PASCAL GetStatus(const char * lpszFileName, ::ex1::file_status& rStatus);

   uint64_t ReadHuge(void * lpBuffer, uint64_t dwCount);
   void WriteHuge(const void * lpBuffer, uint64_t dwCount);

   virtual ex1::file * Duplicate() const;

   virtual file_position seek(file_offset lOff, ::ex1::e_seek nFrom);
   virtual void set_length(file_size dwNewLen);
   virtual file_size get_length() const;

   virtual memory_size_t read(void * lpBuf, memory_size_t nCount);
   virtual void write(const void * lpBuf, memory_size_t nCount);

   virtual void LockRange(file_position dwPos, file_size dwCount);
   virtual void UnlockRange(file_position dwPos, file_size dwCount);

   virtual void Abort();
   virtual void Flush();
   virtual void close();

   virtual bool IsOpened();
#ifdef _DEBUG
   virtual void assert_valid() const;
   virtual void dump(dump_context & dumpcontext) const;
#endif
   enum BufferCommand { bufferRead, bufferWrite, bufferCommit, bufferCheck };
   virtual uint64_t GetBufferPtr(UINT nCommand, uint64_t nCount = 0,
      void ** ppBufStart = NULL, void ** ppBufMax = NULL);

};

class CLASS_DECL_VMSWIN WinFileException :
   virtual public ex1::file_exception
{
public:
   enum
   {
      none,
      generic,
      fileNotFound,
      badPath,
      tooManyOpenFiles,
      accessDenied,
      invalidFile,
      removeCurrentDir,
      directoryFull,
      badSeek,
      hardIO,
      sharingViolation,
      lockViolation,
      diskFull,
      endOfFile
   };

   WinFileException(::ca::application * papp, int cause = none, LONG lOsError = -1, const char * lpszArchiveName = NULL);
   virtual ~WinFileException();

// Attributes
   int      m_cause;
   LONG     m_lOsError;
   string   m_strFileName;


   virtual int get_cause();
   virtual LONG get_os_error();
   virtual string get_file_path();


// Operations
   // convert a App dependent error code to a Cause
   static int PASCAL OsErrorToException(LONG lOsError);
   static int PASCAL ErrnoToException(int nErrno);

   // helper functions to throw exception after converting to a Cause
   static void PASCAL ThrowOsError(::ca::application * papp, LONG lOsError, const char * lpszFileName = NULL);
   static void PASCAL ThrowErrno(::ca::application * papp, int nErrno, const char * lpszFileName = NULL);

   virtual BOOL GetErrorMessage(string & str, PUINT pnHelpContext = NULL);
};

inline WinFileException::WinFileException(::ca::application * papp, int cause, LONG lOsError,
                                          const char * pstrFileName /* = NULL */) :
   ca(papp),
   ex1::file_exception(papp)
   { m_cause = cause; m_lOsError = lOsError; m_strFileName = pstrFileName; }
inline WinFileException::~WinFileException()
   { }


/////////////////////////////////////////////////////////////////////////////
// File status

/*struct FileStatus
{
   class time m_ctime;          // creation date/time of file
   class time m_mtime;          // last modification date/time of file
   class time m_atime;          // last access date/time of file
   LONG m_size;            // logical size of file in bytes
   BYTE m_attribute;       // logical OR of ex1::filesp::Attribute enum values
   BYTE _m_padding;        // pad the structure to a WORD
   WCHAR m_szFullName[_MAX_PATH]; // absolute path name

#ifdef _DEBUG
   void dump(dump_context & dumpcontext) const;
#endif
};*/
