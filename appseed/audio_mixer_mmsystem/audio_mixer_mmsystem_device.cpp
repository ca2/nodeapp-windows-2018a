#include "framework.h"


namespace multimedia
{


   namespace audio_mixer_mmsystem
   {


      device::device(::aura::application * papp) :
         ::object(papp),
         ::multimedia::audio_mixer::device(papp)
         //m_mixerdestinationa(papp)
      {

         //m_mixerdestinationa.set_app(get_app());

         m_pmixer = NULL;

         m_hMixer = NULL;

      }


      device::~device()
      {

         close();

      }

      bool device::initialize(::multimedia::audio_mixer::audio_mixer * pmixer)
      {

         m_pmixer = pmixer;

         return true;

      }

      ::multimedia::e_result device::open(uint32_t uiMixerId, uint32_t dwCallback, uint32_t dwInstance, uint32_t fdwOpen)
      {

         ::multimedia::e_result            mmrc;
         HMIXER              hmx;
         MIXERCAPS           mxcaps;


         if (m_hMixer != NULL)
         {

            ::multimedia::e_result mmrct = close();

            if(::multimedia::result_success != mmrct)
            {

               System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerClose() failed on hmx=%.04Xh, mmr=%u!", m_hMixer, mmrct);

            }

         }

         mmrc = mmsystem::translate(mixerGetDevCaps(uiMixerId, &mxcaps, sizeof(mxcaps)));

         if(::multimedia::result_success != mmrc)
         {

            System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerGetDevCaps() failed on uMxId=%u, mmr=%u!", uiMixerId, mmrc);

            return mmrc;

         }

         mmrc = mmsystem::translate(mixerOpen(&hmx, uiMixerId, dwCallback, dwInstance, fdwOpen));

         if(::multimedia::result_success != mmrc)
         {

            System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerOpen() failed on uMxId=%u, mmr=%u!", uiMixerId, mmrc);

            return mmrc;

         }



         m_hMixer = hmx;

         m_uiMixerID = uiMixerId;

         //    AppSetWindowText(oswindow, "::multimedia::audio_mixer::audio_mixer Device: %s", (char *)mxcaps.szPname);

         return mmrc;

      }


      ::multimedia::e_result device::initialize_capabilities()
      {

         ::multimedia::e_result mmrc;

         mmrc = mmsystem::translate(mixerGetDevCaps((UINT_PTR) m_hMixer, &m_mixercaps, sizeof(MIXERCAPS)));

         if(::multimedia::result_success != mmrc)
         {

            System.simple_message_box(NULL, MB_OK | MB_ICONEXCLAMATION, "mixerGetDevCaps() failed on uMxId=%u, mmr=%u!", m_uiMixerID, mmrc);

            return mmrc;

         }

         return mmrc;

      }


      ::multimedia::e_result device::initialize_destinations()
      {

         sp(::multimedia::audio_mixer_mmsystem::destination)    lpDestination;

         m_mixerdestinationa.set_size_create(m_mixercaps.cDestinations);

         for (int32_t i = 0; (uint32_t) i < m_mixercaps.cDestinations; i++)
         {
            lpDestination = m_mixerdestinationa[i];
            lpDestination->set_device(this);
            lpDestination->mixerGetLineInfo(0, i, MIXER_GETLINEINFOF_DESTINATION);
            //        ::multimedia::audio_mixer::audio_mixer::get_component_name(lpmxl, lpDestination->m_strComponent);

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
         return ::multimedia::result_success;

      }

      ::multimedia::e_result device::get_destination(::multimedia::audio_mixer::e_destination edestination, ::multimedia::audio_mixer::destination **ppDestination)
      {
         
         uint32_t dwComponentType;

         switch(edestination)
         {
         case ::multimedia::audio_mixer::destination_speakers:
            dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
            break;

         default:
            return ::multimedia::result_error;
         };


         if(m_mixerdestinationa.get_size() <= 0)
            initialize_destinations();

         if(m_mixerdestinationa.get_size() <= 0)
            return ::multimedia::result_error;

         for(int32_t i = 0; i < m_mixerdestinationa.get_size(); i++)
         {

            sp(::multimedia::audio_mixer_mmsystem::destination) destination = m_mixerdestinationa[i];

            uint32_t dw = destination->m_mixerline.dwComponentType;

            if(dw == dwComponentType)
            {

               *ppDestination = m_mixerdestinationa[i];

               return ::multimedia::result_success;

            }

         }

         return ::multimedia::result_error;

      }

      void device::map_controls()
      {

         m_mapIDToControl.remove_all();

         for(int32_t i = 0; i < m_mixerdestinationa.get_size(); i++)
         {

            sp(::multimedia::audio_mixer_mmsystem::destination) destination = m_mixerdestinationa[i];

            MapLineControls(destination);

            ::multimedia::audio_mixer::source_array & sourcea = destination->get_source_info();

            for(int32_t j = 0; j < sourcea.get_size(); j++)
            {

               sp(::multimedia::audio_mixer::source) source = sourcea[j];

               MapLineControls(source);

            }

         }

      }


      void device::map_lines()
      {
         
         m_mapIDToLine.remove_all();
         
         for(int32_t i = 0; i < m_mixerdestinationa.get_size(); i++)
         {
            
            sp(::multimedia::audio_mixer_mmsystem::destination) destination = m_mixerdestinationa[i];
            
            m_mapIDToLine.set_at(destination->get_mixer_line().dwLineID, destination);

            ::multimedia::audio_mixer::source_array & sourcea = destination->get_source_info();
            
            for(int32_t j = 0; j < sourcea.get_size(); j++)
            {

               sp(::multimedia::audio_mixer_mmsystem::source) source = sourcea[j];

               m_mapIDToLine.set_at(source->m_mixerline.dwLineID, source);

            }

         }

      }

      void device::OnMixerLineChange(uint32_t dwLineID)
      {
         ::multimedia::audio_mixer::source * pSource;
         if(m_mapIDToLine.Lookup(dwLineID, pSource))
         {
            pSource->OnMixerLineChange();
         }
      }

      void device::OnMixerControlChange(uint32_t dwControlID)
      {
         ::multimedia::audio_mixer::control * pControl;
         if(m_mapIDToControl.Lookup(dwControlID, pControl))
         {
            pControl->OnMixerControlChange();
         }
      }

      void device::MapLineControls(::multimedia::audio_mixer::source * psource)
      {
         ::multimedia::audio_mixer::control_array & controla = psource->get_control_array();
         for(int32_t k = 0; k < controla.get_size(); k++)
         {
            sp(::multimedia::audio_mixer_mmsystem::control) control = controla[k];
            m_mapIDToControl.set_at(control->GetMixerControl().dwControlID, control);
            for(int32_t l = 0; l < control->get_size(); l++)
            {
               sp(::multimedia::audio_mixer::user::control) pcontrol = control->operator [](l);
               m_mapDlgItemIDToControl.set_at(pcontrol->GetDlgCtrlId(), control);
            }
         }
      }

      void device::MapDlgCtrlIDToControls()
      {
         m_mapDlgItemIDToControl.remove_all();

         ::multimedia::audio_mixer::destination_array & destinationa = m_mixerdestinationa;
         for(int32_t i = 0; i < destinationa.get_size(); i++)
         {
            sp(::multimedia::audio_mixer_mmsystem::destination) destination = destinationa[i];
            MapDlgCtrlIDToLineControls(destination);
            ::multimedia::audio_mixer::source_array & sourcea = destination->get_source_info();
            for(int32_t j = 0; j < sourcea.get_size(); j++)
            {
               ::multimedia::audio_mixer::source & source = sourcea(j);
               MapDlgCtrlIDToLineControls(&source);
            }
         }

      }

      void device::MapDlgCtrlIDToLineControls(::multimedia::audio_mixer::source * psource)
      {
         ::multimedia::audio_mixer::control_array & controla = psource->get_control_array();
         for(int32_t k = 0; k < controla.get_size(); k++)
         {
            sp(::multimedia::audio_mixer::control) control = controla[k];
            for(int32_t l = 0; l < control->get_size(); l++)
            {
               sp(::multimedia::audio_mixer::user::control) pcontrol = control->operator[](l);
               m_mapDlgItemIDToControl.set_at(pcontrol->GetDlgCtrlId(), control);
            }
         }
      }


      ::multimedia::e_result device::close()
      {

         ::multimedia::e_result mmrc = ::multimedia::result_success;

         if(m_hMixer != NULL)
         {

            mmrc = mmsystem::translate(mixerClose(m_hMixer));

            m_mixerdestinationa.remove_all();

            m_mapIDToControl.remove_all();

            m_mapDlgItemIDToControl.remove_all();

            m_mapIDToLine.remove_all();

            m_hMixer = NULL;

         }

         return mmrc;

      }

      bool device::OnCommand(WPARAM wparam, LPARAM lparam)
      {

         uint32_t uiID = LOWORD(wparam);

         ::multimedia::audio_mixer::control * pcontrol;

         if(m_mapDlgItemIDToControl.Lookup(uiID, pcontrol)
            && pcontrol->OnCommand(wparam, lparam))
            return true;

         return false;

      }

      const char * device::GetSZPName()
      {

         return m_mixercaps.szPname;

      }


      ::multimedia::audio_mixer::audio_mixer * device::get_mixer()
      {
         return m_pmixer;
      }


   } // namespace audio_mixer_mmsystem


} // namespace multimedia


