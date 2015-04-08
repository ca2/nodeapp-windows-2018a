#pragma once


class simple_mutex
{   
public:


   HANDLE      m_object;


   bool        m_bAlreadyExists;


   simple_mutex(bool bInitiallyOwn = FALSE,const char * lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
   {
      
      m_object = ::CreateMutex(lpsaAttribute,bInitiallyOwn,lpszName);
      
      m_bAlreadyExists = ::GetLastError() == ERROR_ALREADY_EXISTS;

   }

   ~simple_mutex()
   {

      ::CloseHandle(m_object);

   }


   bool already_exists()
   {

      return m_bAlreadyExists;

   }

   bool is_ok()
   {

      return m_object != INVALID_HANDLE_VALUE;

   }

   bool lock(DWORD dwTimeOut = INFINITE)
   {
      
      ::WaitForSingleObject(m_object,dwTimeOut);

   }

   bool unlock()
   {

      ::ReleaseMutex(m_object);

   }

};






