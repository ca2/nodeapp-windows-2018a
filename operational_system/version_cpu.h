// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// afxv_cpu.h - target version/configuration control for various CPUs

/////////////////////////////////////////////////////////////////////////////

#ifdef _MIPS_
// specific overrides for MIPS...
#define ___PACKING    8       // default MIPS alignment (required)
#endif //_MIPS_

/////////////////////////////////////////////////////////////////////////////

#ifdef _PPC_
// specific overrides for PPC...
#define ___PACKING    8       // default PPC alignment (required)
#endif //_PPC_

/////////////////////////////////////////////////////////////////////////////

#ifdef _IA64_
// specific overrides for IA64...
#define ___PACKING    8
#define _SHADOW_DOUBLES 8
#endif //_IA64_

/////////////////////////////////////////////////////////////////////////////

#ifdef AMD64
// specific overrides for AMD64...
#define ___PACKING    8
#endif //AMD64
