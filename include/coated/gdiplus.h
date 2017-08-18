#ifdef WINDOWSEX

#undef new

#define min MIN
#define max MAX

#include <gdiplus.h>

#define new AURA_NEW

#undef min
#undef max

using namespace Gdiplus;

#endif





