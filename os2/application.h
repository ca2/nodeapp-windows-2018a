#pragma once


namespace win2
{


   class CLASS_DECL_win2 application :
      virtual public ::cubebase::application
   {
   public:


      application(sp(::ca::application) papp);
      virtual ~application();
      
      virtual ::user::printer * get_printer(const char * pszDeviceName);

   };


} // namespace win2


