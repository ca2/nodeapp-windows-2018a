#pragma once


//using namespace std;


class SPALIB_API stringa : public std::vector < string >
{
public:

   typedef string BASE_TYPE;

   bool spa_insert(const char * psz);
   void add_tokens(const char * psz, const char * pszSeparator);
   string implode(const char * pszSeparator) const;
   void trim();
   void remove_empty();
   void add_stra(stringa & stringa);
   int  remove(const char * psz);
   void add(const char * psz);
   void add(const string & str);
   string encode_v16();
   void decode_v16(const char * psz);

};


class strintmap : public std::map < string, int >
{
};

class strstrmap : public std::map < string, string >
{
};
