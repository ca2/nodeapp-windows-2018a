#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      namespace user
      {


         level_control::level_control() :
            control(new ::multimedia::audio_mixer::control_data_volume())
         {

         }


         level_control::~level_control()
         {

         }


         void level_control::SetOrientation(e_orientation eorientation)
         {

            UNREFERENCED_PARAMETER(eorientation);

            throw interface_only_exception(::ca2::get_thread_app());

         }


         void level_control::SetRange(int32_t iMin, int32_t iMax)
         {

            UNREFERENCED_PARAMETER(iMin);
            UNREFERENCED_PARAMETER(iMax);

            throw interface_only_exception(::ca2::get_thread_app());

         }


         void level_control::SetLineSize(int32_t iSize)
         {

            UNREFERENCED_PARAMETER(iSize);

            throw interface_only_exception(::ca2::get_thread_app());

         }


         void level_control::SetPageSize(int32_t iPage)
         {

            UNREFERENCED_PARAMETER(iPage);

            throw interface_only_exception(::ca2::get_thread_app());

         }


         int32_t level_control::GetPos() const
         {

            throw interface_only_exception(::ca2::get_thread_app());

         }


         void level_control::SetPos(int32_t iPos)
         {

            UNREFERENCED_PARAMETER(iPos);

            throw interface_only_exception(::ca2::get_thread_app());

         }


         ::multimedia::audio_mixer::control_data_volume & level_control::get_data()
         {

            return *(::multimedia::audio_mixer::control_data_volume* ) m_pdata;

         }


      } // namespace user


   } // namespace audio_mixer_mmsystem


} // namespace multimedia





