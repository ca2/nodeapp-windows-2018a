#include "StdAfx.h"

#ifndef CCVOTAGUS_CA2_SPA
#define PATH_SEPARATOR "\\"
void get_ca2_module_folder_dup(char * lpszModuleFolder)
{
   char lpszModuleFilePath[MAX_PATH * 8];
   HMODULE hmodule = ::GetModuleHandleA("ca2.dll");
   if(hmodule == NULL)
      hmodule = ::GetModuleHandleA("spalib.dll");
   GetModuleFileName(hmodule, lpszModuleFilePath, sizeof(lpszModuleFilePath));
   LPTSTR lpszModuleFileName;
   GetFullPathName(lpszModuleFilePath, sizeof(lpszModuleFilePath), lpszModuleFolder, &lpszModuleFileName);
   lpszModuleFolder[lpszModuleFileName - lpszModuleFolder] = '\0';
   if(strlen(lpszModuleFolder) > 0)
   {
      if(lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '\\'
      || lpszModuleFolder[strlen(lpszModuleFolder) - 1] == '/')
      {
         lpszModuleFolder[strlen(lpszModuleFolder) - 1] = '\0';
      }
   }
}


string dir_beforeca2()
{
   char lpszModuleFolder[MAX_PATH * 8];
   get_ca2_module_folder_dup(lpszModuleFolder);
   char * psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip x86 or x64...
   psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip stage
   psz = strrchr(lpszModuleFolder, '\\');
   if(psz == NULL)
      return lpszModuleFolder;
   *psz = '\0'; // strip ca2
   // now, lpszModuleFolder should be beforeca2
   return lpszModuleFolder;
}
string dir_path(const char * path1, const char * path2)
{
   string str1(path1);
   if(str1.Right(1) != PATH_SEPARATOR)
   {
      str1 = str1 + PATH_SEPARATOR;
   }
   if(path2 == NULL)
   {
      return str1;
   }
   else
   {
      return str1 + path2;
   }
}

string dir_ca2(const char * path = NULL)
{
   if(path == NULL)
      return dir_path(dir_beforeca2(), "ca2");
   else
      return dir_path(dir_ca2(), path);
}

#endif

machine_event::machine_event()
{
   m_hmutex = NULL;
   m_bInitialized = false;
   initialize();
}

machine_event::~machine_event()
{
}

bool machine_event::initialize()
{
   if(m_bInitialized)
      return true;
   SECURITY_ATTRIBUTES MutexAttributes;
   ZeroMemory( &MutexAttributes, sizeof(MutexAttributes) );
   MutexAttributes.nLength = sizeof( MutexAttributes );
   MutexAttributes.bInheritHandle = FALSE; // object uninheritable
   // declare and initialize a security descriptor
   SECURITY_DESCRIPTOR SD;
   BOOL bInitOk = InitializeSecurityDescriptor(
                     &SD,
                     SECURITY_DESCRIPTOR_REVISION );
   if ( bInitOk )
   {
      // give the security descriptor a Null Dacl
      // done using the  "TRUE, (PACL)NULL" here
      BOOL bSetOk = SetSecurityDescriptorDacl( &SD,
                                            TRUE,
                                            (PACL)NULL,
                                            FALSE );
      if ( bSetOk )
      {
         // Make the security attributes point
         // to the security descriptor
         MutexAttributes.lpSecurityDescriptor = &SD;
         m_hmutex = ::CreateMutex(&MutexAttributes, FALSE, "Global\\ca2::fontopus::machine_event::7807e510-5579-11dd-ae16-0800200c7784");
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   m_bInitialized = true;
   return true;
}

bool machine_event::read(machine_event_data * pdata)
{
#ifdef CCVOTAGUS_CA2_SPA
   FILE * f = fopen((dir::ca2("machine\\event\\machine_event.bin")).c_str(), "rb");
#else
   FILE * f = fopen((dir_ca2("machine\\event\\machine_event.bin")), "rb");
#endif
   if(f == NULL)
   {
      memset(pdata, 0, sizeof(machine_event_data));
      return false;
   }
   else
   {
      fseek(f, 0, SEEK_SET);
      pdata->read(f);
      fclose(f);
      return true;
   }
}

bool machine_event::write(machine_event_data * pdata)
{
#ifdef CCVOTAGUS_CA2_SPA
   if(!dir::mk(dir::ca2("machine\\event\\").c_str()))
      return false;
#endif
#ifdef CCVOTAGUS_CA2_SPA
   FILE * f = fopen(dir::ca2("machine\\event\\machine_event.bin").c_str(), "wb+");
#else
   FILE * f = fopen(dir_ca2("machine\\event\\machine_event.bin"), "wb+");
#endif
   if(f != NULL)
   {
      fseek(f, 0, SEEK_SET);
      pdata->write(f);
      fclose(f);
      return true;
   }
   return false;
}


