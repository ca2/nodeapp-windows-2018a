#pragma once


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      namespace user
      {


         class CLASS_DECL_AUDIO_MIXER_MMSYSTEM control :
            virtual public ::ca2::object
         {
         public:


            ::multimedia::audio_mixer::control_data *   m_pdata;


            control(::multimedia::audio_mixer::control_data * pdata);
            virtual ~control();


            virtual sp(::user::interaction) GetWnd() = 0;
            virtual bool Initialize(sp(::user::interaction) pwndParent, uint32_t uiId) = 0;
            virtual int32_t _GetDlgCtrlID() = 0;


         };


         class CLASS_DECL_AUDIO_MIXER_MMSYSTEM control_ptr_array :
            public spa(control)
         {
         };


      } // namespace user


   } // namespace audio_mixer_mmsystem


} // namespace multimedia




