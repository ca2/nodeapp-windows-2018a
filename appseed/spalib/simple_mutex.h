#pragma once


class simple_mutex
{
public:


   HANDLE      m_object;


   bool        m_bAlreadyExists;


   simple_mutex()
   {

      m_object = INVALID_HANDLE_VALUE;

      m_bAlreadyExists = false;

   }

   ~simple_mutex()
   {

      if(m_object != INVALID_HANDLE_VALUE)
      {

         ::CloseHandle(m_object);

      }

   }

   bool create(bool bInitiallyOwn = FALSE,const char * lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL)
   {

      m_object = INVALID_HANDLE_VALUE;

      m_bAlreadyExists = false;

      m_object = ::CreateMutex(lpsaAttribute,bInitiallyOwn,lpszName);

      DWORD dwLastError = ::GetLastError();


      //char sz[24];

      //sprintf(sz,"%d",dwLastError);

      //MessageBox(NULL,sz,"",0);

      m_bAlreadyExists =  dwLastError == ERROR_ALREADY_EXISTS;

      return true;

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






