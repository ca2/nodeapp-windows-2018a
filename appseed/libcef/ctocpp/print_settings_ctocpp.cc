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

#include <algorithm>
#include "ctocpp/print_settings_ctocpp.h"


// STATIC METHODS - Body may be edited by hand.

CefRefPtr<CefPrintSettings> CefPrintSettings::Create() {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_print_settings_t* _retval = cef_print_settings_create();

  // Return type: refptr_same
  return CefPrintSettingsCToCpp::Wrap(_retval);
}


// VIRTUAL METHODS - Body may be edited by hand.

bool CefPrintSettingsCToCpp::IsValid() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_valid))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_valid(_struct);

  // Return type: bool
  return _retval?true:false;
}

bool CefPrintSettingsCToCpp::IsReadOnly() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_read_only))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_read_only(_struct);

  // Return type: bool
  return _retval?true:false;
}

CefRefPtr<CefPrintSettings> CefPrintSettingsCToCpp::Copy() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, copy))
    return NULL;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_print_settings_t* _retval = _struct->copy(_struct);

  // Return type: refptr_same
  return CefPrintSettingsCToCpp::Wrap(_retval);
}

void CefPrintSettingsCToCpp::SetOrientation(bool landscape) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_orientation))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_orientation(_struct,
      landscape);
}

bool CefPrintSettingsCToCpp::IsLandscape() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_landscape))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_landscape(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefPrintSettingsCToCpp::SetPrinterPrintableArea(
    const CefSize& physical_size_device_units,
    const CefRect& printable_area_device_units, bool landscape_needs_flip) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_printer_printable_area))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_printer_printable_area(_struct,
      &physical_size_device_units,
      &printable_area_device_units,
      landscape_needs_flip);
}

void CefPrintSettingsCToCpp::SetDeviceName(const CefString& name) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_device_name))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Unverified params: name

  // Execute
  _struct->set_device_name(_struct,
      name.GetStruct());
}

CefString CefPrintSettingsCToCpp::GetDeviceName() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_device_name))
    return CefString();

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_string_userfree_t _retval = _struct->get_device_name(_struct);

  // Return type: string
  CefString _retvalStr;
  _retvalStr.AttachToUserFree(_retval);
  return _retvalStr;
}

void CefPrintSettingsCToCpp::SetDPI(int dpi) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_dpi))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_dpi(_struct,
      dpi);
}

int CefPrintSettingsCToCpp::GetDPI() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_dpi))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_dpi(_struct);

  // Return type: simple
  return _retval;
}

void CefPrintSettingsCToCpp::SetPageRanges(const PageRangeList& ranges) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_page_ranges))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Translate param: ranges; type: simple_vec_byref_const
  const size_t rangesCount = ranges.size();
  cef_range_t* rangesList = NULL;
  if (rangesCount > 0) {
    rangesList = new cef_range_t[rangesCount];
    DCHECK(rangesList);
    if (rangesList) {
      for (size_t i = 0; i < rangesCount; ++i) {
        rangesList[i] = ranges[i];
      }
    }
  }

  // Execute
  _struct->set_page_ranges(_struct,
      rangesCount,
      rangesList);

  // Restore param:ranges; type: simple_vec_byref_const
  if (rangesList)
    delete [] rangesList;
}

size_t CefPrintSettingsCToCpp::GetPageRangesCount() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_page_ranges_count))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  size_t _retval = _struct->get_page_ranges_count(_struct);

  // Return type: simple
  return _retval;
}

void CefPrintSettingsCToCpp::GetPageRanges(PageRangeList& ranges) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_page_ranges))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Translate param: ranges; type: simple_vec_byref
  size_t rangesSize = ranges.size();
  size_t rangesCount = std::max(GetPageRangesCount(), rangesSize);
  cef_range_t* rangesList = NULL;
  if (rangesCount > 0) {
    rangesList = new cef_range_t[rangesCount];
    DCHECK(rangesList);
    if (rangesList) {
       memset(rangesList, 0, sizeof(cef_range_t)*rangesCount);
    }
    if (rangesList && rangesSize > 0) {
      for (size_t i = 0; i < rangesSize; ++i) {
        rangesList[i] = ranges[i];
      }
    }
  }

  // Execute
  _struct->get_page_ranges(_struct,
      &rangesCount,
      rangesList);

  // Restore param:ranges; type: simple_vec_byref
  ranges.clear();
  if (rangesCount > 0 && rangesList) {
    for (size_t i = 0; i < rangesCount; ++i) {
      ranges.push_back(rangesList[i]);
    }
    delete [] rangesList;
  }
}

void CefPrintSettingsCToCpp::SetSelectionOnly(bool selection_only) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_selection_only))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_selection_only(_struct,
      selection_only);
}

bool CefPrintSettingsCToCpp::IsSelectionOnly() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, is_selection_only))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->is_selection_only(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefPrintSettingsCToCpp::SetCollate(bool collate) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_collate))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_collate(_struct,
      collate);
}

bool CefPrintSettingsCToCpp::WillCollate() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, will_collate))
    return false;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->will_collate(_struct);

  // Return type: bool
  return _retval?true:false;
}

void CefPrintSettingsCToCpp::SetColorModel(ColorModel model) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_color_model))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_color_model(_struct,
      model);
}

CefPrintSettings::ColorModel CefPrintSettingsCToCpp::GetColorModel() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_color_model))
    return COLOR_MODEL_UNKNOWN;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_color_model_t _retval = _struct->get_color_model(_struct);

  // Return type: simple
  return _retval;
}

void CefPrintSettingsCToCpp::SetCopies(int copies) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_copies))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_copies(_struct,
      copies);
}

int CefPrintSettingsCToCpp::GetCopies() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_copies))
    return 0;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  int _retval = _struct->get_copies(_struct);

  // Return type: simple
  return _retval;
}

void CefPrintSettingsCToCpp::SetDuplexMode(DuplexMode mode) {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, set_duplex_mode))
    return;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  _struct->set_duplex_mode(_struct,
      mode);
}

CefPrintSettings::DuplexMode CefPrintSettingsCToCpp::GetDuplexMode() {
  cef_print_settings_t* _struct = GetStruct();
  if (CEF_MEMBER_MISSING(_struct, get_duplex_mode))
    return DUPLEX_MODE_UNKNOWN;

  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  // Execute
  cef_duplex_mode_t _retval = _struct->get_duplex_mode(_struct);

  // Return type: simple
  return _retval;
}


// CONSTRUCTOR - Do not edit by hand.

CefPrintSettingsCToCpp::CefPrintSettingsCToCpp() {
}

template<> cef_print_settings_t* CefCToCpp<CefPrintSettingsCToCpp,
    CefPrintSettings, cef_print_settings_t>::UnwrapDerived(CefWrapperType type,
    CefPrintSettings* c) {
  NOTREACHED() << "Unexpected class type: " << type;
  return NULL;
}

#if DCHECK_IS_ON()
template<> base::AtomicRefCount CefCToCpp<CefPrintSettingsCToCpp,
    CefPrintSettings, cef_print_settings_t>::DebugObjCt = 0;
#endif

template<> CefWrapperType CefCToCpp<CefPrintSettingsCToCpp, CefPrintSettings,
    cef_print_settings_t>::kWrapperType = WT_PRINT_SETTINGS;
