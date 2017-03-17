#include "framework.h"


int run_file(const char * pszFile, int nCmdShow);

HANDLE g_hmutexInstall = NULL;

bool g_bStarterStart = false;
MSG g_msg;
bool g_bAdmin = false;

SPALIB_API bool spa_get_admin()
{
   return g_bAdmin;
}

SPALIB_API void spa_set_admin(bool bSet)
{
   g_bAdmin = bSet;
}


//SPALIB_API string spa_get_platform()
//{
//}

