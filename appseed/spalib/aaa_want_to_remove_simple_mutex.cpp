#include "stdafx.h"


simple_mutex::simple_mutex(bool bInitiallyOwn,const char * lpszName,LPSECURITY_ATTRIBUTES lpsaAttribute)
{

   m_object = NULL;

   m_bAlreadyExists = false;

   m_object = ::CreateMutex(lpsaAttribute,bInitiallyOwn,lpszName);

   DWORD dwLastError = ::GetLastError();


   //char sz[24];

   //sprintf(sz,"%d",dwLastError);

   //MessageBox(NULL,sz,"",0);

   m_bAlreadyExists =  dwLastError == ERROR_ALREADY_EXISTS;

}


simple_mutex::~simple_mutex()
{

   if(m_object != INVALID_HANDLE_VALUE)
   {

      ::CloseHandle(m_object);

   }

}



//simple_mutex::simple_mutex(::aura::application * papp,bool bInitiallyOwn,const char * pstrName,LPSECURITY_ATTRIBUTES lpsaAttribute /* = NULL */):
//sync_object(pstrName)
//{
//
//   static exception::translator * p = NULL;
//
//   if(p == NULL)
//   {
//
//      p = new ::exception::translator();
//      p->attach();
//
//   }
//
//#ifdef _WIN32
//
//   m_object = ::CreateMutexExW(lpsaAttribute,pstrName == NULL ? NULL : (const wchar_t *) ::str::international::utf8_to_unicode(pstrName),bInitiallyOwn ?  CREATE_MUTEX_INITIAL_OWNER : 0,MUTEX_ALL_ACCESS);
//
//   if(m_object == NULL)
//   {
//
//      DWORD dwError1 = ::GetLastError();
//
//
//
//      if(pstrName != NULL)
//      {
//
//         m_object = ::OpenMutexW(SYNCHRONIZE,FALSE,::str::international::utf8_to_unicode(pstrName));
//
//      }
//
//      if(m_object == NULL)
//      {
//
//         DWORD dwError2 = ::GetLastError();
//
//         throw resource_exception(papp);
//
//      }
//
//   }
//
//#elif defined(ANDROID)
//
//   if(pstrName != NULL && *pstrName != '\0')
//   {
//
//      m_pmutex = NULL;
//
//      if(str::begins_ci(pstrName,"Global"))
//      {
//
//         m_strName = ::dir::path("/var/tmp",pstrName);
//
//      }
//      else
//      {
//
//         m_strName = ::dir::path(getenv("HOME"),pstrName);
//
//      }
//
//      int isCreator = 0;
//
//      if((m_psem = sem_open(m_strName,O_CREAT | O_EXCL,0666,1)) != SEM_FAILED)
//      {
//
//         // We got here first
//
//         isCreator = 1;
//
//      }
//      else
//      {
//
//         if(errno != EEXIST)
//            throw resource_exception(get_app());
//
//         // We're not first.  Try again
//
//         m_psem = sem_open(m_strName,0);
//
//         if(m_psem == SEM_FAILED)
//            throw resource_exception(get_app());;
//
//      }
//   }
//   else
//   {
//
//      m_pmutex = new pthread_mutex_t;
//
//
//
//      m_psem = SEM_FAILED;
//
//      pthread_mutexattr_t attr;
//
//      pthread_mutexattr_init(&attr);
//
//      pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
//
//      pthread_mutex_init((pthread_mutex_t *)m_pmutex,&attr);
//
//   }
//
//#else
//
//   if(pstrName != NULL && *pstrName != '\0')
//   {
//
//      m_pmutex = NULL;
//
//
//      if(str::begins_ci(pstrName,"Global"))
//      {
//         m_strName = ::dir::path("/var/tmp",pstrName);
//      }
//      else
//      {
//         m_strName = ::dir::path(getenv("HOME"),pstrName);
//      }
//
//      ::dir::mk(::dir::name(m_strName));
//
//      ::file_put_contents_dup(m_strName,m_strName);
//
//      m_key = ftok(m_strName,0); //Generate a unique key or supply a value
//
//      m_semid = semget(
//         m_key, // a unique identifier to identify semaphore set
//         1,  // number of semaphore in the semaphore set
//         0666 | IPC_CREAT // permissions (rwxrwxrwx) on the new
//         //semaphore set and creation flag
//         );
//
//      if(m_semid < 0)
//         throw resource_exception(get_app());
//
//      semun semctl_arg;
//
//      //set Initial value for the resource
//      semctl_arg.val = 1; //Setting semval to 1
//
//      semctl(m_semid,0,SETVAL,semctl_arg);
//
//
//   }
//   else
//   {
//
//      m_pmutex = new pthread_mutex_t;
//
//      m_key = 0;
//
//      m_semid = -1;
//
//      pthread_mutexattr_t attr;
//
//      pthread_mutexattr_init(&attr);
//
//      pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
//
//      pthread_mutex_init((pthread_mutex_t *)m_pmutex,&attr);
//
//   }
//
//
//#endif
//
//}


#ifdef WINDOWS

//simple_mutex::simple_mutex(::aura::application * papp,const char * pstrName,HANDLE h):
//object(papp),
//sync_object(pstrName)
//{
//
//   m_object = h;
//
//}

#elif defined(ANDROID)

//simple_mutex::simple_mutex(const char * pstrName,sem_t * psem):
//sync_object(pstrName)
//{
//
//   m_strName      = pstrName;
//   m_psem         = psem;
//
//}
//
//#else
//
//simple_mutex::simple_mutex(const char * pstrName,key_t key,int32_t semid):
//sync_object(pstrName)
//{
//
//   m_strName      = pstrName;
//   m_key          = key;
//   m_semid        = semid;
//
//}

#endif




//simple_mutex::~simple_mutex()
//{

//#if defined(ANDROID)
//
//   if(m_psem != SEM_FAILED)
//   {
//
//      sem_wait(m_psem);
//
//   }
//   else
//   {
//
//      pthread_mutex_destroy((pthread_mutex_t *)m_pmutex);
//
//      if(m_pmutex != NULL)
//      {
//         delete (pthread_mutex_t *)m_pmutex;
//      }
//
//   }
//
//
//#elif defined(LINUX) || defined(APPLEOS)
//
//   if(m_semid >= 0)
//   {
//
//      semun ignored_argument;
//
//      semctl(m_semid,1,IPC_RMID,ignored_argument);
//
//   }
//   else
//   {
//
//      pthread_mutex_destroy((pthread_mutex_t *)m_pmutex);
//
//      if(m_pmutex != NULL)
//      {
//         delete (pthread_mutex_t *)m_pmutex;
//      }
//
//   }
//
//#endif

//}


#ifdef ANDROID

//wait_result simple_mutex::wait(const duration & duration)
//{
//
//   uint32_t dwTimeout = duration.lock_duration();
//
//   timespec delay;
//
//   delay.tv_sec = 0;
//   delay.tv_nsec = 1000000;  // 1 milli sec delay
//
//   int32_t irc;
//
//   bool bFirst = true;
//
//   uint32_t start = ::get_tick_count();
//
//   if(m_psem != SEM_FAILED)
//   {
//      //Wait for Zero
//
//      while((dwTimeout == (uint32_t)INFINITE || ::get_tick_count() - start < dwTimeout) || bFirst)
//      {
//
//         bFirst = false;
//
//         int32_t ret = sem_timedwait(m_psem,&delay);
//
//         if(ret < 0)
//         {
//            /* check whether somebody else has the simple_mutex */
//            if(errno == ETIMEDOUT)
//            {
//               /* sleep for delay time */
//               //nanosleep(&delay, NULL);
//            }
//            else
//            {
//               return wait_result(wait_result::Failure);
//            }
//         }
//         else
//         {
//            return wait_result(wait_result::Event0);
//         }
//      }
//
//   }
//   else
//   {
//
//      if(duration.is_pos_infinity())
//      {
//
//         irc = pthread_mutex_lock((pthread_mutex_t *)m_pmutex);
//         if(!irc)
//         {
//            return wait_result(wait_result::Event0);
//         }
//         else
//         {
//            return wait_result(wait_result::Failure);
//
//         }
//
//      }
//      else
//      {
//         while((::get_tick_count() - start < dwTimeout) || bFirst)
//         {
//
//            bFirst = false;
//
//            // Tries to acquire the simple_mutex and access the shared resource,
//            // if success, access the shared resource,
//            // if the shared reosurce already in use, it tries every 1 milli sec
//            // to acquire the resource
//            // if it does not acquire the simple_mutex within 2 secs delay,
//            // then it is considered to be failed
//
//            irc = pthread_mutex_trylock((pthread_mutex_t *)m_pmutex);
//            if(!irc)
//            {
//               return wait_result(wait_result::Event0);
//            }
//            else
//            {
//               // check whether somebody else has the simple_mutex
//               if(irc == EPERM)
//               {
//                  // Yes, Resource already in use so sleep
//                  nanosleep(&delay,NULL);
//               }
//               else
//               {
//                  return wait_result(wait_result::Failure);
//               }
//            }
//         }
//      }
//   }
//
//   return wait_result(wait_result::Timeout);
//
//}
//
//
//bool simple_mutex::lock(const duration & duration)
//{
//
//   wait_result result = wait(duration);
//
//   if(!result.signaled())
//      return false;
//
//   return true;
//
//}


#elif !defined(WINDOWS)

//wait_result simple_mutex::wait(const duration & duration)
//{
//
//   uint32_t dwTimeout = duration.lock_duration();
//
//   timespec delay;
//
//   delay.tv_sec = 0;
//   delay.tv_nsec = 1000000;  // 1 milli sec delay
//
//   int32_t irc;
//
//   bool bFirst = true;
//
//   uint32_t start = ::get_tick_count();
//
//   if(dwTimeout == (uint32_t)INFINITE)
//   {
//      dwTimeout = 30000 + 1977;
//      ((::duration&)  duration) = millis(dwTimeout);
//   }
//
//   if(m_semid >= 0)
//   {
//      //Wait for Zero
//
//      struct sembuf operation[1] ;
//
//
//      while((dwTimeout == (uint32_t)INFINITE || ::get_tick_count() - start < dwTimeout) || bFirst)
//      {
//
//         bFirst = false;
//
//         //Call Wait for Zero with IPC_NOWAIT option,so it will be
//         // non blocking
//
//         operation[0].sem_op = -1; // Wait
//         operation[0].sem_num = 0;
//         operation[0].sem_flg = IPC_NOWAIT;
//
//         int32_t ret = semop(m_semid,operation,1);
//
//         if(ret < 0)
//         {
//            /* check whether somebody else has the simple_mutex */
//            if(errno == EPERM || errno == EAGAIN)
//            {
//               /* sleep for delay time */
//               nanosleep(&delay,NULL);
//            }
//            else
//            {
//               return wait_result(wait_result::Failure);
//            }
//         }
//         else
//         {
//            return wait_result(wait_result::Event0);
//         }
//      }
//
//   }
//   else
//   {
//
//      if(duration.is_pos_infinity())
//      {
//
//         irc = pthread_mutex_lock((pthread_mutex_t *)m_pmutex);
//         if(!irc)
//         {
//            return wait_result(wait_result::Event0);
//         }
//         else
//         {
//            return wait_result(wait_result::Failure);
//
//         }
//
//      }
//      else
//      {
//         while((::get_tick_count() - start < dwTimeout) || bFirst)
//         {
//
//            bFirst = false;
//
//            // Tries to acquire the simple_mutex and access the shared resource,
//            // if success, access the shared resource,
//            // if the shared reosurce already in use, it tries every 1 milli sec
//            // to acquire the resource
//            // if it does not acquire the simple_mutex within 2 secs delay,
//            // then it is considered to be failed
//
//            irc = pthread_mutex_trylock((pthread_mutex_t *)m_pmutex);
//            if(!irc)
//            {
//               return wait_result(wait_result::Event0);
//            }
//            else
//            {
//               // check whether somebody else has the simple_mutex
//               if(irc == EPERM) // owned by own thread !! OK !!
//               {
//                  return wait_result(wait_result::Failure);
//               }
//               else if(irc == EBUSY)
//               {
//                  // Yes, Resource already in use so sleep
//                  nanosleep(&delay,NULL);
//               }
//               else
//               {
//                  return wait_result(wait_result::Failure);
//               }
//            }
//         }
//      }
//   }
//
//   if(dwTimeout == 30000 + 1977 && g_iMutex == 0)
//   {
//      g_iMutex++;
//      printf("HAHAHA");
//   }
//
//   return wait_result(wait_result::Timeout);
//
//}
//
//
//bool simple_mutex::lock(const duration & duration)
//{
//
//   wait_result result = wait(duration);
//
//   if(!result.signaled())
//      return false;
//
//   return true;
//
//}

#endif











bool simple_mutex::unlock()
{

#ifdef _WIN32

   return ::ReleaseMutex(m_object) != FALSE;

#elif defined(ANDROID)


   if(m_psem != SEM_FAILED)
   {

      return sem_post(m_psem) == 0;

   }
   else
   {

      return pthread_mutex_unlock((pthread_mutex_t *)m_pmutex) != 0;

   }


#else

   if(m_semid >= 0)
   {

      struct sembuf operation[1] ;

      operation[0].sem_op = 1; //Release the resource so Wait in process
      // 1 will be triggered
      operation[0].sem_num = 0;
      operation[0].sem_flg = SEM_UNDO;

      //Release semaphore
      semop(m_semid,operation,1);
      union semun sem_union;
      sem_union.val = 0;
      return semctl(m_semid,0,GETVAL,sem_union);

   }
   else
   {

      return pthread_mutex_unlock((pthread_mutex_t *)m_pmutex) != 0;

   }

#endif

}
//
//
//
//simple_mutex * simple_mutex::open_mutex(::aura::application * papp,const char * pstrName)
//{
//
//#ifdef WINDOWS
//
//   HANDLE h = ::OpenMutexW(SYNCHRONIZE,FALSE,::str::international::utf8_to_unicode(pstrName));
//
//   if(h == NULL || h == INVALID_HANDLE_VALUE)
//      return NULL;
//
//   simple_mutex * pmutex = canew(simple_mutex(papp,pstrName,h));
//
//   return pmutex;
//
//#elif defined(ANDROID)
//
//#else
//
//   if(pstrName == NULL || *pstrName == '\0')
//      return NULL;
//
//   string strName = pstrName;
//
//   key_t key = ftok(strName,0); //Generate a unique key or supply a value
//
//   int32_t semid = semget(
//      key, // a unique identifier to identify semaphore set
//      1,  // number of semaphore in the semaphore set
//      0666 // permissions (rwxrwxrwx) on the new
//      //semaphore set and creation flag
//      );
//   if(semid < 0)
//   {
//      return NULL;
//   }
//
//   simple_mutex * pmutex = new simple_mutex(strName,key,semid);
//
//   return pmutex;
//
//#endif
//
//}
//
//
//void wait_until_mutex_does_not_exist(const char * pszName)
//{
//   simple_mutex * pmutex = new simple_mutex(get_thread_app(),false,"Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
//   if(::GetLastError() == ERROR_ALREADY_EXISTS)
//   {
//      while(::GetLastError() == ERROR_ALREADY_EXISTS)
//      {
//         delete pmutex;
//         Sleep(184);
//         pmutex = new simple_mutex(get_thread_app(),false,"Global\\::ca::fontopus::ca2_spa::7807e510-5579-11dd-ae16-0800200c7784");
//      }
//   }
//   delete pmutex;
//}
//
//
//simple_mutex * g_pmutexUiDestroyed = NULL;
//
//
//CLASS_DECL_AURA simple_mutex * get_ui_destroyed_mutex()
//{
//
//   return g_pmutexUiDestroyed;
//
//}
//
//
//
