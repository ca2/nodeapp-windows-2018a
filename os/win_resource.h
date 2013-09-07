#pragma once


class CLASS_DECL_win WinResource :
   virtual public ::core::resource
{
public:


   WinResource(sp(base_application) papp);
   virtual ~WinResource();
   
   bool ReadResource(string & str, HINSTANCE hinst, UINT nID, const char * lpcszType);
   bool ReadResource(::file::binary_buffer & file, HINSTANCE hinst, UINT nID, const char * lpcszType);


};
