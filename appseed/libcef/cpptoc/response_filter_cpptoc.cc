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

#include "cpptoc/response_filter_cpptoc.h"


namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

int CEF_CALLBACK response_filter_init_filter(
    struct _cef_response_filter_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;

  // Execute
  bool _retval = CefResponseFilterCppToC::Get(self)->InitFilter();

  // Return type: bool
  return _retval;
}

cef_response_filter_status_t CEF_CALLBACK response_filter_filter(
    struct _cef_response_filter_t* self, void* data_in, size_t data_in_size,
    size_t* data_in_read, void* data_out, size_t data_out_size,
    size_t* data_out_written) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return RESPONSE_FILTER_ERROR;
  // Verify param: data_in_read; type: simple_byref
  DCHECK(data_in_read);
  if (!data_in_read)
    return RESPONSE_FILTER_ERROR;
  // Verify param: data_out; type: simple_byaddr
  DCHECK(data_out);
  if (!data_out)
    return RESPONSE_FILTER_ERROR;
  // Verify param: data_out_written; type: simple_byref
  DCHECK(data_out_written);
  if (!data_out_written)
    return RESPONSE_FILTER_ERROR;
  // Unverified params: data_in

  // Translate param: data_in_read; type: simple_byref
  size_t data_in_readVal = data_in_read?*data_in_read:0;
  // Translate param: data_out_written; type: simple_byref
  size_t data_out_writtenVal = data_out_written?*data_out_written:0;

  // Execute
  cef_response_filter_status_t _retval = CefResponseFilterCppToC::Get(
      self)->Filter(
      data_in,
      data_in_size,
      data_in_readVal,
      data_out,
      data_out_size,
      data_out_writtenVal);

  // Restore param: data_in_read; type: simple_byref
  if (data_in_read)
    *data_in_read = data_in_readVal;
  // Restore param: data_out_written; type: simple_byref
  if (data_out_written)
    *data_out_written = data_out_writtenVal;

  // Return type: simple
  return _retval;
}

}  // namespace


// CONSTRUCTOR - Do not edit by hand.

CefResponseFilterCppToC::CefResponseFilterCppToC() {
  GetStruct()->init_filter = response_filter_init_filter;
  GetStruct()->filter = response_filter_filter;
}

template<> CefRefPtr<CefResponseFilter> CefCppToC<CefResponseFilterCppToC,
    CefResponseFilter, cef_response_filter_t>::UnwrapDerived(
    CefWrapperType type, cef_response_filter_t* s) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCppToC<CefResponseFilterCppToC,
    CefResponseFilter, cef_response_filter_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCppToC<CefResponseFilterCppToC, CefResponseFilter,
    cef_response_filter_t>::kWrapperType = WT_RESPONSE_FILTER;
