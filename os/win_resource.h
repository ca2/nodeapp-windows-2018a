#pragma once


class CLASS_DECL_win WinResource :
   virtual public ::ca2::resource
{
public:


   WinResource(sp(base_application) papp);
   virtual ~WinResource();
   
   bool ReadResource(string & str, HINSTANCE hinst, UINT nID, const char * lpcszType);
   bool ReadResource(::file::file & file, HINSTANCE hinst, UINT nID, const char * lpcszType);


};
