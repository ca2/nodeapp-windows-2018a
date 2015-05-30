#pragma once


class simple_mutex
{
public:


   HANDLE      m_object;


   bool        m_bAlreadyExists;


   simple_mutex(bool bInitiallyOwn = FALSE,const char * lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
   {

      m_object = INVALID_HANDLE_VALUE;

      m_bAlreadyExists = false;

      m_object = ::CreateMutex(lpsaAttribute,bInitiallyOwn,lpszName);

      DWORD dwLastError = ::GetLastError();


      //char sz[24];

      //sprintf(sz,"%d",dwLastError);

      //MessageBox(NULL,sz,"",0);

      m_bAlreadyExists =  dwLastError == ERROR_ALREADY_EXISTS;

   }


   ~simple_mutex()
   {

      if(m_object != INVALID_HANDLE_VALUE)
      {

         ::CloseHandle(m_object);

      }

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
      
      return ::WaitForSingleObject(m_object,dwTimeOut) == WAIT_OBJECT_0;
   }

   bool unlock()
   {

      return ::ReleaseMutex(m_object) != FALSE;

   }

};






