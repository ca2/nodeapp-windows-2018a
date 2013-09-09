#pragma once


namespace win
{


   class stdio_file : 
      virtual public ::win::file,
      virtual public ::file::text_buffer
   {
   public:


      FILE* m_pStream;    // stdio FILE
      // m_hFile from base class is _fileno(m_pStream)



      stdio_file(sp(base_application) papp);
      virtual ~stdio_file();

      virtual void write_string(const char * lpsz);
      virtual char * read_string(char * lpsz, UINT nMax);
      virtual UINT read_string(string & rString);

      
      void dump(dump_context & dumpcontext) const;
      virtual file_position get_position() const;
      virtual bool open(const char * lpszFileName, UINT nOpenFlags);
      virtual ::primitive::memory_size read(void * lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);
      virtual file_position seek(file_offset lOff, ::file::e_seek nFrom);
      virtual void Abort();
      virtual void Flush();
      virtual void close();
      virtual file_size get_length() const;

      // Unsupported APIs
      virtual ::file::buffer_sp  Duplicate() const;
      virtual void LockRange(file_position dwPos, file_size dwCount);
      virtual void UnlockRange(file_position dwPos, file_size dwCount);


   };



} // namespace win



