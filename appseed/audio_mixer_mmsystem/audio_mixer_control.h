#pragma once


namespace audio_mixer
{



   class CLASS_DECL_CA2_MULTIMEDIA control :
      public ::audio_mixer::user::control_ptr_array
   {
   public:


      enum Styles
      {
         StyleV001 = 1
      };


      ::audio_mixer::source                * m_pmixersource;
      MIXERCONTROL                     m_mixercontrol;
#ifdef WINDOWS
      MIXERCONTROLDETAILS              m_mixercontroldetails;
#endif

      control(sp(::ca2::application) papp);
      control(control & control);
      virtual ~control();

      using ::audio_mixer::user::control_ptr_array::add;
      virtual int_ptr add(::audio_mixer::user::control * pcontrol);

      uint32_t GetControlType() const;
      bool OnCommand(WPARAM wParam, LPARAM lParam);
      ::audio_mixer::control_data * GetWindowDataByDlgCtrlID(uint32_t nID);
      ::audio_mixer::user::control * GetControlByDlgCtrlID(uint32_t nID);
      //   bool OnNotify(uint32_t nID, LPNMHDR lpnmhdr);
      void OnVHScroll(uint32_t nSBCode, uint32_t nPos, sp(::user::interaction) pScrollBar);
      ::audio_mixer::control_data * GetWindowData(int32_t iType);
      void OnMixerControlChange();
      ::audio_mixer::user::control * GetControl(int32_t iType);
      ::audio_mixer::user::control * GetControlByIndex(::index iIndex);
      void Prepare();
      bool CreateWindowsVolumeV001(sp(::user::interaction) pParent, uint32_t nStartID, uint32_t * nNextID);

      //  bool _001CreateMuteControl(sp(::user::interaction) pParent, CRuntimeClass * pruntimeclassMuteControl, uint32_t nStartID, uint32_t * nNextID);
      bool _001CreateMuteControl(sp(::user::interaction) pParent, uint32_t nStartID, uint32_t * nNextID);

      bool CreateWindows(sp(::user::interaction) pParent, int32_t iStyle);

      control & operator =(const control & control);


      MIXERCONTROL & GetMixerControl();
#ifdef WINDOWS
      MIXERCONTROLDETAILS & GetMixerControlDetails();
#endif


   };


   class control_array :
      public spa(control)
   {
   public:


      control_array(sp(::ca2::application) papp);


   };


} // namespace audio_mixer







