#pragma once


namespace app_app
{


   class SPALIB_API socket_handler :
      virtual public ::netserver::socket_handler
   {
   public:


      socket_handler(::aura::application * papp);


   };


} // namespace app_app



