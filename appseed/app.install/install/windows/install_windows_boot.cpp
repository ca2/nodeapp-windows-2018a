//#include "framework.h"


#ifdef WINDOWSEX

/*
int32_t spaboot_registry_register(const char * lpszFile)
{
   reg_delete_tree_dup(HKEY_CLASSES_ROOT, ".spaboot");
   HKEY hkey;
   if(ERROR_SUCCESS != ::RegCreateKey(
	   HKEY_CLASSES_ROOT,
	   ".spaboot",
	   &hkey))
      return 1;
   char strValue[2048];
   strcpy_dup(strValue, "ca2_spaboot_file");
   if(ERROR_SUCCESS != ::RegSetValueEx(hkey, NULL, 0, REG_SZ,
			(LPBYTE)strValue, (lstrlen(strValue))+1)*sizeof(char))
      return 1;
   strcpy_dup(strValue, "application/x-ca2_spaboot_file");
   if(ERROR_SUCCESS != ::RegSetValueEx(hkey, "ContentType", 0, REG_SZ,
			(LPBYTE)strValue, (lstrlen(strValue)+1)*sizeof(char)))
      return 1;
   if(ERROR_SUCCESS != ::RegCloseKey(hkey))
      return 1;
   reg_delete_tree_dup(HKEY_CLASSES_ROOT, "ca2_spaboot_file");
   if(ERROR_SUCCESS != ::RegCreateKey(
	   HKEY_CLASSES_ROOT,
	   "ca2_spaboot_file\\shell",
	   &hkey))
      return 1;
   strcpy_dup(strValue, "open");
   if(ERROR_SUCCESS != ::RegSetValueEx(hkey, NULL, 0, REG_SZ,
			(LPBYTE)strValue, (lstrlen(strValue)+1)*sizeof(char)))
      return 1;
   if(ERROR_SUCCESS != ::RegCloseKey(hkey))
      return 1;
   if(ERROR_SUCCESS != ::RegCreateKey(
	   HKEY_CLASSES_ROOT,
	   "ca2_spaboot_file\\shell\\open",
	   &hkey))
      return 1;
   strcpy_dup(strValue, "&Open");
   if(ERROR_SUCCESS != ::RegSetValueEx(hkey, NULL, 0, REG_SZ,
			(LPBYTE)strValue, (lstrlen(strValue)+1)*sizeof(char)))
      return 1;
   if(ERROR_SUCCESS != ::RegCloseKey(hkey))
      return 1;
   if(ERROR_SUCCESS != ::RegCreateKey(
	   HKEY_CLASSES_ROOT,
	   "ca2_spaboot_file\\shell\\open\\command",
	   &hkey))
      return 1;
   strcpy_dup(strValue, "\"");
   strcat_dup(strValue, lpszFile);
   strcat_dup(strValue, "\" \"%L\"");
   if(ERROR_SUCCESS != ::RegSetValueEx(hkey, NULL, 0, REG_SZ,
			(LPBYTE)strValue, (lstrlen(strValue)+1)*sizeof(char)))
      return 1;
   if(ERROR_SUCCESS != ::RegCloseKey(hkey))
      return 1;

   reg_delete_tree_dup(HKEY_CLASSES_ROOT, "MIME\\DataBase\\Content Type\\application/x-ca2_spaboot_file");
   if(ERROR_SUCCESS != ::RegCreateKey(
	   HKEY_CLASSES_ROOT,
	   "MIME\\DataBase\\Content Type\\application/x-ca2_spaboot_file",
	   &hkey))
      return 1;
   strcpy_dup(strValue, ".spaboot");
   if(ERROR_SUCCESS != ::RegSetValueEx(hkey, "Extension", 0, REG_SZ,
			(LPBYTE)strValue, (lstrlen(strValue)+1)*sizeof(char)))
      return 1;
   if(ERROR_SUCCESS != ::RegCloseKey(hkey))
      return 1;
   return 0;
}
*/









#endif