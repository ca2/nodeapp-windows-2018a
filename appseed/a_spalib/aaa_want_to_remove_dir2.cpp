#include "StdAfx.h"

string dir::afterca2()
{
   return ca2().c_str();
}

string dir::appdata(const char * lpcsz)
{
   char buf[4096];
   memset(buf, 0, sizeof(buf));
#ifdef WIN32
/*   SHGetSpecialFolderPath(
      NULL,
      buf,
      CSIDL_COMMON_APPDATA,
      FALSE);*/
   strcat(buf, dir::element().c_str());
#endif
   string str = buf;
   str = path(str.c_str(), "appdata");
   return path(str.c_str(), lpcsz);
}
