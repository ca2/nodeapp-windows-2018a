//#include "framework.h"


#ifdef WINDOWSEX

//#include <WinSvc.h>


int32_t stop_service()
{
   SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

	if (hdlSCM == 0) return ::GetLastError();
 
   SC_HANDLE hdlServ = ::OpenService(
		hdlSCM,                    // SCManager database 
		"ca2_WinService_Spa_stage",               // name of service 
		SC_MANAGER_ALL_ACCESS);                     // no password 
 
   DWORD Ret = 0;
   if (!hdlServ) 
   {
      Ret = ::GetLastError();
      return Ret;
   }
   SERVICE_STATUS ss;
   memset_dup(&ss, 0, sizeof(ss));
   ::ControlService(hdlServ, SERVICE_CONTROL_STOP, &ss);

   CloseServiceHandle(hdlServ);

   return Ret;
}


int32_t remove_service()
{
	SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

	if (hdlSCM == 0) return ::GetLastError();
 
   SC_HANDLE hdlServ = ::OpenService(
		hdlSCM,                    // SCManager database 
		"ca2_WinService_Spa_stage",               // name of service 
		SC_MANAGER_ALL_ACCESS);                     // no password 
 
   DWORD Ret = 0;
   if (!hdlServ)
   {
      Ret = ::GetLastError();
      return Ret;
   }

   ::DeleteService(hdlServ);

   CloseServiceHandle(hdlServ);

/*   STARTUPINFO si;
   PROCESS_INFORMATION pi;
   LPSTR lpsz = strdup_dup(("sc delete ca2_WinService_Spa_" + g_strVersionShift));
   if(!::CreateProcess(NULL, lpsz, 
      NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
      return 1;
   _ca_free(lpsz, 0);*/

   return Ret;
}





#endif // WINDOWSEX