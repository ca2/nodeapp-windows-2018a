#pragma once


class CLASS_DECL_win WinResource :
   virtual public gen::resource
{
public:


   WinResource(::ca::application * papp);
   virtual ~WinResource();
   
   bool ReadResource(string & str, HINSTANCE hinst, UINT nID, const char * lpcszType);
   bool ReadResource(gen::file & file, HINSTANCE hinst, UINT nID, const char * lpcszType);


};
