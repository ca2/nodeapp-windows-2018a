#include "framework.h"


namespace audio_mixer
{


   control::control(sp(::ca2::application) papp) :
      ca2(papp)
   {

      m_pmixersource = NULL;
      m_mixercontrol.cbStruct = sizeof(MIXERCONTROL);
      #ifdef WINDOWS
      m_mixercontroldetails.cbStruct = sizeof(MIXERCONTROLDETAILS);
      m_mixercontroldetails.paDetails = NULL;
#endif
   }


   control::control(control & control)
   {

      operator =(control);

   }


   control::~control()
   {
#ifdef WINDOWS
      if(m_mixercontroldetails.paDetails != NULL)
         free(m_mixercontroldetails.paDetails);
        #endif

      ::audio_mixer::user::control * pcontrol;

      for(int32_t i = 0; i < this->get_size(); i++)
      {

         if((pcontrol = this->element_at(i)) != NULL)
         {

            delete pcontrol;

         }

      }

   }

   bool control::CreateWindows(sp(::user::interaction) pParent, int32_t iStyle)
   {
      UNREFERENCED_PARAMETER(pParent);
      UNREFERENCED_PARAMETER(iStyle);
      return true;
   }



   bool control::CreateWindowsVolumeV001(
      sp(::user::interaction)pParent,
      uint32_t nStartID,
      uint32_t * nNextID)
   {
#ifdef WINDOWS
      ASSERT(m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME);

      static char        szScrollBar[] = TEXT("scrollbar");
      static char        szTitle[]     = TEXT("Fader Class: '%s'");

      uint32_t                        cb;
      uint32_t                        lcChannels;
      uint32_t                        lcMultipleItems;
      int32_t                         nRange;
      int32_t                         nPageInc;


      ASSERT(m_pmixersource != NULL);

      lcChannels = (uint32_t)m_pmixersource->m_mixerline.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
         m_mixercontroldetails.cChannels = 1;

      lcMultipleItems = 1;
      if (MIXERCONTROL_CONTROLF_MULTIPLE & m_mixercontrol.fdwControl)
         lcMultipleItems = (uint32_t)m_mixercontrol.cMultipleItems;

      if(lcChannels > 2)
         return false;
      if(lcMultipleItems != 1)
         return false;

      cb = lcChannels * lcMultipleItems * sizeof(MIXERCONTROLDETAILS_UNSIGNED);
      m_mixercontroldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
      if(m_mixercontroldetails.paDetails != NULL)
         free(m_mixercontroldetails.paDetails);
      m_mixercontroldetails.paDetails = malloc(cb);

      nRange = (int32_t) min(32767, m_mixercontrol.Metrics.cSteps - 1);
      bool bOk = false;
      for(int32_t i = 5; i <= 10; i++)
      {
         nPageInc = nRange / i;
         if(nPageInc < 1)
         {
            bOk = true;
            nPageInc = 1;
            break;
         }
         if(nRange % i == 0)
         {
            bOk = true;
            break;
         }
      }
      if(!bOk)
      {
         for(int32_t i = 5; i <= 10; i++)
         {
            nPageInc = nRange / i;
            if(nRange % i > (nPageInc / 2))
            {
               bOk = true;
               break;
            }
         }
      }

      uint32_t nID = nStartID;
      uint32_t nVolumeID = nID++;
      uint32_t nVolumeLabelID = nID++;
      uint32_t nBalanceID = nID++;
      uint32_t nBalanceLabelID = nID++;

      rect rect(0, 0, 0, 0);

      ::audio_mixer::user::level_control * plevelcontrol = m_pmixersource->get_device()->get_mixer()->get_callback()->allocate_level_control();
      if(plevelcontrol == NULL)
         return false;
      plevelcontrol->Initialize(pParent, nVolumeID);
      ::audio_mixer::control_data_volume & mcdvVolume = plevelcontrol->get_data();
      mcdvVolume.m_iType = ::audio_mixer::control_data::TypeStereoVolume;
      mcdvVolume.m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
      mcdvVolume.m_uiLineID = ((MIXERLINE *) m_pmixersource)->dwLineID;
      mcdvVolume.m_uiControlID = m_mixercontrol.dwControlID;
      mcdvVolume.nRange  = nRange;
      mcdvVolume.nPageInc = nPageInc;
      plevelcontrol->SetOrientation(::audio_mixer::user::level_control::orientation_vertical);
      plevelcontrol->SetRange(0, mcdvVolume.nRange);
      plevelcontrol->SetLineSize(1);
      plevelcontrol->SetPageSize(mcdvVolume.nPageInc);
      *plevelcontrol->m_pdata = mcdvVolume;
      add(plevelcontrol);


      string str;
      ::audio_mixer::user::label * plabelVolume = m_pmixersource->get_device()->get_mixer()->get_callback()->allocate_label();
      if(plabelVolume == NULL)
         return false;
      plabelVolume->Initialize(pParent, nVolumeLabelID);
      //xxx   str.load_string(IDS_MIXER_VOLUME);
      plabelVolume->SetText(str);
      ::audio_mixer::control_data & mcdVolumeLabel = plabelVolume->get_data();
      mcdVolumeLabel.m_iType = ::audio_mixer::control_data::TypeStereoVolumeLabel;
      mcdVolumeLabel.m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
      mcdVolumeLabel.m_uiLineID = ((MIXERLINE *) m_pmixersource)->dwLineID;
      mcdVolumeLabel.m_uiControlID = m_mixercontrol.dwControlID;
      add(plabelVolume);


      if(lcChannels == 2)
      {
         ::audio_mixer::user::level_control * pcontrol = m_pmixersource->get_device()->get_mixer()->get_callback()->allocate_level_control();
         if(pcontrol == NULL)
            return false;

         pcontrol->Initialize(pParent, nBalanceID);
         ::audio_mixer::control_data_volume & mcdvBalance = pcontrol->get_data();
         mcdvBalance.m_iType = ::audio_mixer::control_data::TypeStereoBalance;
         mcdvBalance.m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
         mcdvBalance.m_uiLineID = ((MIXERLINE *) m_pmixersource)->dwLineID;
         mcdvBalance.m_uiControlID = m_mixercontrol.dwControlID;
         mcdvBalance.nRange  = nRange;
         mcdvBalance.nPageInc = nPageInc;

         pcontrol->SetOrientation(::audio_mixer::user::level_control::orientation_horizontal);
         pcontrol->SetRange(0, mcdvBalance.nRange);
         pcontrol->SetLineSize(1);
         pcontrol->SetPageSize(mcdvBalance.nPageInc);

         add(pcontrol);

         ::audio_mixer::user::label * plabel = m_pmixersource->get_device()->get_mixer()->get_callback()->allocate_label();
         if(plabel == NULL)
            return false;

         plabel->Initialize(pParent, nBalanceLabelID);
         ::audio_mixer::control_data & mcdBalanceLabel = plabel->get_data();
         mcdBalanceLabel.m_iType = ::audio_mixer::control_data::TypeStereoBalanceLabel;
         mcdBalanceLabel.m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
         mcdBalanceLabel.m_uiLineID = ((MIXERLINE *) m_pmixersource)->dwLineID;
         mcdBalanceLabel.m_uiControlID = m_mixercontrol.dwControlID;
         //xxx      str.load_string(IDS_MIXER_BALANCE);
         plabel->SetText(str);
         add(plabel);
      }

      *nNextID = nID;
      #endif
      return true;
   }

   bool control::_001CreateMuteControl(
      sp(::user::interaction)pParent,
      uint32_t nStartID,
      uint32_t * nNextID)
   {
   #ifdef WINDOWS
      ASSERT((m_mixercontrol.dwControlType &  MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN);

      uint32_t                        cb;
      uint32_t                        lcChannels;
      uint32_t                        lcMultipleItems;


      ASSERT(m_pmixersource != NULL);

      lcChannels = (uint32_t)m_pmixersource->m_mixerline.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
         lcChannels = 1;

      lcMultipleItems = 1;
      if (MIXERCONTROL_CONTROLF_MULTIPLE & m_mixercontrol.fdwControl)
         lcMultipleItems = (uint32_t)m_mixercontrol.cMultipleItems;

      //if(lcChannels > 2)
      //  return false;
      //if(lcMultipleItems != 1)
      //  return false;

      int32_t iItemCount = lcChannels * lcMultipleItems;

      cb = iItemCount * sizeof(MIXERCONTROLDETAILS_BOOLEAN);
      m_mixercontroldetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
      if(m_mixercontroldetails.paDetails != NULL)
         free(m_mixercontroldetails.paDetails);
      m_mixercontroldetails.paDetails = malloc(cb);

      uint32_t nID = nStartID;
      uint32_t nMuteID;



      string str;
      if(m_pmixersource == m_pmixersource->get_destination())
      {
         str = System.load_string("mix::mute_all");
      }
      else
      {
         str = System.load_string("mix::mute");
      }
      for(int32_t iItem = 0; iItem < iItemCount; iItem++)
      {
         nMuteID = nID++;
         ::audio_mixer::user::toggle_control * pbtMute = m_pmixersource->get_device()->get_mixer()->get_callback()->allocate_toggle_control();
         pbtMute->Initialize(pParent, nMuteID);
         pbtMute->set_label(str);
         ::audio_mixer::control_data_switch & mcdmMute = pbtMute->get_data();
         mcdmMute.m_iType = ::audio_mixer::control_data::TypeUniformMute;
         mcdmMute.m_uiMixerID = m_pmixersource->get_device()->m_uiMixerID;
         mcdmMute.m_uiControlID = m_mixercontrol.dwControlID;
         mcdmMute.m_uiLineID = ((MIXERLINE *) m_pmixersource)->dwLineID;
         add(pbtMute);
      }

      *nNextID = nID;
      #endif
      return true;
   }

   /*void control::OnArrayReallocation(void *pNewPointer)
   {
   m_pmixersource = (::audio_mixer::source *) pNewPointer;
   }
   */

   void control::Prepare()
   {
   #ifdef WINDOWS
      m_mixercontroldetails.dwControlID = m_mixercontrol.dwControlID;
      #endif
   }

   ::audio_mixer::user::control * control::GetControl(int32_t iType)
   {
      ::audio_mixer::user::control  * pusercontrol = NULL;
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         pusercontrol = this->element_at(i);
         if(pusercontrol == NULL)
            continue;
         if(pusercontrol->m_pdata->get_type() == iType)
         {
            return pusercontrol;
         }
      }
      return NULL;
   }

   ::audio_mixer::user::control * control::GetControlByIndex(::index iIndex)
   {
      return this->element_at(iIndex);
   }

   void control::OnMixerControlChange()
   {
      if(this->get_size() <= 0)
         return;
      ::multimedia::result                        mmrc;
      //    oswindow                            htxt;
      //    PMACONTROLINSTANCE_FADER        pmaci_fader;
      //    LPMACONTROLINSTANCE             pmaci;
      //    LPMIXERLINE                     pmxl;
      //    LPMIXERCONTROL                  pmxctrl;
      #ifdef WINDOWS
      PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u;
      #endif
      int32_t                             nRange;
      //    int32_t                             nValue;
      uint32_t                            cChannels;
      uint32_t                            cMultipleItems;
      //    uint32_t                            u;
      //    uint32_t                            uIndex;
      //    uint32_t                            v;
      //    MIXERCONTROLDETAILS             mxcd;
      //   oswindow                            hsbFocus;
      //    oswindow                            hsb;


      //    hsbFocus = GetFocus();
      //    if (NULL == hsbFocus)
      //    {
      //        hsbFocus = get_child_by_id(oswindow, IDD_MACONTROL_MULTICHANNEL_BASE);
      //    }
      //    else
      //    {
      //        uIndex = GetDlgCtrlId(hsbFocus);
      //        if (uIndex < IDD_MACONTROL_MULTICHANNEL_BASE)
      //        {
      //            hsbFocus = get_child_by_id(oswindow, IDD_MACONTROL_MULTICHANNEL_BASE);
      //        }
      //    }


      //
      //
      //
      //    pmaci_fader = (PMACONTROLINSTANCE_FADER)(uint32_t)GetWindowLong(oswindow, DWL_USER);
      //    pmaci       = pmaci_fader->pmaci;
      //    pmxl        = pmaci->pmxl;
      //    pmxctrl     = pmaci->pmxctrl;
      //    pmxcd_u     = &pmaci_fader->pmxcd_u[0];
      //    nRange      = pmaci_fader->nRange;
#ifdef WINDOWS
      cChannels = (uint32_t)m_pmixersource->m_mixerline.cChannels;
      if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
         cChannels = 1;

      m_mixercontroldetails.cChannels = cChannels;


      //
      //
      //
      //    m_mixercontroldetails.cChannels      = cChannels;
      m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;

      mmrc = mixerGetControlDetails(
         (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
         &m_mixercontroldetails,
         MIXER_GETCONTROLDETAILSF_VALUE);
      if (MMSYSERR_NOERROR != mmrc)
      {
         //        System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION,
         //                "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
         //              m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
         return;
      }


      cMultipleItems = 1;
      if (MIXERCONTROL_CONTROLF_MULTIPLE & m_mixercontrol.fdwControl)
         cMultipleItems = (uint32_t)m_mixercontrol.cMultipleItems;
      if(m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME)
      {
         ASSERT(cMultipleItems == 1);
         if(cChannels == 2)
         {
            ::audio_mixer::user::level_control * pslVolume =
               (::audio_mixer::user::level_control *)
               (GetControl(::audio_mixer::control_data::TypeStereoVolume));
            ::audio_mixer::control_data_volume * lpmcdVolume = (::audio_mixer::control_data_volume *) GetWindowData(::audio_mixer::control_data::TypeStereoVolume);
            ::audio_mixer::user::level_control * pslBalance =
               (::audio_mixer::user::level_control *)
               (GetControl(::audio_mixer::control_data::TypeStereoBalance));
            double dActualBalance = pslBalance->GetPos();

            nRange = lpmcdVolume->nRange;
            pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
            int32_t nLeftValue = (int32_t)MulDivRN(pmxcd_u[0].dwValue - m_mixercontrol.Bounds.dwMinimum, nRange, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum);
            int32_t nRightValue = (int32_t)MulDivRN(pmxcd_u[1].dwValue - m_mixercontrol.Bounds.dwMinimum, nRange, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum);


            int32_t nMaxValue = max(nLeftValue, nRightValue);
            int32_t nBalance = nMaxValue == 0 ?
               nRange / 2 :
            (nRange +
               MulDivRN(nRightValue - nLeftValue, nRange, nMaxValue)) / 2;


            pslVolume->SetPos(nRange - nMaxValue);

            double dRangeSlack = nMaxValue == 0 ?
               nRange / 2 :
            ((double) nRange) / (2 * nMaxValue) + 0.5;
            if(dActualBalance > nBalance + dRangeSlack ||
               dActualBalance < nBalance - dRangeSlack)
            {
               pslBalance->SetPos(nBalance);
            }
            TRACE("Left %d Right %d nBalance %d\n", nLeftValue, nRightValue, nBalance);
         }
         else if(cChannels == 1)
         {
            ::audio_mixer::user::level_control * pslVolume = (::audio_mixer::user::level_control *)
               (GetControl(::audio_mixer::control_data::TypeStereoVolume));
            ::audio_mixer::control_data_volume * lpmcdVolume = (::audio_mixer::control_data_volume *) GetWindowData(::audio_mixer::control_data::TypeStereoVolume);

            nRange = lpmcdVolume->nRange;
            pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
            int32_t nValue = (int32_t)MulDivRN(pmxcd_u[0].dwValue - m_mixercontrol.Bounds.dwMinimum, nRange, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum);

            pslVolume->SetPos(nRange - nValue);
         }
      }
      else if((m_mixercontrol.dwControlType & MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN)
      {
         for(int32_t i = 0; i < this->get_size(); i++)
         {
            ::audio_mixer::user::toggle_control * pmutecontrol = (::audio_mixer::user::toggle_control *)
               GetControlByIndex(i);


            PMIXERCONTROLDETAILS_BOOLEAN pmxcd_f = (PMIXERCONTROLDETAILS_BOOLEAN) m_mixercontroldetails.paDetails;
            bool fValue = pmxcd_f[cMultipleItems - i - 1].fValue != FALSE;

            pmutecontrol->_001SetCheck(fValue ? check::checked : check::unchecked);
         }
      }
      #endif
   }

   ::audio_mixer::control_data * control::GetWindowData(int32_t iType)
   {
      ::audio_mixer::user::control * pusercontrol = NULL;
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         pusercontrol = this->element_at(i);
         if(pusercontrol == NULL)
            continue;
         if(pusercontrol->m_pdata->get_type() == iType)
         {
            return pusercontrol->m_pdata;
         }
      }
      return NULL;
   }

   /*bool control::OnNotify(uint32_t nID, LPNMHDR lpnmhdr)
   {
   UNREFERENCED_PARAMETER(nID);
   if(lpnmhdr->code == TB_BOTTOM ||
   lpnmhdr->code == TB_ENDTRACK ||
   lpnmhdr->code == TB_LINEDOWN ||
   lpnmhdr->code == TB_LINEUP ||
   lpnmhdr->code == TB_PAGEDOWN ||
   lpnmhdr->code == TB_PAGEUP ||
   lpnmhdr->code == TB_THUMBPOSITION ||
   lpnmhdr->code == TB_THUMBTRACK ||
   lpnmhdr->code == TB_TOP)
   {
   audio_mixer::control * pcontrol = GetControlByDlgCtrlID(lpnmhdr->idFrom);
   if(pcontrol != NULL)
   {
   //            ::audio_mixer::user::level_control * plevelcontrol = (::audio_mixer::user::level_control *) pcontrol;
   /*if(base < CSliderCtrl >::bases(pusercontrol))
   {
   CSliderCtrl * pSlider = (CSliderCtrl *) pusercontrol;
   ::audio_mixer::control_data * pData;
   if(NULL != (pData = GetWindowDataByDlgCtrlID(lpnmhdr->idFrom)))
   {

   if(pData->get_type() == ::audio_mixer::control_data::TypeStereoBalance ||
   pData->get_type() == ::audio_mixer::control_data::TypeStereoVolume)
   {
   CSliderCtrl * pslBalance;
   CSliderCtrl * pslVolume;
   if(pData->get_type() == ::audio_mixer::control_data::TypeStereoBalance)
   {
   pslBalance = pSlider;
   pslVolume = (CSliderCtrl *) GetControl(::audio_mixer::control_data::TypeStereoVolume)->GetWnd();
   ASSERT(pslVolume != NULL);
   }
   else if(pData->get_type() == ::audio_mixer::control_data::TypeStereoVolume)
   {
   pslVolume = pSlider;
   pslBalance = (CSliderCtrl *) GetControl(::audio_mixer::control_data::TypeStereoBalance)->GetWnd();
   ASSERT(pslVolume != NULL);
   }
   m_mixercontroldetails.cChannels = (uint32_t)m_pmixersource->m_mixerline.cChannels;
   if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
   m_mixercontroldetails.cChannels = 1;

   //
   //
   //
   //                        m_mixercontroldetails.cChannels      = cChannels;
   m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;
   if(m_mixercontroldetails.cChannels == 2)
   {
   ::audio_mixer::control_data_volume * lpmcdVolume = (::audio_mixer::control_data_volume *) GetWindowData(::audio_mixer::control_data::TypeStereoVolume);
   int32_t nRange = lpmcdVolume->nRange;
   int32_t nBalance  = pslBalance->GetPos();
   int32_t nMaxValue = nRange - pslVolume->GetPos();
   int32_t nMinValue = nBalance * 2 * nMaxValue / nRange;
   int32_t nLeftValue;
   int32_t nRightValue;
   if(nBalance == nRange / 2)
   {
   nLeftValue = nMaxValue;
   nRightValue = nMaxValue;
   }
   else if(nBalance > nRange / 2)
   {
   nLeftValue = nMinValue;
   nRightValue = nMaxValue;
   }
   else
   {
   nLeftValue = nMaxValue;
   nRightValue = nMinValue;
   }

   PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
   pmxcd_u[0].dwValue = (int32_t)MulDivRN(
   nLeftValue ,
   m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
   nRange) +  m_mixercontrol.Bounds.dwMinimum;
   pmxcd_u[1].dwValue = (int32_t)MulDivRN(
   nRightValue ,
   m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
   nRange) +  m_mixercontrol.Bounds.dwMinimum;
   ::multimedia::result mmrc = mixerSetControlDetails(
   (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
   &m_mixercontroldetails,
   MIXER_GETCONTROLDETAILSF_VALUE);
   if (MMSYSERR_NOERROR == mmrc)
   {
   OnMixerControlChange();
   }
   else
   {
   //                                System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION,
   //                                        "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
   //                                      m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
   }

   }
   else if(m_mixercontroldetails.cChannels == 1)
   {
   CSliderCtrl * pslVolume = (CSliderCtrl *) GetControl(::audio_mixer::control_data::TypeStereoVolume)->GetWnd();
   ::audio_mixer::control_data_volume * lpmcdVolume = (::audio_mixer::control_data_volume *) GetWindowData(::audio_mixer::control_data::TypeStereoVolume);
   int32_t nRange = lpmcdVolume->nRange;
   int32_t nValue = nRange - pslVolume->GetPos();
   PMIXERCONTROLDETAILS_UNSIGNED pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
   pmxcd_u[0].dwValue = (int32_t)MulDivRN(nValue, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum, nRange) + m_mixercontrol.Bounds.dwMinimum;
   ::multimedia::result mmrc = mixerSetControlDetails(
   (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
   &m_mixercontroldetails,
   MIXER_GETCONTROLDETAILSF_VALUE);
   if (MMSYSERR_NOERROR == mmrc)
   {
   OnMixerControlChange();
   }
   else
   {
   //                                System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION,
   ///                                        "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
   //                                     m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
   }
   }
   return true;
   }
   }*/

   //            }
   /*}

   }
   return false;

   }*/

   ::audio_mixer::user::control * control::GetControlByDlgCtrlID(uint32_t nID)
   {
      ::audio_mixer::user::control * pusercontrol = NULL;
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         pusercontrol = this->element_at(i);
         if(pusercontrol == NULL)
            continue;
         if((uint32_t) pusercontrol->_GetDlgCtrlID() == nID)
         {
            return pusercontrol;
         }
      }
      return NULL;

   }

   ::audio_mixer::control_data * control::GetWindowDataByDlgCtrlID(uint32_t nID)
   {
      ::audio_mixer::user::control * pusercontrol = NULL;
      for(int32_t i = 0; i < this->get_size(); i++)
      {
         pusercontrol = this->element_at(i);
         if(pusercontrol == NULL)
            continue;
         if((uint32_t) pusercontrol->GetWnd()->GetDlgCtrlId() == nID)
         {
            return pusercontrol->m_pdata;
         }
      }
      return NULL;
   }

   void control::OnVHScroll(uint32_t nSBCode, uint32_t nPos, sp(::user::interaction) pScrollBar)
   {
   #ifdef WINDOWS
      UNREFERENCED_PARAMETER(nPos);
      sp(::user::interaction) pParamWnd = pScrollBar;
      if(nSBCode == SB_LEFT ||
         nSBCode == SB_ENDSCROLL ||
         nSBCode == SB_LINELEFT ||
         nSBCode == SB_LINERIGHT ||
         nSBCode == SB_PAGELEFT ||
         nSBCode == SB_PAGERIGHT ||
         nSBCode == SB_THUMBPOSITION ||
         nSBCode == SB_THUMBTRACK ||
         nSBCode == SB_RIGHT ||
         nSBCode == SB_BOTTOM ||
         nSBCode == SB_ENDSCROLL ||
         nSBCode == SB_LINEUP ||
         nSBCode == SB_LINEDOWN ||
         nSBCode == SB_PAGEUP ||
         nSBCode == SB_PAGEDOWN ||
         nSBCode == SB_TOP )
      {
         ::audio_mixer::user::control * pcontrol = GetControlByDlgCtrlID((uint32_t) pParamWnd->GetDlgCtrlId());
         if(pcontrol != NULL)
         {
            ::audio_mixer::user::level_control * pSlider = dynamic_cast<::audio_mixer::user::level_control*>(pcontrol);
            if(NULL != pSlider)
            {
               //mix::SliderInterface * pSlider = dynamic_cast < ::audio_mixer::user::level_control * > ( pusercontrol);
               ::audio_mixer::control_data * pData;
               if(NULL != (pData = GetWindowDataByDlgCtrlID((uint32_t) pParamWnd->GetDlgCtrlId())))
               {

                  if(pData->get_type() == ::audio_mixer::control_data::TypeStereoBalance ||
                     pData->get_type() == ::audio_mixer::control_data::TypeStereoVolume)
                  {
                     ::audio_mixer::user::level_control * pslBalance;
                     ::audio_mixer::user::level_control * pslVolume;
                     if(pData->get_type() == ::audio_mixer::control_data::TypeStereoBalance)
                     {
                        pslBalance = pSlider;
                        pslVolume = dynamic_cast < ::audio_mixer::user::level_control * >
                           (GetControl(::audio_mixer::control_data::TypeStereoVolume));
                        ASSERT(pslVolume != NULL);
                     }
                     else if(pData->get_type() == ::audio_mixer::control_data::TypeStereoVolume)
                     {
                        pslVolume = pSlider;
                        pslBalance = dynamic_cast < ::audio_mixer::user::level_control * >
                           ( GetControl(::audio_mixer::control_data::TypeStereoBalance));
                        ASSERT(pslVolume != NULL);
                     }
                     m_mixercontroldetails.cChannels = (uint32_t)m_pmixersource->m_mixerline.cChannels;
                     if (MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
                        m_mixercontroldetails.cChannels = 1;

                     //
                     //
                     //
                     //m_mixercontroldetails.cChannels      = cChannels;
                     m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;
                     if(m_mixercontroldetails.cChannels == 2)
                     {
                        ::audio_mixer::control_data_volume * lpmcdVolume = (::audio_mixer::control_data_volume *) GetWindowData(::audio_mixer::control_data::TypeStereoVolume);
                        int32_t nRange = lpmcdVolume->nRange;
                        int32_t nBalance  = pslBalance->GetPos();
                        int32_t nMaxValue = nRange - pslVolume->GetPos();
                        int32_t nLeftValue;
                        int32_t nRightValue;
                        if(nBalance == nRange / 2)
                        {
                           nLeftValue = nMaxValue;
                           nRightValue = nMaxValue;
                        }
                        else if(nBalance > nRange / 2)
                        {
                           nLeftValue =  2 * nMaxValue - MulDivRN(nBalance, 2 * nMaxValue, nRange);
                           nRightValue = nMaxValue;
                        }
                        else
                        {
                           nLeftValue = nMaxValue;
                           nRightValue = MulDivRN(nBalance, 2 * nMaxValue, nRange);
                        }

                        PMIXERCONTROLDETAILS_UNSIGNED   pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
                        pmxcd_u[0].dwValue = (int32_t)MulDivRN(
                           nLeftValue ,
                           m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
                           nRange) +  m_mixercontrol.Bounds.dwMinimum;
                        pmxcd_u[1].dwValue = (int32_t)MulDivRN(
                           nRightValue ,
                           m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum,
                           nRange) +  m_mixercontrol.Bounds.dwMinimum;
                        ::multimedia::result mmrc = mixerSetControlDetails(
                           (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
                           &m_mixercontroldetails,
                           MIXER_GETCONTROLDETAILSF_VALUE);
                        if (MMSYSERR_NOERROR == mmrc)
                        {
                           OnMixerControlChange();
                        }
                        else
                        {
                           //                                System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION,
                           //                                        "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                           //                                      m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
                        }

                     }
                     else if(m_mixercontroldetails.cChannels == 1)
                     {
                        //CSliderCtrl * pslVolume = (CSliderCtrl *) GetWindow(::audio_mixer::control_data::TypeStereoVolume);
                        ::audio_mixer::control_data_volume * lpmcdVolume = (::audio_mixer::control_data_volume *) GetWindowData(::audio_mixer::control_data::TypeStereoVolume);
                        int32_t nRange = lpmcdVolume->nRange;
                        int32_t nValue = nRange - pslVolume->GetPos();
                        PMIXERCONTROLDETAILS_UNSIGNED pmxcd_u = (PMIXERCONTROLDETAILS_UNSIGNED) m_mixercontroldetails.paDetails;
                        pmxcd_u[0].dwValue = (int32_t)MulDivRN(nValue, m_mixercontrol.Bounds.dwMaximum - m_mixercontrol.Bounds.dwMinimum, nRange) + m_mixercontrol.Bounds.dwMinimum;
                        ::multimedia::result mmrc = mixerSetControlDetails(
                           (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
                           &m_mixercontroldetails,
                           MIXER_GETCONTROLDETAILSF_VALUE);
                        if (MMSYSERR_NOERROR == mmrc)
                        {
                           OnMixerControlChange();
                        }
                        else
                        {
                           System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION,
                              "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                              m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
                        }
                     }
                     return ;
                  }
               }

            }
         }

      }
      #endif
   }

   bool control::OnCommand(WPARAM wParam, LPARAM lParam)
   {
   #ifdef WINDOWS
      UNREFERENCED_PARAMETER(lParam);
      WORD wNotifyCode = HIWORD(wParam);
      WORD wID = LOWORD(wParam);

      ::audio_mixer::user::control * pwnd = GetControlByDlgCtrlID((uint32_t)wID);

      ::audio_mixer::user::toggle_control * pmutecontrol =
         dynamic_cast < ::audio_mixer::user::toggle_control  * >
         (pwnd);

      if(pmutecontrol != NULL)
      {
         ::audio_mixer::control_data * pData;
         if(NULL != (pData = GetWindowDataByDlgCtrlID((uint32_t) wID)))
         {
            if(pData->get_type() == ::audio_mixer::control_data::TypeUniformMute)
            {
               if(wNotifyCode == BN_CLICKED)
               {
                  m_mixercontroldetails.cChannels = (uint32_t)m_pmixersource->m_mixerline.cChannels;
                  if(MIXERCONTROL_CONTROLF_UNIFORM & m_mixercontrol.fdwControl)
                     m_mixercontroldetails.cChannels = 1;


                  ::index iSel = pmutecontrol->get_data().m_iIndex;

                  //m_mixercontroldetails.cChannels      = cChannels;
                  m_mixercontroldetails.cMultipleItems = m_mixercontrol.cMultipleItems;
                  int32_t cMultipleItems = m_mixercontrol.cMultipleItems;
                  PMIXERCONTROLDETAILS_BOOLEAN   pmxcd_f = (PMIXERCONTROLDETAILS_BOOLEAN) m_mixercontroldetails.paDetails;
                  for(int32_t i = 0; i < this->get_size(); i++)
                  {
                     bool fValue = i == iSel ? 1 : 0;
                     pmxcd_f[cMultipleItems - i - 1].fValue = fValue;
                  }
                  ::multimedia::result mmrc = mixerSetControlDetails(
                     (HMIXEROBJ)m_pmixersource->get_device()->m_hMixer,
                     &m_mixercontroldetails,
                     MIXER_GETCONTROLDETAILSF_VALUE);
                  if(MMSYSERR_NOERROR == mmrc)
                  {
                     OnMixerControlChange();
                  }
                  else
                  {
                     System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION,
                        "mixerGetControlDetails(ctrlid=%.08lXh) failed on hmx=%.04Xh, mmr=%u!",
                        m_mixercontrol.dwControlID, m_pmixersource->get_device()->m_hMixer, mmrc);
                  }
                  return TRUE;
               }
            }
         }
      }
      #endif
      return FALSE;
   }

   control & control::operator =(const control & control)
   {
      m_pmixersource = control.m_pmixersource ;
      return *this;
   }

   uint32_t control::GetControlType() const
   {
      return m_mixercontrol.dwControlType;
   }

   int_ptr control::add(::audio_mixer::user::control * pcontrol)
   {
      pcontrol->m_pdata->m_iIndex = this->get_size();
      return control_ptr_array::add(pcontrol);
   }


   MIXERCONTROL & control::GetMixerControl()
   {
      return m_mixercontrol;
   }
#ifdef WINDOWS
   MIXERCONTROLDETAILS & control::GetMixerControlDetails()
   {
      return m_mixercontroldetails;
   }
#endif

   ::audio_mixer::control_array::control_array(sp(::ca2::application) papp) :
      ca2(papp),
      spa(control) (papp)
   {
   }


} // namespace audio_mixer







