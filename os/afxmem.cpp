#include "StdAfx.h"




_PNH CLASS_DECL_VMSWIN AfxSetNewHandler(_PNH pfnNewHandler)
{
   AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
   _PNH pfnOldHandler = pState->m_pfnNewHandler;
   pState->m_pfnNewHandler = pfnNewHandler;
   return pfnOldHandler;
}



/////////////////////////////////////////////////////////////////////////////
// stop on a specific primitive::memory request

// Obsolete API
/*
void CLASS_DECL_VMSWIN AfxSetAllocStop(LONG lRequestNumber)
{
   _CrtSetBreakAlloc(lRequestNumber);
}
*/
BOOL CLASS_DECL_VMSWIN AfxCheckMemory()
  // check all of primitive::memory (look for primitive::memory tromps)
{
   return _CrtCheckMemory();
}
/*
// -- true if block of exact size, allocated on the heap
// -- set *plRequestNumber to request number (or 0)
BOOL CLASS_DECL_VMSWIN AfxIsMemoryBlock(const void * pData, UINT nBytes,
      LONG* plRequestNumber)
{
   return _CrtIsMemoryBlock(pData, nBytes, plRequestNumber, NULL, NULL);
}

*/