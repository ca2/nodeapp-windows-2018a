#pragma once

class SPALIB_API stra : public std::vector < std::string >
{
public:
   bool spa_insert(const char * psz);
   void add_tokens(const char * psz, const char * pszSeparator);
   std::string implode(const char * pszSeparator) const;
   void trim();
   void remove_empty();
   void add_stra(stra & stra);
   int  remove(const char * psz);

   std::string encode_v16();
   void decode_v16(const char * psz);

};


class strintmap : public std::map < std::string, int >
{
};

class strstrmap : public std::map < std::string, std::string >
{
};
