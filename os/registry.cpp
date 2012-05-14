#include "framework.h"


namespace win
{


   registry::registry()
   {

   }

   registry::~registry()
   {

   }

   bool registry::RegQueryValue(
      HKEY hKey,
      const char * lpSubKey,
      string &str)
   {
      DWORD cbValue;
      DWORD dwType;
      if(ERROR_SUCCESS != ::RegQueryValueEx(hKey, lpSubKey, NULL, &dwType , NULL, &cbValue))
         return false;
      if(dwType != REG_SZ)
         return false;
      LPTSTR lpsz = str.GetBuffer(cbValue);
      if(ERROR_SUCCESS != ::RegQueryValueEx(hKey, lpSubKey, NULL, &dwType, (LPBYTE) lpsz, &cbValue))
      {
         str.ReleaseBuffer();
         return false;
      }
      str.ReleaseBuffer();
      return true;

   }


   registry::Key::Key()
   {
      m_hkey = NULL;
   }

   registry::Key::Key(HKEY hkey, const char * lpcszSubKey, bool bCreate)
   {
      m_hkey = NULL;
      OpenKey(hkey, lpcszSubKey, bCreate);
   }

   registry::Key::~Key()
   {
      if(m_hkey != NULL)
      {
         CloseKey();
      }
   }

   bool registry::Key::OpenKey(HKEY hkey, const char * lpcszSubKey, bool bCreate)
   {
      if(bCreate)
      {
         return ERROR_SUCCESS == ::RegCreateKey(
            hkey,
            lpcszSubKey,
            &m_hkey);

      }
      else
      {
         return ERROR_SUCCESS == ::RegOpenKey(
            hkey,
            lpcszSubKey,
            &m_hkey);
      }
   }

   void registry::Key::CloseKey()
   {
      if(m_hkey != NULL)
      {
         if(ERROR_SUCCESS == ::RegCloseKey(
            m_hkey))
         {
            m_hkey = NULL;
         }
      }
   }

   bool registry::Key::QueryValue(
      const char * lpcszValueName,
      string &str)
   {
      DWORD cbValue;
      DWORD dwType;
      if(ERROR_SUCCESS != ::RegQueryValueEx(m_hkey, lpcszValueName, NULL, &dwType , NULL, &cbValue))
         return false;
      if(dwType != REG_SZ)
         return false;
      LPTSTR lpsz = str.GetBuffer(cbValue);
      if(ERROR_SUCCESS != ::RegQueryValueEx(m_hkey, lpcszValueName, NULL, &dwType, (LPBYTE) lpsz, &cbValue))
      {
         str.ReleaseBuffer();
         return false;
      }
      str.ReleaseBuffer();
      return true;

   }

   bool registry::Key::QueryValue(const char * lpcszValueName, primitive::memory & mem)
   {
      DWORD cbValue;
      DWORD dwType;
      if(ERROR_SUCCESS != ::RegQueryValueEx(m_hkey, lpcszValueName, NULL, &dwType , NULL, &cbValue))
         return false;
      if(dwType != REG_BINARY)
         return false;
      mem.allocate(cbValue);
      if(ERROR_SUCCESS != ::RegQueryValueEx(m_hkey, lpcszValueName, NULL, &dwType, (LPBYTE) mem.get_data(), &cbValue))
      {
         mem.allocate(0);
         return false;
      }
      return true;

   }

   bool registry::Key::SetValue(
      const char * lpcszValueName,
      const char * lpcszValue)
   {
      return ERROR_SUCCESS ==
         RegSetValueEx(m_hkey, lpcszValueName, NULL, REG_SZ,
               (LPBYTE)lpcszValue, (lstrlen(lpcszValue)+1)*sizeof(char));
   }


   bool registry::Key::DeleteValue(
      const char * lpcszValueName)
   {
      return ERROR_SUCCESS ==
         ::RegDeleteValue(m_hkey, (LPTSTR)lpcszValueName);
   }

   bool registry::Key::DeleteKey()
   {
      ASSERT(FALSE);
      // please verify if
      // using NULL for the value parameter
      // deletes the key.
      return ERROR_SUCCESS ==
         ::RegDeleteKey(m_hkey, NULL);
   }

   ::count registry::Key::EnumKey(stringa & stra)
   {
      DWORD dwMaxSubKeyLen;
      RegQueryInfoKey(
         m_hkey,
         NULL,
         NULL,
        NULL,
        NULL,
        &dwMaxSubKeyLen,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);
      int iSize = max(dwMaxSubKeyLen, 1024);
      char *buf = (char *) malloc(iSize);
      int iKey = 0;
      while(::RegEnumKey(m_hkey, iKey, buf, iSize) == ERROR_SUCCESS)
      {
         stra.add(buf);
         iKey++;
      }
      free(buf);
      return iKey;
   }


   registry::Key::operator HKEY()
   {
      return m_hkey;
   }

   ::count registry::Key::EnumValueName(stringa & stra)
   {
      DWORD dwMaxValueNameLen = 16384;
      char * pszBuf = (char *) malloc(dwMaxValueNameLen);
      LONG l;
      DWORD dwIndex = 0;
      DWORD dwLen = dwMaxValueNameLen;
      while(ERROR_SUCCESS == (l = RegEnumValue(
         m_hkey,
         dwIndex,
         pszBuf,
         &dwLen,
         NULL,
         NULL,
         NULL,
         NULL)))
      {
         stra.add(pszBuf);
         dwIndex++;
         dwLen = dwMaxValueNameLen;
      }
      free(pszBuf);
      return stra.get_count();
   }


} // namespace win


