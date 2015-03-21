#include "StdAfx.h"

void stringa::trim()
{
   for(unsigned int ui = 0; ui < size(); ui++)
   {
      str_trim(at(ui));
   }
}

void stringa::remove_empty()
{
   for(unsigned int ui = 0; ui < size();)
   {
      if(at(ui).length() == 0)
      {
         erase(begin() + ui);
      }
      else
      {
         ui++;
      }
   }
}

int  stringa::remove(const char * psz)
{
   int iCount = 0;
   for(unsigned int ui = 0; ui < size();)
   {
      if(at(ui) == psz)
      {
         erase(begin() + ui);
         iCount++;
      }
      else
      {
         ui++;
      }
   }
   return iCount;
}


void stringa::add_stra(stringa & stringa)
{
   for(unsigned int ui = 0; ui < stringa.size(); ui++)
   {
      push_back(stringa.at(ui));
   }
}


void stringa::add_tokens(const char * psz, const char * pszSeparator)
{
	const char * pszPosBeg = psz;
	const char * pszPosEnd;
	int iLenSeparator = strlen(pszSeparator);
	std::string str;
	while(pszPosEnd = strstr(pszPosBeg, pszSeparator))
	{
		str.assign(pszPosBeg, pszPosEnd - pszPosBeg);
		insert(end(), str);
		pszPosBeg = pszPosEnd + iLenSeparator;
	}
	if(pszPosBeg - psz == strlen(psz))
		return;
	str.assign(pszPosBeg);
	insert(end(), str);
}


std::string stringa::encode_v16()
{
   std::string strEncode;
   for(unsigned int ui = 0; ui < size(); ui++)
   {
      std::string & str = at(ui);
      for(unsigned int uj = 0; uj < str.length(); uj++)
      {
         char sz[3];
         sprintf(sz, "%02X", (unsigned char) str[uj]);
         strEncode += sz;
      }
      strEncode += "00";
   }
   return strEncode;
}


void stringa::decode_v16(const char * psz)
{
   if(psz == NULL)
      return;
   std::string str;
   while(*psz != '\0')
   {
      psz++;
      if(*psz == '\0')
         break;
      char sz[3];
      sz[0] = psz[-1];
      sz[1] = psz[0];
      sz[2] = '\0';
      char * pszEnd;
      int iConversion = ::strtol(sz, &pszEnd, 16);
      char ch = static_cast < char > (iConversion);
      if(ch == '\0')
      {
         push_back(str);
         str.clear();
      }
      else
      {
         sz[0] = ch;
         sz[1] = '\0';
         str += sz;
      }
      psz++;
   }

}


void stringa::add(const char * psz)
{

   add(string(psz));

}


void stringa::add(const string & str)
{
   
   push_back(str);

}