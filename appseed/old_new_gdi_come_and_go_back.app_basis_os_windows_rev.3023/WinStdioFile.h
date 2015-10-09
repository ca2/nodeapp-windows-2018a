#pragma once

#include "WinFile.h"

/////////////////////////////////////////////////////////////////////////////
// STDIO file implementation

class WinStdioFile : 
   virtual public WinFile,
   virtual public ex1::text_file
{
public:


   FILE* m_pStream;    // stdio FILE
                       // m_hFile from base class is _fileno(m_pStream)



   WinStdioFile(::ca::application * papp);


   virtual void write_string(const char * lpsz);
   virtual char * read_string(char * lpsz, UINT nMax);
   virtual UINT read_string(string & rString);

   virtual ~WinStdioFile();
#ifdef _DEBUG
   void dump(dump_context & dumpcontext) const;
#endif
   virtual file_position get_position() const;
   virtual BOOL open(const char * lpszFileName, UINT nOpenFlags, ex1::file_exception_sp * pError = NULL);
   virtual memory_size_t read(void * lpBuf, memory_size_t nCount);
   virtual void write(const void * lpBuf, memory_size_t nCount);
   virtual file_position seek(file_offset lOff, ::ex1::e_seek nFrom);
   virtual void Abort();
   virtual void Flush();
   virtual void close();
   virtual file_size get_length() const;

   // Unsupported APIs
   virtual ex1::file * Duplicate() const;
   virtual void LockRange(file_position dwPos, file_size dwCount);
   virtual void UnlockRange(file_position dwPos, file_size dwCount);
};
