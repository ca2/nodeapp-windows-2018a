#pragma once


class CLASS_DECL_win WinResource :
   virtual public ::ca::resource
{
public:


   WinResource(::ca::applicationsp papp);
   virtual ~WinResource();
   
   bool ReadResource(string & str, HINSTANCE hinst, UINT nID, const char * lpcszType);
   bool ReadResource(::ca::file & file, HINSTANCE hinst, UINT nID, const char * lpcszType);


};
