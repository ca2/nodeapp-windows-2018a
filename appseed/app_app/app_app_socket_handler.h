#pragma once


namespace user_service
{


   class CLASS_DECL_APP_CORE_USER_SERVICE socket_handler :
      virtual public ::sockets::socket_handler
   {
   public:


      socket_handler(::aura::application * papp);


   };


} // namespace user_service



