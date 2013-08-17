#pragma once


#define  VERSION_MINOR              0x00
#define  VERSION_MAJOR              0x04
#define  SEQ_VERSION                ((uint32_t)(WORD)((BYTE)VERSION_MINOR | (((WORD)(BYTE)VERSION_MAJOR) << 8)))

//#define SEQ_F_EOF           0x00000001L
//#define SEQ_F_COLONIZED     0x00000002L
//#define SEQ_F_WAITING       0x00000004L
//#define SEQ_F_SETTING_POS   0x00000008L

//#define SEQ_S_NOFILE        0
//#define SEQ_S_OPENED        1
//#define SEQ_S_PREROLLING    2
//#define SEQ_S_PREROLLED     3
//#define SEQ_S_PLAYING       4
//#define SEQ_S_PAUSED        5
//#define SEQ_S_STOPPING      6
//#define SEQ_S_RESET         7

#define MMSG_DONE                   (WM_USER+20)




namespace music
{

   
   namespace midi_mmsystem
   {
      
      
      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM midi_out
      {
      public:
         
         
#ifdef WINDOWS
         
         
         HMIDIOUT         m_hstream;
         
         
#elif defined(MACOS)
         
         
         MIDIPortRef       m_port;
         MIDIEndpointRef   m_endpoint;
         
         
#endif
         
         
      };
      
      
#ifdef WINDOWS
      
      
      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM midi_stream
      {
      public:
         
         
         HMIDISTRM         m_hstream;
         
         
      };
      
      
#elif defined(MACOS)
      
      
      typedef midi_out midi_stream;
   
      
#endif
      
      
      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM midi_pack
      {
      public:
         
         
#ifdef WINDOWS
         

         MIDIHDR              m_midihdr;
         primitive::memory    m_storage;
         bool                 m_bPrepared;
         

#elif defined(MACOS)

         MIDIPacketList * m_packetlist;
         
         
#endif
         
         
      };
      

      

      typedef struct tag_preroll
      {
         imedia::position       tkBase;
         imedia::position       tkEnd;
      } PREROLL, FAR *LPPREROLL;

      struct midi_callback_data
      {
         sequence *          m_psequence;
      };






      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM sequence :
         virtual public ::ikaraoke::karaoke
      {
      public:
         class BufferArray;

         class buffer
         {
         public:
            
            
            midi_pack   m_pack;

            
            bool IsPrepared();
            ::multimedia::result midiStreamOut(midi_stream hmidiout);
            ::multimedia::result midiOutPrepareHeader(midi_out hmidiout);
            friend class BufferArray;

            midi_pack * get_pack();
            ::multimedia::result midiOutUnprepareHeader(midi_out hmidiout);
            void Reset();
            void Initialize(int32_t iSize, uint32_t dwUser);
            void SetNextMidiHdr(LPMIDIHDR);
            
         
            
         };

         class BufferArray :
            public array < buffer, buffer >
         {
         public:
            ::multimedia::result midiStreamOut(midi_stream hmidiout);
            ::multimedia::result midiOutPrepareHeader(midi_out hmidiout);
            ::multimedia::result midiOutUnprepareHeader(midi_out hmidiout);
            void Reset();
            void Initialize(int32_t iCount, int32_t iSize, uint32_t dwUser);
         };

         enum e_flag
         {
            FlagNull                        = (long) 0x00000000,
            FlagEOF                        = (long) 0x00000001,
            FlagColonized                 = (long) 0x00000002,
            FlagWaiting                   = (long) 0x00000004,
            FlagSettingPos                  = (long) 0x00000008,
            FlagIsInSpecialModeV001       = (long) 0x00000010,
            FlagWasInSpecialModeV001      = (long) 0x00000020,
            FlagSpecialModeV001End        = (long) 0x00000040,
            FlagTempoChange               = (long) 0x00000080,
            FlagStop                        = (long) 0x00000100,
            FlagMidiOutDeviceChange         = (long) 0x00000200,
            FlagStopAndRestart            = (long) 0x00000400,

         };

         class flags :
            public ::flags<e_flag>
         {
         public:
         };

         enum SpecialModeV001Operations
         {
            SPMV001GMReset = 1,
            SPMV001TempoChange = 2
         };
         enum Status
         {
            StatusNoFile = 0,
            StatusOpened = 1,
            StatusPreRolling = 2,
            StatusPreRolled = 3,
            StatusPlaying = 4,
            StatusPaused = 5,
            StatusStopping = 6,
            StatusReset = 7,
            StatusSuccess = 8
         };


         enum e_event
         {
            EventMidiPlaybackStart,
            EventStopped,
            EventMidiPlaybackEnd,
            EventMidiStreamOut,
            EventSpecialModeV001End,
         };

         class Event
         {
         public:
            e_event            m_eevent;
            sequence *    m_psequence;
            LPMIDIHDR         m_lpmh;
         };

         class PlayerLink
         {
         public:
            void ModifyFlag(e_flag eflagAdd, e_flag eflagRemove);
            bool TestFlag(e_flag eflag);
            void SetFlag(e_flag eflag);
            e_flag GetFlag();
            e_flag                  m_eflag;
            imedia::position                  m_tkRestart;
            ::ca::smart_pointer < player::command >  m_spcommand;
            void OnFinishCommand(player::e_command ecommand);
            void SetCommand(player::command * pcommand);
            PlayerLink();
         };

         BufferArray     m_buffera;

         event  m_evMmsgDone;

         event            m_eventMidiPlaybackEnd;
         mutex            m_mutex;
         uint32_t           m_uiDeviceID;          /* Requested MIDI device ID for MMSYSTEM        */
         uint32_t           m_uiState;             /* Sequencer state (SEQ_S_xxx)                  */
         uint32_t            m_uiPreviousState;
         uint32_t           m_uiPreSpecialModeV001State;


         imedia::position          m_tkLength;           /* Length of longest track                      */
         ::multimedia::result       m_mmrcLastErr;        /* Error return from last sequencer operation   */

         imedia::time        m_msLength;
         ::primitive::memory_file    m_memfile;

         sp(::music::midi::file)      m_pfile;
         midi_stream      m_hstream;

         uint32_t           m_uiSpecialModeV001Operation;
         int_array   m_iaLevel;
         uint32_t       m_dwTimeDivision;     /* File time division                           */

         LPBYTE      m_lpbAlloc;           /* Streaming buffers -- initial allocation      */
         uint32_t       m_cbPreroll;          /* Streaming buffers -- size of lpmhPreroll     */
         uint32_t       m_cbPrerollNominalMax;
         uint32_t        m_uBuffersInMMSYSTEM; /* Streaming buffers -- in use                  */
         imedia::position       m_tkPrerollBase;

         imedia::position    m_tkBase;             /* Where playback started from in stream        */
         imedia::position    m_tkEnd;              /* Where playback should end                    */

         flags                     m_flags; /* Various sequencer flags  */
         int32_t                     m_iOpenMode;

         midi_callback_data m_midicallbackdata;
         PlayerLink   m_playerlink;
         ::event       m_evBuffersZero;
         //uint32_t          m_cBuffer;            /* Number of streaming buffers to alloc         */
         uint32_t          m_cbBuffer;           /* size of each streaming buffer                */
         //uint32_t           m_uMCIDeviceID;       /* Our MCI device ID given to us                */
         //uint32_t           m_uMCITimeFormat;     /* Current time format                          */
         //uint32_t           m_uMCITimeDiv;        /* MCI_SEQ_DIV_xxx for current file             */
         //oswindow                     m_oswindow_;            /* Where to post MMSG_DONE when done playing   */
         sequence_thread *    m_pthread;




         sequence(sp(::ca2::application) papp);
         virtual ~sequence();
         file & GetFile();

         PlayerLink & GetPlayerLink();
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
         void OnEvent(e_event event);
         void OnEvent(Event * pevent);
         void SetLevelMeter(int32_t iLevel);
         ::multimedia::result CloseStream();
         bool SetMidiOutDevice(uint32_t uiDevice);
         int32_t SetKeyShift(int32_t iKeyShift);
         int32_t GetKeyShift();

         void OnMidiPlaybackEnd(sequence::Event * pevent);
         int32_t SetTempoShift(int32_t iTempoShift);

         sequence & get_midi_sequence();
         void OnPositionCB(LPMIDIHDR lpmidihdr);
         void OnDone(midi_stream hmidistream, midi_pack * lpmidihdr);
         virtual void GetTimeLength(imedia::time & time);
         virtual void GetPositionLength(imedia::position & position);

         using ::ikaraoke::karaoke::TimeToPosition;
         using ::ikaraoke::karaoke::PositionToTime;

         virtual imedia::position TimeToPosition(imedia::time time);
         virtual imedia::time PositionToTime(imedia::position position);

         virtual bool IsOpened();

         bool IsNull();
         void SetTempoChangeFlag(bool bSet = true);
         bool IsChangingTempo();

         int32_t GetTempoShift();
         void GetMidiDoneData(LPMIDIDONEDATA lpmdd);
         bool IsInSpecialModeV001();
         bool WasInSpecialModeV001();
         void SetSpecialModeV001Operation(uint32_t uiOperation);


         ::multimedia::result AllocBuffers();

         VOID FreeBuffers();

         //::multimedia::result OpenFile(const char * lpFileName, int32_t openMode);
         ::multimedia::result OpenFile(sequence & sequence, int32_t iOpenMode);
         ::multimedia::result OpenFile(::ca2::file & ar, int32_t openMode);
         ::multimedia::result OpenFile(const char * lpFileName, int32_t openMode);
         ::multimedia::result OpenFile(primitive::memory * pmemorystorage, int32_t openMode, ::music::e_storage estorage);

         ::multimedia::result CloseFile();
         ::multimedia::result SaveFile(const char * lpFileName);
         ::multimedia::result SaveFile();
         ::multimedia::result SaveFile(::ca2::filesp &ar);
         ::multimedia::result Preroll(::ca2::thread * pthread, LPPREROLL lpPreroll, bool bThrow);
         ::multimedia::result Start();

         ::multimedia::result Pause();

         ::multimedia::result Restart();

         //::multimedia::result Stop(uint32_t dwEllapse);
         ::multimedia::result Stop();

         void get_position(imedia::position  & time);
         void get_time(imedia::time  & time);

         ::multimedia::result get_ticks(imedia::position & time);
         ::multimedia::result get_millis(imedia::time & time);


         imedia::position MillisecsToTicks(imedia::time msOffset);

         imedia::time TicksToMillisecs(imedia::position tkOffset);

         bool IsPlaying();

         /*   void MidiOutCallbackDeprecated(
         HMIDISTRM hms,
         uint32_t uMsg,
         uint32_t dwUser,
         uint32_t dw1,
         uint32_t dw2,
         player::player * lpMidiPlayer,
         CXfplayerView * pview);*/
#ifdef WINDOWS
         static void CALLBACK MidiOutProc(HMIDIOUT hmo, uint32_t wMsg, uint32_t dwInstance, uint32_t dwParam1, uint32_t dwParam2);
         
#elif defined(MACOS)
#endif

         bool IsSettingPosition();
         void SetSettingPositionFlag(bool bSet = TRUE);

         imedia::position GetQuarterNote();
      };



      typedef struct tag_midiDoneData
      {

         sequence::flags         m_flags;
         uint32_t                    m_uiSeqState;
         uint32_t                    m_uiSeqPreviousState;

         inline bool SequenceIsInSpecialModeV001()
         {
            return m_flags.is_signalized(sequence::FlagIsInSpecialModeV001);
         }
         inline bool SequenceWasInSpecialModeV001()
         {
            return m_flags.is_signalized(sequence::FlagWasInSpecialModeV001);
         }
         inline uint32_t SequenceGetPreviousState()
         {
            return m_uiSeqPreviousState;
         }
         inline uint32_t SequenceGetState()
         {
            return m_uiSeqState;
         }

      } MIDIDONEDATA, * LPMIDIDONEDATA;


   } // namespace midi_mmsystem


} // namespace music







