#include "installer.h"


const char * calc_id();


int installer(const char * param);
int APIENTRY ca2_cube_install(const char * pszId);

int spaboot_start()
{

   char szSpabootInstall[MAX_PATH * 4];
   int iRetry = 0;
   while(iRetry < 8)
   {

      if(!get_temp_file_name_dup(szSpabootInstall, sizeof(szSpabootInstall), "app-install", "exe"))
         return -1;

      if(is_file_ok(szSpabootInstall, "app-install.exe"))
      {
         if(ms_download_dup("veriterse.net", "/spa?download=app-install.exe&authnone", szSpabootInstall))
         {
            if(is_file_ok(szSpabootInstall, "app-install.exe"))
            {
               break;
            }
         }
      }



   }
   if(iRetry >= 8)
      return -1;

   iRetry = 0;
   while(iRetry < 5)
   {
      update_spa_installed();
      if(is_spa_installed())
      {  
         break;
      }
      installer("install_spaboot");
      iRetry++;
   }
   if(!is_spa_installed())
   {  
      return 1;
   }

   iRetry = 0;
   while(iRetry < 5)
   {
      update_spa_updated();
      if(is_spa_updated())
      {  
         break;
      }
      installer("install_spaboot");
      iRetry++;
   }
   if(!is_spa_updated())
   {  
      return 1;
   }

   const char * id = calc_id();
   if(id == NULL)
      id = "spaboot_install";

   iRetry = 0;
   while(iRetry < 5)
   {
      update_ca2_installed();
      if(is_ca2_installed())
      {  
         break;
      }
      ca2_cube_install(id);
      iRetry++;
   }
   if(!is_ca2_installed())
   {  
      return 1;
   }

   iRetry = 0;
   while(iRetry < 5)
   {
      update_ca2_updated();
      if(is_ca2_updated())
      {  
         break;
      }
      ca2_cube_install(id);
      iRetry++;
   }
   if(!is_ca2_updated())
   {  
      return 1;
   }

   if(stricmp_dup(id, "spaboot_install"))
   {
      cube_run(id);
   }



   return 0;

}



const char * calc_id()
{

   char szModulePath[MAX_PATH * 3];
   ::GetModuleFileNameA((HINSTANCE) NULL, szModulePath, sizeof(szModulePath));
   return read_resource_as_string_dup(::GetModuleHandleA(szModulePath), 1984, "application_id");

}




int installer(const char * param)
{
   char szModulePath[MAX_PATH * 3];
   ::GetModuleFileNameA((HINSTANCE) ::GetModuleHandleA("npca2.dll"), szModulePath, sizeof(szModulePath));

   char path[MAX_PATH * 3];
   char * file = NULL;
   ::GetFullPathNameA(szModulePath, sizeof(path), path, &file);
   file[0] = '\0';

   strcat_dup(path, "app-install.exe");

   installer_call_sync(path, param);
   return 0;

}

HANDLE g_hmutexBoot;


int APIENTRY ca2_cube_install(const char * pszId)
{

   g_hmutexBoot = ::CreateMutex(NULL, FALSE, "Global\\ca2::fontopus::ccvotagus_ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
   if(::GetLastError() == ERROR_ALREADY_EXISTS)
   {
      while(::GetLastError() == ERROR_ALREADY_EXISTS)
      {
         ::CloseHandle(g_hmutexBoot);
         Sleep(1984 + 1977);
         g_hmutexBoot = ::CreateMutex(NULL, FALSE, "Global\\ca2::fontopus::ccvotagus_ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
      }
   }
   ::CloseHandle(g_hmutexBoot);

   char * szParameters = (char *) malloc(strlen_dup(pszId) + 256);
   strcpy_dup(szParameters, "synch_spaadmin:");
   strcat_dup(szParameters, "starter_start=");
   strcat_dup(szParameters, pszId);
   strcat_dup(szParameters, " background");

   installer(szParameters);

   free(szParameters);

   return 0;
}


