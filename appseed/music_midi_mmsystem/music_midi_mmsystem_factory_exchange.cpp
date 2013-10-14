#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      factory_exchange::factory_exchange(sp(base_application) papp) :
         element(papp),
         ::multimedia::mmsystem::factory_exchange(papp)
      {

         System.factory().cloneable_large < sequence               >  (System.type_info < ::music::midi::sequence                  > ());
         System.factory().cloneable_large < file              >  (System.type_info < ::music::midi::file::buffer                 > ());
         System.factory().cloneable_large < sequence_thread              >  (System.type_info < ::music::midi::sequence_thread                 > ());
         System.factory().cloneable_large < ::music::midi::mmsystem::player::player              >  (System.type_info < ::music::midi::player::player                 > ());
         System.factory().cloneable_large < ::music::midi::mmsystem::player:: player_interface              >  (System.type_info < ::music::midi::player::player_interface                 > ());

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace midi_mmsystem


} // namespace music


void ca2_factory_exchange(sp(base_application) papp)
{

   ::music::midi::mmsystem::factory_exchange factoryexchange(papp);

}









