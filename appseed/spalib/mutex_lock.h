#pragma once



class  mutex_lock
{
public:


   HANDLE m_hmutex;
   inline mutex_lock(HANDLE hmutex)
   {
      m_hmutex  = hmutex; 
      if(m_hmutex != NULL)
      {
         WaitForSingleObject(m_hmutex,INFINITE);
      }
   }
   inline ~mutex_lock()
   {
      if(m_hmutex != NULL)
      {
         ReleaseMutex(m_hmutex);
      }

   }


};
