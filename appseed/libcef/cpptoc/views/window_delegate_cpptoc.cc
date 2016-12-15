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

#include "cpptoc/views/window_delegate_cpptoc.h"
#include "ctocpp/views/view_ctocpp.h"
#include "ctocpp/views/window_ctocpp.h"


namespace {

// MEMBER FUNCTIONS - Body may be edited by hand.

void CEF_CALLBACK window_delegate_on_window_created(
    struct _cef_window_delegate_t* self, cef_window_t* window) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: window; type: refptr_diff
  DCHECK(window);
  if (!window)
    return;

  // Execute
  CefWindowDelegateCppToC::Get(self)->OnWindowCreated(
      CefWindowCToCpp::Wrap(window));
}

void CEF_CALLBACK window_delegate_on_window_destroyed(
    struct _cef_window_delegate_t* self, cef_window_t* window) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: window; type: refptr_diff
  DCHECK(window);
  if (!window)
    return;

  // Execute
  CefWindowDelegateCppToC::Get(self)->OnWindowDestroyed(
      CefWindowCToCpp::Wrap(window));
}

int CEF_CALLBACK window_delegate_is_frameless(
    struct _cef_window_delegate_t* self, cef_window_t* window) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: window; type: refptr_diff
  DCHECK(window);
  if (!window)
    return 0;

  // Execute
  bool _retval = CefWindowDelegateCppToC::Get(self)->IsFrameless(
      CefWindowCToCpp::Wrap(window));

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK window_delegate_can_resize(struct _cef_window_delegate_t* self,
    cef_window_t* window) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: window; type: refptr_diff
  DCHECK(window);
  if (!window)
    return 0;

  // Execute
  bool _retval = CefWindowDelegateCppToC::Get(self)->CanResize(
      CefWindowCToCpp::Wrap(window));

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK window_delegate_can_maximize(
    struct _cef_window_delegate_t* self, cef_window_t* window) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: window; type: refptr_diff
  DCHECK(window);
  if (!window)
    return 0;

  // Execute
  bool _retval = CefWindowDelegateCppToC::Get(self)->CanMaximize(
      CefWindowCToCpp::Wrap(window));

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK window_delegate_can_minimize(
    struct _cef_window_delegate_t* self, cef_window_t* window) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: window; type: refptr_diff
  DCHECK(window);
  if (!window)
    return 0;

  // Execute
  bool _retval = CefWindowDelegateCppToC::Get(self)->CanMinimize(
      CefWindowCToCpp::Wrap(window));

  // Return type: bool
  return _retval;
}

int CEF_CALLBACK window_delegate_can_close(struct _cef_window_delegate_t* self,
    cef_window_t* window) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: window; type: refptr_diff
  DCHECK(window);
  if (!window)
    return 0;

  // Execute
  bool _retval = CefWindowDelegateCppToC::Get(self)->CanClose(
      CefWindowCToCpp::Wrap(window));

  // Return type: bool
  return _retval;
}

cef_size_t CEF_CALLBACK window_delegate_get_preferred_size(
    struct _cef_view_delegate_t* self, cef_view_t* view) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return CefSize();
  // Verify param: view; type: refptr_diff
  DCHECK(view);
  if (!view)
    return CefSize();

  // Execute
  cef_size_t _retval = CefWindowDelegateCppToC::Get(
      reinterpret_cast<cef_window_delegate_t*>(self))->GetPreferredSize(
      CefViewCToCpp::Wrap(view));

  // Return type: simple
  return _retval;
}

cef_size_t CEF_CALLBACK window_delegate_get_minimum_size(
    struct _cef_view_delegate_t* self, cef_view_t* view) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return CefSize();
  // Verify param: view; type: refptr_diff
  DCHECK(view);
  if (!view)
    return CefSize();

  // Execute
  cef_size_t _retval = CefWindowDelegateCppToC::Get(
      reinterpret_cast<cef_window_delegate_t*>(self))->GetMinimumSize(
      CefViewCToCpp::Wrap(view));

  // Return type: simple
  return _retval;
}

cef_size_t CEF_CALLBACK window_delegate_get_maximum_size(
    struct _cef_view_delegate_t* self, cef_view_t* view) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return CefSize();
  // Verify param: view; type: refptr_diff
  DCHECK(view);
  if (!view)
    return CefSize();

  // Execute
  cef_size_t _retval = CefWindowDelegateCppToC::Get(
      reinterpret_cast<cef_window_delegate_t*>(self))->GetMaximumSize(
      CefViewCToCpp::Wrap(view));

  // Return type: simple
  return _retval;
}

int CEF_CALLBACK window_delegate_get_height_for_width(
    struct _cef_view_delegate_t* self, cef_view_t* view, int width) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return 0;
  // Verify param: view; type: refptr_diff
  DCHECK(view);
  if (!view)
    return 0;

  // Execute
  int _retval = CefWindowDelegateCppToC::Get(
      reinterpret_cast<cef_window_delegate_t*>(self))->GetHeightForWidth(
      CefViewCToCpp::Wrap(view),
      width);

  // Return type: simple
  return _retval;
}

void CEF_CALLBACK window_delegate_on_parent_view_changed(
    struct _cef_view_delegate_t* self, cef_view_t* view, int added,
    cef_view_t* parent) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: view; type: refptr_diff
  DCHECK(view);
  if (!view)
    return;
  // Verify param: parent; type: refptr_diff
  DCHECK(parent);
  if (!parent)
    return;

  // Execute
  CefWindowDelegateCppToC::Get(reinterpret_cast<cef_window_delegate_t*>(
      self))->OnParentViewChanged(
      CefViewCToCpp::Wrap(view),
      added?true:false,
      CefViewCToCpp::Wrap(parent));
}

void CEF_CALLBACK window_delegate_on_child_view_changed(
    struct _cef_view_delegate_t* self, cef_view_t* view, int added,
    cef_view_t* child) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;
  // Verify param: view; type: refptr_diff
  DCHECK(view);
  if (!view)
    return;
  // Verify param: child; type: refptr_diff
  DCHECK(child);
  if (!child)
    return;

  // Execute
  CefWindowDelegateCppToC::Get(reinterpret_cast<cef_window_delegate_t*>(
      self))->OnChildViewChanged(
      CefViewCToCpp::Wrap(view),
      added?true:false,
      CefViewCToCpp::Wrap(child));
}

}  // namespace


// CONSTRUCTOR - Do not edit by hand.

CefWindowDelegateCppToC::CefWindowDelegateCppToC() {
  GetStruct()->on_window_created = window_delegate_on_window_created;
  GetStruct()->on_window_destroyed = window_delegate_on_window_destroyed;
  GetStruct()->is_frameless = window_delegate_is_frameless;
  GetStruct()->can_resize = window_delegate_can_resize;
  GetStruct()->can_maximize = window_delegate_can_maximize;
  GetStruct()->can_minimize = window_delegate_can_minimize;
  GetStruct()->can_close = window_delegate_can_close;
  GetStruct()->base.base.get_preferred_size =
      window_delegate_get_preferred_size;
  GetStruct()->base.base.get_minimum_size = window_delegate_get_minimum_size;
  GetStruct()->base.base.get_maximum_size = window_delegate_get_maximum_size;
  GetStruct()->base.base.get_height_for_width =
      window_delegate_get_height_for_width;
  GetStruct()->base.base.on_parent_view_changed =
      window_delegate_on_parent_view_changed;
  GetStruct()->base.base.on_child_view_changed =
      window_delegate_on_child_view_changed;
}

template<> CefRefPtr<CefWindowDelegate> CefCppToC<CefWindowDelegateCppToC,
    CefWindowDelegate, cef_window_delegate_t>::UnwrapDerived(
    CefWrapperType type, cef_window_delegate_t* s) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCppToC<CefWindowDelegateCppToC,
    CefWindowDelegate, cef_window_delegate_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCppToC<CefWindowDelegateCppToC, CefWindowDelegate,
    cef_window_delegate_t>::kWrapperType = WT_WINDOW_DELEGATE;
