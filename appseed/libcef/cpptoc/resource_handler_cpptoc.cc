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

#include "cpptoc/resource_handler_cpptoc.h"
#include "ctocpp/callback_ctocpp.h"
#include "ctocpp/request_ctocpp.h"
#include "ctocpp/response_ctocpp.h"


namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

int CEF_CALLBACK resource_handler_process_request(
    struct _cef_resource_handler_t* self, cef_request_t* request,
    cef_callback_t* callback) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: request; type: refptr_diff
  DCHECK(request);
  if (!request)
    return 0;
  // Verify param: callback; type: refptr_diff
  DCHECK(callback);
  if (!callback)
    return 0;

  // Execute
  bool _retval = CefResourceHandlerCppToC::Get(self)->ProcessRequest(
      CefRequestCToCpp::Wrap(request),
      CefCallbackCToCpp::Wrap(callback));

  // Return type: bool
  return _retval;
}

void CEF_CALLBACK resource_handler_get_response_headers(
    struct _cef_resource_handler_t* self, struct _cef_response_t* response,
    int64* response_length, cef_string_t* redirectUrl) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: response; type: refptr_diff
  DCHECK(response);
  if (!response)
    return;
  // Verify param: response_length; type: simple_byref
  DCHECK(response_length);
  if (!response_length)
    return;
  // Verify param: redirectUrl; type: string_byref
  DCHECK(redirectUrl);
  if (!redirectUrl)
    return;

  // Translate param: response_length; type: simple_byref
  int64 response_lengthVal = response_length?*response_length:0;
  // Translate param: redirectUrl; type: string_byref
  CefString redirectUrlStr(redirectUrl);

  // Execute
  CefResourceHandlerCppToC::Get(self)->GetResponseHeaders(
      CefResponseCToCpp::Wrap(response),
      response_lengthVal,
      redirectUrlStr);

  // Restore param: response_length; type: simple_byref
  if (response_length)
    *response_length = response_lengthVal;
}

int CEF_CALLBACK resource_handler_read_response(
    struct _cef_resource_handler_t* self, void* data_out, int bytes_to_read,
    int* bytes_read, cef_callback_t* callback) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: data_out; type: simple_byaddr
  DCHECK(data_out);
  if (!data_out)
    return 0;
  // Verify param: bytes_read; type: simple_byref
  DCHECK(bytes_read);
  if (!bytes_read)
    return 0;
  // Verify param: callback; type: refptr_diff
  DCHECK(callback);
  if (!callback)
    return 0;

  // Translate param: bytes_read; type: simple_byref
  int bytes_readVal = bytes_read?*bytes_read:0;

  // Execute
  bool _retval = CefResourceHandlerCppToC::Get(self)->ReadResponse(
      data_out,
      bytes_to_read,
      bytes_readVal,
      CefCallbackCToCpp::Wrap(callback));

  // Restore param: bytes_read; type: simple_byref
  if (bytes_read)
    *bytes_read = bytes_readVal;

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK resource_handler_can_get_cookie(
    struct _cef_resource_handler_t* self, const struct _cef_cookie_t* cookie) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: cookie; type: struct_byref_const
  DCHECK(cookie);
  if (!cookie)
    return 0;

  // Translate param: cookie; type: struct_byref_const
  CefCookie cookieObj;
  if (cookie)
    cookieObj.Set(*cookie, false);

  // Execute
  bool _retval = CefResourceHandlerCppToC::Get(self)->CanGetCookie(
      cookieObj);

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK resource_handler_can_set_cookie(
    struct _cef_resource_handler_t* self, const struct _cef_cookie_t* cookie) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: cookie; type: struct_byref_const
  DCHECK(cookie);
  if (!cookie)
    return 0;

  // Translate param: cookie; type: struct_byref_const
  CefCookie cookieObj;
  if (cookie)
    cookieObj.Set(*cookie, false);

  // Execute
  bool _retval = CefResourceHandlerCppToC::Get(self)->CanSetCookie(
      cookieObj);

  // Return type: bool
  return _retval;
}

void CEF_CALLBACK resource_handler_cancel(
    struct _cef_resource_handler_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;

  // Execute
  CefResourceHandlerCppToC::Get(self)->Cancel();
}

}  // namespace


// CONSTRUCTOR - Do not edit by hand.

CefResourceHandlerCppToC::CefResourceHandlerCppToC() {
  GetStruct()->process_request = resource_handler_process_request;
  GetStruct()->get_response_headers = resource_handler_get_response_headers;
  GetStruct()->read_response = resource_handler_read_response;
  GetStruct()->can_get_cookie = resource_handler_can_get_cookie;
  GetStruct()->can_set_cookie = resource_handler_can_set_cookie;
  GetStruct()->cancel = resource_handler_cancel;
}

template<> CefRefPtr<CefResourceHandler> CefCppToC<CefResourceHandlerCppToC,
    CefResourceHandler, cef_resource_handler_t>::UnwrapDerived(
    CefWrapperType type, cef_resource_handler_t* s) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCppToC<CefResourceHandlerCppToC,
    CefResourceHandler, cef_resource_handler_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCppToC<CefResourceHandlerCppToC,
    CefResourceHandler, cef_resource_handler_t>::kWrapperType =
    WT_RESOURCE_HANDLER;
