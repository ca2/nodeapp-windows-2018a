#include "framework.h"


namespace audio_mixer
{


   device::device(::audio_mixer::audio_mixer * pmixer)
   {

      m_mixerdestinationa.set_app(pmixer->get_app());

      m_pmixer = pmixer;
#ifdef WINDOWS
      m_hMixer = NULL;
#endif
   }


   device::~device()
   {

      close();

   }


   ::multimedia::result device::open(uint32_t uiMixerId, uint32_t dwCallback, uint32_t dwInstance, uint32_t fdwOpen)
   {

    #ifdef WINDOWS
      ::multimedia::result            mmrc;
      HMIXER              hmx;
      MIXERCAPS           mxcaps;


      if (m_hMixer != NULL)
      {

         ::multimedia::result mmrct = close();

         if (MMSYSERR_NOERROR != mmrct)
         {

            System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerClose() failed on hmx=%.04Xh, mmr=%u!", m_hMixer, mmrct);

         }

      }

      mmrc = mixerGetDevCaps(uiMixerId, &mxcaps, sizeof(mxcaps));

      if (MMSYSERR_NOERROR != mmrc)
      {

         System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerGetDevCaps() failed on uMxId=%u, mmr=%u!", uiMixerId, mmrc);

         return mmrc;

      }

      mmrc = mixerOpen(&hmx, uiMixerId, dwCallback, dwInstance, fdwOpen);

      if (MMSYSERR_NOERROR != mmrc)
      {

         System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerOpen() failed on uMxId=%u, mmr=%u!", uiMixerId, mmrc);

         return mmrc;

      }



      m_hMixer = hmx;

      m_uiMixerID = uiMixerId;

      //    AppSetWindowText(oswindow, "::audio_mixer::audio_mixer Device: %s", (char *)mxcaps.szPname);

      return mmrc;
#else
        return MMSYSERR_ERROR;
#endif

   }


   ::multimedia::result device::initialize_capabilities()
   {
#ifdef WINDOWS
      ::multimedia::result mmrc;

      mmrc = mixerGetDevCaps((uint32_t) m_hMixer, &m_mixercaps, sizeof(MIXERCAPS));

      if (MMSYSERR_NOERROR != mmrc)
      {

         System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerGetDevCaps() failed on uMxId=%u, mmr=%u!", m_uiMixerID, mmrc);

         return mmrc;

      }

      return mmrc;
#else
        return MMSYSERR_ERROR;
#endif

   }


   ::multimedia::result device::initialize_destinations()
   {
#ifdef WINDOWS
      ::audio_mixer::destination *    lpDestination;

      m_mixerdestinationa.set_size(m_mixercaps.cDestinations);

      for (int32_t i = 0; (uint32_t) i < m_mixercaps.cDestinations; i++)
      {
         lpDestination = m_mixerdestinationa(i);
         lpDestination->set_device(this);
         lpDestination->mixerGetLineInfo(0, i, MIXER_GETLINEINFOF_DESTINATION);
         //        ::audio_mixer::audio_mixer::get_component_name(lpmxl, lpDestination->m_strComponent);

         //        //
         //      //
         //    //
         //  wsprintf(ach, gszLineFormatList,
         //         (char *)szLineTypeDst,
         //       (MIXERLINE_LINEF_ACTIVE & mxl.fdwLine) ? '*' : ' ',
         //     (LPTSTR)szComponent,
         //   mxl.dwLineID,
         //   mxl.fdwLine,
         // mxl.cControls,
         // mxl.cConnections,
         //(char *)mxl.szName);

         //        if (0 != (APP_OPTF_DEBUGLOG & gfuAppOptions))
         //      {
         //        MixAppDebugLog(ach);
         //      MixAppDebugLog(gszCRLF);
         //  }

         //        nIndex = ListBox_AddString(ptlb->hlb, ach);
         //      ListBox_SetItemData(ptlb->hlb, nIndex, mxl.dwLineID);

      }


      //
      //
      //
      //    SetWindowRedraw(ptlb->hlb, TRUE);
      return MMSYSERR_NOERROR;
#else
        return MMSYSERR_ERROR;
#endif

   }

   ::multimedia::result device::get_destination(uint32_t dwComponentType, ::audio_mixer::destination **ppDestination)
   {
   #ifdef WINDOWS
      if(m_mixerdestinationa.get_size() <= 0)
         initialize_destinations();
      if(m_mixerdestinationa.get_size() <= 0)
         return MMSYSERR_ERROR;
      for(int32_t i = 0; i < m_mixerdestinationa.get_size(); i++)
      {
         uint32_t dw = m_mixerdestinationa[i].m_mixerline.dwComponentType;
         if(dw == dwComponentType)
         {
            *ppDestination = m_mixerdestinationa(i);
            return MMSYSERR_NOERROR;
         }
      }
      return MMSYSERR_ERROR;
      #else
        return MMSYSERR_ERROR;
#endif

   }

   void device::map_controls()
   {
      m_mapIDToControl.remove_all();

      for(int32_t i = 0; i < m_mixerdestinationa.get_size(); i++)
      {
         ::audio_mixer::destination & destination = m_mixerdestinationa[i];
         MapLineControls(&destination);
         ::audio_mixer::source_array & sourcea = destination.get_source_info();
         for(int32_t j = 0; j < sourcea.get_size(); j++)
         {
            ::audio_mixer::source & source = sourcea[j];
            MapLineControls(&source);
         }
      }
   }

   void device::map_lines()
   {
      m_mapIDToLine.remove_all();
      for(int32_t i = 0; i < m_mixerdestinationa.get_size(); i++)
      {
         ::audio_mixer::destination & destination = m_mixerdestinationa[i];
         m_mapIDToLine.set_at(
            destination.get_mixer_line().dwLineID,
            &destination);
         ::audio_mixer::source_array & sourcea = destination.get_source_info();
         for(int32_t j = 0; j < sourcea.get_size(); j++)
         {
            ::audio_mixer::source & source = sourcea[j];
            m_mapIDToLine.set_at(source.get_mixer_line().dwLineID, &source);
         }
      }
   }

   void device::OnMixerLineChange(uint32_t dwLineID)
   {
      ::audio_mixer::source * pSource;
      if(m_mapIDToLine.Lookup(dwLineID, pSource))
      {
         pSource->OnMixerLineChange();
      }
   }

   void device::OnMixerControlChange(uint32_t dwControlID)
   {
      ::audio_mixer::control * pControl;
      if(m_mapIDToControl.Lookup(dwControlID, pControl))
      {
         pControl->OnMixerControlChange();
      }
   }

   void device::MapLineControls(::audio_mixer::source * psource)
   {
      ::audio_mixer::control_array & controla = psource->get_control_array();
      for(int32_t k = 0; k < controla.get_size(); k++)
      {
         ::audio_mixer::control & control = controla[k];
         m_mapIDToControl.set_at(control.GetMixerControl().dwControlID, &control);
         for(int32_t l = 0; l < control.get_size(); l++)
         {
            ::audio_mixer::user::control * pcontrol = control[l];
            m_mapDlgItemIDToControl.set_at(pcontrol->_GetDlgCtrlID(), &control);
         }
      }
   }

   void device::MapDlgCtrlIDToControls()
   {
      m_mapDlgItemIDToControl.remove_all();

      ::audio_mixer::destination_array & destinationa = m_mixerdestinationa;
      for(int32_t i = 0; i < destinationa.get_size(); i++)
      {
         ::audio_mixer::destination & destination = destinationa[i];
         MapDlgCtrlIDToLineControls(&destination);
         ::audio_mixer::source_array & sourcea = destination.get_source_info();
         for(int32_t j = 0; j < sourcea.get_size(); j++)
         {
            ::audio_mixer::source & source = sourcea[j];
            MapDlgCtrlIDToLineControls(&source);
         }
      }

   }

   void device::MapDlgCtrlIDToLineControls(::audio_mixer::source * psource)
   {
      ::audio_mixer::control_array & controla = psource->get_control_array();
      for(int32_t k = 0; k < controla.get_size(); k++)
      {
         ::audio_mixer::control & control = controla[k];
         for(int32_t l = 0; l < control.get_size(); l++)
         {
            ::audio_mixer::user::control * pcontrol = control[l];
            m_mapDlgItemIDToControl.set_at(pcontrol->_GetDlgCtrlID(), &control);
         }
      }
   }


   ::multimedia::result device::close()
   {
#ifdef WINDOWS
      ::multimedia::result mmrc;

      if(m_hMixer != NULL)
      {

         mmrc = mixerClose(m_hMixer);

         m_mixerdestinationa.remove_all();

         m_mapIDToControl.remove_all();

         m_mapDlgItemIDToControl.remove_all();

         m_mapIDToLine.remove_all();

         m_hMixer = NULL;

      }

      return mmrc;
#else
        return MMSYSERR_ERROR;
#endif

   }

   bool device::OnCommand(WPARAM wparam, LPARAM lparam)
   {

      uint32_t uiID = LOWORD(wparam);

      ::audio_mixer::control * pcontrol;

      if(m_mapDlgItemIDToControl.Lookup(uiID, pcontrol)
         && pcontrol->OnCommand(wparam, lparam))
         return true;

      return false;

   }

   const char * device::GetSZPName()
   {
   #ifdef WINDOWS
      return m_mixercaps.szPname;
      #else
      return NULL;
      #endif
   }


   ::audio_mixer::audio_mixer * device::get_mixer()
   {
      return m_pmixer;
   }


} // namespace audio_mixer





