#pragma once


namespace music
{


   namespace midi_mmsystem
   {


      class event_v5;


      class CLASS_DECL_VERIWELL_MULTIMEDIA_MUSIC_MIDI_MMSYSTEM event_v1 : 
         public ::music::midi::event_base  
      {
      public:


         imedia::position           m_tkDelta;           
         imedia::position           m_tkPosition;
         BYTE                       m_abEvent[3];        
         uint32_t                   m_cbParm;            
         byte *                     m_hpbParm; 
         uint32_t                   m_cbImage;
         byte *                     m_hpbImage;
         sp(::music::midi::file)          m_pFile;
         ::music::midi::track *               m_pTrack;
         int32_t                    m_iFlags;


         event_v1();
         virtual ~event_v1();


         using ::music::midi::event_base::SetParam;
         virtual void clear();

         virtual ::music::midi::event_base & operator = (const ::music::midi::event_base & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual ::music::midi::event_base & operator = (const event_v5 & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual event_v1 & operator = (const event_v1 & eventSrc);

         inline imedia::position GetDelta() const;
         inline void SetDelta(imedia::position tkDelta);
         inline imedia::position get_position() const;
         inline void SetPosition(imedia::position tkPosition);
         inline BYTE GetFullType() const;
         inline void SetFullType(BYTE bValue);
         inline BYTE get_type() const;
         inline void set_type(BYTE bValue);
         inline BYTE GetTrack() const;
         inline void SetTrack(BYTE bValue);
         inline BYTE GetMetaType() const;
         inline void SetMetaType(BYTE bValue);
         inline BYTE GetChB1() const;
         inline void SetChB1(BYTE bValue);
         inline BYTE GetChB2() const;
         inline void SetChB2(BYTE bValue);
         inline BYTE GetNotePitch() const;
         inline void SetNotePitch(BYTE bValue);
         inline BYTE GetNoteVelocity() const;
         inline void SetNoteVelocity(BYTE bValue);
         void SetImage(byte * hpbImage);
         inline byte * GetImage() const;
         inline ::primitive::memory_size GetImageSize() const;
         inline void SetImageSize(::primitive::memory_size cbSize);
         //inline void SetParam(byte * hpbParam);
         inline void SetParam(void * pVoid, ::primitive::memory_size iSize);
         virtual VMSRESULT SetParam(::ca2::file & file, ::primitive::memory_size iLength = -1);
         inline byte * GetParam() const;
         //inline void SetParamSize(uint32_t cbSize);
         inline ::primitive::memory_size GetParamSize() const;

         virtual int32_t GetFlags() const;
         virtual void SetFlags(int32_t iFlag);

         static index CompareTkPosition(event_v1 * pa1, event_v1 * pa2);
         static void swap(event_v1 * p1, event_v1 * p2);

         imedia::position    _GetDelta() const;
         void     _SetDelta(imedia::position tkDelta);
         imedia::position    _GetPosition() const;
         void     _SetPosition(imedia::position tkPosition);
         BYTE     _GetFullType() const;
         void     _SetFullType(BYTE bValue);
         BYTE     _GetType() const;
         void     _SetType(BYTE bValue);
         BYTE     _GetTrack() const;
         void     _SetTrack(BYTE bValue);
         BYTE     _GetMetaType() const;
         void     _SetMetaType(BYTE bValue);
         BYTE     _GetChB1() const;
         void     _SetChB1(BYTE bValue);
         BYTE     _GetChB2() const;
         void     _SetChB2(BYTE bValue);
         BYTE     _GetNotePitch() const;
         void     _SetNotePitch(BYTE bValue);
         BYTE     _GetNoteVelocity() const;
         void     _SetNoteVelocity(BYTE bValue);
         void     _SetImage(byte * hpbImage);
         byte *   _GetImage() const;
         ::primitive::memory_size      _GetImageSize() const;
         void     _SetImageSize(::primitive::memory_size cbSize);
         void     _SetParam(void * pVoid, ::primitive::memory_size iSize);
         byte *   _GetParam() const;
         ::primitive::memory_size      _GetParamSize() const;

         int32_t      _GetFlags() const;
         void     _SetFlags(int32_t iFlag);
      };


   } // namespace midi_mmsystem


} // namespace music





