#pragma once


class simple_mutex
{   
public:


   HANDLE m_object;


   simple_mutex(bool bInitiallyOwn = FALSE,const char * lpszName = NULL,LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
   virtual ~simple_mutex();


   virtual bool lock(DWORD dwTimeOut = INFINITE);
   virtual bool unlock();



};






