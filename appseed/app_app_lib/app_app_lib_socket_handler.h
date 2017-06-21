#pragma once


namespace a_spa
{


   class SPALIB_API socket_handler :
      virtual public ::netserver::socket_handler
   {
   public:


      socket_handler(::aura::application * papp);


   };


} // namespace a_spa



