#pragma once


namespace music
{


   namespace midi
   {


      class event_v1;


      class CLASS_DECL_VERIWELL_MULTIMEDIA event_v8 :
         public ::music::midi::event_base,
         public ::ca2::object
      {
      public:


         union
         {
            uint32_t                m_lpdwData[3];
            int16_t                m_lpshData[3 * sizeof(uint32_t) / sizeof(int16_t)];
            uint16_t       m_lpushData[3 * sizeof(uint32_t) / sizeof(int16_t)];
            BYTE                 m_lpbData[3 * sizeof(uint32_t)];
            struct
            {
               uint32_t               m_dw1;
               int32_t               m_iParamSize;
               void *            m_pParam;
            };
         };


         int32_t      m_iFlags;
         int32_t      m_iAllocate;
         void *   m_pvoid;
         uint32_t    m_dwInternal;


         event_v8();
         virtual ~event_v8();


         using ::music::midi::event_base::SetParam;

         virtual void clear();

         virtual VMSRESULT WriteParam(LPVOID lpData, ::primitive::memory_size iSize);
         virtual VMSRESULT FromData(LPVOID lpData, LPINT lpiLeft, BYTE &bRunningStatus);

         virtual ::music::midi::event_base & operator = (const ::music::midi::event_base & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual ::music::midi::event_base & operator = (const event_v5 & eventSrc) { return ::music::midi::event_base::operator = (eventSrc); }
         virtual event_v8 & operator = (const event_v8 & eventSrc);

         imedia::position GetDelta() const;
         void SetDelta(imedia::position tkDelta);
         imedia::position get_position() const;
         void SetPosition(imedia::position tkPosition);
         BYTE GetFullType() const;
         void SetFullType(BYTE bValue);
         BYTE get_type() const;
         void set_type(BYTE bValue);
         BYTE GetTrack() const;
         void SetTrack(BYTE bValue);
         BYTE GetMetaType() const;
         void SetMetaType(BYTE bValue);
         BYTE GetChB1() const;
         void SetChB1(BYTE bValue);
         BYTE GetChB2() const;
         void SetChB2(BYTE bValue);
         BYTE GetNotePitch() const;
         void SetNotePitch(BYTE bValue);
         BYTE GetNoteVelocity() const;
         void SetNoteVelocity(BYTE bValue);
         void SetImage(byte * hpbImage);
         byte * GetImage() const;
         ::primitive::memory_size GetImageSize() const;
         void SetImageSize(::primitive::memory_size cbSize);
         virtual void SetParam(void * pVoid, ::primitive::memory_size iSize);
         void _Free();
         bool _Allocate(int32_t iSize);
         byte * GetParam() const;
         ::primitive::memory_size GetParamSize() const;
         virtual VMSRESULT SetParam(::ca2::file & file, ::primitive::memory_size iLength = -1);
         virtual int32_t GetFlags() const;
         virtual void SetFlags(int32_t iFlag);

         VMSRESULT GetMidiEventV001(event_v1 * lpMidiEventV001,  uint32_t dwPositionTicks);


         BYTE     _GetType() const;
         BYTE     _GetTrack() const;
         BYTE     _GetMetaType() const;
         void     _SetMetaType(BYTE bValue);
         BYTE     _GetFullType() const;
         void     _SetFullType(BYTE bValue);
         BYTE     _GetChB1() const;
         void     _SetChB1(BYTE bValue);
         BYTE     _GetChB2() const;
         void     _SetChB2(BYTE bValue);

         void     _SetImage(byte * hpbImage);
         byte *   _GetImage() const;
         ::primitive::memory_size      _GetImageSize() const;
         void     _SetImageSize(::primitive::memory_size cbSize);

         int32_t      _GetFlags() const;
         void     _SetFlags(int32_t iFlag);

      protected:

         void SetParamSize(::primitive::memory_size iSize);


      };


   } // namespace midi


} // namespace music





