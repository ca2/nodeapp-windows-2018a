#pragma once


class simple_mutex
{
public:


   HANDLE      m_object;


   bool        m_bAlreadyExists;


   simple_mutex(bool bInitiallyOwn = FALSE,const char * lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);


   ~simple_mutex();


   bool already_exists()
   {

      return m_bAlreadyExists;

   }

   bool is_ok()
   {

      return m_object != NULL;

   }

   bool lock(DWORD dwTimeOut = INFINITE)
   {
      
      return ::WaitForSingleObject(m_object,dwTimeOut) == WAIT_OBJECT_0;
   }

   bool unlock();
   //{

   //   return ::ReleaseMutex(m_object) != FALSE;

   //}

};






