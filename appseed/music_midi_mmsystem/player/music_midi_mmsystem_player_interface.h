#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      namespace player
      {



         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM player_interface :
            virtual public callback
         {
         public:


            sp(::music::midi::midi)             m_psection;
            player                         * m_pmidiplayer;


            player_interface(sp(::ca2::application) papp);
            virtual ~player_interface();


            virtual ::ca2::thread * GetMidiPlayerCallbackThread() = 0;
            virtual ::music::midi::sequence & get_midi_sequence() = 0;
            virtual bool OnOpenMidiPlayer();
            player * GetMidiPlayer();


            virtual bool Initialize(sp(::music::midi::midi) pcentral);
            virtual bool Finalize();


            bool OpenMidiPlayer();
            void MusicTempoMinus();
            void MusicTempoReset();
            void MusicUpdateTransposeMinusTone(cmd_ui * pcmdui);
            void MusicUpdateTransposePlusTone(cmd_ui * pcmdui);
            void MusicUpdateTransposeReset(cmd_ui * pcmdui);
            void MusicUpdateTransposePlus(cmd_ui * pcmdui);
            void MusicUpdateTransposeMinus(cmd_ui * pcmdui);
            void MusicUpdateTempoMinus(cmd_ui * pcmdui);
            void MusicUpdateTempoPlus(cmd_ui * pcmdui);
            void MusicUpdateTempoReset(cmd_ui * pcmdui);
            void MusicTransposeMinusTone();
            void MusicTransposePlusTone();
            void MusicTransposeReset();
            void MusicTransposePlus();
            void MusicTransposeMinus();
            void MusicTempoPlus();


            virtual void OnMidiPlayerNotifyEvent(notify_event * pdata); 

            virtual void OnChangeMidiPlayerTranspose();
            virtual void OnChangeMidiPlayerTempoShift();

         };


      } // namespace player


   } // namespace midi_mmsystem


} // namespace music


