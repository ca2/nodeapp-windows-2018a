#include "StdAfx.h"
#include <stddef.h>



/////////////////////////////////////////////////////////////////////////////
// no_track_object

#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
#undef new
void * PASCAL no_track_object::operator new(size_t nSize, const char *, int)
{
   return no_track_object::operator new(nSize);
}
#define new DEBUG_NEW
void PASCAL no_track_object::operator delete(void * pObject, const char *, int)
{
   if (pObject != NULL)
      ::LocalFree(pObject);
}
#endif

#undef new
void * PASCAL no_track_object::operator new(size_t nSize)
{
   void * p = ::LocalAlloc(LPTR, nSize);
   if (p == NULL)
      AfxThrowMemoryException();
   return p;
}
#define new DEBUG_NEW

void PASCAL no_track_object::operator delete(void * p)
{
   if (p != NULL)
      ::LocalFree(p);
}


struct thread_data : public no_track_object
{
   thread_data* pNext; // required to be member of simple_list
   int nCount;         // current size of pData
   LPVOID* pData;      // actual thread local data (indexed by nSlot)
};

struct slot_data
{
   DWORD dwFlags;      // slot flags (allocated/not allocated)
   HINSTANCE hInst;    // module which owns this slot
};

// flags used for slot_data::dwFlags above
#define SLOT_USED   0x01    // slot is allocated


thread_local_storage::thread_local_storage()
{
   m_tlsIndex = TlsAlloc();
   if (m_tlsIndex == (DWORD)-1)
      AfxThrowMemoryException();
}


thread_local_storage::~thread_local_storage()
{
   if (m_tlsIndex != (DWORD)-1)
   {
      TlsFree(m_tlsIndex);
      DEBUG_ONLY(m_tlsIndex = (DWORD)-1);
   }
}

void thread_local_storage::delete_data()
{
   get_slot_data()->delete_data();
}

#undef new

thread_slot_data * thread_local_storage::get_slot_data()
{
   thread_slot_data * pdata = (thread_slot_data *) TlsGetValue(m_tlsIndex);
   if(pdata == NULL)
   {
      void * p = malloc(sizeof(thread_slot_data));
      pdata = ::new(p) thread_slot_data();
      TlsSetValue(m_tlsIndex, (LPVOID) pdata);
   }
   return pdata;
}
#define new DEBUG_NEW


thread_slot_data::thread_slot_data()
{
   memset(m_pa, 0, sizeof(m_pa));
}

void thread_slot_data::delete_data()
{
   for(int i = 0; i < 1024; i++)
   {
      if(m_pa[i] != NULL)
      {
         delete m_pa[i];
      }
   }
   free(this);
}


/////////////////////////////////////////////////////////////////////////////
// thread_local_object


/////////////////////////////////////////////////////////////////////////////
// CProcessLocalData

no_track_object* process_local_object::get_data(
   no_track_object* ( * pfnCreateObject)())
{
   if (m_pObject == NULL)
   {
      AfxLockGlobals(CRIT_PROCESSLOCAL);
      try
      {
         if (m_pObject == NULL)
            m_pObject = (*pfnCreateObject)();
      }
      catch(base_exception * pe)
      {
         AfxUnlockGlobals(CRIT_PROCESSLOCAL);
         ::ca::rethrow(pe);
      }
      
      AfxUnlockGlobals(CRIT_PROCESSLOCAL);
   }
   return m_pObject;
}

process_local_object::~process_local_object()
{
   if (m_pObject != NULL)
      delete m_pObject;
}

/////////////////////////////////////////////////////////////////////////////
// Init/Term for thread/process local data

/*void CLASS_DECL_VMSWIN AfxInitLocalData(HINSTANCE hInst)
{
   if (_afxThreadData != NULL)
      _afxThreadData->AssignInstance(hInst);
}

void CLASS_DECL_VMSWIN AfxTermLocalData(HINSTANCE hInst, BOOL bAll)
{
   if (_afxThreadData != NULL)
      _afxThreadData->DeleteValues(hInst, bAll);
}*/

// This reference count is needed to support Win32s, such that the
// thread-local and process-local data is not destroyed prematurely.
// It is basically a reference count of the number of processes that
// have attached to the ca2 API DLL.

AFX_STATIC_DATA long _afxTlsRef = 0;

void CLASS_DECL_VMSWIN AfxTlsAddRef()
{
   ++_afxTlsRef;
}

void CLASS_DECL_VMSWIN AfxTlsRelease()
{
   if (_afxTlsRef == 0 || --_afxTlsRef == 0)
   {
      if (_afxThreadData != NULL)
      {
         _afxThreadData->~thread_local_storage();
         _afxThreadData = NULL;
      }
   }
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// thread_slot_data

// global _afxThreadData used to allocate thread local indexes
BYTE __afxThreadData[sizeof(thread_local_storage)];
thread_local_storage * _afxThreadData;


