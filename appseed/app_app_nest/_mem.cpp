#include "StdAfx.h"


#ifdef _X86_
#pragma function(memcmp)
int memcmp(const void * m1, const void * m2, size_t s)
{
   return memcmp_dup((void *) m1, (void *) m2, s);
}
#pragma function(memset)
void * memset( void *_dst, int _val, size_t _sz )
{
   return memset_dup(_dst, _val, _sz);
}
#else
/*void * memset(void * p, int uch, size_t iSize)
{
   return memset_dup(dst, src, iSize);
}
*/
#pragma function(memcmp)
int memcmp(const void * m1, const void * m2, size_t s)
{
   return memcmp_dup((void *) m1, (void *) m2, s);
}
#pragma function(memset)
void * memset( void *_dst, int _val, size_t _sz )
{
    unsigned char * puch = (unsigned char * ) _dst;
   while(_sz > 0)
   {
      *puch = static_cast < unsigned char > (_val);
      puch++;
      _sz--;
   }
   return _dst;
}

/*#pragma function(calloc)
void * calloc(  size_t sz, size_t sz2 )
{
   return ca2_alloc(sz * sz2);
}*/

#pragma function(memcpy)
void * memcpy(void * dst, const void * src, size_t iSize)
{
   return memcpy_dup(dst, src, iSize);
}

#endif
