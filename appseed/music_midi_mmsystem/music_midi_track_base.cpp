#include "framework.h"


namespace music
{


   namespace midi
   {


      track_base::track_base(sp(::ca2::application) papp) :
         ca2(papp)
      {

      }

      track_base::~track_base()
      {

      }

      bool track_base::IsType(e_type e_type)
      {
         return get_type() == e_type;
      }


   } // namespace midi


} // namespace music









