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

#include "cpptoc/views/view_delegate_cpptoc.h"
#include "cpptoc/views/window_delegate_cpptoc.h"
#include "ctocpp/views/box_layout_ctocpp.h"
#include "ctocpp/views/browser_view_ctocpp.h"
#include "ctocpp/views/button_ctocpp.h"
#include "ctocpp/views/display_ctocpp.h"
#include "ctocpp/views/fill_layout_ctocpp.h"
#include "ctocpp/image_ctocpp.h"
#include "ctocpp/views/layout_ctocpp.h"
#include "ctocpp/menu_model_ctocpp.h"
#include "ctocpp/views/panel_ctocpp.h"
#include "ctocpp/views/scroll_view_ctocpp.h"
#include "ctocpp/views/textfield_ctocpp.h"
#include "ctocpp/views/view_ctocpp.h"
#include "ctocpp/views/window_ctocpp.h"


// STATIC METHODS - Body may be edited by hand.

CefRefPtr<CefWindow> CefWindow::CreateTopLevelWindow(
    CefRefPtr<CefWindowDelegate> delegate) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: delegate

  // Execute
  cef_window_t* _retval = cef_window_create_top_level(
      CefWindowDelegateCppToC::Wrap(delegate));

  // Return type: refptr_same
  return CefWindowCToCpp::Wrap(_retval);
}


// VIRTUAL METHODS - Body may be edited by hand.

void CefWindowCToCpp::Show() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, show))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->show(_struct);
}

void CefWindowCToCpp::Hide() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, hide))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->hide(_struct);
}

void CefWindowCToCpp::CenterWindow(const CefSize& size) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, center_window))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->center_window(_struct,
      &size);
}

void CefWindowCToCpp::Close() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, close))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->close(_struct);
}

bool CefWindowCToCpp::IsClosed() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_closed))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_closed(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWindowCToCpp::Activate() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, activate))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->activate(_struct);
}

void CefWindowCToCpp::Deactivate() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, deactivate))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->deactivate(_struct);
}

bool CefWindowCToCpp::IsActive() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_active))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_active(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWindowCToCpp::BringToTop() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, bring_to_top))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->bring_to_top(_struct);
}

void CefWindowCToCpp::SetAlwaysOnTop(bool on_top) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_always_on_top))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_always_on_top(_struct,
      on_top);
}

bool CefWindowCToCpp::IsAlwaysOnTop() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_always_on_top))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_always_on_top(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWindowCToCpp::Maximize() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, maximize))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->maximize(_struct);
}

void CefWindowCToCpp::Minimize() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, minimize))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->minimize(_struct);
}

void CefWindowCToCpp::Restore() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, restore))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->restore(_struct);
}

void CefWindowCToCpp::SetFullscreen(bool fullscreen) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_fullscreen))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_fullscreen(_struct,
      fullscreen);
}

bool CefWindowCToCpp::IsMaximized() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_maximized))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_maximized(_struct);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::IsMinimized() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_minimized))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_minimized(_struct);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::IsFullscreen() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_fullscreen))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_fullscreen(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWindowCToCpp::SetTitle(const CefString& title) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_title))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: title

  // Execute
  _struct->set_title(_struct,
      title.GetStruct());
}

CefString CefWindowCToCpp::GetTitle() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_title))
    return CefString();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_title(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

void CefWindowCToCpp::SetWindowIcon(CefRefPtr<CefImage> image) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_window_icon))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: image; type: refptr_same
  DCHECK(image.get());
  if (!image.get())
    return;

  // Execute
  _struct->set_window_icon(_struct,
      CefImageCToCpp::Unwrap(image));
}

CefRefPtr<CefImage> CefWindowCToCpp::GetWindowIcon() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_window_icon))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_image_t* _retval = _struct->get_window_icon(_struct);

  // Return type: refptr_same
  return CefImageCToCpp::Wrap(_retval);
}

void CefWindowCToCpp::SetWindowAppIcon(CefRefPtr<CefImage> image) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_window_app_icon))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: image; type: refptr_same
  DCHECK(image.get());
  if (!image.get())
    return;

  // Execute
  _struct->set_window_app_icon(_struct,
      CefImageCToCpp::Unwrap(image));
}

CefRefPtr<CefImage> CefWindowCToCpp::GetWindowAppIcon() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_window_app_icon))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_image_t* _retval = _struct->get_window_app_icon(_struct);

  // Return type: refptr_same
  return CefImageCToCpp::Wrap(_retval);
}

void CefWindowCToCpp::ShowMenu(CefRefPtr<CefMenuModel> menu_model,
    const CefPoint& screen_point,
    cef_menu_anchor_position_t anchor_position) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, show_menu))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: menu_model; type: refptr_same
  DCHECK(menu_model.get());
  if (!menu_model.get())
    return;

  // Execute
  _struct->show_menu(_struct,
      CefMenuModelCToCpp::Unwrap(menu_model),
      &screen_point,
      anchor_position);
}

void CefWindowCToCpp::CancelMenu() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, cancel_menu))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->cancel_menu(_struct);
}

CefRefPtr<CefDisplay> CefWindowCToCpp::GetDisplay() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_display))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_display_t* _retval = _struct->get_display(_struct);

  // Return type: refptr_same
  return CefDisplayCToCpp::Wrap(_retval);
}

CefRect CefWindowCToCpp::GetClientAreaBoundsInScreen() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_client_area_bounds_in_screen))
    return CefRect();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_rect_t _retval = _struct->get_client_area_bounds_in_screen(_struct);

  // Return type: simple
  return _retval;
}

void CefWindowCToCpp::SetDraggableRegions(
    const std::vector<CefDraggableRegion>& regions) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_draggable_regions))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: regions

  // Translate param: regions; type: simple_vec_byref_const
  const size_t regionsCount = regions.size();
  cef_draggable_region_t* regionsList = NULL;
  if (regionsCount > 0) {
    regionsList = new cef_draggable_region_t[regionsCount];
    DCHECK(regionsList);
    if (regionsList) {
      for (size_t i = 0; i < regionsCount; ++i) {
        regionsList[i] = regions[i];
      }
    }
  }

  // Execute
  _struct->set_draggable_regions(_struct,
      regionsCount,
      regionsList);

  // Restore param:regions; type: simple_vec_byref_const
  if (regionsList)
    delete [] regionsList;
}

CefWindowHandle CefWindowCToCpp::GetWindowHandle() {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_window_handle))
    return kNullWindowHandle;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_window_handle_t _retval = _struct->get_window_handle(_struct);

  // Return type: simple
  return _retval;
}

void CefWindowCToCpp::SendKeyPress(int key_code, uint32 event_flags) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, send_key_press))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->send_key_press(_struct,
      key_code,
      event_flags);
}

void CefWindowCToCpp::SendMouseMove(int screen_x, int screen_y) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, send_mouse_move))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->send_mouse_move(_struct,
      screen_x,
      screen_y);
}

void CefWindowCToCpp::SendMouseEvents(cef_mouse_button_type_t button,
    bool mouse_down, bool mouse_up) {
  cef_window_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, send_mouse_events))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->send_mouse_events(_struct,
      button,
      mouse_down,
      mouse_up);
}

CefRefPtr<CefWindow> CefWindowCToCpp::AsWindow() {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_window))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_window_t* _retval = _struct->as_window(_struct);

  // Return type: refptr_same
  return CefWindowCToCpp::Wrap(_retval);
}

CefRefPtr<CefFillLayout> CefWindowCToCpp::SetToFillLayout() {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_to_fill_layout))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_fill_layout_t* _retval = _struct->set_to_fill_layout(_struct);

  // Return type: refptr_same
  return CefFillLayoutCToCpp::Wrap(_retval);
}

CefRefPtr<CefBoxLayout> CefWindowCToCpp::SetToBoxLayout(
    const CefBoxLayoutSettings& settings) {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_to_box_layout))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_box_layout_t* _retval = _struct->set_to_box_layout(_struct,
      &settings);

  // Return type: refptr_same
  return CefBoxLayoutCToCpp::Wrap(_retval);
}

CefRefPtr<CefLayout> CefWindowCToCpp::GetLayout() {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_layout))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_layout_t* _retval = _struct->get_layout(_struct);

  // Return type: refptr_same
  return CefLayoutCToCpp::Wrap(_retval);
}

void CefWindowCToCpp::Layout() {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, layout))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->layout(_struct);
}

void CefWindowCToCpp::AddChildView(CefRefPtr<CefView> view) {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, add_child_view))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_same
  DCHECK(view.get());
  if (!view.get())
    return;

  // Execute
  _struct->add_child_view(_struct,
      CefViewCToCpp::Unwrap(view));
}

void CefWindowCToCpp::AddChildViewAt(CefRefPtr<CefView> view, int index) {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, add_child_view_at))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_same
  DCHECK(view.get());
  if (!view.get())
    return;
  // Verify param: index; type: simple_byval
  DCHECK_GE(index, 0);
  if (index < 0)
    return;

  // Execute
  _struct->add_child_view_at(_struct,
      CefViewCToCpp::Unwrap(view),
      index);
}

void CefWindowCToCpp::ReorderChildView(CefRefPtr<CefView> view, int index) {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, reorder_child_view))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_same
  DCHECK(view.get());
  if (!view.get())
    return;

  // Execute
  _struct->reorder_child_view(_struct,
      CefViewCToCpp::Unwrap(view),
      index);
}

void CefWindowCToCpp::RemoveChildView(CefRefPtr<CefView> view) {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, remove_child_view))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_same
  DCHECK(view.get());
  if (!view.get())
    return;

  // Execute
  _struct->remove_child_view(_struct,
      CefViewCToCpp::Unwrap(view));
}

void CefWindowCToCpp::RemoveAllChildViews() {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, remove_all_child_views))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->remove_all_child_views(_struct);
}

size_t CefWindowCToCpp::GetChildViewCount() {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_child_view_count))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  size_t _retval = _struct->get_child_view_count(_struct);

  // Return type: simple
  return _retval;
}

CefRefPtr<CefView> CefWindowCToCpp::GetChildViewAt(int index) {
  cef_panel_t* _struct = reinterpret_cast<cef_panel_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_child_view_at))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: index; type: simple_byval
  DCHECK_GE(index, 0);
  if (index < 0)
    return NULL;

  // Execute
  cef_view_t* _retval = _struct->get_child_view_at(_struct,
      index);

  // Return type: refptr_same
  return CefViewCToCpp::Wrap(_retval);
}

CefRefPtr<CefBrowserView> CefWindowCToCpp::AsBrowserView() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_browser_view))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_browser_view_t* _retval = _struct->as_browser_view(_struct);

  // Return type: refptr_same
  return CefBrowserViewCToCpp::Wrap(_retval);
}

CefRefPtr<CefButton> CefWindowCToCpp::AsButton() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_button))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_button_t* _retval = _struct->as_button(_struct);

  // Return type: refptr_same
  return CefButtonCToCpp::Wrap(_retval);
}

CefRefPtr<CefPanel> CefWindowCToCpp::AsPanel() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_panel))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_panel_t* _retval = _struct->as_panel(_struct);

  // Return type: refptr_same
  return CefPanelCToCpp::Wrap(_retval);
}

CefRefPtr<CefScrollView> CefWindowCToCpp::AsScrollView() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_scroll_view))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_scroll_view_t* _retval = _struct->as_scroll_view(_struct);

  // Return type: refptr_same
  return CefScrollViewCToCpp::Wrap(_retval);
}

CefRefPtr<CefTextfield> CefWindowCToCpp::AsTextfield() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, as_textfield))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_textfield_t* _retval = _struct->as_textfield(_struct);

  // Return type: refptr_same
  return CefTextfieldCToCpp::Wrap(_retval);
}

CefString CefWindowCToCpp::GetTypeString() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_type_string))
    return CefString();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_type_string(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

CefString CefWindowCToCpp::ToString(bool include_children) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, to_string))
    return CefString();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->to_string(_struct,
      include_children);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

bool CefWindowCToCpp::IsValid() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_valid))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_valid(_struct);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::IsAttached() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_attached))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_attached(_struct);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::IsSame(CefRefPtr<CefView> that) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_same))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: that; type: refptr_same
  DCHECK(that.get());
  if (!that.get())
    return false;

  // Execute
  int _retval = _struct->is_same(_struct,
      CefViewCToCpp::Unwrap(that));

  // Return type: bool
  return _retval?true:false;
}

CefRefPtr<CefViewDelegate> CefWindowCToCpp::GetDelegate() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_delegate))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_view_delegate_t* _retval = _struct->get_delegate(_struct);

  // Return type: refptr_diff
  return CefViewDelegateCppToC::Unwrap(_retval);
}

CefRefPtr<CefWindow> CefWindowCToCpp::GetWindow() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_window))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_window_t* _retval = _struct->get_window(_struct);

  // Return type: refptr_same
  return CefWindowCToCpp::Wrap(_retval);
}

int CefWindowCToCpp::GetID() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_id))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_id(_struct);

  // Return type: simple
  return _retval;
}

void CefWindowCToCpp::SetID(int id) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_id))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_id(_struct,
      id);
}

CefRefPtr<CefView> CefWindowCToCpp::GetParentView() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_parent_view))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_view_t* _retval = _struct->get_parent_view(_struct);

  // Return type: refptr_same
  return CefViewCToCpp::Wrap(_retval);
}

CefRefPtr<CefView> CefWindowCToCpp::GetViewForID(int id) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_view_for_id))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_view_t* _retval = _struct->get_view_for_id(_struct,
      id);

  // Return type: refptr_same
  return CefViewCToCpp::Wrap(_retval);
}

void CefWindowCToCpp::SetBounds(const CefRect& bounds) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_bounds))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_bounds(_struct,
      &bounds);
}

CefRect CefWindowCToCpp::GetBounds() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_bounds))
    return CefRect();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_rect_t _retval = _struct->get_bounds(_struct);

  // Return type: simple
  return _retval;
}

CefRect CefWindowCToCpp::GetBoundsInScreen() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_bounds_in_screen))
    return CefRect();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_rect_t _retval = _struct->get_bounds_in_screen(_struct);

  // Return type: simple
  return _retval;
}

void CefWindowCToCpp::SetSize(const CefSize& size) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_size))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_size(_struct,
      &size);
}

CefSize CefWindowCToCpp::GetSize() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_size))
    return CefSize();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_size_t _retval = _struct->get_size(_struct);

  // Return type: simple
  return _retval;
}

void CefWindowCToCpp::SetPosition(const CefPoint& position) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_position))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_position(_struct,
      &position);
}

CefPoint CefWindowCToCpp::GetPosition() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_position))
    return CefPoint();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_point_t _retval = _struct->get_position(_struct);

  // Return type: simple
  return _retval;
}

CefSize CefWindowCToCpp::GetPreferredSize() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_preferred_size))
    return CefSize();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_size_t _retval = _struct->get_preferred_size(_struct);

  // Return type: simple
  return _retval;
}

void CefWindowCToCpp::SizeToPreferredSize() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, size_to_preferred_size))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->size_to_preferred_size(_struct);
}

CefSize CefWindowCToCpp::GetMinimumSize() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_minimum_size))
    return CefSize();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_size_t _retval = _struct->get_minimum_size(_struct);

  // Return type: simple
  return _retval;
}

CefSize CefWindowCToCpp::GetMaximumSize() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_maximum_size))
    return CefSize();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_size_t _retval = _struct->get_maximum_size(_struct);

  // Return type: simple
  return _retval;
}

int CefWindowCToCpp::GetHeightForWidth(int width) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_height_for_width))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_height_for_width(_struct,
      width);

  // Return type: simple
  return _retval;
}

void CefWindowCToCpp::InvalidateLayout() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, invalidate_layout))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->invalidate_layout(_struct);
}

void CefWindowCToCpp::SetVisible(bool visible) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_visible))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_visible(_struct,
      visible);
}

bool CefWindowCToCpp::IsVisible() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_visible))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_visible(_struct);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::IsDrawn() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_drawn))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_drawn(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWindowCToCpp::SetEnabled(bool enabled) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_enabled))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_enabled(_struct,
      enabled);
}

bool CefWindowCToCpp::IsEnabled() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_enabled))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_enabled(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWindowCToCpp::SetFocusable(bool focusable) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_focusable))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_focusable(_struct,
      focusable);
}

bool CefWindowCToCpp::IsFocusable() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_focusable))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_focusable(_struct);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::IsAccessibilityFocusable() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, is_accessibility_focusable))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_accessibility_focusable(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefWindowCToCpp::RequestFocus() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, request_focus))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->request_focus(_struct);
}

void CefWindowCToCpp::SetBackgroundColor(cef_color_t color) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, set_background_color))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_background_color(_struct,
      color);
}

cef_color_t CefWindowCToCpp::GetBackgroundColor() {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, get_background_color))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_color_t _retval = _struct->get_background_color(_struct);

  // Return type: simple
  return _retval;
}

bool CefWindowCToCpp::ConvertPointToScreen(CefPoint& point) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, convert_point_to_screen))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->convert_point_to_screen(_struct,
      &point);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::ConvertPointFromScreen(CefPoint& point) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, convert_point_from_screen))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->convert_point_from_screen(_struct,
      &point);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::ConvertPointToWindow(CefPoint& point) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, convert_point_to_window))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->convert_point_to_window(_struct,
      &point);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::ConvertPointFromWindow(CefPoint& point) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, convert_point_from_window))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->convert_point_from_window(_struct,
      &point);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::ConvertPointToView(CefRefPtr<CefView> view,
    CefPoint& point) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, convert_point_to_view))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_same
  DCHECK(view.get());
  if (!view.get())
    return false;

  // Execute
  int _retval = _struct->convert_point_to_view(_struct,
      CefViewCToCpp::Unwrap(view),
      &point);

  // Return type: bool
  return _retval?true:false;
}

bool CefWindowCToCpp::ConvertPointFromView(CefRefPtr<CefView> view,
    CefPoint& point) {
  cef_view_t* _struct = reinterpret_cast<cef_view_t*>(GetStruct());
  if (CEF_MEMBER_MISSING(_struct, convert_point_from_view))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Verify param: view; type: refptr_same
  DCHECK(view.get());
  if (!view.get())
    return false;

  // Execute
  int _retval = _struct->convert_point_from_view(_struct,
      CefViewCToCpp::Unwrap(view),
      &point);

  // Return type: bool
  return _retval?true:false;
}


// CONSTRUCTOR - Do not edit by hand.

CefWindowCToCpp::CefWindowCToCpp() {
}

template<> cef_window_t* CefCToCpp<CefWindowCToCpp, CefWindow,
    cef_window_t>::UnwrapDerived(CefWrapperType type, CefWindow* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCToCpp<CefWindowCToCpp, CefWindow,
    cef_window_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCToCpp<CefWindowCToCpp, CefWindow,
    cef_window_t>::kWrapperType = WT_WINDOW;
