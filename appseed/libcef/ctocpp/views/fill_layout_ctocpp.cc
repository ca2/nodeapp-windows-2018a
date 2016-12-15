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

#include "ctocpp/views/box_layout_ctocpp.h"
#include "ctocpp/views/fill_layout_ctocpp.h"


// VIRTUAL METHODS - Body may be edited by hand.

CefRefPtr<CefBoxLayout> CefFillLayoutCToCpp::AsBoxLayout() {
  cef_layout_t* _struct = reinterpret_cast<cef_layout_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_box_layout))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_box_layout_t* _retval = _struct->as_box_layout(_struct);

  // Return type: refptr_same
  return CefBoxLayoutCToCpp::Wrap(_retval);
}

CefRefPtr<CefFillLayout> CefFillLayoutCToCpp::AsFillLayout() {
  cef_layout_t* _struct = reinterpret_cast<cef_layout_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_fill_layout))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_fill_layout_t* _retval = _struct->as_fill_layout(_struct);

  // Return type: refptr_same
  return CefFillLayoutCToCpp::Wrap(_retval);
}

bool CefFillLayoutCToCpp::IsValid() {
  cef_layout_t* _struct = reinterpret_cast<cef_layout_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_valid))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_valid(_struct);

  // Return type: bool
  return _retval?true:false;
}


// CONSTRUCTOR - Do not edit by hand.

CefFillLayoutCToCpp::CefFillLayoutCToCpp() {
}

template<> cef_fill_layout_t* CefCToCpp<CefFillLayoutCToCpp, CefFillLayout,
    cef_fill_layout_t>::UnwrapDerived(CefWrapperType type, CefFillLayout* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCToCpp<CefFillLayoutCToCpp, CefFillLayout,
    cef_fill_layout_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCToCpp<CefFillLayoutCToCpp, CefFillLayout,
    cef_fill_layout_t>::kWrapperType = WT_FILL_LAYOUT;
