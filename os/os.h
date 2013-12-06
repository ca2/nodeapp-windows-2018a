#pragma once

#define SECURITY_WIN32

#include "base/base/base.h"
#include <shlobj.h>
#include <Security.h>
#include <shlobj.h>

#include "win_exception.h"





#ifdef _WINDOWS_DESKTOP_LIBRARY
    #define CLASS_DECL_win  CLASS_DECL_EXPORT
#else
    #define CLASS_DECL_win  CLASS_DECL_IMPORT
#endif

string get_error_message(DWORD dwError);

//sp(base_application)     win_instantiate_application(sp(base_application) pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

CLASS_DECL_win bool __initialize(bool bDLL = FALSE, DWORD dwVersion = _MFC_VER);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific primitive::memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_win void __set_alloc_stop(LONG lRequestNumber);



#ifdef DEBUG

// Return TRUE if primitive::memory is sane or print out what is wrong
CLASS_DECL_win bool __check_memory();

// Return TRUE if valid primitive::memory block of nBytes
CLASS_DECL_win bool __is_memory_block(const void * p, UINT nBytes, LONG* plRequestNumber = NULL);

#endif

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_win void __throw_last_cleanup();

// other out-of-line helper functions
CLASS_DECL_win void __try_cleanup();


/////////////////////////////////////////////////////////////////////////////
// Global implementation helpers


namespace win
{
   

   class window;


} // namespace win



// window creation hooking
CLASS_DECL_win void hook_window_create(sp(::win::window) pwindow);
CLASS_DECL_win bool unhook_window_create();
CLASS_DECL_win void reset_message_cache();


#include "win1.h"
#include "win_implementation.h"
#include "win_state.h"
#include "win_handle.h"
#include "win_dir.h"
#include "win_folder_watch.h"
#include "win_factory_exchange.h"
#include "win_uac_tools.h"
#include "win_window_draw.h"
#include "win_thread.h"
#include "win_window.h"
#include "win_port_forward.h"
//#include "win_printer.h"

CLASS_DECL_win WNDPROC __get_window_procedure();
#define __window_procedure (*__get_window_procedure())

#define WIN_THREAD(pthread) (dynamic_cast < ::win::thread * > (dynamic_cast < thread * >(pthread)))
#define WIN_WINDOW(pwnd) ((sp(::win::window))(pwnd))


#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "vfw32.lib") 
#pragma comment(lib, "opengl32.lib") 
#pragma comment(lib, "Wtsapi32.lib") 
#pragma comment(lib, "Secur32.lib") 
#pragma comment(lib, "Msimg32.lib") 
#pragma comment(lib, "Psapi.lib") 

CLASS_DECL_win void __trace_message(const char * lpszPrefix, signal_details * pobj);
CLASS_DECL_win void __trace_message(const char * lpszPrefix, LPMSG lpmsg);

CLASS_DECL_win bool __cdecl __is_idle_message(signal_details * pobj);
CLASS_DECL_win bool __cdecl __is_idle_message(MSG* pMsg);


CLASS_DECL_win void __process_window_procedure_exception(::exception::base*, signal_details * pobj);
CLASS_DECL_win void __cdecl __pre_translate_message(signal_details * pobj);

//#include "win_printer.h"

#include "win_application.h"



extern HINSTANCE g_hinstance;