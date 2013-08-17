#include "framework.h"


namespace music
{


   namespace midi_mmsystem
   {


      namespace player
      {


         player_interface::player_interface(sp(::ca2::application) papp) :
            ca2(papp),
            callback(papp)
         {
            m_psection = NULL;
            m_pmidiplayer = NULL;
         }

         player_interface::~player_interface()
         {

         }


         bool player_interface::Initialize(sp(::music::midi::midi) pcentral)
         {

            if(!initialize())
               return false;

            m_psection = pcentral;

            return true;
         }


         bool player_interface::Finalize()
         {

            if(!finalize())
               return false;

            return true;
         }


         bool player_interface::OpenMidiPlayer()
         {
            try
            {
               m_pmidiplayer = __begin_thread < player >(
                  get_app(),
                  ::ca2::scheduling_priority_normal,
                  0,
                  CREATE_SUSPENDED);
            }
            catch(memory_exception *pe)
            {
               System.simple_message_box(NULL, _T("No primitive::memory to perform this operation." ));
               pe->Delete();
               return false;        
            }

            m_pmidiplayer->SetMidiCentral(m_psection);

            m_pmidiplayer->SetCallbackWindow(&m_wnd);
            if(VMS_FAILED(m_pmidiplayer->Initialize(
               GetMidiPlayerCallbackThread())))
            {
               return false;
            }



            if(!OnOpenMidiPlayer())
            {
               return false;
            }
            m_pmidiplayer->ResumeThread();
            m_pmidiplayer->m_evInitialized.wait();
            return true;
         }

         bool player_interface::OnOpenMidiPlayer()
         {
            GetMidiPlayer()->SetInterface(this);
            return true;
         }


         // Event handling
         void player_interface::OnMidiPlayerNotifyEvent(notify_event * pdata)
         {
            callback::OnMidiPlayerNotifyEvent(pdata);
            switch(pdata->m_enotifyevent)
            {
            case music::midi::player::notify_event_set_sequence:
               //      pdata->m_pplayer->GetSequence().m_midicallbackdata.lpThreadV1 = GetMidiPlayerCallbackThread();
               break;
            }

         }

         ::music::midi::sequence & player_interface::GetMidiSequence()
         {
            return *((::music::midi::sequence *)NULL);
         }

         void player_interface::MusicTempoMinus()
         {
            player * pplayer = GetMidiPlayer();
            ::music::midi::sequence & sequence = GetMidiSequence();
            int32_t iTempoShift = sequence.GetTempoShift() - 1;
            if(iTempoShift >= -10 || iTempoShift <= 10)
            {
               pplayer->SetTempoShift(iTempoShift);
               OnChangeMidiPlayerTempoShift();
            }
         }

         void player_interface::MusicTempoReset()
         {
            player * pplayer = GetMidiPlayer();
            pplayer->SetTempoShift(0);
            OnChangeMidiPlayerTempoShift();
         }


         void player_interface::MusicTransposeMinusTone()
         {
            ::music::midi::sequence & sequence = GetMidiSequence();
            int32_t iKeyShift = sequence.GetKeyShift() - 2;
            if(iKeyShift >= -12 || iKeyShift <= 12)
            {
               sequence.SetKeyShift(iKeyShift);
               OnChangeMidiPlayerTranspose();
            }
         }

         void player_interface::MusicUpdateTempoReset(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               pcmdui->Enable(sequence.GetTempoShift() != 0);
               pcmdui->_001SetCheck(sequence.GetTempoShift() == 0 ? check::checked : check::unchecked);
            }
            else
            {
               pcmdui->Enable(false);
               pcmdui->_001SetCheck(check::unchecked);
            }
         }

         void player_interface::MusicTempoPlus()
         {
            player * pplayer = GetMidiPlayer();
            ::music::midi::sequence & sequence = GetMidiSequence();
            int32_t iTempoShift = sequence.GetTempoShift() + 1;
            if(iTempoShift >= -10 || iTempoShift <= 10)
            {
               pplayer->SetTempoShift(iTempoShift);
               OnChangeMidiPlayerTempoShift();
            }
         }

         void player_interface::MusicUpdateTempoPlus(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               int32_t iTempoShift = sequence.GetTempoShift() + 1;
               pcmdui->Enable(iTempoShift <= 10);
            }
            else
            {
               pcmdui->Enable(false);
            }
         }

         void player_interface::MusicUpdateTempoMinus(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               int32_t iTempoShift = sequence.GetTempoShift() - 1;
               pcmdui->Enable(iTempoShift >= -10);
            }
            else
            {
               pcmdui->Enable(false);
            }

         }

         void player_interface::MusicTransposeMinus()
         {
            ::music::midi::sequence & sequence = GetMidiSequence();
            int32_t iKeyShift = sequence.GetKeyShift() - 1;
            if(iKeyShift >= -12 || iKeyShift <= 12)
            {
               sequence.SetKeyShift(iKeyShift);
               OnChangeMidiPlayerTempoShift();
            }

         }

         void player_interface::MusicTransposePlus()
         {
            ::music::midi::sequence & sequence = GetMidiSequence();
            int32_t iKeyShift = sequence.GetKeyShift() + 1;
            if(iKeyShift >= -12 || iKeyShift <= 12)
            {
               sequence.SetKeyShift(iKeyShift);
               OnChangeMidiPlayerTranspose();
            }
         }

         void player_interface::MusicTransposeReset()
         {
            ::music::midi::sequence & sequence = GetMidiSequence();
            sequence.SetKeyShift(0);
            OnChangeMidiPlayerTranspose();
         }

         void player_interface::MusicTransposePlusTone()
         {
            ::music::midi::sequence & sequence = GetMidiSequence();
            int32_t iKeyShift = sequence.GetKeyShift() + 2;
            if(iKeyShift >= -12 || iKeyShift <= 12)
            {
               sequence.SetKeyShift(iKeyShift);
               OnChangeMidiPlayerTranspose();
            }
         }

         void player_interface::MusicUpdateTransposeMinus(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               int32_t iKeyShift = sequence.GetKeyShift() - 1;
               pcmdui->Enable(iKeyShift >= -12);
            }
            else
            {
               pcmdui->Enable(false);
            }
         }

         void player_interface::MusicUpdateTransposePlus(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               int32_t iKeyShift = sequence.GetKeyShift() + 1;
               pcmdui->Enable(iKeyShift <= 12);
            }
            else
            {
               pcmdui->Enable(false);
            }
         }

         void player_interface::MusicUpdateTransposeReset(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               pcmdui->Enable(sequence.GetKeyShift() != 0);
               pcmdui->_001SetCheck(sequence.GetKeyShift() == 0 ? check::checked : check::unchecked);
            }
            else
            {
               pcmdui->Enable(false);
            }
         }

         void player_interface::MusicUpdateTransposePlusTone(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               int32_t iKeyShift = sequence.GetKeyShift() + 1;
               pcmdui->Enable(iKeyShift <= 12);
            }
            else
            {
               pcmdui->Enable(false);
            }

         }

         void player_interface::MusicUpdateTransposeMinusTone(cmd_ui *pcmdui)
         {
            if(GetMidiPlayer())
            {
               ::music::midi::sequence & sequence = GetMidiSequence();
               int32_t iKeyShift = sequence.GetKeyShift() - 2;
               pcmdui->Enable(iKeyShift >= -12);
            }
            else
            {
               pcmdui->Enable(false);
            }
         }



         void player_interface::OnChangeMidiPlayerTempoShift()
         {

         }

         void player_interface::OnChangeMidiPlayerTranspose()
         {

         }


         player * player_interface::GetMidiPlayer()
         {
            return m_pmidiplayer;
         }


      } // namespace player


   } // namespace midi_mmsystem


} // namespace music









