#include "StdAfx.h"

std::string dir::afterca2()
{
   return ca2().c_str();
}

std::string dir::appdata(const char * lpcsz)
{
   char buf[4096];
   memset(buf, 0, sizeof(buf));
#ifdef WIN32
/*   SHGetSpecialFolderPath(
      NULL,
      buf,
      CSIDL_COMMON_APPDATA,
      FALSE);*/
   strcat(buf, dir::ca2().c_str());
#endif
   std::string str = buf;
   str = path(str.c_str(), "appdata");
   return path(str.c_str(), lpcsz);
}
