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

#ifndef CEF_LIBCEF_DLL_CTOCPP_PRINT_DIALOG_CALLBACK_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_PRINT_DIALOG_CALLBACK_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/cef_print_handler.h"
#include "include/capi/cef_print_handler_capi.h"
#include "ctocpp/ctocpp.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefPrintDialogCallbackCToCpp
    : public CefCToCpp<CefPrintDialogCallbackCToCpp, CefPrintDialogCallback,
        cef_print_dialog_callback_t> {
 public:
  CefPrintDialogCallbackCToCpp();

  // CefPrintDialogCallback methods.
  void Continue(CefRefPtr<CefPrintSettings> settings) OVERRIDE;
  void Cancel() OVERRIDE;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_PRINT_DIALOG_CALLBACK_CTOCPP_H_
