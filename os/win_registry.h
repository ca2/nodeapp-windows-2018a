#pragma once


namespace win
{


   class CLASS_DECL_win registry :
      virtual public ::radix::object
   {
   public:


      class CLASS_DECL_win Key
      {
      public:


         HKEY m_hkey;


         Key();
         Key(HKEY hkey, const char * lpcszSubKey, bool bCreate);
         virtual ~Key();
      

         bool OpenKey(HKEY hkey, const char * lpcszSubKey, bool bCreate);
         void CloseKey();
         
         bool QueryValue(const char * lpcszValueName, string &strVAlue);

         bool QueryValue(const char * lpcszValueName, primitive::memory & mem);

         bool SetValue(const char * lpcszValueName, const char * lpcszValue);

         bool DeleteValue(const char * lpcszValueName);

         bool DeleteKey();

         ::count EnumKey(stringa & stra);

         ::count EnumValueName(stringa & stra);
         
         operator HKEY();

      };

      registry();
      virtual ~registry();
      /*bool RegQueryValue(
         HKEY hKey,       // handle to key to query
         const wchar_t * lpSubKey,
                      // name of subkey to query
         CWStr &str);*/

      bool RegQueryValue(
         HKEY hKey,       // handle to key to query
         const char * lpSubKey,
                      // name of subkey to query
         string &str);


      
   };


} // namespace win


