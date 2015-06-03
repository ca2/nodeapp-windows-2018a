#include "a_spalib.h"

int run_file(const char * pszFile, int nCmdShow);

HANDLE g_hmutexInstall = NULL;

string g_strLastHost;
string g_strCurrentHost;
bool g_bStarterStart = false;
MSG g_msg;
bool g_bAdmin = false;
string g_strId = "";
string g_strPlatform = "";

SPALIB_API bool spa_get_admin()
{
   return g_bAdmin;
}

SPALIB_API void spa_set_admin(bool bSet)
{
   g_bAdmin = bSet;
}

SPALIB_API string spa_get_id()
{
   return g_strId;
}

SPALIB_API void spa_set_id(const char * psz)
{
   g_strId = psz;
}


//SPALIB_API string spa_get_platform()
//{
//}

