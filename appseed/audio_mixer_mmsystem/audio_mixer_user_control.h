#pragma once


namespace audio_mixer
{


   namespace user
   {


      class CLASS_DECL_CA2_MULTIMEDIA control
      {
      public:


         ::audio_mixer::control_data *   m_pdata;

         
         control(::audio_mixer::control_data * pdata);
         virtual ~control();


         virtual sp(::user::interaction) GetWnd() = 0;
         virtual bool Initialize(sp(::user::interaction) pwndParent, uint32_t uiId) = 0;
         virtual int32_t _GetDlgCtrlID() = 0;

      
      };

   
      class CLASS_DECL_CA2_MULTIMEDIA control_ptr_array :
         public comparable_array < control *, control *>
      {
      };


   } // namespace user


} // namespace audio_mixer



