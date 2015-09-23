#pragma once


#define  VERSION_MINOR              0x00
#define  VERSION_MAJOR              0x04
#define  SEQ_VERSION                ((uint32_t)(WORD)((BYTE)VERSION_MINOR | (((WORD)(BYTE)VERSION_MAJOR) << 8)))

#define MMSG_DONE                   (WM_USER+20)


namespace music
{


   namespace midi
   {


      namespace mmsystem
      {


         class buffer;
         class sequence;
         class sequence_thread;


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM midi_callback_data
         {
         public:


            sequence *          m_psequence;


         };


         class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM sequence :
            virtual public ::music::midi::sequence
         {
         public:

            class buffer_array;


            class buffer :
               virtual public ::music::midi::object
            {
            public:


               LPMIDIHDR            m_lpmidihdr;
               memory    m_storage;
               bool                 m_bPrepared;
               buffer_array *       m_pbuffera;


               buffer(::aura::application * papp);
               virtual ~buffer();

               bool IsPrepared();
               void Reset();
               void Initialize(int32_t iSize, buffer_array * pbuffera);

               ::multimedia::e_result midiStreamOut(HMIDISTRM hmidiout);
               ::multimedia::e_result midiOutPrepareHeader(HMIDIOUT hmidiout);
               ::multimedia::e_result midiOutUnprepareHeader(HMIDIOUT hmidiout);


               inline LPMIDIHDR GetMidiHdr() { return m_lpmidihdr; }

               //void SetNextMidiHdr(LPMIDIHDR lpNext);

               midi_callback_data * get_midi_callback_data();

               static buffer * get(LPMIDIHDR lpmidihdr);

               static sequence * get_sequence(LPMIDIHDR lpmidihdr);

            };


            class buffer_array :
               virtual public spa(buffer)
            {
            public:

               
               midi_callback_data * m_pcallbackdata;


               buffer_array(::aura::application * papp);
               virtual ~buffer_array();

               void Reset();
               void Initialize(int32_t iCount, int32_t iSize, midi_callback_data * pcallbackdata);


               ::multimedia::e_result midiStreamOut(HMIDISTRM hmidiout);
               ::multimedia::e_result midiOutPrepareHeader(HMIDIOUT hmidiout);
               ::multimedia::e_result midiOutUnprepareHeader(HMIDIOUT hmidiout);

            };

            class event :
               public ::music::midi::sequence::event
            {
            public:


               LPMIDIHDR                     m_lpmh;


            };

            uint32_t                      m_cbPreroll;          /* Streaming buffers -- size of lpmhPreroll     */

            uint32_t                      m_cbPrerollNominalMax;

            buffer_array                  m_buffera;

            HMIDISTRM                     m_hstream;

            midi_callback_data            m_midicallbackdata;

            int32_t                       m_iBuffersInMMSYSTEM;



            sequence(::aura::application * papp);
            virtual ~sequence();


            inline uint32_t GetState();
            inline uint32_t GetPreviousState();
            uint32_t SetState(uint32_t uiState);

            void SetSpecialModeV001Flag(bool bSet = true);

            void MuteAll(bool bMute = true, int32_t iExcludeTrack = -1);
            void MuteTrack(int32_t iIndex, bool bMute = true);


            virtual int32_t GetDefaultCodePage();

            void Prepare(::ikaraoke::data & data);
            void Prepare(int32_t iTrack, ::ikaraoke::data & data);
            void Prepare(
               string2a & str2a,
               imedia::position_2darray & tka2DTokensTicks,
               int32_t iMelodyTrack,
               int2a & ia2TokenLine,
               ::ikaraoke::data & data);

            imedia::position GetPositionTicks();
            void SetLevelMeter(int32_t iLevel);
            ::multimedia::e_result CloseStream();
            ::music::e_result close_device();
            bool SetMidiOutDevice(uint32_t uiDevice);
            int32_t SetKeyShift(int32_t iKeyShift);
            int32_t GetKeyShift();

            void OnMidiPlaybackEnd(::music::midi::sequence::event * pevent);
            virtual ::music::e_result SetTempoShift(double dTempoShift);

            void OnPositionCB(LPMIDIHDR lpmidihdr);
            void OnDone(HMIDISTRM hmidistream, LPMIDIHDR lpmidihdr);
            virtual void GetTimeLength(imedia::time & time);
            virtual void GetPositionLength(imedia::position & position);

            using ::ikaraoke::karaoke::TimeToPosition;
            using ::ikaraoke::karaoke::PositionToTime;

            virtual imedia::position TimeToPosition(imedia::time time);
            virtual imedia::time PositionToTime(imedia::position position);

            virtual bool IsOpened();

            virtual bool IsNull();
            virtual void SetTempoChangeFlag(bool bSet = true);
            virtual bool IsChangingTempo();

            virtual double GetTempoShift();
            virtual void GetMidiDoneData(::music::midi::LPMIDIDONEDATA lpmdd);
            virtual bool IsInSpecialModeV001();
            virtual bool WasInSpecialModeV001();
            virtual void SetSpecialModeV001Operation(uint32_t uiOperation);

            virtual void OnEvent(::music::midi::sequence::event * pevent);


            ::multimedia::e_result AllocBuffers();

            VOID FreeBuffers();

            //::multimedia::e_result OpenFile(const char * lpFileName, int32_t openMode);
            e_result OpenFile(::music::midi::sequence & sequence, int32_t iOpenMode);
            e_result OpenFile(::file::stream_buffer & ar, int32_t openMode);
            e_result OpenFile(const char * lpFileName, int32_t openMode);
            e_result OpenFile(memory * pmemorystorage, int32_t openMode, ::music::e_storage estorage);

            e_result CloseFile();
            e_result SaveFile(const char * lpFileName);
            e_result SaveFile();
            e_result SaveFile(::file::buffer_sp &ar);

            ::multimedia::e_result Preroll(::thread * pthread, ::music::midi::LPPREROLL lpPreroll, bool bThrow);
            ::multimedia::e_result Start();

            ::multimedia::e_result Pause();

            ::multimedia::e_result Restart();

            //::multimedia::e_result Stop(uint32_t dwEllapse);
            ::multimedia::e_result Stop();

            void GetPosition(imedia::position  & time);
            void get_time(imedia::time  & time);

            ::multimedia::e_result get_ticks(imedia::position & time);
            ::multimedia::e_result get_millis(imedia::time & time);


            imedia::position MillisecsToTicks(imedia::time msOffset);

            imedia::time TicksToMillisecs(imedia::position tkOffset);

            bool IsPlaying();

            // static void CALLBACK MidiOutProc(HMIDIOUT hmo, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);


            bool IsSettingPosition();
            void SetSettingPositionFlag(bool bSet = TRUE);

            imedia::position GetQuarterNote();


            inline sp(::music::midi::mmsystem::buffer) file()
            {
               return get_file();
            }

            inline sp(::music::midi::mmsystem::sequence_thread) thread()
            {
               return m_pthread;
            }

            using ::music::midi::sequence::create_new_event;
            virtual ::music::midi::sequence::event * create_new_event(::music::midi::sequence::e_event eevent, LPMIDIHDR lpmidihdr);


            virtual ::multimedia::e_result SendGMReset();


            virtual ::multimedia::e_result mm_start();

         };


      } // namespace mmsystem


   } // namespace midi


} // namespace music







