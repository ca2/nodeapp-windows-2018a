#pragma once

class CLASS_DECL_win FileFind : public ::radix::object
{
public:
   FileFind();
   virtual ~FileFind();

// Attributes
public:
   int64_t get_length() const;
   virtual string GetFileName() const;
   virtual string GetFilePath() const;
   virtual string GetFileTitle() const;
   virtual string GetFileURL() const;
   virtual string GetRoot() const;

   virtual bool GetLastWriteTime(FILETIME* pTimeStamp) const;
   virtual bool GetLastAccessTime(FILETIME* pTimeStamp) const;
   virtual bool GetCreationTime(FILETIME* pTimeStamp) const;
   virtual bool GetLastWriteTime(::datetime::time& refTime) const;
   virtual bool GetLastAccessTime(::datetime::time& refTime) const;
   virtual bool GetCreationTime(::datetime::time& refTime) const;

   virtual bool MatchesMask(DWORD dwMask) const;

   virtual bool IsDots() const;
   // these aren't virtual because they all use MatchesMask(), which is
   bool IsReadOnly() const;
   bool IsDirectory() const;
   bool IsCompressed() const;
   bool IsSystem() const;
   bool IsHidden() const;
   bool IsTemporary() const;
   bool IsNormal() const;
   bool IsArchived() const;

// Operations
   void close();
   virtual bool FindFile(const char * pstrName = NULL, DWORD dwUnused = 0);
   virtual bool FindNextFile();

protected:
   virtual void CloseContext();

// ementation
protected:
   WIN32_FIND_DATAW * m_pFoundInfo;
   WIN32_FIND_DATAW * m_pNextInfo;
   HANDLE m_hContext;
   bool m_bGotLast;
   string m_strRoot;
   char m_chDirSeparator;     // not '\\' for Internet classes

public:

   void dump(dump_context & dumpcontext) const;
   void assert_valid() const;

   
};

// FileFind
inline bool FileFind::IsReadOnly() const
   { return MatchesMask(FILE_ATTRIBUTE_READONLY); }
inline bool FileFind::IsDirectory() const
   { return MatchesMask(FILE_ATTRIBUTE_DIRECTORY); }
inline bool FileFind::IsCompressed() const
   { return MatchesMask(FILE_ATTRIBUTE_COMPRESSED); }
inline bool FileFind::IsSystem() const
   { return MatchesMask(FILE_ATTRIBUTE_SYSTEM); }
inline bool FileFind::IsHidden() const
   { return MatchesMask(FILE_ATTRIBUTE_HIDDEN); }
inline bool FileFind::IsTemporary() const
   { return MatchesMask(FILE_ATTRIBUTE_TEMPORARY); }
inline bool FileFind::IsNormal() const
   { return MatchesMask(FILE_ATTRIBUTE_NORMAL); }
inline bool FileFind::IsArchived() const
   { return MatchesMask(FILE_ATTRIBUTE_ARCHIVE); }
