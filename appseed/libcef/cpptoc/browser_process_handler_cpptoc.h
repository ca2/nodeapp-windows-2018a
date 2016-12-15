// Copyright (c) 2016 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#ifndef CEF_LIBCEF_DLL_CPPTOC_BROWSER_PROCESS_HANDLER_CPPTOC_H_
#define CEF_LIBCEF_DLL_CPPTOC_BROWSER_PROCESS_HANDLER_CPPTOC_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/cef_browser_process_handler.h"
#include "include/capi/cef_browser_process_handler_capi.h"
#include "cpptoc/cpptoc.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefBrowserProcessHandlerCppToC
    : public CefCppToC<CefBrowserProcessHandlerCppToC, CefBrowserProcessHandler,
        cef_browser_process_handler_t> {
 public:
  CefBrowserProcessHandlerCppToC();
};

#endif  // CEF_LIBCEF_DLL_CPPTOC_BROWSER_PROCESS_HANDLER_CPPTOC_H_
